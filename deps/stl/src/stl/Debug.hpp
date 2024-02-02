//
// Created by Maksym Pasichnyk on 04.09.2023.
//
#pragma once

#include "Trait.hpp"
#include "Option.hpp"

namespace stl {
    template<typename>
    struct Debug;
    struct DebugList;
    struct DebugStruct;

    trait(Write,
        (write_str, void(std::string_view))
    )

    struct PadAdapter {
        dyn<Write&> buf;
        bool on_newline;

        void write_str(this PadAdapter& self, std::string_view s) {
            if (self.on_newline) {
                self.buf.write_str("    ");
            }
            self.buf.write_str(s);
            self.on_newline = s.ends_with('\n');
        }
    };

    struct Formatter {
        dyn<Write&> buf;

        auto debug_list(this Formatter& self) -> DebugList;
        auto debug_struct(this Formatter& self, std::string_view name) -> DebugStruct;

        auto write_str(this Formatter& self, std::string_view s) -> Formatter& {
            self.buf.write_str(s);
            return self;
        }

        template<typename Fn>
        auto pad(this Formatter& self, Fn&& fn) -> Formatter& {
            auto adapter = PadAdapter(self.buf, true);
            std::forward<Fn>(fn)(Formatter(&adapter));
            return self;
        }

        auto fmt(this Formatter& self, auto&& arg) -> Formatter& {
            using U = std::remove_cvref_t<decltype(arg)>;
            Debug<U>::format(std::forward<decltype(arg)>(arg), self);
            return self;
        }
    };

    struct DebugStruct {
        Formatter& formatter;
        bool has_fields;

        auto field(this DebugStruct& self, std::string_view name, auto&& value) -> DebugStruct& {
            auto prefix = self.has_fields ? ",\n" : " {\n";
            self.formatter.write_str(prefix);
            self.formatter.pad([&](auto adapter) {
                adapter.write_str(name);
                adapter.write_str(": ");
                adapter.fmt(value);
            });
            self.has_fields = true;
            return self;
        }

        void finish(this DebugStruct& self) {
            if (self.has_fields) {
                self.formatter.write_str("\n");
                self.formatter.write_str("}");
            }
        }
    };

    struct DebugList {
        Formatter& formatter;
        bool has_fields;

        auto entry(this DebugList& self, auto&& value) -> DebugList& {
            auto prefix = self.has_fields ? ",\n" : "\n";
            self.formatter.write_str(prefix);
            self.formatter.pad([&](auto adapter) {
                adapter.fmt(std::forward<decltype(value)>(value));
            });
            self.has_fields = true;
            return self;
        }

        auto entries(this DebugList& self, auto&& values) -> DebugList& {
            for (auto& value: values) {
                self.entry(value);
            }
            return self;
        }

        void finish(this DebugList& self) {
            if (self.has_fields) {
                self.formatter.write_str("\n");
            }
            self.formatter.write_str("]");
        }
    };

    template<typename T>
    struct DebugFormatter : std::formatter<std::string> {
        auto format(T const& self, auto& ctx) -> decltype(ctx.out()) {
            struct Wrapper {
                std::format_context& ctx;

                void write_str(this Wrapper& self, std::string_view s) {
                    self.ctx.advance_to(std::format_to(self.ctx.out(), "{}", s));
                }
            };
            auto wrapper = Wrapper(ctx);
            auto fmt = Formatter(wrapper);
            Debug<T>::format(self, fmt);
            return ctx.out();
        }
    };
}

inline auto stl::Formatter::debug_list(this Formatter& self) -> DebugList {
    self.write_str("[");
    return DebugList(self, false);
}


inline auto stl::Formatter::debug_struct(this Formatter& self, std::string_view name) -> DebugStruct {
    self.write_str(name);
    return DebugStruct(self, false);
}

template<>
struct stl::Debug<bool> {
    static void format(bool const& self, auto& fmt) {
        fmt.write_str(std::to_string(self));
    }
};

template<>
struct stl::Debug<i32> {
    static void format(i32 const& self, auto& fmt) {
        fmt.write_str(std::to_string(self));
    }
};

template<>
struct stl::Debug<f32> {
    static void format(f32 const& self, auto& fmt) {
        fmt.write_str(std::to_string(self));
    }
};

template<>
struct stl::Debug<std::string> {
    static void format(std::string const& self, auto& fmt) {
        fmt.write_str(std::format("\"{}\"", self));
    }
};

template<>
struct stl::Debug<std::string_view> {
    static void format(std::string_view const& self, auto& fmt) {
        fmt.write_str(std::format("\"{}\"", self));
    }
};

template<typename T, typename Allocator>
struct stl::Debug<std::vector<T, Allocator>> {
    static void format(std::vector<T, Allocator> const& self, auto& fmt) {
        fmt.debug_list()
           .entries(self)
           .finish();
    }
};

template<typename T>
struct stl::Debug<std::shared_ptr<T>> {
    static void format(std::shared_ptr<T> const& self, auto& fmt) {
        return Debug<std::remove_cvref_t<T>>::format(*self, fmt);
    }
};

template<typename T>
struct stl::Debug<stl::Option<T>> {
    static void format(Option<T> const& self, auto& fmt) {
        match_(self) {
            case_(None) {
                fmt.write_str("None");
            },
            case_(auto&& arg) {
                Debug<std::remove_cvref_t<T>>::format(std::forward<decltype(arg)>(arg), fmt);
            }
        };
    }
};