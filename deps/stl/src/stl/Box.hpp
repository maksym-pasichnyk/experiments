//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

#include "pch.hpp"

namespace stl {
    // todo: non nullable Box<T>, instead use Option<Box<T>>

    template<typename T, typename D = std::default_delete<T>>
    struct Box;

    template<typename T, typename D>
    struct Box final {
        std::unique_ptr<T, D> ptr;

        constexpr Box() noexcept : ptr(nullptr) {}
        constexpr explicit(true) Box(T* ptr) noexcept : ptr(ptr) {}
        constexpr Box(Box&& other) noexcept = default;
        constexpr Box(Box const& other) noexcept = delete;

        constexpr auto operator=(Box&& other) noexcept -> Box& = default;
        constexpr auto operator=(Box const& other) noexcept -> Box& = delete;

        constexpr explicit(true) operator bool(this Box const& self) noexcept {
            return self.ptr.get() != nullptr;
        }

        constexpr auto operator->(this Box const& self) noexcept -> T* {
            return self.ptr.get();
        }

        constexpr auto operator*(this Box const& self) noexcept -> T& {
            return *self.ptr.get();
        }

        constexpr auto release(this Box& self) noexcept -> T* {
            return self.ptr.release();
        }

        template<typename... Args>
        static constexpr auto make(Args&&... args) -> Box {
            return Box(new T(std::forward<Args>(args)...));
        }

        static constexpr auto make_for_overwrite() -> Box {
            return Box(new T);
        }
    };

    template<typename T, typename D>
    struct Box<T[], D> final {
        using Span = std::span<T>;

        Span ptr;

        constexpr Box() noexcept : ptr() {}
        constexpr explicit(true) Box(Span ptr) noexcept : ptr(ptr) {}

        constexpr Box(Box&& other) noexcept : ptr(other.release()) {}
        constexpr Box(Box const& other) noexcept = delete;

        constexpr ~Box() noexcept {
            D()(this->ptr.data());
        }

        constexpr auto operator=(this Box& self, Box&& other) noexcept -> Box& {
            std::swap(self.ptr, other.ptr);
            return self;
        }
        constexpr auto operator=(this Box& self, Box const& other) noexcept -> Box = delete;

        constexpr explicit(true) operator bool(this Box const& self) noexcept {
            return self.ptr.data() != nullptr;
        }

        constexpr auto operator[](this Box const& self, size_t i) noexcept -> T& {
            return self.ptr[i];
        }

        constexpr auto operator->(this Box const& self) noexcept -> Span* {
            return std::addressof(self.ptr);
        }

        constexpr auto operator*(this Box const& self) noexcept -> Span {
            return self.ptr;
        }

        constexpr auto release(this Box& self) noexcept -> Span {
            return std::exchange(self.ptr, Span());
        }

        constexpr auto size(this Box const& self) noexcept -> size_t {
            return self.ptr.size();
        }

        constexpr auto size_bytes(this Box const& self) noexcept -> size_t {
            return self.ptr.size_bytes();
        }

        constexpr auto data(this Box const& self) noexcept -> T* {
            return self.ptr.data();
        }

        static constexpr auto make(size_t len) -> Box {
            return Box(std::span(new T[len](), len));
        }

        static constexpr auto make_for_overwrite(size_t len) -> Box {
            return Box(std::span(new T[len], len));
        }
    };

    template<typename T, typename D = std::default_delete<T>>
    Box(T*) -> Box<T, D>;

    template<typename T, typename D = std::default_delete<T[]>>
    Box(std::span<T>) -> Box<T[], D>;

    // todo: Box<std::string_view>
    // todo: Box<T[N]>
    // todo: Box<dyn<T>>
}