//
// Created by Maksym Pasichnyk on 04.01.2024.
//
#pragma once

#include "core.hpp"

struct FreeListNode {
    VkDeviceSize size;
    VkDeviceSize offs;
};

struct VkAllocation {
    VkDeviceSize size;
    VkDeviceSize offset;
    VkDeviceMemory device_memory;
};

struct FreeListBlock {
    std::list<FreeListNode> nodes;
    VkDeviceMemory          device_memory;
    VkDeviceSize            allocation_size;
    u32                     memory_type_index;

    static auto from_raw_parts(VkDeviceMemory device_memory, VkDeviceSize allocation_size, u32 memory_type_index) -> FreeListBlock {
        return FreeListBlock{
            .nodes = {
                FreeListNode{
                    .size = allocation_size,
                    .offs = 0zu,
                }
            },
            .device_memory = device_memory,
            .allocation_size = allocation_size,
            .memory_type_index = memory_type_index
        };
    }

    void reset(this FreeListBlock& self) {
        self.nodes.clear();
        self.nodes.emplace_back(FreeListNode{
            .size = self.allocation_size,
            .offs = 0zu,
        });
    }

    auto try_deallocate(this FreeListBlock& self, VkAllocation allocation) -> bool {
        if (self.device_memory != allocation.device_memory) {
            return false;
        }
        self.nodes.emplace_back(FreeListNode{
            .size = allocation.size,
            .offs = allocation.offset,
        });
        return true;
    }

    auto try_allocate(this FreeListBlock& self, VkDeviceSize size, VkDeviceSize alignment, VkAllocation* p_allocation) -> bool {
        for (auto blk = self.nodes.begin(); blk != self.nodes.end(); ++blk) {
            size_t aligned_offset = ((blk->offs + alignment - 1) / alignment) * alignment;
            if (blk->size >= (aligned_offset + size)) {
                p_allocation->size = size;
                p_allocation->offset = aligned_offset;
                p_allocation->device_memory = self.device_memory;

                if (blk->size > (aligned_offset + size)) {
                    self.nodes.insert(blk, FreeListNode{
                        .size = blk->size - (aligned_offset + size),
                        .offs = aligned_offset + size
                    });
                }
                self.nodes.erase(blk);
                return true;
            }
        }
        return false;
    }
};

struct BufferAllocateInfo {
    VkBuffer buffer;
    VkDeviceSize size;
    VkDeviceSize offset;
    VkAllocation allocation;
};

struct LinearBufferAllocator {
    VkBuffer buffer;
    VkDeviceSize offset;
    VkAllocation allocation;

    static auto from_raw_parts(VkBuffer buffer, VkAllocation allocation) -> LinearBufferAllocator {
        return LinearBufferAllocator{
            .buffer = buffer,
            .offset = 0zu,
            .allocation = allocation,
        };
    }

    void reset(this LinearBufferAllocator& self) {
        self.offset = 0;
    }

    void cleanup(this LinearBufferAllocator& self, GpuDevice* gpu) {
        gpu->delete_buffer(self.buffer, self.allocation);
    }

    auto try_allocate(this LinearBufferAllocator& self, VkDeviceSize size, VkDeviceSize alignment, BufferAllocateInfo* p_buffer_allocate_info) -> bool {
        size_t aligned_offset = ((self.offset + alignment - 1) / alignment) * alignment;
        if ((aligned_offset + size) > self.allocation.size) {
            return false;
        }
        self.offset = aligned_offset + size;
        p_buffer_allocate_info->buffer = self.buffer;
        p_buffer_allocate_info->size = size;
        p_buffer_allocate_info->offset = aligned_offset;
        p_buffer_allocate_info->allocation = self.allocation;
        return true;
    }
};

struct FreeListBufferAllocator {
    VkBuffer buffer;
    VkAllocation allocation;
    std::list<FreeListNode> nodes;

    static auto from_raw_parts(VkBuffer buffer, VkAllocation allocation) -> FreeListBufferAllocator {
        return FreeListBufferAllocator{
            .buffer = buffer,
            .allocation = allocation,
            .nodes = {
                FreeListNode{
                    .size = allocation.size,
                    .offs = 0zu,
                }
            }
        };
    }

    void reset(this FreeListBufferAllocator& self) {
        self.nodes.clear();
        self.nodes.emplace_back(FreeListNode{
            .size = self.allocation.size,
            .offs = 0zu,
        });
    }

    void cleanup(this FreeListBufferAllocator& self, GpuDevice* gpu) {
        gpu->delete_buffer(self.buffer, self.allocation);
    }

    auto try_allocate(this FreeListBufferAllocator& self, VkDeviceSize size, VkDeviceSize alignment, BufferAllocateInfo* p_buffer_allocate_info) -> bool {
        for (auto blk = self.nodes.begin(); blk != self.nodes.end(); ++blk) {
            size_t aligned_offset = ((blk->offs + alignment - 1) / alignment) * alignment;
            if (blk->size >= (aligned_offset + size)) {
                p_buffer_allocate_info->buffer = self.buffer;
                p_buffer_allocate_info->size = size;
                p_buffer_allocate_info->offset = aligned_offset;
                p_buffer_allocate_info->allocation = self.allocation;

                if (blk->size > (aligned_offset + size)) {
                    self.nodes.insert(blk, FreeListNode{
                        .size = blk->size - (aligned_offset + size),
                        .offs = aligned_offset + size
                    });
                }
                self.nodes.erase(blk);
                return true;
            }
        }
        return false;
    }
};

template<typename Allocator>
struct BufferPool {
    VkDeviceSize page_size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags flags;
    std::vector<Allocator> allocators;

    static auto init(VkDeviceSize page_size, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags) -> BufferPool {
        return BufferPool {
            .page_size = page_size,
            .usage = usage,
            .flags = flags,
            .allocators = {}
        };
    }

    void reset(this BufferPool& self, GpuDevice* gpu) {
        std::ranges::for_each(std::views::iota(0zu, self.allocators.size()), [&](size_t block_index) {
            self.allocators[block_index].reset();
        });
    }

    void cleanup(this BufferPool& self, GpuDevice* gpu) {
        std::ranges::for_each(std::views::iota(0zu, self.allocators.size()), [&](size_t block_index) {
            self.allocators[block_index].cleanup(gpu);
        });
        self.allocators.clear();
    }

    auto allocate(this BufferPool& self, GpuDevice* gpu, VkDeviceSize size, VkDeviceSize alignment, BufferAllocateInfo* p_buffer_allocate_info) -> VkResult {
        for (auto& block : std::views::reverse(self.allocators)) {
            if (block.try_allocate(size, alignment, p_buffer_allocate_info)) {
                return VK_SUCCESS;
            }
        }

        VkDeviceSize aligned_size = ((size + self.page_size - 1) / self.page_size) * self.page_size;

        VkBuffer buffer;
        VkAllocation allocation;
        auto result = gpu->create_buffer(aligned_size, self.usage, self.flags, &buffer, &allocation);
        if (result != VK_SUCCESS) {
            return result;
        }
        self.allocators.emplace_back(Allocator::from_raw_parts(buffer, allocation));
        auto _ = self.allocators.back().try_allocate(size, alignment, p_buffer_allocate_info);
        return VK_SUCCESS;
    }
};

struct ResourceAllocator {
    std::vector<VkDescriptorPool> pools;

    void reset(this ResourceAllocator& self, GpuDevice* gpu) {
        std::ranges::for_each(std::views::iota(0zu, self.pools.size()), [&](size_t pool_index) {
            gpu->reset_bind_group_pool(self.pools[pool_index], VkDescriptorPoolResetFlags());
        });
    }

    void cleanup(this ResourceAllocator& self, GpuDevice* gpu) {
        std::ranges::for_each(std::views::iota(0zu, self.pools.size()), [&](size_t pool_index) {
            gpu->delete_bind_group_pool(self.pools[pool_index]);
        });
        self.pools.clear();
    }

    auto allocate(this ResourceAllocator& self, GpuDevice* gpu, std::span<VkDescriptorSetLayout const> layouts, VkDescriptorSet* p_bind_groups) -> VkResult {
        for (auto pool : self.pools) {
            if (gpu->allocate_bind_groups(pool, layouts, p_bind_groups) == VK_SUCCESS) {
                return VK_SUCCESS;
            }
        }

        VkDescriptorPool bind_group_pool;
        auto result = gpu->create_bind_group_pool(
            1000,
            (VkDescriptorPoolSize[]) {
                VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER, 1000),
                VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000),
                VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000),
                VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000),
                VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000),
            },
            &bind_group_pool
        );
        if (result != VK_SUCCESS) {
            return result;
        }

        self.pools.emplace_back(bind_group_pool);
        return gpu->allocate_bind_groups(self.pools.back(), layouts, p_bind_groups);
    }
};
