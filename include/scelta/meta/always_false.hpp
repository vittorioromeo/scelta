// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>

namespace scelta::meta
{
    /// @brief Wrapper around `std::false_type`.
    /// @details Used to produce nicer error messages via `static_assert`.
    /// It always evaluates to `std::false_type`, but it doesn't immediately
    /// trigger a static assertion as it depends on `T`.
    template <typename T>
    struct always_false : std::false_type
    {
    };

    /// @brief Type alias for `always_false`.
    template <typename T>
    using always_false_t = typename always_false<T>::type;

    /// @brief Variable template for `always_false`.
    template <typename T>
    inline constexpr always_false_t<T> always_false_v{};
}
