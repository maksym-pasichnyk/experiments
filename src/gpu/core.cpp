//
// Created by Maksym Pasichnyk on 20.10.2023.
//
#include "core.hpp"
#include "vtable.hpp"

static constexpr auto features_1_1 = std::array{
    std::tuple(&VkPhysicalDeviceVulkan11Features::storageBuffer16BitAccess, "VK_KHR_16bit_storage"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::uniformAndStorageBuffer16BitAccess, "VK_KHR_16bit_storage"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::storagePushConstant16, "VK_KHR_16bit_storage"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::storageInputOutput16, "VK_KHR_16bit_storage"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::multiview, "VK_KHR_multiview"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::multiviewGeometryShader, "VK_KHR_multiview"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::multiviewTessellationShader, "VK_KHR_multiview"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::variablePointersStorageBuffer, "VK_KHR_variable_pointers"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::variablePointers, "VK_KHR_variable_pointers"),
//    std::tuple(&VkPhysicalDeviceVulkan11Features::protectedMemory, ""),
    std::tuple(&VkPhysicalDeviceVulkan11Features::samplerYcbcrConversion, "VK_KHR_sampler_ycbcr_conversion"),
    std::tuple(&VkPhysicalDeviceVulkan11Features::shaderDrawParameters, "VK_KHR_shader_draw_parameters"),
};
static constexpr auto features_1_2 = std::array{
    std::tuple(&VkPhysicalDeviceVulkan12Features::samplerMirrorClampToEdge, "VK_KHR_sampler_mirror_clamp_to_edge"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::drawIndirectCount, "VK_KHR_draw_indirect_count"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::storageBuffer8BitAccess, "VK_KHR_8bit_storage"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::uniformAndStorageBuffer8BitAccess, "VK_KHR_8bit_storage"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::storagePushConstant8, "VK_KHR_8bit_storage"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderBufferInt64Atomics, "VK_KHR_shader_atomic_int64"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderSharedInt64Atomics, "VK_KHR_shader_atomic_int64"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderFloat16, "VK_KHR_shader_float16_int8"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderInt8, "VK_KHR_shader_float16_int8"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderInputAttachmentArrayDynamicIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderUniformTexelBufferArrayDynamicIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderStorageTexelBufferArrayDynamicIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderUniformBufferArrayNonUniformIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderSampledImageArrayNonUniformIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderStorageBufferArrayNonUniformIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderStorageImageArrayNonUniformIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderInputAttachmentArrayNonUniformIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderUniformTexelBufferArrayNonUniformIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderStorageTexelBufferArrayNonUniformIndexing, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingUniformBufferUpdateAfterBind, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingSampledImageUpdateAfterBind, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingStorageImageUpdateAfterBind, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingStorageBufferUpdateAfterBind, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingUniformTexelBufferUpdateAfterBind, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingStorageTexelBufferUpdateAfterBind, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingUpdateUnusedWhilePending, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingPartiallyBound, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::descriptorBindingVariableDescriptorCount, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::runtimeDescriptorArray, "VK_EXT_descriptor_indexing"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::samplerFilterMinmax, "VK_EXT_sampler_filter_minmax"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::scalarBlockLayout, "VK_EXT_scalar_block_layout"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::imagelessFramebuffer, "VK_KHR_imageless_framebuffer"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::uniformBufferStandardLayout, "VK_KHR_uniform_buffer_standard_layout"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderSubgroupExtendedTypes, "VK_KHR_shader_subgroup_extended_types"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::separateDepthStencilLayouts, "VK_KHR_separate_depth_stencil_layouts"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::hostQueryReset, "VK_EXT_host_query_reset"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::timelineSemaphore, "VK_KHR_timeline_semaphore"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::bufferDeviceAddress, "VK_KHR_buffer_device_address"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::bufferDeviceAddressCaptureReplay, "VK_KHR_buffer_device_address"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::bufferDeviceAddressMultiDevice, "VK_KHR_buffer_device_address"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::vulkanMemoryModel, "VK_KHR_vulkan_memory_model"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::vulkanMemoryModelDeviceScope, "VK_KHR_vulkan_memory_model"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::vulkanMemoryModelAvailabilityVisibilityChains, "VK_KHR_vulkan_memory_model"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderOutputViewportIndex, "VK_EXT_shader_viewport_index_layer"),
    std::tuple(&VkPhysicalDeviceVulkan12Features::shaderOutputLayer, "VK_EXT_shader_viewport_index_layer"),
//    std::tuple(&VkPhysicalDeviceVulkan12Features::subgroupBroadcastDynamicId, "")
};
static constexpr auto features_1_3 = std::array{
    std::tuple(&VkPhysicalDeviceVulkan13Features::robustImageAccess, "VK_EXT_image_robustness"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::inlineUniformBlock, "VK_EXT_inline_uniform_block"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::descriptorBindingInlineUniformBlockUpdateAfterBind, "VK_EXT_inline_uniform_block"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::pipelineCreationCacheControl, "VK_EXT_pipeline_creation_cache_control"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::privateData, "VK_EXT_private_data"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::shaderDemoteToHelperInvocation, "VK_EXT_shader_demote_to_helper_invocation"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::shaderTerminateInvocation, "VK_KHR_shader_terminate_invocation"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::subgroupSizeControl, "VK_EXT_subgroup_size_control"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::computeFullSubgroups, "VK_EXT_subgroup_size_control"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::synchronization2, "VK_KHR_synchronization2"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::textureCompressionASTC_HDR, "VK_EXT_texture_compression_astc_hdr"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::shaderZeroInitializeWorkgroupMemory, "VK_KHR_zero_initialize_workgroup_memory"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::dynamicRendering, "VK_KHR_dynamic_rendering"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::shaderIntegerDotProduct, "VK_KHR_shader_integer_dot_product"),
    std::tuple(&VkPhysicalDeviceVulkan13Features::maintenance4, "VK_KHR_maintenance4")
};

auto query_memory_type_index(GpuDevice* device, u32 memory_type_bits, VkMemoryPropertyFlags flags) -> u32 {
    for (u32 i = 0; i < device->adapter->device_memory_properties.memoryTypeCount; i += 1) {
        if ((memory_type_bits & (1 << i)) == 0) {
            continue;
        }
        if ((device->adapter->device_memory_properties.memoryTypes[i].propertyFlags & flags) != flags) {
            continue;
        }
        return i;
    }
    return std::numeric_limits<u32>::max();
}

auto query_vulkan_struct(VkBaseInStructure const* pStruct, VkStructureType sType) -> VkBaseInStructure const* {
    for (auto* kls = pStruct; kls != nullptr; kls = kls->pNext) {
        if (kls->sType == sType) {
            return kls;
        }
    }
    return nullptr;
}

auto gpu_create_context(PFN_vkGetInstanceProcAddr p_get_proc_addr) -> GpuContext* {
    return new GpuContext(p_get_proc_addr);
}

GpuContext::GpuContext(PFN_vkGetInstanceProcAddr get_proc_addr) : vtable(get_proc_addr) {}

auto GpuContext::enumerate_instance_version(this GpuContext& self) -> u32 {
    u32 version;
    self.vtable.vkEnumerateInstanceVersion(&version);
    return version;
}

auto GpuContext::create_instance(this GpuContext& self, GpuInstanceCreateInfo const& create_info) -> GpuInstance* {
    auto vk_create_info = VkInstanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = create_info.pNext,
        .flags = create_info.flags,
        .pApplicationInfo = create_info.p_application_info,
        .enabledLayerCount = static_cast<u32>(create_info.enabled_layer_names.size()),
        .ppEnabledLayerNames = create_info.enabled_layer_names.data(),
        .enabledExtensionCount = static_cast<u32>(create_info.enabled_extension_names.size()),
        .ppEnabledExtensionNames = create_info.enabled_extension_names.data(),
    };
    return new GpuInstance(&self, &vk_create_info);
}

GpuInstance::GpuInstance(GpuContext* context, VkInstanceCreateInfo const* p_create_info) : context(context->retain()), handle(), adapter_count() {
    this->api_version = p_create_info->pApplicationInfo ? p_create_info->pApplicationInfo->apiVersion : VK_API_VERSION_1_0;
    this->context->vtable.vkCreateInstance(p_create_info, nullptr, &this->handle);
    this->vtable = new InstanceDispatcher(context->vtable.vkGetInstanceProcAddr, this->handle);
    this->vtable->vkEnumeratePhysicalDevices(this->handle, &this->adapter_count, nullptr);
    auto* physical_devices = new VkPhysicalDevice[this->adapter_count];
    this->vtable->vkEnumeratePhysicalDevices(this->handle, &this->adapter_count, physical_devices);
    this->adapters = new GpuAdapter*[this->adapter_count];
    for (u32 i = 0; i < this->adapter_count; i += 1) {
        this->adapters[i] = new GpuAdapter(this, physical_devices[i]);
    }
    delete[] physical_devices;
}

GpuInstance::~GpuInstance() {
    for (u32 i = 0; i < this->adapter_count; i += 1) {
        this->adapters[i]->release();
    }
    this->vtable->vkDestroyInstance(this->handle, nullptr);
    this->context->release();
    delete[] this->adapters;
    delete this->vtable;
}

auto GpuInstance::create_debug_utils(this GpuInstance& self, GpuDebugUtilsCreateInfo const& create_info) -> GpuDebugUtils* {
    auto vk_create_info = VkDebugUtilsMessengerCreateInfoEXT{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = {},
        .flags = create_info.flags,
        .messageSeverity = create_info.severity,
        .messageType = create_info.type,
        .pfnUserCallback = create_info.callback,
        .pUserData = create_info.p_user_data,
    };
    return new GpuDebugUtils(&self, &vk_create_info);
}

auto GpuInstance::surface_from_handle(this GpuInstance& self, VkSurfaceKHR surface) -> GpuSurface* {
    return new GpuSurface(&self, surface);
}

auto GpuInstance::enumerate_adapters(this GpuInstance const& self) -> std::span<GpuAdapter*> {
    return std::span(self.adapters, self.adapter_count);
}

GpuDebugUtils::GpuDebugUtils(GpuInstance* instance, VkDebugUtilsMessengerCreateInfoEXT const* p_create_info) : instance(instance->retain()), handle() {
    this->instance->vtable->vkCreateDebugUtilsMessengerEXT(this->instance->handle, p_create_info, nullptr, &this->handle);
}

GpuDebugUtils::~GpuDebugUtils() {
    this->instance->vtable->vkDestroyDebugUtilsMessengerEXT(this->instance->handle, this->handle, nullptr);
    this->instance->release();
}

GpuSurface::GpuSurface(GpuInstance* instance, VkSurfaceKHR handle) : instance(instance->retain()), handle(handle) {}

GpuSurface::~GpuSurface() {
    this->instance->vtable->vkDestroySurfaceKHR(this->instance->handle, this->handle, nullptr);
    this->instance->release();
}

GpuAdapter::GpuAdapter(GpuInstance* instance, VkPhysicalDevice handle) : instance(instance), handle(handle), queue_family_count() {
    this->instance->vtable->vkGetPhysicalDeviceQueueFamilyProperties(this->handle, &this->queue_family_count, nullptr);
    this->queue_family_properties = new VkQueueFamilyProperties[this->queue_family_count];
    this->instance->vtable->vkGetPhysicalDeviceQueueFamilyProperties(this->handle, &this->queue_family_count, this->queue_family_properties);
    this->instance->vtable->vkGetPhysicalDeviceMemoryProperties(this->handle, &this->device_memory_properties);
    this->instance->vtable->vkGetPhysicalDeviceFeatures(this->handle, &this->core_0_1);
}

GpuAdapter::~GpuAdapter() {
    delete[] this->queue_family_properties;
}

#include <set>

auto GpuAdapter::create_device(this GpuAdapter& self, GpuDeviceCreateInfo const& create_info) -> GpuDevice* {
    std::set<const char*> required_extensions = {};

    u32 device_extension_property_count;
    self.instance->vtable->vkEnumerateDeviceExtensionProperties(self.handle, nullptr, &device_extension_property_count, nullptr);
    auto* device_extension_properties = new VkExtensionProperties[device_extension_property_count];
    self.instance->vtable->vkEnumerateDeviceExtensionProperties(self.handle, nullptr, &device_extension_property_count, device_extension_properties);
    for (u32 i = 0; i < device_extension_property_count; i += 1) {
        if (strcmp(device_extension_properties[i].extensionName, "VK_KHR_portability_subset") == 0) {
            required_extensions.emplace("VK_KHR_portability_subset");
        }
    }
    delete[] device_extension_properties;

    auto core_1_3 = VkPhysicalDeviceVulkan13Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = {}
    };
    auto core_1_2 = VkPhysicalDeviceVulkan12Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = &core_1_3
    };
    auto core_1_1 = VkPhysicalDeviceVulkan11Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .pNext = &core_1_2
    };
    auto features = VkPhysicalDeviceFeatures2{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &core_1_1,
        .features = {}
    };

    self.instance->vtable->vkGetPhysicalDeviceFeatures2(self.handle, &features);

    auto* impl_1_1 = (VkPhysicalDeviceVulkan11Features*)query_vulkan_struct((VkBaseInStructure const*)create_info.pNext, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES);
    auto* impl_1_2 = (VkPhysicalDeviceVulkan12Features*)query_vulkan_struct((VkBaseInStructure const*)create_info.pNext, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES);
    auto* impl_1_3 = (VkPhysicalDeviceVulkan13Features*)query_vulkan_struct((VkBaseInStructure const*)create_info.pNext, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES);

    auto major = VK_API_VERSION_MAJOR(self.instance->api_version);
    auto minor = VK_API_VERSION_MINOR(self.instance->api_version);
    auto patch = VK_API_VERSION_PATCH(self.instance->api_version);

    if (impl_1_1 && (major <= 1 && minor < 1)) {
        for (auto& [property, extension] : features_1_1) {
            if (impl_1_1->*property == VK_TRUE) {
                required_extensions.emplace(extension);
            }
        }
    }
    if (impl_1_2 && (major <= 1 && minor < 2)) {
        for (auto& [property, extension] : features_1_2) {
            if (impl_1_2->*property == VK_TRUE) {
                required_extensions.emplace(extension);
            }
        }
    }
    if (impl_1_3 && (major <= 1 && minor < 3)) {
        for (auto& [property, extension] : features_1_3) {
            if (impl_1_3->*property == VK_TRUE) {
                required_extensions.emplace(extension);
            }
        }
    }

    std::vector<const char*> extensions;
    extensions.append_range(required_extensions);
    extensions.append_range(create_info.enabled_extension_names);

    auto vk_create_info = VkDeviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = create_info.pNext,
        .flags = create_info.flags,
        .queueCreateInfoCount = static_cast<u32>(create_info.queue_create_infos.size()),
        .pQueueCreateInfos = create_info.queue_create_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = {},
        .enabledExtensionCount = static_cast<u32>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
        .pEnabledFeatures = {}
    };
    return new GpuDevice(&self, &vk_create_info);
}

auto GpuAdapter::get_queue_family_properties(this GpuAdapter const& self) -> std::span<VkQueueFamilyProperties const> {
    return std::span(self.queue_family_properties, self.queue_family_count);
}

GpuDevice::GpuDevice(GpuAdapter* adapter, VkDeviceCreateInfo const* p_create_info) : adapter(adapter->retain()), handle() {
    this->adapter->instance->vtable->vkCreateDevice(this->adapter->handle, p_create_info, nullptr, &this->handle);
    this->vtable = new DeviceDispatcher(this->adapter->instance->vtable->vkGetDeviceProcAddr, this->handle);
}

GpuDevice::~GpuDevice() {
    this->vtable->vkDestroyDevice(this->handle, nullptr);
    this->adapter->release();
    delete this->vtable;
}

void GpuDevice::wait_idle(this GpuDevice& self) {
    self.vtable->vkDeviceWaitIdle(self.handle);
}

auto GpuDevice::wait_for_fences(this GpuDevice& self, u32 fence_count, VkFence const* p_fences, bool wait_all, u64 timeout) -> VkResult {
    return self.vtable->vkWaitForFences(
        self.handle,
        fence_count,
        p_fences,
        wait_all ? VK_TRUE : VK_FALSE,
        timeout
    );
}

auto GpuDevice::reset_fences(this GpuDevice& self, u32 fence_count, VkFence const* p_fences) -> VkResult {
    return self.vtable->vkResetFences(self.handle, fence_count, p_fences);
}

auto GpuDevice::map_memory(this GpuDevice& self, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** pp_data) -> VkResult {
    return self.vtable->vkMapMemory(self.handle, memory, offset, size, flags, pp_data);
}

void GpuDevice::unmap_memory(this GpuDevice& self, VkDeviceMemory memory) {
    return self.vtable->vkUnmapMemory(self.handle, memory);
}

auto GpuDevice::create_semaphore(this GpuDevice& self, VkSemaphore* p_semaphore) -> VkResult {
    return self.vtable->vkCreateSemaphore(
        self.handle,
        (VkSemaphoreCreateInfo[]){{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = (VkSemaphoreTypeCreateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
                .pNext = {},
                .semaphoreType = VK_SEMAPHORE_TYPE_BINARY,
                .initialValue = 0
            }}
        }},
        nullptr,
        p_semaphore
    );
}

auto GpuDevice::create_timeline_semaphore(this GpuDevice& self, u64 initial_value, VkSemaphore* p_semaphore) -> VkResult {
    return self.vtable->vkCreateSemaphore(
        self.handle,
        (VkSemaphoreCreateInfo[]){{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = (VkSemaphoreTypeCreateInfo[]){{
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
                .pNext = {},
                .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
                .initialValue = initial_value
            }}
        }},
        nullptr,
        p_semaphore
    );
}

void GpuDevice::delete_semaphore(this GpuDevice& self, VkSemaphore semaphore) {
    self.vtable->vkDestroySemaphore(self.handle, semaphore, nullptr);
}

auto GpuDevice::create_fence(this GpuDevice& self, bool signaled, VkFence* p_fence) -> VkResult {
    auto flags = VkFenceCreateFlags();
    if (signaled) {
        flags |= VK_FENCE_CREATE_SIGNALED_BIT;
    }
    return self.vtable->vkCreateFence(
        self.handle,
        (VkFenceCreateInfo[]){{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = flags
        }},
        nullptr,
        p_fence
    );
}

void GpuDevice::delete_fence(this GpuDevice& self, VkFence fence) {
    self.vtable->vkDestroyFence(self.handle, fence, nullptr);
}

auto GpuDevice::create_image(this GpuDevice& self, GpuImageCreateInfo const& create_info) -> GpuImage* {
    auto vk_create_info = VkImageCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = {},
        .flags = create_info.flags,
        .imageType = create_info.type,
        .format = create_info.format,
        .extent = create_info.extent,
        .mipLevels = create_info.mipLevels,
        .arrayLayers = create_info.arrayLayers,
        .samples = create_info.samples,
        .tiling = create_info.tiling,
        .usage = create_info.usage,
        .sharingMode = create_info.sharingMode,
        .queueFamilyIndexCount = static_cast<u32>(create_info.queueFamilyIndices.size()),
        .pQueueFamilyIndices = create_info.queueFamilyIndices.data(),
        .initialLayout = create_info.initialLayout,
    };
    return new GpuImage(&self, &vk_create_info);
}

//(VkImageViewCreateInfo[]){{
//.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
//.pNext = {},
//.flags = {},
//.image = compute_image->handle,
//.viewType = VK_IMAGE_VIEW_TYPE_2D,
//.format = VK_FORMAT_R8G8B8A8_UNORM,
//.components = VkComponentMapping{
//    .r = VK_COMPONENT_SWIZZLE_R,
//    .g = VK_COMPONENT_SWIZZLE_G,
//    .b = VK_COMPONENT_SWIZZLE_B,
//    .a = VK_COMPONENT_SWIZZLE_A,
//},
//.subresourceRange = VkImageSubresourceRange{
//    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
//    .baseMipLevel = 0,
//    .levelCount = 1,
//    .baseArrayLayer = 0,
//    .layerCount = 1
//}
//}}

auto GpuDevice::create_image_view(this GpuDevice& self, VkImageViewCreateInfo const* p_create_info, VkImageView* pImageView) -> VkResult {
    return self.vtable->vkCreateImageView(self.handle, p_create_info, nullptr, pImageView);
}

void GpuDevice::delete_image_view(this GpuDevice& self, VkImageView imageView) {
    self.vtable->vkDestroyImageView(self.handle, imageView, nullptr);
}

auto GpuDevice::create_buffer(this GpuDevice& self, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer* p_buffer) -> VkResult {
    return self.vtable->vkCreateBuffer(
        self.handle,
        (VkBufferCreateInfo[]) {{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = size,
            .usage = usage
        }},
        nullptr,
        p_buffer
    );
}

void GpuDevice::delete_buffer(this GpuDevice& self, VkBuffer buffer) {
    self.vtable->vkDestroyBuffer(self.handle, buffer, nullptr);
}

auto GpuDevice::get_device_address(this GpuDevice& self, VkBuffer buffer) -> VkDeviceAddress {
    return self.vtable->vkGetBufferDeviceAddress(
        self.handle,
        (VkBufferDeviceAddressInfo[]) {{
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = buffer
        }}
    );
}

auto GpuDevice::create_graphics_pipelines(this GpuDevice& self, VkPipelineCache pipeline_cache, std::span<VkGraphicsPipelineCreateInfo const> p_create_infos, VkPipeline* p_pipelines) -> VkResult {
    return self.vtable->vkCreateGraphicsPipelines(
        self.handle,
        pipeline_cache,
        p_create_infos.size(),
        p_create_infos.data(),
        nullptr,
        p_pipelines
    );
}

void GpuDevice::delete_pipeline(this GpuDevice& self, VkPipeline pipeline) {
    self.vtable->vkDestroyPipeline(self.handle, pipeline, nullptr);
}

auto GpuDevice::create_pipeline_layout(this GpuDevice& self, GpuPipelineLayoutCreateInfo const& create_info) -> VkPipelineLayout {
    auto vk_create_info = VkPipelineLayoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = {},
        .flags = create_info.flags,
        .setLayoutCount = static_cast<u32>(create_info.bind_group_layouts.size()),
        .pSetLayouts = create_info.bind_group_layouts.data(),
        .pushConstantRangeCount = static_cast<u32>(create_info.push_constant_ranges.size()),
        .pPushConstantRanges = create_info.push_constant_ranges.data(),
    };
    VkPipelineLayout handle;
    self.vtable->vkCreatePipelineLayout(self.handle, &vk_create_info, nullptr, &handle);
    return handle;
}

void GpuDevice::delete_pipeline_layout(this GpuDevice& self, VkPipelineLayout pipeline_layout) {
    self.vtable->vkDestroyPipelineLayout(self.handle, pipeline_layout, nullptr);
}

auto GpuDevice::create_bind_group_pool(this GpuDevice& self, u32 max_sets, std::span<VkDescriptorPoolSize const> pool_sizes, VkDescriptorPool* p_bind_group_pool) -> VkResult {
    return self.vtable->vkCreateDescriptorPool(
        self.handle,
        (VkDescriptorPoolCreateInfo[]){{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets = max_sets,
            .poolSizeCount = u32(pool_sizes.size()),
            .pPoolSizes = pool_sizes.data()
        }},
        nullptr,
        p_bind_group_pool
    );
}

void GpuDevice::delete_bind_group_pool(this GpuDevice& self, VkDescriptorPool bind_group_pool) {
    self.vtable->vkDestroyDescriptorPool(self.handle, bind_group_pool, nullptr);
}

auto GpuDevice::create_sampler(this GpuDevice& self, VkSamplerCreateInfo const* p_create_info, VkSampler* p_sampler) -> VkResult {
    return self.vtable->vkCreateSampler(self.handle, p_create_info, nullptr, p_sampler);
}

void GpuDevice::delete_sampler(this GpuDevice& self, VkSampler sampler) {
    self.vtable->vkDestroySampler(self.handle, sampler, nullptr);
}

auto GpuDevice::allocate_bind_groups(this GpuDevice& self, VkDescriptorPool bind_group_pool, std::span<VkDescriptorSetLayout const> layouts, VkDescriptorSet* p_bind_groups) -> VkResult {
    return self.vtable->vkAllocateDescriptorSets(
        self.handle,
        (VkDescriptorSetAllocateInfo[]) {{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = bind_group_pool,
            .descriptorSetCount = u32(layouts.size()),
            .pSetLayouts = layouts.data()
        }},
        p_bind_groups
    );
}

void GpuDevice::update_bind_groups(this GpuDevice& self, std::span<VkWriteDescriptorSet const> write_bind_groups, std::span<VkCopyDescriptorSet const> copy_bind_groups) {
    self.vtable->vkUpdateDescriptorSets(
        self.handle,
        write_bind_groups.size(),
        write_bind_groups.data(),
        copy_bind_groups.size(),
        copy_bind_groups.data()
    );
}

auto GpuDevice::create_command_pool(this GpuDevice& self, VkCommandPoolCreateFlags flags, u32 queue_family_index, VkCommandPool* p_command_pool) -> VkResult {
    return self.vtable->vkCreateCommandPool(
        self.handle,
        (VkCommandPoolCreateInfo[]){{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = flags,
            .queueFamilyIndex = queue_family_index
        }},
        nullptr,
        p_command_pool
    );
}

void GpuDevice::delete_command_pool(this GpuDevice& self, VkCommandPool command_pool) {
    self.vtable->vkDestroyCommandPool(self.handle, command_pool, nullptr);
}

auto GpuDevice::reset_command_pool(this GpuDevice& self, VkCommandPool command_pool, VkCommandPoolResetFlags flags) -> VkResult {
    return self.vtable->vkResetCommandPool(self.handle, command_pool, flags);
}

auto GpuDevice::create_command_buffers(this GpuDevice& self, VkCommandPool command_pool, VkCommandBufferLevel level, u32 command_buffer_count, VkCommandBuffer* p_command_buffers) -> VkResult {
    return self.vtable->vkAllocateCommandBuffers(
        self.handle,
        (VkCommandBufferAllocateInfo[]){{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = command_pool,
            .level = level,
            .commandBufferCount = command_buffer_count,
        }},
        p_command_buffers
    );
}

void GpuDevice::delete_command_buffer(this GpuDevice& self, VkCommandPool command_pool, u32 command_buffer_count, VkCommandBuffer* p_command_buffers) {
    self.vtable->vkFreeCommandBuffers(self.handle, command_pool, command_buffer_count, p_command_buffers);
}

auto GpuDevice::reset_bind_group_pool(this GpuDevice& self, VkDescriptorPool bind_group_pool, VkDescriptorPoolResetFlags flags) -> VkResult {
    return self.vtable->vkResetDescriptorPool(self.handle, bind_group_pool, flags);
}

auto GpuDevice::create_memory(this GpuDevice& self, VkMemoryRequirements requirements, VkMemoryPropertyFlags flags, VkDeviceMemory* p_device_memory) -> VkResult {
    auto memory_type_index = query_memory_type_index(&self, requirements.memoryTypeBits, flags);
    return self.vtable->vkAllocateMemory(
        self.handle,
        (VkMemoryAllocateInfo[]) {{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = (VkMemoryAllocateFlagsInfo[]) {{
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
                .flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
            }},
            .allocationSize = requirements.size,
            .memoryTypeIndex = memory_type_index
        }},
        nullptr,
        p_device_memory
    );
}

void GpuDevice::delete_memory(this GpuDevice& self, VkDeviceMemory device_memory) {
    self.vtable->vkFreeMemory(self.handle, device_memory, nullptr);
}

auto GpuDevice::acquire_next_image(this GpuDevice& self, VkSwapchainKHR swapchain, u64 timeout, VkSemaphore semaphore, VkFence fence, u32 device_mask, u32* p_image_index) -> VkResult {
    return self.vtable->vkAcquireNextImage2KHR(
        self.handle,
        (VkAcquireNextImageInfoKHR[]) {{
            .sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR,
            .swapchain = swapchain,
            .timeout = timeout,
            .semaphore = semaphore,
            .fence = fence,
            .deviceMask = device_mask
        }},
        p_image_index
    );
}

auto GpuDevice::get_image_memory_requirements(this GpuDevice& self, VkImage image) -> VkMemoryRequirements {
    VkMemoryRequirements requirements;
    self.vtable->vkGetImageMemoryRequirements(self.handle, image, &requirements);
    return requirements;
}

auto GpuDevice::get_buffer_memory_requirements(this GpuDevice& self, VkBuffer buffer) -> VkMemoryRequirements {
    VkMemoryRequirements requirements;
    self.vtable->vkGetBufferMemoryRequirements(self.handle, buffer, &requirements);
    return requirements;
}

auto GpuDevice::create_swapchain(this GpuDevice& self, GpuSwapchainCreateInfo const& create_info) -> GpuSwapchain* {
    auto vk_create_info = VkSwapchainCreateInfoKHR{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = {},
        .flags = create_info.flags,
        .surface = create_info.surface->handle,
        .minImageCount = create_info.min_image_count,
        .imageFormat = create_info.image_format,
        .imageColorSpace = create_info.image_color_space,
        .imageExtent = create_info.image_extent,
        .imageArrayLayers = create_info.image_array_layers,
        .imageUsage = create_info.image_usage,
        .imageSharingMode = create_info.image_sharing_mode,
        .queueFamilyIndexCount = static_cast<u32>(create_info.queue_family_indices.size()),
        .pQueueFamilyIndices = create_info.queue_family_indices.data(),
        .preTransform = create_info.pre_transform,
        .compositeAlpha = create_info.composite_alpha,
        .presentMode = create_info.present_mode,
        .clipped = create_info.clipped,
        .oldSwapchain = create_info.old_swapchain,
    };
    return new GpuSwapchain(&self, &vk_create_info);
}

auto GpuDevice::create_shader_module(this GpuDevice& self, GpuShaderModuleCreateInfo const& create_info) -> GpuShaderModule* {
    auto vk_create_info = VkShaderModuleCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = {},
        .flags = create_info.flags,
        .codeSize = create_info.size,
        .pCode = std::bit_cast<u32 const*>(create_info.data),
    };
    return new GpuShaderModule(&self, &vk_create_info);
}

auto GpuDevice::create_bind_group_layout(this GpuDevice& self, GpuDescriptorSetLayoutCreateInfo const& create_info) -> VkDescriptorSetLayout {
    auto vk_create_info = VkDescriptorSetLayoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = {},
        .flags = create_info.flags,
        .bindingCount = static_cast<u32>(create_info.bindings.size()),
        .pBindings = create_info.bindings.data()
    };

    VkDescriptorSetLayout handle;
    self.vtable->vkCreateDescriptorSetLayout(self.handle, &vk_create_info, nullptr, &handle);
    return handle;
}

void GpuDevice::delete_bind_group_layout(this GpuDevice& self, VkDescriptorSetLayout bind_group_layout) {
    self.vtable->vkDestroyDescriptorSetLayout(self.handle, bind_group_layout, nullptr);
}

auto GpuDevice::create_compute_pipeline(this GpuDevice& self, GpuComputePipelineCreateInfo const& create_info) -> GpuComputePipeline* {
    auto vk_specialization_info = VkSpecializationInfo{
        .mapEntryCount = static_cast<u32>(create_info.stage.specialization.entries.size()),
        .pMapEntries = create_info.stage.specialization.entries.data(),
        .dataSize = create_info.stage.specialization.size,
        .pData = create_info.stage.specialization.data
    };
    auto vk_create_info = VkComputePipelineCreateInfo{
        .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .pNext = {},
        .flags = create_info.flags,
        .stage = VkPipelineShaderStageCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = {},
            .flags = create_info.stage.flags,
            .stage = VK_SHADER_STAGE_COMPUTE_BIT,
            .module = create_info.stage.module,
            .pName = create_info.stage.name,
            .pSpecializationInfo = &vk_specialization_info,
        },
        .layout = create_info.layout,
        .basePipelineHandle = {}, // create_info.base_pipeline_handle,
        .basePipelineIndex = {}, // create_info.base_pipeline_index
    };
    return new GpuComputePipeline(&self, &vk_create_info);
}

GpuImage::GpuImage(GpuDevice* device, VkImageCreateInfo const* p_create_info) : device(device), handle() {
    this->device->vtable->vkCreateImage(this->device->handle, p_create_info, nullptr, &this->handle);
}

GpuImage::~GpuImage() {
    for (auto& [_, image_view] : this->views) {
        this->device->vtable->vkDestroyImageView(this->device->handle, image_view, nullptr);
    }

    this->device->vtable->vkDestroyImage(this->device->handle, this->handle, nullptr);
    this->device->release();
}

auto GpuImage::subresource(this GpuImage& self, GpuImageSubresource const& info) -> VkImageView {
    for (auto& [info_, image_view] : self.views) {
        if (std::memcmp(&info_, &info, sizeof(GpuImageSubresource)) == 0) {
            return image_view;
        }
    }

    auto vk_create_info = VkImageViewCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = {},
        .flags = info.flags,
        .image = self.handle,
        .viewType = info.type,
        .format = info.format,
        .components = info.components,
        .subresourceRange = info.subresourceRange
    };

    VkImageView image_view;
    self.device->vtable->vkCreateImageView(self.device->handle, &vk_create_info, nullptr, &image_view);
    self.views.emplace_back(info, image_view);
    return image_view;
}

GpuSwapchain::GpuSwapchain(GpuDevice* device, const VkSwapchainCreateInfoKHR* p_create_info) : device(device->retain()), handle(), image_count() {
    this->device->vtable->vkCreateSwapchainKHR(this->device->handle, p_create_info, nullptr, &this->handle);
    this->device->vtable->vkGetSwapchainImagesKHR(this->device->handle, this->handle, &this->image_count, nullptr);
    this->images = new VkImage[this->image_count];
    this->device->vtable->vkGetSwapchainImagesKHR(this->device->handle, this->handle, &this->image_count, this->images);
}

GpuSwapchain::~GpuSwapchain() {
    this->device->vtable->vkDestroySwapchainKHR(this->device->handle, this->handle, nullptr);
    this->device->release();
    delete[] this->images;
}

GpuShaderModule::GpuShaderModule(GpuDevice* device, VkShaderModuleCreateInfo const* p_create_info) : device(device->retain()) {
    this->device->vtable->vkCreateShaderModule(this->device->handle, p_create_info, nullptr, &this->handle);
}

GpuShaderModule::~GpuShaderModule() {
    this->device->vtable->vkDestroyShaderModule(this->device->handle, this->handle, nullptr);
    this->device->release();
}

GpuComputePipeline::GpuComputePipeline(GpuDevice* device, VkComputePipelineCreateInfo const* p_create_info) : device(device->retain()) {
    this->device->vtable->vkCreateComputePipelines(this->device->handle, nullptr, 1, p_create_info, nullptr, &this->handle);
}

GpuComputePipeline::~GpuComputePipeline() {
    this->device->vtable->vkDestroyPipeline(this->device->handle, this->handle, nullptr);
    this->device->release();
}