//
// Created by Maksym Pasichnyk on 22.01.2024.
//
#pragma once

#include "stl/pch.hpp"

struct FreeListBlock {
    size_t size;
    size_t offs;
    size_t next;
};

struct FreeList {
    size_t total_size;
    size_t max_entries;
    size_t head_ref;
    FreeListBlock blk[];
};

static auto free_list_create(size_t size) -> FreeList* {
    auto max_entries = size / sizeof(FreeListBlock);
    auto requirement = sizeof(FreeList) + sizeof(FreeListBlock) * max_entries;

    auto* mem = ::operator new(requirement, std::align_val_t(alignof(FreeList)));
    auto* free_list = std::construct_at(static_cast<FreeList*>(mem), FreeList{
        .total_size = size,
        .max_entries = max_entries,
        .head_ref = 0zu
    });
    std::construct_at(&free_list->blk[0], FreeListBlock{
        .size = size,
        .offs = 0zu,
        .next = 0zu,
    });
    for (size_t i = 1; i < max_entries; i += 1) {
        std::construct_at(&free_list->blk[i], FreeListBlock{
            .size = std::numeric_limits<size_t>::max(),
            .offs = std::numeric_limits<size_t>::max(),
            .next = 0zu,
        });
    }
    return free_list;
}

static void free_list_delete(FreeList* free_list) {
    auto requirement = sizeof(FreeList) + sizeof(FreeListBlock) * free_list->max_entries;
    for (size_t i = 0; i < free_list->max_entries; i += 1) {
        std::destroy_at(&free_list->blk[i]);
    }
    std::destroy_at(free_list);
    ::operator delete(free_list, requirement, std::align_val_t(alignof(FreeList)));
}

static auto free_list_allocate(FreeList* free_list, size_t size, size_t* p_offset) -> bool {
    for (auto node_ref = free_list->head_ref, prev_ref = std::numeric_limits<size_t>::max(); node_ref != std::numeric_limits<size_t>::max(); prev_ref = node_ref, node_ref = free_list->blk[node_ref].next) {
        if (free_list->blk[node_ref].size >= size) {
            *p_offset = free_list->blk[node_ref].offs;
            if (free_list->blk[node_ref].size > size) {
                free_list->blk[node_ref].size -= size;
                free_list->blk[node_ref].offs += size;
            } else {
                auto blk_ref = 0zu;
                if (prev_ref != std::numeric_limits<size_t>::max()) {
                    free_list->blk[prev_ref].next = free_list->blk[node_ref].next;
                    blk_ref = node_ref;
                } else {
                    blk_ref = free_list->head_ref;
                    free_list->head_ref = free_list->blk[node_ref].next;
                }
            }
            return true;
        }
    }
    return false;
}

static auto free_list_deallocate(FreeList* free_list, size_t size, size_t offset) -> bool {
    for (auto node_ref = free_list->head_ref, prev_ref = std::numeric_limits<size_t>::max(); node_ref != std::numeric_limits<size_t>::max(); prev_ref = node_ref, node_ref = free_list->blk[node_ref].next) {
        if (free_list->blk[node_ref].offs >= offset) {
            if (free_list->blk[node_ref].offs > offset) {
            } else {
                free_list->blk[node_ref].size += size;
                free_list->blk[node_ref].offs -= size;
            }
            return true;
        }
    }
}