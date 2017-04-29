// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "copy_cv_ptr_ref.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    template <typename Before, typename After, typename T>
    struct replace_all : std::conditional<std::is_same_v<T, Before>, After, T>
    {
    };

    template <typename Before, typename After, typename T>
    using replace_all_t = typename replace_all<Before, After, T>::type;

    // Usage of C++17: `template <...> typename`.
    template <typename Before, typename After,
        template <typename...> typename T, typename... Ts>
    struct replace_all<Before, After, T<Ts...>>
    {
        using type = T<replace_all_t<Before, After, Ts>...>;
    };

    // TODO: document move use in recursive
    // TODO: generalize with above?
    namespace wip
    {
        template <typename Before, typename After, typename T>
        struct ra_cv
            : std::conditional<std::is_same_v<remove_cv_ref_ptr_t<T>, Before>,
                  copy_cv_ptr_ref_t<T, After>, T>
        {
        };

        template <typename Before, typename After, typename T>
        using ra_cv_t = typename ra_cv<Before, After, T>::type;

        // Usage of C++17: `template <...> typename`.
        template <typename Before, typename After,
            template <typename...> typename T, typename... Ts>
        struct ra_cv<Before, After, T<Ts...>>
        {
            using type = T<ra_cv_t<Before, After, Ts>...>;
        };
    }
}