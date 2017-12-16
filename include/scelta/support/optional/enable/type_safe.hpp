// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// This header assumes that `type_safe::optional` was already properly included
// beforehand.

#include "../../../traits/adt/alternatives.hpp"
#include "../../../traits/adt/visit.hpp"
#include "../../../traits/adt/visit_optional.hpp"
#include "../../../traits/optional.hpp"
#include "../../../utils/returns.hpp"

#define SCELTA_SUPPORT_OPTIONAL_TYPE_SAFE 1

namespace scelta::traits
{
    namespace adt
    {
        template <typename T>
        struct visit<::type_safe::basic_optional<T>> : impl::visit_optional_t
        {
        };

        template <typename T>
        struct alternatives<::type_safe::basic_optional<T>>
            : impl::alternative_tuple<typename T::value_type>
        {
        };
    }

    namespace optional
    {
        template <typename T>
        struct access<::type_safe::basic_optional<T>>
        {
            // `type_safe::basic_optional` does not provide an `operator*`
            // overload.

            // clang-format off
            template <typename Optional>
            constexpr auto operator()(Optional&& o)
                SCELTA_RETURNS(
                    FWD(o).value()
                )
            // clang-format on
        };

        template <typename T>
        struct make<::type_safe::basic_optional<T>>
        {
            // `T` in this context is a storage policy. In order to create a new
            // optional instance, we can use `rebind` to change the stored type.

            // clang-format off
            template <typename X>
            constexpr auto operator()(X&& x)
                SCELTA_RETURNS(
                    ::type_safe::basic_optional<
                        typename T::template rebind<std::decay_t<X>>
                    >{FWD(x)}
                )
            // clang-format on
        };
    }
}
