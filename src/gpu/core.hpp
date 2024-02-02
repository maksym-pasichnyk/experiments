//
// Created by Maksym Pasichnyk on 20.10.2023.
//

#pragma once

#include "stl/pch.hpp"
#include "vtable.hpp"

struct GpuImage;
struct GpuDevice;
struct GpuAdapter;
struct GpuSurface;
struct GpuContext;
struct GpuInstance;
struct GpuSwapchain;
struct GpuDebugUtils;
struct GpuShaderModule;
struct GpuComputePipeline;
struct GpuGraphicsPipeline;

struct GpuImageCreateInfo final {
    VkImageCreateFlags       flags;
    VkImageType              type;
    VkFormat                 format;
    VkExtent3D               extent;
    uint32_t                 mipLevels;
    uint32_t                 arrayLayers;
    VkSampleCountFlagBits    samples;
    VkImageTiling            tiling;
    VkImageUsageFlags        usage;
    VkSharingMode            sharingMode;
    std::span<u32 const>     queueFamilyIndices;
    VkImageLayout            initialLayout;
};

struct GpuImageSubresource final {
    VkImageViewCreateFlags     flags;
    VkImageViewType            type;
    VkFormat                   format;
    VkComponentMapping         components;
    VkImageSubresourceRange    subresourceRange;
};

struct GpuDeviceCreateInfo final {
    const void*                              pNext;
    VkDeviceCreateFlags                      flags;
    std::span<VkDeviceQueueCreateInfo const> queue_create_infos;
    std::span<char const* const>             enabled_extension_names;
};

struct GpuInstanceCreateInfo final {
    const void*                     pNext;
    VkInstanceCreateFlags           flags;
    VkApplicationInfo const*        p_application_info;
    std::span<char const* const>    enabled_layer_names;
    std::span<char const* const>    enabled_extension_names;
};

struct GpuSwapchainCreateInfo final {
    VkSwapchainCreateFlagsKHR        flags;
    GpuSurface*                      surface;
    uint32_t                         min_image_count;
    VkFormat                         image_format;
    VkColorSpaceKHR                  image_color_space;
    VkExtent2D                       image_extent;
    uint32_t                         image_array_layers;
    VkImageUsageFlags                image_usage;
    VkSharingMode                    image_sharing_mode;
    std::span<u32 const>             queue_family_indices;
    VkSurfaceTransformFlagBitsKHR    pre_transform;
    VkCompositeAlphaFlagBitsKHR      composite_alpha;
    VkPresentModeKHR                 present_mode;
    VkBool32                         clipped;
    VkSwapchainKHR                   old_swapchain;
};

struct GpuDebugUtilsCreateInfo final {
    VkDebugUtilsMessengerCreateFlagsEXT     flags;
    VkDebugUtilsMessageSeverityFlagsEXT     severity;
    VkDebugUtilsMessageTypeFlagsEXT         type;
    PFN_vkDebugUtilsMessengerCallbackEXT    callback;
    void*                                   p_user_data;
};

struct GpuShaderModuleCreateInfo final {
    VkShaderModuleCreateFlags   flags;
    const void*                 data;
    size_t                      size;
};

struct GpuSpecializationInfo final {
    std::span<VkSpecializationMapEntry const>   entries;
    const void*                                 data;
    size_t                                      size;
};

struct GpuPipelineLayoutCreateInfo final {
    VkPipelineLayoutCreateFlags             flags;
    std::span<VkDescriptorSetLayout const>  bind_group_layouts;
    std::span<VkPushConstantRange const>    push_constant_ranges;
};

struct GpuPipelineComputeStageCreateInfo final {
    VkPipelineShaderStageCreateFlags    flags;
    VkShaderModule                      module;
    const char*                         name;
    GpuSpecializationInfo               specialization;
};

struct GpuComputePipelineCreateInfo final {
    VkPipelineCreateFlags              flags;
    GpuPipelineComputeStageCreateInfo  stage;
    VkPipelineLayout                   layout;
//    VkPipeline                         base_pipeline_handle;
//    i32                                base_pipeline_index;
};

struct GpuDescriptorSetLayoutCreateInfo final {
    VkDescriptorSetLayoutCreateFlags                flags;
    std::span<VkDescriptorSetLayoutBinding const>   bindings;
};

struct GpuObject {
    explicit GpuObject() : __strong_references(1) {}
    virtual ~GpuObject() = default;

    template<typename Self>
    auto retain(this Self& self) -> Self* {
        self.GpuObject::__strong_references.fetch_add(1);
        return std::addressof(self);
    }

    template<typename Self>
    void release(this Self& self) {
        if (self.GpuObject::__strong_references.fetch_sub(1) == 1) {
            delete std::addressof(self);
        }
    }

private:
    std::atomic_uint64_t __strong_references;
};

struct GpuContext final : GpuObject {
    ContextDispatcher vtable;

    explicit GpuContext(PFN_vkGetInstanceProcAddr get_proc_addr);

    auto enumerate_instance_version(this GpuContext& self) -> u32;
    auto create_instance(this GpuContext& self, GpuInstanceCreateInfo const& create_info) -> GpuInstance*;
};

struct GpuInstance final : GpuObject {
    GpuContext* context;
    u32 api_version;
    VkInstance handle;
    InstanceDispatcher* vtable;
    u32 adapter_count;
    GpuAdapter** adapters;

    GpuInstance(GpuContext* context, VkInstanceCreateInfo const* p_create_info);
    ~GpuInstance() override;

    auto create_debug_utils(this GpuInstance& self, GpuDebugUtilsCreateInfo const& create_info) -> GpuDebugUtils*;
    auto surface_from_handle(this GpuInstance& self, VkSurfaceKHR surface) -> GpuSurface*;
    auto enumerate_adapters(this GpuInstance const& self) -> std::span<GpuAdapter*>;
};

struct GpuDebugUtils final : GpuObject {
    GpuInstance* instance;
    VkDebugUtilsMessengerEXT handle;

    GpuDebugUtils(GpuInstance* instance, VkDebugUtilsMessengerCreateInfoEXT const* p_create_info);
    ~GpuDebugUtils() override;
};

struct GpuSurface final : GpuObject {
    GpuInstance* instance;
    VkSurfaceKHR handle;

    GpuSurface(GpuInstance* instance, VkSurfaceKHR handle);
    ~GpuSurface() override;
};

struct GpuAdapter final : GpuObject {
    GpuInstance* instance;
    VkPhysicalDevice handle;
    u32 queue_family_count;
    VkQueueFamilyProperties* queue_family_properties;
    VkPhysicalDeviceMemoryProperties device_memory_properties;
    VkPhysicalDeviceFeatures core_0_1;

    GpuAdapter(GpuInstance* instance, VkPhysicalDevice handle);
    ~GpuAdapter() override;

    auto create_device(this GpuAdapter& self, GpuDeviceCreateInfo const& create_info) -> GpuDevice*;
    auto get_queue_family_properties(this GpuAdapter const& self) -> std::span<VkQueueFamilyProperties const>;
};

struct GpuDevice final : GpuObject {
    GpuAdapter* adapter;
    VkDevice handle;
    DeviceDispatcher* vtable;

    GpuDevice(GpuAdapter* adapter, VkDeviceCreateInfo const* p_create_info);
    ~GpuDevice() override;

    void wait_idle(this GpuDevice& self);
    auto wait_for_fences(this GpuDevice& self, u32 fence_count, VkFence const* p_fences, bool wait_all, u64 timeout) -> VkResult;
    auto reset_fences(this GpuDevice& self, u32 fence_count, VkFence const* p_fences) -> VkResult;
    auto map_memory(this GpuDevice& self, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** pp_data) -> VkResult;
    void unmap_memory(this GpuDevice& self, VkDeviceMemory memory);
    auto create_semaphore(this GpuDevice& self, VkSemaphore* p_semaphore) -> VkResult;
    auto create_timeline_semaphore(this GpuDevice& self, u64 initial_value, VkSemaphore* p_semaphore) -> VkResult;
    void delete_semaphore(this GpuDevice& self, VkSemaphore semaphore);
    auto create_fence(this GpuDevice& self, bool signaled, VkFence* p_fence) -> VkResult;
    void delete_fence(this GpuDevice& self, VkFence fence);
    auto create_image(this GpuDevice& self, GpuImageCreateInfo const& create_info) -> GpuImage*;
    auto create_image_view(this GpuDevice& self, VkImageViewCreateInfo const* p_create_info, VkImageView* pImageView) -> VkResult;
    void delete_image_view(this GpuDevice& self, VkImageView imageView);
    auto create_buffer(this GpuDevice& self, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer* p_buffer) -> VkResult;
    void delete_buffer(this GpuDevice& self, VkBuffer buffer);
    auto get_device_address(this GpuDevice& self, VkBuffer buffer) -> VkDeviceAddress;
    auto create_graphics_pipelines(this GpuDevice& self, VkPipelineCache pipeline_cache, std::span<VkGraphicsPipelineCreateInfo const> p_create_infos, VkPipeline* p_pipelines) -> VkResult;
    void delete_pipeline(this GpuDevice& self, VkPipeline pipeline);
    auto create_pipeline_layout(this GpuDevice& self, GpuPipelineLayoutCreateInfo const& create_info) -> VkPipelineLayout;
    void delete_pipeline_layout(this GpuDevice& self, VkPipelineLayout pipeline_layout);
    auto create_bind_group_pool(this GpuDevice& self, u32 max_sets, std::span<VkDescriptorPoolSize const> pool_sizes, VkDescriptorPool* p_bind_group_pool) -> VkResult;
    void delete_bind_group_pool(this GpuDevice& self, VkDescriptorPool bind_group_pool);
    auto create_sampler(this GpuDevice& self, VkSamplerCreateInfo const* p_create_info, VkSampler* p_sampler) -> VkResult;
    void delete_sampler(this GpuDevice& self, VkSampler sampler);
    auto allocate_bind_groups(this GpuDevice& self, VkDescriptorPool bind_group_pool, std::span<VkDescriptorSetLayout const> layouts, VkDescriptorSet* p_bind_groups) -> VkResult;
    void update_bind_groups(this GpuDevice& self, std::span<VkWriteDescriptorSet const> write_bind_groups, std::span<VkCopyDescriptorSet const> copy_bind_groups);
    auto create_command_pool(this GpuDevice& self, VkCommandPoolCreateFlags flags, u32 queue_family_index, VkCommandPool* p_command_pool) -> VkResult;
    void delete_command_pool(this GpuDevice& self, VkCommandPool command_pool);
    auto reset_command_pool(this GpuDevice& self, VkCommandPool command_pool, VkCommandPoolResetFlags flags) -> VkResult;
    auto create_command_buffers(this GpuDevice& self, VkCommandPool command_pool, VkCommandBufferLevel level, u32 command_buffer_count, VkCommandBuffer* p_command_buffers) -> VkResult;
    void delete_command_buffer(this GpuDevice& self, VkCommandPool command_pool, u32 command_buffer_count, VkCommandBuffer* p_command_buffers);
    auto reset_bind_group_pool(this GpuDevice& self, VkDescriptorPool bind_group_pool, VkDescriptorPoolResetFlags flags) -> VkResult;
    auto create_memory(this GpuDevice& self, VkMemoryRequirements requirements, VkMemoryPropertyFlags flags, VkDeviceMemory* p_device_memory) -> VkResult;
    void delete_memory(this GpuDevice& self, VkDeviceMemory device_memory);
    auto acquire_next_image(this GpuDevice& self, VkSwapchainKHR swapchain, u64 timeout, VkSemaphore semaphore, VkFence fence, u32 device_mask, u32* p_image_index) -> VkResult;
    auto get_image_memory_requirements(this GpuDevice& self, VkImage image) -> VkMemoryRequirements;
    auto get_buffer_memory_requirements(this GpuDevice& self, VkBuffer buffer) -> VkMemoryRequirements;

    auto create_swapchain(this GpuDevice& self, GpuSwapchainCreateInfo const& create_info) -> GpuSwapchain*;
    auto create_shader_module(this GpuDevice& self, GpuShaderModuleCreateInfo const& create_info) -> GpuShaderModule*;

    auto create_bind_group_layout(this GpuDevice& self, GpuDescriptorSetLayoutCreateInfo const& create_info) -> VkDescriptorSetLayout;
    void delete_bind_group_layout(this GpuDevice& self, VkDescriptorSetLayout bind_group_layout);

    auto create_compute_pipeline(this GpuDevice& self, GpuComputePipelineCreateInfo const& create_info) -> GpuComputePipeline*;
};

struct GpuImage final : GpuObject {
    GpuDevice* device;
    VkImage handle;
    std::vector<std::tuple<GpuImageSubresource, VkImageView>> views;

    GpuImage(GpuDevice* device, VkImageCreateInfo const* p_create_info);
    ~GpuImage() override;

    auto subresource(this GpuImage& self, GpuImageSubresource const& info) -> VkImageView;
};

struct GpuSwapchain final : GpuObject {
    GpuDevice* device;
    VkSwapchainKHR handle;
    u32 image_count;
    VkImage* images;
    
    GpuSwapchain(GpuDevice* device, VkSwapchainCreateInfoKHR const* p_create_info);
    ~GpuSwapchain() override;
};

struct GpuShaderModule final : GpuObject {
    GpuDevice* device;
    VkShaderModule handle;

    GpuShaderModule(GpuDevice* device, VkShaderModuleCreateInfo const* p_create_info);
    ~GpuShaderModule() override;
};

struct GpuComputePipeline final : GpuObject {
    GpuDevice* device;
    VkPipeline handle;

    GpuComputePipeline(GpuDevice* device, VkComputePipelineCreateInfo const* p_create_info);
    ~GpuComputePipeline() override;
};

auto gpu_create_context(PFN_vkGetInstanceProcAddr p_get_proc_addr) -> GpuContext*;
