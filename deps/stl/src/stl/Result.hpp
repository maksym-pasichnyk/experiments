//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

//#include "lib.hpp"
#include "Enum.hpp"

namespace stl {
    template<typename T, typename E>
    struct Result final : public Enum<T, E> {
        using Enum = Enum<T, E>;
        using Enum::Enum;

    private:
        template<typename Self>
        [[nodiscard]] constexpr auto unwrap(this Self&& self) -> decltype(auto) {
            if (!self) {
                std::println(stderr, "{}", *self.template get_if<E>());
                std::abort();
            }
            return std::forward_like<Self>(*self.template get_if<T>());
        }

        template<typename Self>
        [[nodiscard]] constexpr auto unwrap_unchecked(this Self&& self) -> decltype(auto) {
            return std::forward_like<Self>(*self.template get_if<T>());
        }

    public:
        [[nodiscard]] constexpr auto is_ok(this Result const& self) noexcept -> bool {
            return self.Enum::template is<T>();
        }

        [[nodiscard]] constexpr auto is_err(this Result const& self) noexcept -> bool {
            return self.Enum::template is<E>();
        }

        [[nodiscard]] explicit(true) constexpr operator bool(this Result const& self) noexcept {
            return self.is_ok();
        }
    };
}