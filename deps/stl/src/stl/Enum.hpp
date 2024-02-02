//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

#include "pch.hpp"

#include <utility>

namespace stl {
    template<typename...>
    struct Enum;

    template<typename... Variant>
    struct Enum : std::variant<Variant...> {
        using variant = std::variant<Variant...>;
        using variant::variant;

    public:
        template<typename T, typename Self>
        [[nodiscard]] constexpr auto as(this Self&& self) -> decltype(auto) {
            return std::get<T>(std::forward<Self>(self));
        }

    public:
        template<typename T>
        [[nodiscard]] constexpr auto is(this Enum const& self) -> bool {
            return std::holds_alternative<T>(self);
        }

        template<typename T, typename Self>
        [[nodiscard]] constexpr auto get_if(this Self&& self) -> decltype(auto) {
            return std::get_if<T>(std::addressof(std::forward<Self>(self)));
        }

        constexpr friend auto operator <=>(Enum const&, Enum const&) -> bool = default;
    };

    /*
    template<typename T1, typename T2> requires std::is_empty_v<T1>
    struct Enum<T1, T2&> {
        T2* ptr;

        Enum(T1) : ptr(nullptr) {}
        Enum(T2& reference) : ptr(std::addressof(reference)) {}

        template<std::same_as<T1>>
        [[nodiscard]] auto is() const -> bool {
            return ptr == nullptr;
        }
        template<std::same_as<T2&>>
        [[nodiscard]] auto is() const -> bool {
            return ptr != nullptr;
        }

        template<std::same_as<T2&>>
        [[nodiscard]] auto get_if()& -> decltype(auto) {
            return ptr;
        }

        template<std::same_as<T2&>>
        [[nodiscard]] auto get_if() const& -> decltype(auto) {
            return ptr;
        }

        template<std::same_as<T2&>>
        [[nodiscard]] auto get_if()&& -> decltype(auto) = delete;

        template<std::same_as<T2&>>
        [[nodiscard]] auto get_if() const&& -> decltype(auto) = delete;
    };
    */

    template<>
    struct Enum<> {};
}