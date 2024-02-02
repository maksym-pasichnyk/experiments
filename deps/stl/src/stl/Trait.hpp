//
// Created by Maksym Pasichnyk on 30.09.2023.
//

#pragma once

namespace stl {
    template<typename Fn>
    struct trait_function_pointer;

    template<typename R, typename... Args>
    struct trait_function_pointer<R(Args...)> {
        using type = R(*)(void*, Args...);
    };

    template<typename R, typename... Args>
    struct trait_function_pointer<R(Args...) const> {
        using type = R(*)(const void*, Args...);
    };

    template<typename Fn>
    using trait_function_pointer_t = trait_function_pointer<Fn>::type;

    template<typename Self, typename Fn>
    struct trait_member_pointer;

    template<typename Self, typename R, typename... Args>
    struct trait_member_pointer<Self, R(Args...)> {
        using type = R(*)(Self&, Args...);
    };

    template<typename Self, typename R, typename... Args>
    struct trait_member_pointer<Self, R(Args...) const> {
        using type = R(*)(Self const&, Args...);
    };

    template<typename Self, typename Fn>
    using trait_member_pointer_t = trait_member_pointer<Self, Fn>::type;

    template<typename R, typename... Args>
    struct trait_function_result;

    template<typename R, typename... Args>
    struct trait_function_result<R(Args...)> {
        using type = R;
    };

    template<typename R, typename... Args>
    struct trait_function_result<R(Args...) const> {
        using type = R;
    };

    template<typename Fn>
    using trait_function_result_t = trait_function_result<Fn>::type;

    template <typename> struct dyn;
    template <typename Self, typename Trait>
    concept impl = Trait::template implements<Self>;
}