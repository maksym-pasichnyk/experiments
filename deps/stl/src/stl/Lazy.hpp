//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

namespace stl {
    template<typename Fn>
    struct Lazy final : Fn {
        explicit(true) operator auto() noexcept(noexcept(Fn::operator ()())) {
            return Fn::operator ()();
        }
    };

    template<typename Fn>
    Lazy(Fn) -> Lazy<Fn>;
}