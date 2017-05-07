// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../meta/replace_all.hpp"
#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
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