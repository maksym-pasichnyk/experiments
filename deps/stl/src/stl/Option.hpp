//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

//#include "lib.hpp"
#include "Enum.hpp"
#include "defines.hpp"
#include "Overloads.hpp"

namespace stl {
    struct None {
        constexpr friend auto operator ==(None const&, None const&) -> bool = default;
    };

    template<typename>
    struct Option;

    template<typename U>
    constexpr auto Some(U&& value) -> Option<U> {
        return Option<U>(std::forward<U>(value));
    }

    template<typename T>
    struct Option final : public Enum<None, T> {
    public:
        using Enum = Enum<None, T>;
        using Enum::Enum;

    public:
        template<typename Self>
        [[nodiscard]] constexpr auto unwrap(this Self&& self) -> decltype(auto) {
            return std::forward<Self>(self).template as<T>();
        }

        template<typename Self>
        [[nodiscard]] constexpr auto unwrap_unchecked(this Self&& self) -> decltype(auto) {
            return std::forward_like<Self>(*self.template get_if<T>());
        }

        template<typename Self, typename U>
        [[nodiscard]] constexpr auto unwrap_or(this Self&& self, U&& other) -> T {
            static_assert(std::convertible_to<U, T>, "U is not convertible to T");
            return self ? *std::forward<Self>(self) : std::forward<U>(other);
        }

        template<typename Self>
        [[nodiscard]] constexpr auto expect(this Self&& self, std::string_view msg) -> decltype(auto) {
            if (!self) {
                throw std::runtime_error(std::string(msg));
            }
            return *std::forward<Self>(self);
        }

        template<typename Self, typename Fn>
        [[nodiscard]] constexpr auto map(this Self&& self, Fn&& fn) -> decltype(auto) {
            return self ? Some(fn(*std::forward<Self>(self))) : None();
        }

        template<typename Self, typename U, typename Fn>
        [[nodiscard]] constexpr auto map_or(this Self&& self, U&& other, Fn&& fn) -> decltype(auto) {
            return self ? fn(*std::forward<Self>(self)) : std::forward<U>(other);
        }

        template<typename Self, typename Fn>
        [[nodiscard]] constexpr auto and_then(this Self&& self, Fn&& fn) -> decltype(auto) {
            return self ? fn(*std::forward<Self>(self)) : None();
        }

        template<typename Self, typename Fn>
        [[nodiscard]] constexpr auto filter(this Self&& self, Fn&& fn) -> decltype(auto) {
            return self && fn(*self) ? std::forward<Self>(self) : None();
        }

    public:
        [[nodiscard]] constexpr auto is_some(this Option const& self) noexcept -> bool {
            return self.template is<T>();
        }

        [[nodiscard]] constexpr auto is_none(this Option const& self) noexcept -> bool {
            return !self.is_some();
        }

        [[nodiscard]] explicit(true) constexpr operator bool(this Option const& self) noexcept {
            return self.is_some();
        }

        template<typename Self>
        [[nodiscard]] constexpr auto operator *(this Self&& self) -> decltype(auto) {
            return std::forward<Self>(self).unwrap_unchecked();
        }

        [[nodiscard]] constexpr auto operator ->(this Option& self) -> decltype(auto) {
            return self.template get_if<T>();
        }

        [[nodiscard]] constexpr auto operator ->(this Option const& self) -> decltype(auto) {
            return self.template get_if<T>();
        }

        [[nodiscard]] constexpr auto operator ->(this Option&& self) -> decltype(auto) = delete;

        [[nodiscard]] constexpr auto operator ->(this Option const&& self) -> decltype(auto) = delete;
    };
}