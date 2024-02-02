//
// Created by Maksym Pasichnyk on 25.09.2023.
//
#pragma once

#include "pch.hpp"
#include "Debug.hpp"
#include "Defer.hpp"
#include "Enum.hpp"
#include "Lazy.hpp"
#include "Option.hpp"
#include "Overloads.hpp"
#include "Arc.hpp"
#include "Box.hpp"
#include "Result.hpp"
#include "Trait.hpp"
#include "defines.hpp"

namespace stl {
    [[noreturn]] void unimplemented(std::source_location location = std::source_location::current());
    [[noreturn]] void unimplemented(std::string_view msg, std::source_location location = std::source_location::current());
}
