//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

namespace stl {
    template<typename Fn>
    struct Defer final : Fn {
        ~Defer() {
            Fn::operator ()();
        }
    };

    template<typename Fn>
    Defer(Fn) -> Defer<Fn>;
}