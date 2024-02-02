//
// Created by Maksym Pasichnyk on 11.01.2024.
//
#include "lib.hpp"

void stl::unimplemented(std::source_location location) {
    std::println(stderr, "{}:{}:{}: not implemented", location.file_name(), location.line(), location.column());
    std::terminate();
}

void stl::unimplemented(std::string_view msg, std::source_location location) {
    std::println(stderr, "{}:{}:{}: not implemented: {}", location.file_name(), location.line(), location.column(), msg);
    std::terminate();
}
