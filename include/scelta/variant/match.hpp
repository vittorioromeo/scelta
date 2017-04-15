// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils.hpp"
#include "./visit.hpp"

namespace scelta
{
    namespace impl
    {
        template <typename Visitor>
        struct bound_visitor : Visitor
        {
            template <typename FwdVisitor>
            constexpr bound_visitor(FwdVisitor&& visitor) noexcept(
                noexcept(Visitor{FWD(visitor)}))
                : Visitor{FWD(visitor)}
            {
            }

// clang-format off
#define DEFINE_BOUND_VISITOR_CALL(m_visitor_type, m_ref_qualifier)           \
    template <typename... Variants>                                          \
    constexpr auto operator()(Variants&&... variants) m_ref_qualifier        \
          noexcept(                                                          \
          noexcept(visit(std::declval<m_visitor_type>(), FWD(variants)...))) \
        ->decltype(visit(std::declval<m_visitor_type>(), FWD(variants)...))  \
    {                                                                        \
        return visit(static_cast<m_visitor_type>(*this), FWD(variants)...);  \
    }

            DEFINE_BOUND_VISITOR_CALL(Visitor&,       &)
            DEFINE_BOUND_VISITOR_CALL(const Visitor&, const&)
            DEFINE_BOUND_VISITOR_CALL(Visitor&&,      &&)

            #undef DEFINE_BOUND_VISITOR_CALL
            // clang-format on
        };

        // clang-format off
        template <typename Visitor>
        constexpr auto make_bound_visitor(Visitor&& visitor) 
            SCELTA_RETURNS(
                bound_visitor<std::decay_t<Visitor>>{FWD(visitor)}
            )
        // clang-format on
    }

    // clang-format off
    template <typename... Fs>
    constexpr auto match(Fs&&... fs) 
        SCELTA_RETURNS(impl::make_bound_visitor(overload(FWD(fs)...)))
    // clang-format on
}