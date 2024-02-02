//
// Created by Maksym Pasichnyk on 20.10.2023.
//

#pragma once

#include <any>
#include <map>
#include <span>
#include <stack>
#include <queue>
#include <print>
#include <mdspan>
#include <vector>
#include <ranges>
#include <memory>
#include <string>
#include <format>
#include <thread>
#include <fstream>
#include <cassert>
#include <variant>
#include <numbers>
#include <utility>
#include <iostream>
#include <algorithm>
#include <typeindex>
#include <functional>
#include <string_view>
#include <type_traits>
#include <source_location>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = long double;

constexpr auto operator "" _i8(u64 self) noexcept -> i8 { return i8(self); }
constexpr auto operator "" _i16(u64 self) noexcept -> i16 { return i16(self); }
constexpr auto operator "" _i32(u64 self) noexcept -> i32 { return i32(self); }
constexpr auto operator "" _i64(u64 self) noexcept -> i64 { return i64(self); }
constexpr auto operator "" _u8(u64 self) noexcept -> u8 { return u8(self); }
constexpr auto operator "" _u16(u64 self) noexcept -> u16 { return u16(self); }
constexpr auto operator "" _u32(u64 self) noexcept -> u32 { return u32(self); }
constexpr auto operator "" _u64(u64 self) noexcept -> u64 { return u64(self); }
constexpr auto operator "" _f32(f64 self) noexcept -> f32 { return f32(self); }
constexpr auto operator "" _f64(f64 self) noexcept -> f64 { return f64(self); }