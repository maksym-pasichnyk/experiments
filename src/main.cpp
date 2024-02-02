#include "gpu/core.hpp"

#include <SDL_video.h>
#include <SDL_vulkan.h>
#include <SDL_events.h>

enum : u64 { MAX_FRAMES_IN_FLIGHT = 3 };

static auto debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT types, const VkDebugUtilsMessengerCallbackDataEXT* callback, void* user_data) -> VkBool32 {
    switch (severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
        std::println(stdout, "[verbose]: {}\n", callback->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
        std::println(stdout, "[info]: {}\n", callback->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
        std::println(stdout, "[warn]: {}\n", callback->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
        std::println(stdout, "[error]: {}\n", callback->pMessage);
        break;
    }
    default: {
        std::unreachable();
    }
    }
    return VK_FALSE;
}

static auto file_read_bytes(std::string const& path) -> std::optional<std::vector<char>> {
    if (auto stream = std::ifstream(path, std::ios::binary); stream.is_open()) {
        return std::vector(std::istreambuf_iterator(stream), {});
    }
    return std::nullopt;
}

auto main() -> i32 {
    auto* window = SDL_CreateWindow("Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN);
    auto* context = gpu_create_context(PFN_vkGetInstanceProcAddr(SDL_Vulkan_GetVkGetInstanceProcAddr()));
    auto* instance = context->create_instance({
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .p_application_info = (VkApplicationInfo[]){{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "",
            .applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0),
            .pEngineName = "kompute",
            .engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0),
            .apiVersion = VK_API_VERSION_1_2
        }},
        .enabled_layer_names = {
            "VK_LAYER_KHRONOS_validation"
        },
        .enabled_extension_names = {
            "VK_KHR_surface",
            "VK_EXT_debug_utils",
            "VK_MVK_macos_surface",
            "VK_KHR_portability_enumeration",
            "VK_KHR_get_physical_device_properties2"
        },
    });
    auto* debug_utils = instance->create_debug_utils({
        .flags = {},
        .severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .type = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
        .callback = &debug_utils_callback,
        .p_user_data = {}
    });
    auto* adapter = instance->enumerate_adapters().front();
    auto core_1_1 = VkPhysicalDeviceVulkan11Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .pNext = {}
    };
    auto core_1_2 = VkPhysicalDeviceVulkan12Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = &core_1_1,
        .timelineSemaphore = VK_TRUE,
        .bufferDeviceAddress = VK_TRUE
    };
    auto core_1_3 = VkPhysicalDeviceVulkan13Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &core_1_2,
        .inlineUniformBlock = VK_TRUE,
        .synchronization2 = VK_TRUE
    };
    auto* device = adapter->create_device({
        .pNext = (VkPhysicalDeviceFeatures2[]){{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &core_1_3,
            .features = {
                .shaderInt64 = VK_TRUE
            }
        }},
        .queue_create_infos = {
            VkDeviceQueueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = 0,
                .queueCount = 1,
                .pQueuePriorities = (f32[]){1}
            }
        },
        .enabled_extension_names = {
            "VK_KHR_swapchain",
            "VK_KHR_copy_commands2"
        }
    });
    auto* surface = instance->surface_from_handle(({
        VkSurfaceKHR vk_surface;
        SDL_Vulkan_CreateSurface(window, instance->handle, &vk_surface);
        vk_surface;
    }));
    auto* swapchain = device->create_swapchain({
        .flags = {},
        .surface = surface,
        .min_image_count = 3,
        .image_format = VK_FORMAT_B8G8R8A8_UNORM,
        .image_color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .image_extent = VkExtent2D(1600, 1200),
        .image_array_layers = 1,
        .image_usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE,
        .queue_family_indices = {},
        .pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .present_mode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_FALSE,
        .old_swapchain = {}
    });
    auto compute_shader_bytes = file_read_bytes("../shaders/ps.comp.spv").value();
    auto compute_shader_module = device->create_shader_module({
        .flags = {},
        .data = compute_shader_bytes.data(),
        .size = compute_shader_bytes.size(),
    });
    auto compute_bind_group_layout = device->create_bind_group_layout({
        .flags = {},
        .bindings = {
            VkDescriptorSetLayoutBinding{
                .binding = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
                .pImmutableSamplers = nullptr
            }
        }
    });
    auto compute_pipeline_layout = device->create_pipeline_layout({
        .flags = {},
        .bind_group_layouts = { compute_bind_group_layout },
        .push_constant_ranges = {}
    });
    auto compute_pipeline = device->create_compute_pipeline({
        .flags = VK_PIPELINE_CREATE_DISPATCH_BASE_BIT,
        .stage = GpuPipelineComputeStageCreateInfo{
            .flags = {},
            .module = compute_shader_module->handle,
            .name = "main",
            .specialization = GpuSpecializationInfo{
                .entries = {
                    VkSpecializationMapEntry(0, 0, sizeof(u32)),
                    VkSpecializationMapEntry(1, 4, sizeof(u32)),
                    VkSpecializationMapEntry(2, 8, sizeof(u32)),
                },
                .data = (u32[]){32, 32, 1},
                .size = sizeof(u32[3]),
            },
        },
        .layout = compute_pipeline_layout,
//        .base_pipeline_handle = {},
//        .base_pipeline_index = {},
    });

    VkDeviceMemory compute_image_memory;
    auto* compute_image = device->create_image({
        .flags = {},
        .type = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .extent = VkExtent3D(1600, 1200, 1),
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndices = {},
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    });
    device->create_memory(device->get_image_memory_requirements(compute_image->handle), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &compute_image_memory);
    device->vtable->vkBindImageMemory(device->handle, compute_image->handle, compute_image_memory, 0);

    VkSemaphore timeline_semaphore;
    VkSemaphore submit_semaphores[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore acquire_semaphores[MAX_FRAMES_IN_FLIGHT];
    VkCommandPool command_pools[MAX_FRAMES_IN_FLIGHT];
    VkCommandBuffer command_buffers[MAX_FRAMES_IN_FLIGHT];
    VkDescriptorPool bind_group_pools[MAX_FRAMES_IN_FLIGHT];

    device->vtable->vkCreateSemaphore(
        device->handle,
        (VkSemaphoreCreateInfo[]){{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = (VkSemaphoreTypeCreateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
                .pNext = {},
                .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
                .initialValue = 0
            }}
        }},
        nullptr,
        &timeline_semaphore
    );
    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i += 1) {
        device->vtable->vkCreateSemaphore(
            device->handle,
            (VkSemaphoreCreateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                .pNext = (VkSemaphoreTypeCreateInfo[]){{
                    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
                    .pNext = {},
                    .semaphoreType = VK_SEMAPHORE_TYPE_BINARY,
                }}
            }},
            nullptr,
            &submit_semaphores[i]
        );
        device->vtable->vkCreateSemaphore(
            device->handle,
            (VkSemaphoreCreateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                .pNext = (VkSemaphoreTypeCreateInfo[]){{
                    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
                    .pNext = {},
                    .semaphoreType = VK_SEMAPHORE_TYPE_BINARY,
                }}
            }},
            nullptr,
            &acquire_semaphores[i]
        );
        device->vtable->vkCreateCommandPool(
            device->handle,
            (VkCommandPoolCreateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext = {},
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = 0
            }},
            nullptr,
            &command_pools[i]
        );
        device->vtable->vkAllocateCommandBuffers(
            device->handle,
            (VkCommandBufferAllocateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .commandPool = command_pools[i],
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1,
            }},
            &command_buffers[i]
        );
        device->vtable->vkCreateDescriptorPool(
            device->handle,
            (VkDescriptorPoolCreateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .pNext = {},
                .flags = {},
                .maxSets = 1,
                .poolSizeCount = 1,
                .pPoolSizes = (VkDescriptorPoolSize[]) {
                    VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1)
                }
            }},
            nullptr,
            &bind_group_pools[i]
        );
    }

    u32 frame_index = 0;
    u32 total_frame_index = 0;
    u32 surface_image_index = 0;

    bool Quit = false;
    while (!Quit) {
        SDL_Event Event;
        while (SDL_PollEvent(&Event) == 1) {
            if (Event.type == SDL_QUIT) {
                Quit = true;
            }
        }

        if (total_frame_index >= MAX_FRAMES_IN_FLIGHT) {
            device->vtable->vkWaitSemaphores(
                device->handle,
                (VkSemaphoreWaitInfo[]){{
                    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
                    .pNext = {},
                    .flags = {},
                    .semaphoreCount = 1,
                    .pSemaphores = (VkSemaphore[]){ timeline_semaphore },
                    .pValues = (u64[]) { total_frame_index - MAX_FRAMES_IN_FLIGHT + 1 },
                }},
                std::numeric_limits<u64>::max()
            );
            device->vtable->vkResetDescriptorPool(device->handle, bind_group_pools[frame_index], VkDescriptorPoolResetFlags());
        }

        device->vtable->vkAcquireNextImage2KHR(
            device->handle,
            (VkAcquireNextImageInfoKHR[]){{
                .sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR,
                .pNext = {},
                .swapchain = swapchain->handle,
                .timeout = std::numeric_limits<u64>::max(),
                .semaphore = acquire_semaphores[frame_index],
                .fence = {},
                .deviceMask = 1
            }},
            &surface_image_index
        );
        device->vtable->vkBeginCommandBuffer(
            command_buffers[frame_index],
            (VkCommandBufferBeginInfo[]){{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
            }}
        );
        device->vtable->vkCmdPipelineBarrier2(
            command_buffers[frame_index],
            (VkDependencyInfo[]) {{
                .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
                .pNext = {},
                .dependencyFlags = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers = (VkImageMemoryBarrier2[]) {
                    VkImageMemoryBarrier2{
                        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                        .srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
                        .srcAccessMask = VK_ACCESS_2_NONE,
                        .dstStageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
                        .dstAccessMask = VK_ACCESS_2_SHADER_STORAGE_READ_BIT | VK_ACCESS_2_SHADER_STORAGE_WRITE_BIT,
                        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                        .newLayout = VK_IMAGE_LAYOUT_GENERAL,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = compute_image->handle,
                        .subresourceRange = VkImageSubresourceRange{
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                        }
                    }
                }
            }}
        );

        VkDescriptorSet compute_bind_group;
        device->vtable->vkAllocateDescriptorSets(
            device->handle,
            (VkDescriptorSetAllocateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext = {},
                .descriptorPool = bind_group_pools[frame_index],
                .descriptorSetCount = 1,
                .pSetLayouts = (VkDescriptorSetLayout[]){
                    compute_bind_group_layout
                }
            }},
            &compute_bind_group
        );
        device->vtable->vkUpdateDescriptorSets(
            device->handle,
            1, (VkWriteDescriptorSet[]){
                VkWriteDescriptorSet{
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext = {},
                    .dstSet = compute_bind_group,
                    .dstBinding = 0,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                    .pImageInfo = (VkDescriptorImageInfo[]){{
                        .sampler = {},
                        .imageView = compute_image->subresource({
                            .flags = {},
                            .type = VK_IMAGE_VIEW_TYPE_2D,
                            .format = VK_FORMAT_R8G8B8A8_UNORM,
                            .components = VkComponentMapping{
                                .r = VK_COMPONENT_SWIZZLE_R,
                                .g = VK_COMPONENT_SWIZZLE_G,
                                .b = VK_COMPONENT_SWIZZLE_B,
                                .a = VK_COMPONENT_SWIZZLE_A,
                            },
                            .subresourceRange = VkImageSubresourceRange{
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1
                            }
                        }),
                        .imageLayout = VK_IMAGE_LAYOUT_GENERAL
                    }},
                    .pBufferInfo = {},
                    .pTexelBufferView = {},
                }
            },
            0, (VkCopyDescriptorSet[]){}
        );
        device->vtable->vkCmdBindPipeline(command_buffers[frame_index], VK_PIPELINE_BIND_POINT_COMPUTE, compute_pipeline->handle);
        device->vtable->vkCmdBindDescriptorSets(command_buffers[frame_index], VK_PIPELINE_BIND_POINT_COMPUTE, compute_pipeline_layout, 0, 1, &compute_bind_group, 0, {});

        auto GroupSizeX = (1600 + 32 - 1) / 32;
        auto GroupSizeY = (1200 + 32 - 1) / 32;
        device->vtable->vkCmdDispatchBase(command_buffers[frame_index], 0, 0, 0, GroupSizeX, GroupSizeY, 1);
        device->vtable->vkCmdPipelineBarrier2(
            command_buffers[frame_index],
            (VkDependencyInfo[]) {{
                .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
                .pNext = {},
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
                .imageMemoryBarrierCount = 2,
                .pImageMemoryBarriers = (VkImageMemoryBarrier2[]) {
                    VkImageMemoryBarrier2{
                        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                        .srcStageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
                        .srcAccessMask = VK_ACCESS_2_SHADER_STORAGE_READ_BIT | VK_ACCESS_2_SHADER_STORAGE_WRITE_BIT,
                        .dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
                        .dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
                        .oldLayout = VK_IMAGE_LAYOUT_GENERAL,
                        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = compute_image->handle,
                        .subresourceRange = VkImageSubresourceRange{
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                        }
                    },
                    VkImageMemoryBarrier2{
                        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                        .srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
                        .srcAccessMask = VK_ACCESS_2_NONE,
                        .dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
                        .dstAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
                        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = swapchain->images[surface_image_index],
                        .subresourceRange = VkImageSubresourceRange{
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                        }
                    }
                }
            }}
        );
        device->vtable->vkCmdBlitImage2(
            command_buffers[frame_index],
            (VkBlitImageInfo2[]){{
                .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
                .pNext = {},
                .srcImage = compute_image->handle,
                .srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                .dstImage = swapchain->images[surface_image_index],
                .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .regionCount = 1,
                .pRegions = (VkImageBlit2[]){
                    VkImageBlit2{
                        .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
                        .pNext = {},
                        .srcSubresource = VkImageSubresourceLayers{
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .mipLevel = 0,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                        },
                        .srcOffsets = {
                            VkOffset3D{
                                .x = 0,
                                .y = 0,
                                .z = 0
                            },
                            VkOffset3D{
                                .x = i32(1600),
                                .y = i32(1200),
                                .z = 1
                            },
                        },
                        .dstSubresource = VkImageSubresourceLayers{
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .mipLevel = 0,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                        },
                        .dstOffsets = {
                            VkOffset3D{
                                .x = 0,
                                .y = 0,
                                .z = 0
                            },
                            VkOffset3D{
                                .x = i32(1600),
                                .y = i32(1200),
                                .z = 1
                            },
                        }
                    }
                },
                .filter = VK_FILTER_NEAREST
            }}
        );
        device->vtable->vkCmdPipelineBarrier2(
            command_buffers[frame_index],
            (VkDependencyInfo[]) {{
                .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
                .pNext = {},
                .dependencyFlags = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers = (VkImageMemoryBarrier2[]) {
                    VkImageMemoryBarrier2{
                        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                        .srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
                        .srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
                        .dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
                        .dstAccessMask = VK_ACCESS_2_NONE,
                        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = swapchain->images[surface_image_index],
                        .subresourceRange = VkImageSubresourceRange{
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                        }
                    }
                }
            }}
        );
        VkQueue queue;
        device->vtable->vkGetDeviceQueue2(
            device->handle,
            (VkDeviceQueueInfo2[]){{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
                .pNext = {},
                .flags = {},
                .queueFamilyIndex = 0,
                .queueIndex = 0
            }},
            &queue
        );
        device->vtable->vkEndCommandBuffer(command_buffers[frame_index]);
        device->vtable->vkQueueSubmit2(
            queue,
            1,
            (VkSubmitInfo2[]){{
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
                .pNext = {},
                .flags = {},
                .waitSemaphoreInfoCount = 1,
                .pWaitSemaphoreInfos = (VkSemaphoreSubmitInfo[]) {
                    VkSemaphoreSubmitInfo{
                        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                        .pNext = {},
                        .semaphore = acquire_semaphores[frame_index],
                        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                    }
                },
                .commandBufferInfoCount = 1,
                .pCommandBufferInfos = (VkCommandBufferSubmitInfo[]){{
                    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
                    .pNext = {},
                    .commandBuffer = command_buffers[frame_index],
                    .deviceMask = 0
                }},
                .signalSemaphoreInfoCount = 2,
                .pSignalSemaphoreInfos = (VkSemaphoreSubmitInfo[]) {
                    VkSemaphoreSubmitInfo{
                        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                        .pNext = {},
                        .semaphore = submit_semaphores[frame_index],
                        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                    },
                    VkSemaphoreSubmitInfo{
                        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                        .pNext = {},
                        .semaphore = timeline_semaphore,
                        .value = total_frame_index + 1,
                        .stageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
                    },
                }
            }},
            nullptr
        );
        device->vtable->vkQueuePresentKHR(
            queue,
            (VkPresentInfoKHR[]) {{
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .pNext = {},
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = (VkSemaphore[]) {
                    submit_semaphores[frame_index]
                },
                .swapchainCount = 1,
                .pSwapchains = (VkSwapchainKHR[]) {
                    swapchain->handle
                },
                .pImageIndices = (u32[]) {
                    surface_image_index
                },
                .pResults = {}
            }}
        );
        total_frame_index += 1;
        frame_index += 1;
        frame_index %= MAX_FRAMES_IN_FLIGHT;
    }
    device->vtable->vkDeviceWaitIdle(device->handle);

    device->delete_memory(compute_image_memory);
    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i += 1) {
        device->delete_semaphore(submit_semaphores[i]);
        device->delete_semaphore(acquire_semaphores[i]);
        device->delete_command_pool(command_pools[i]);
        device->delete_bind_group_pool(bind_group_pools[i]);
    }
    device->delete_semaphore(timeline_semaphore);

    compute_shader_module->release();
    compute_pipeline->release();
    compute_image->release();
    device->delete_pipeline_layout(compute_pipeline_layout);
    device->delete_bind_group_layout(compute_bind_group_layout);
    swapchain->release();
    surface->release();
    device->release();
    debug_utils->release();
    instance->release();
    context->release();

    SDL_DestroyWindow(window);
    return 0;
}
