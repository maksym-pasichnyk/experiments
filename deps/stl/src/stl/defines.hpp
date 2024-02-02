//
// Created by Maksym Pasichnyk on 26.09.2023.
//
#pragma once

#define PARENS ()
#define EXPAND(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) __VA_ARGS__

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define TRAIT_INTERFACE_POINTER_I(name, ...) stl::trait_function_pointer_t<__VA_ARGS__> name##_;
#define TRAIT_INTERFACE_POINTER(...) TRAIT_INTERFACE_POINTER_I __VA_ARGS__

#define TRAIT_IMPL_CONSTRAINT_I(name, ...) static_cast<stl::trait_member_pointer_t<Impl, __VA_ARGS__>>(&Impl::name);
#define TRAIT_IMPL_CONSTRAINT(...) TRAIT_IMPL_CONSTRAINT_I __VA_ARGS__

#define TRAIT_INTERFACE_THUNK_I(name, ...) (stl::trait_function_pointer_t<__VA_ARGS__>) static_cast<stl::trait_member_pointer_t<Impl, __VA_ARGS__>>(&Impl::name),
#define TRAIT_INTERFACE_THUNK(...) TRAIT_INTERFACE_THUNK_I __VA_ARGS__

#define TRAIT_METHOD_I(name, ...)                                                               \
auto name(this Self const &self, auto&&... args) -> stl::trait_function_result_t<__VA_ARGS__> { \
    return self.vptr->name##_(self.impl, static_cast<decltype(args)&&>(args)...);               \
}
#define TRAIT_METHOD(...) TRAIT_METHOD_I __VA_ARGS__

#define trait(Trait, ...)                                      \
struct Trait {                                                 \
    struct vtable {                                            \
        size_t size;                                           \
        size_t align;                                          \
        FOR_EACH(TRAIT_INTERFACE_POINTER, __VA_ARGS__)         \
    };                                                         \
    template<typename Impl>                                    \
    static constexpr auto implements = requires {              \
        FOR_EACH(TRAIT_IMPL_CONSTRAINT, __VA_ARGS__)           \
    };                                                         \
    template<typename Impl>                                    \
    inline static const auto vtable_for = vtable{              \
        sizeof(Impl),                                          \
        alignof(Impl),                                         \
        FOR_EACH(TRAIT_INTERFACE_THUNK, __VA_ARGS__)           \
    };                                                         \
};                                                             \
template<>                                                     \
struct stl::dyn<Trait&> {                                      \
    using Self = stl::dyn<Trait&>;                             \
    using vtable = Trait::vtable;                              \
                                                               \
    void*         impl = {};                                   \
    vtable const* vptr = {};                                   \
                                                               \
    template<typename Impl>                                    \
    constexpr dyn(Impl* impl) noexcept                         \
    : impl(impl)                                               \
    , vptr(&Trait::template vtable_for<Impl>) {}               \
                                                               \
    FOR_EACH(TRAIT_METHOD, __VA_ARGS__)                        \
};                                                             \
template<>                                                     \
struct stl::dyn<Trait const&> {                                \
    using Self = stl::dyn<Trait const&>;                       \
    using vtable = Trait::vtable;                              \
                                                               \
    const void*   impl = {};                                   \
    vtable const* vptr = {};                                   \
                                                               \
    template<typename Impl>                                    \
    constexpr dyn(Impl const* impl) noexcept                   \
    : impl(impl)                                               \
    , vptr(&Trait::template vtable_for<Impl>) {}               \
                                                               \
    FOR_EACH(TRAIT_METHOD, __VA_ARGS__)                        \
};

#define defer_(...) auto _ = stl::Defer([&] __VA_ARGS__)
#define match_(...) std::forward_as_tuple(__VA_ARGS__) | stl::Overloads
#define case_ [&]