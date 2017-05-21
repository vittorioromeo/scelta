// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
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
            // clang-format off
            template <typename FwdVisitor>
            constexpr bound_visitor(FwdVisitor&& visitor) noexcept(
                noexcept(Visitor{FWD(visitor)}))
                : Visitor{FWD(visitor)}
            {
            }

#define DEFINE_BOUND_VISITOR_CALL(m_refq)                                  \
    template <typename... Variants>                                        \
    constexpr auto operator()(Variants&&... variants) m_refq               \
    SCELTA_NOEXCEPT_AND_TRT(                                               \
        ::scelta::visit(std::declval<Visitor m_refq>(), FWD(variants)...)) \
    {                                                                      \
        return ::scelta::visit(                                            \
            static_cast<Visitor m_refq>(*this), FWD(variants)...);         \
    }

            DEFINE_BOUND_VISITOR_CALL(&)
            DEFINE_BOUND_VISITOR_CALL(const&)
            DEFINE_BOUND_VISITOR_CALL(&&)

#undef DEFINE_BOUND_VISITOR_CALL
        };

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
        SCELTA_RETURNS(
            impl::make_bound_visitor(::scelta::overload(FWD(fs)...))
        )
    // clang-format on
}
