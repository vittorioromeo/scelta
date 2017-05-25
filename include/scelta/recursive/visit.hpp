// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/copy_cv_ptr_ref.hpp"
#include "../utils/fwd.hpp"
#include "../visitation/visit.hpp"
#include "./original_type.hpp"
#include <type_traits>

namespace scelta::recursive
{
    // Recursive visitation: currently an alias for non-recursive
    // visitation.
    // This may change if recursive data structures are represented in a
    // different way (e.g. data member instead of inheritance).
    template <typename Return, typename Visitor, typename... Variants>
    constexpr Return visit(Visitor&& visitor, Variants&&... variants)
    {
        return ::scelta::impl::visit_impl(
            ::scelta::impl::recursive_tag<Return>{}, FWD(visitor),
            static_cast<meta::copy_cv_ptr_ref_t<Variants&&,
                impl::original_type_t<std::decay_t<Variants>>>>(variants)...);
    }
}
