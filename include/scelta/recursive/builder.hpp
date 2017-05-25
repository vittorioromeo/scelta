// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/replace_all.hpp"
#include "./placeholder.hpp"
#include <type_traits>

namespace scelta::recursive
{
    template <typename>
    class builder;

    template <template <typename...> typename T, typename... Ts>
    class builder<T<Ts...>>
    {
    public:
        struct type;

        template <typename U>
        using resolve = impl::replace_placeholders<type, U>;
        // TODO:
        // * consider moving types with placeholders to the right so that return
        // type deduction with `experimental::match` is more robust
        // * nice error message when there are no base cases?

    private:
        using resolved_type = resolve<T<Ts...>>;

    public:
        struct type : resolved_type
        {
            using resolved_type::resolved_type;

            // This type alias is detected when performing recursive visitation
            // to find `resolved_type`'s homogenizer.
            using base_type = resolved_type;
        };
    };

    template <typename Builder>
    using type = typename Builder::type;

    template <typename Builder, typename T>
    using resolve = typename Builder::template resolve<T>;
}
