//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

#include <tuple>
#include <variant>
#include <functional>

namespace stl {
    template<typename... Fn> struct Overloads : Fn ... { using Fn::operator()...; };
    template<typename... Fn> Overloads(Fn...) -> Overloads<Fn...>;

    template<typename T, typename... Fn>
    constexpr auto operator|(T&& args, Overloads<Fn...> const& cases) -> decltype(auto) {
        return std::apply([&cases]<typename... U>(U&&... args) -> decltype(auto) {
            return std::visit(cases, std::forward<U>(args)...);
        }, args);
    }
}