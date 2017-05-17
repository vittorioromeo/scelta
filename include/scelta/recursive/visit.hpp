// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../meta/replace_all.hpp"
#include "../meta/y_combinator.hpp"
#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include "../visitation/visit.hpp"
#include <type_traits>

namespace scelta::recursive
{
    namespace impl
    {
        template <typename T, typename = void>
        struct original_type
        {
            using type = T;
        };

        template <typename T>
        struct original_type<T, std::void_t<typename T::base_type>>
        {
            using type = typename T::base_type;
        };

        // Evaluates to `T` if `T::base_type` does not exist, otherwise
        // evaluates to `T::base_type`. Used to retrieve the "original"
        // visitable type from a recursive wrapper.
        template <typename T>
        using original_type_t = typename original_type<T>::type;
    }

    // Recursive visitation: currently an alias for non-recursive
    // visitation.
    // This may change if recursive data structures are represented in a
    // different way (e.g. data member instead of inheritance).
    template <typename Return, typename Visitor, typename... Variants>
    constexpr Return visit(Visitor&& visitor, Variants&&... variants)
    {
        return ::scelta::impl::visit_impl(::scelta::impl::recursive_tag<Return>{},
            FWD(visitor),
            static_cast<meta::copy_cv_ptr_ref_t<Variants&&,
                impl::original_type_t<std::decay_t<Variants>>>>(variants)...);
    }
}
