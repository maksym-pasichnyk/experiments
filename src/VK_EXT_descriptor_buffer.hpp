////
//// Created by Maksym Pasichnyk on 30.01.2024.
////
//#pragma once
//
//#include "stl/pch.hpp"
//#include "gpu/core.hpp"
//
//struct GpuDescriptorSetLayout {
//    VkDeviceSize layout_size_in_bytes;
//    std::vector<VkDescriptorSetLayoutBinding> bindings;
//};
//
//struct GpuDescriptorSet {
//    VkDescriptorType type;
//    union {
//        VkSampler sampler;
//        VkDescriptorImageInfo image_info;
//        VkDescriptorAddressInfoEXT address_info;
//    };
//};
//
//struct GpuPipelineLayout {
//    std::vector<VkDescriptorSetLayout> layouts;
//};
//
//struct GpuDescriptorBufferBindingInfoEXT {
//    VkDeviceAddress       address;
//    VkBufferUsageFlags    usage;
//};
//
//struct GpuCommandBuffer {
//    VkDevice LogicalDevice;
//    VkDescriptorPool DescriptorPool;
//    std::vector<GpuDescriptorBufferBindingInfoEXT> BindingInfos;
//};
//
//static std::unordered_map<VkDescriptorSetLayout, GpuDescriptorSetLayout*> mappings1;
//static std::unordered_map<VkPipelineLayout, GpuPipelineLayout*> mappings2;
//static std::unordered_map<VkCommandBuffer, GpuCommandBuffer*> mappings3;
//
////VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) -> VkResult {
//    auto result = vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
//    if (result == VK_SUCCESS) {
//        for (u32 i = 0; i < pAllocateInfo->commandBufferCount; i += 1) {
//            auto* gpu_command_buffer = new GpuCommandBuffer();
//            gpu_command_buffer->LogicalDevice = device;
//            vkCreateDescriptorPool(
//                device,
//                (VkDescriptorPoolCreateInfo[]){{
//                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
//                    .pNext = {},
//                    .flags = {},
//                    .maxSets = 1000,
//                    .poolSizeCount = 1,
//                    .pPoolSizes = (VkDescriptorPoolSize[]) {
//                        VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000)
//                    }
//                }},
//                nullptr,
//                &gpu_command_buffer->DescriptorPool
//            );
//            mappings3.insert_or_assign(pCommandBuffers[i], gpu_command_buffer);
//        }
//    }
//    return result;
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
//    for (u32 i = 0; i < commandBufferCount; i += 1) {
//        if (mappings3.contains(pCommandBuffers[i])) {
//            vkDestroyDescriptorPool(device, mappings3.at(pCommandBuffers[i])->DescriptorPool, nullptr);
//            delete mappings3.at(pCommandBuffers[i]);
//            mappings3.erase(pCommandBuffers[i]);
//        }
//    }
//    vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
//}
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) -> VkResult {
//    auto* p_command_buffer = mappings3.at(commandBuffer);
//    vkResetDescriptorPool(p_command_buffer->LogicalDevice, p_command_buffer->DescriptorPool, {});
//    return vkBeginCommandBuffer(commandBuffer, pBeginInfo);
//}
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) -> VkResult {
//    auto result = vkCreateDescriptorSetLayout(
//        device,
//        (VkDescriptorSetLayoutCreateInfo[]) {{
//            .sType = pCreateInfo->sType,
//            .pNext = pCreateInfo->pNext,
//            .flags = pCreateInfo->flags ^ (pCreateInfo->flags & VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT),
//            .bindingCount = pCreateInfo->bindingCount,
//            .pBindings = pCreateInfo->pBindings
//        }},
//        pAllocator,
//        pSetLayout
//    );
//    if (result == VK_SUCCESS && (pCreateInfo->flags & VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT)) {
//        VkDeviceSize layout_size_in_bytes = 0;
//        for (u32 i = 0; i < pCreateInfo->bindingCount; i += 1) {
//            layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(GpuDescriptorSet);
////            switch (pCreateInfo->pBindings[i].descriptorType) {
////            case VK_DESCRIPTOR_TYPE_SAMPLER:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkSampler);
////                break;
////            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorImageInfo);
////                break;
////            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorImageInfo);
////                break;
////            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorImageInfo);
////                break;
////            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorAddressInfoEXT);
////                break;
////            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorAddressInfoEXT);
////                break;
////            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorAddressInfoEXT);
////                break;
////            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorAddressInfoEXT);
////                break;
////            case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
////                layout_size_in_bytes += pCreateInfo->pBindings[i].descriptorCount * sizeof(VkDescriptorImageInfo);
////                break;
////            default:
////                std::abort();
////            }
//        }
//        auto bindings = std::vector(pCreateInfo->pBindings, pCreateInfo->pBindings + pCreateInfo->bindingCount);
//        mappings1.insert_or_assign(*pSetLayout, new GpuDescriptorSetLayout(layout_size_in_bytes, std::move(bindings)));
//    }
//    return result;
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) {
//    if (mappings1.contains(descriptorSetLayout)) {
//        delete mappings1.at(descriptorSetLayout);
//        mappings1.erase(descriptorSetLayout);
//    }
//    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
//}
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) -> VkResult {
//    auto result = vkCreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
//    if (result == VK_SUCCESS) {
//        auto* gpu_pipeline_layout = new GpuPipelineLayout();
//        gpu_pipeline_layout->layouts.assign(pCreateInfo->pSetLayouts, pCreateInfo->pSetLayouts + pCreateInfo->setLayoutCount);
//        mappings2.insert_or_assign(*pPipelineLayout, gpu_pipeline_layout);
//    }
//    return result;
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) {
//    if (mappings2.contains(pipelineLayout)) {
//        delete mappings2.at(pipelineLayout);
//        mappings2.erase(pipelineLayout);
//    }
//    vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkCmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) {
//    auto* p_command_buffer = mappings3.at(commandBuffer);
//    p_command_buffer->BindingInfos.resize(bufferCount);
//    for (u32 i = 0; i < bufferCount; i += 1) {
//        p_command_buffer->BindingInfos[i].address = pBindingInfos[i].address;
//        p_command_buffer->BindingInfos[i].usage = pBindingInfos[i].usage;
//    }
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkCmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) {
//    auto* p_command_buffer = mappings3.at(commandBuffer);
//    auto* p_pipeline_layout = mappings2.at(layout);
//    auto sets = std::make_unique_for_overwrite<VkDescriptorSet[]>(setCount);
//    vkAllocateDescriptorSets(
//        p_command_buffer->LogicalDevice,
//        (VkDescriptorSetAllocateInfo[]) {{
//            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//            .pNext = {},
//            .descriptorPool = p_command_buffer->DescriptorPool,
//            .descriptorSetCount = setCount,
//            .pSetLayouts = &p_pipeline_layout->layouts[firstSet]
//        }},
//        &sets[0]
//    );
//
//    std::vector<VkWriteDescriptorSet> writes;
//    for (u32 i = 0; i < setCount; i += 1) {
//        auto* p_descriptor_layout = mappings1.at(p_pipeline_layout->layouts[firstSet + i]);
//        for (u32 binding_index = 0; binding_index < p_descriptor_layout->bindings.size(); binding_index += 1) {
//            std::vector<VkBufferView> buffer_views;
//            std::vector<VkDescriptorImageInfo> image_infos;
//            std::vector<VkDescriptorBufferInfo> buffer_infos;
//
//            switch (p_descriptor_layout->bindings[i].descriptorType) {
//            case VK_DESCRIPTOR_TYPE_SAMPLER:
//                image_infos.emplace_back(VkDescriptorImageInfo{
//                    .sampler = {},
//                    .imageView = {},
//                    .imageLayout = VK_IMAGE_LAYOUT_GENERAL
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
//                image_infos.emplace_back(VkDescriptorImageInfo{
//                    .sampler = {},
//                    .imageView = {},
//                    .imageLayout = VK_IMAGE_LAYOUT_GENERAL
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
//                image_infos.emplace_back(VkDescriptorImageInfo{
//                    .sampler = {},
//                    .imageView = {},
//                    .imageLayout = VK_IMAGE_LAYOUT_GENERAL
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
//                image_infos.emplace_back(VkDescriptorImageInfo{
//                    .sampler = {},
//                    .imageView = {},
//                    .imageLayout = VK_IMAGE_LAYOUT_GENERAL
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
//                buffer_infos.emplace_back(VkDescriptorBufferInfo{
//                    .buffer = {},
//                    .offset = {},
//                    .range = VK_WHOLE_SIZE,
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
//                buffer_infos.emplace_back(VkDescriptorBufferInfo{
//                    .buffer = {},
//                    .offset = {},
//                    .range = VK_WHOLE_SIZE,
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
//                buffer_infos.emplace_back(VkDescriptorBufferInfo{
//                    .buffer = {},
//                    .offset = {},
//                    .range = VK_WHOLE_SIZE,
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
//                buffer_infos.emplace_back(VkDescriptorBufferInfo{
//                    .buffer = {},
//                    .offset = {},
//                    .range = VK_WHOLE_SIZE,
//                });
//                break;
//            case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
//                image_infos.emplace_back(VkDescriptorImageInfo{
//                    .sampler = {},
//                    .imageView = {},
//                    .imageLayout = VK_IMAGE_LAYOUT_GENERAL
//                });
//                break;
//            default:
//                std::abort();
//            }
//            writes.emplace_back(VkWriteDescriptorSet{
//                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
//                .pNext = {},
//                .dstSet = sets[i],
//                .dstBinding = p_descriptor_layout->bindings[i].binding,
//                .dstArrayElement = 0,
//                .descriptorCount = p_descriptor_layout->bindings[i].descriptorCount,
//                .descriptorType = p_descriptor_layout->bindings[i].descriptorType,
//                .pImageInfo = image_infos.empty() ? nullptr : image_infos.data(),
//                .pBufferInfo = buffer_infos.empty() ? nullptr : buffer_infos.data(),
//                .pTexelBufferView = buffer_views.empty() ? nullptr : buffer_views.data(),
//            });
//        }
//    }
//
//    vkUpdateDescriptorSets(p_command_buffer->LogicalDevice, writes.size(), &writes[0], 0, (VkCopyDescriptorSet[]) {});
//    vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, &sets[0], 0, {});
//}
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkGetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) -> VkResult {
//    return VK_SUCCESS;
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkGetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) {
//    switch (pDescriptorInfo->type) {
//    case VK_DESCRIPTOR_TYPE_SAMPLER:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .sampler = *pDescriptorInfo->data.pSampler}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .image_info = *pDescriptorInfo->data.pCombinedImageSampler}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .image_info = *pDescriptorInfo->data.pSampledImage}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .image_info = *pDescriptorInfo->data.pStorageImage}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .address_info = *pDescriptorInfo->data.pUniformTexelBuffer}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .address_info = *pDescriptorInfo->data.pStorageTexelBuffer}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .address_info = *pDescriptorInfo->data.pUniformBuffer}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .address_info = *pDescriptorInfo->data.pStorageBuffer}}, sizeof(GpuDescriptorSet));
//        break;
//    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
//        std::memcpy(pDescriptor, (GpuDescriptorSet[]) {{.type = pDescriptorInfo->type, .image_info = *pDescriptorInfo->data.pInputAttachmentImage}}, sizeof(GpuDescriptorSet));
//        break;
//    default:
//        std::abort();
//    }
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkGetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) {
//}
//VKAPI_ATTR void VKAPI_CALL VK_EXT_descriptor_buffer_vkGetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) {
//    *pLayoutSizeInBytes = mappings1.at(layout)->layout_size_in_bytes;
//}
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkGetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) -> VkResult {
//    return VK_SUCCESS;
//}
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkGetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) -> VkResult {
//    return VK_SUCCESS;
//}
//VKAPI_ATTR auto VKAPI_CALL VK_EXT_descriptor_buffer_vkGetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) -> VkResult {
//    return VK_SUCCESS;
//}
