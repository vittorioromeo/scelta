// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "./returns.hpp"

namespace scelta
{
    struct nullopt_t
    {
    };

    inline constexpr nullopt_t nullopt{};
}

namespace scelta::impl
{
    // clang-format off
    template <typename... Ts>
    constexpr auto to_nullopt(Ts&&...)
        SCELTA_RETURNS(
            ::scelta::nullopt
        )

    template <typename Visitor>
    constexpr auto visit_optional(Visitor&& visitor)
        SCELTA_RETURNS(
            FWD(visitor)()
        )

    // Invokes the continuation `c` with `f(o)`.
    template <typename F, typename Continuation, typename Optional>
    constexpr auto call_with_optional(F&& f, Continuation&& c, Optional&& o)
        SCELTA_RETURNS(
            o ? FWD(c)(FWD(f)(meta::forward_like<Optional>(*o)))
              : FWD(c)(FWD(f)(to_nullopt()))
        )

    namespace impl
    {
        template <typename Visitor, typename T>
        struct bind_one
        {
            static_assert(std::is_reference_v<Visitor>);
            static_assert(std::is_reference_v<T>);

            Visitor _visitor;
            T _x;

            constexpr bind_one(Visitor visitor, T x) noexcept
                : _visitor{FWD(visitor)}, _x{FWD(x)}
            {
            }

            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) const
                SCELTA_RETURNS(
                    std::forward<Visitor>(_visitor)(
                        std::forward<T>(_x), FWD(xs)...)
                )
        };



        template <typename Visitor>
        struct binder
        {
            static_assert(std::is_reference_v<Visitor>);

            Visitor _visitor;

            constexpr binder(Visitor visitor) noexcept
                : _visitor{FWD(visitor)}
            {
            }

            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) const
                SCELTA_RETURNS(
                    bind_one<Visitor, Ts&&...>{FWD(_visitor), FWD(xs)...}
                )
        };

        template <typename F>
        auto bind_ref(F&& f)
            SCELTA_RETURNS(
                binder<F&&>{FWD(f)}
            )

        struct visit_optional_t
        {
            template <typename Visitor, typename Optional, typename... Optionals>
            constexpr auto operator()(Visitor&& visitor, Optional&& o, Optionals&&... os) const
                SCELTA_RETURNS(
                    call_with_optional(
                        bind_ref(FWD(visitor)),
                        bind_ref(*this),
                        FWD(o)
                    )
                )
        }
    }

    // TODO: noexcept incorrect
    template <typename Visitor, typename Optional, typename... Optionals>
    constexpr auto visit_optional(
        Visitor&& visitor, Optional&& o, Optionals&&... os) ->decltype(auto)
        // SCELTA_NOEXCEPT_AND_TRT(FWD(visitor)(to_nullopt(), to_nullopt(os)...))
    {
        return call_with_optional(
            impl::bind_ref(FWD(visitor)),
            [&](auto&& bound_visitor) SCELTA_RETURNS(
                visit_optional(FWD(bound_visitor), FWD(os)...)
            ),
            FWD(o)
        );
    }
    // clang-format on
}
