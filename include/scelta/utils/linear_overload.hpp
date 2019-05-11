// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"
#include <type_traits>

// TODO: cleanup

namespace scelta
{
    namespace impl
    {
        template <typename...>
        struct linear_overloader
        {
        };

        template <typename T, typename... Ts>
        struct linear_overloader<T, Ts...> : T
        {
        private:
            using U = linear_overloader<Ts...>;
            U _u;

#define DEFINE_SELECT(m_refq)                                                 \
private:                                                                      \
    template <typename... Args, typename = decltype(std::declval<T m_refq>()( \
                                    std::declval<Args>()...))>                \
    constexpr T m_refq select(int) m_refq noexcept                            \
    {                                                                         \
        return static_cast<T m_refq>(*this);                                  \
    }                                                                         \
                                                                              \
    template <typename... Args>                                               \
    constexpr U m_refq select(...) m_refq noexcept                            \
    {                                                                         \
        return _u;                                                            \
    }                                                                         \
                                                                              \
public:                                                                       \
    template <typename... Args>                                               \
    constexpr decltype(auto) operator()(Args&&... xs) m_refq                  \
    {                                                                         \
        return select<Args...>(0)(FWD(xs)...);                                \
    }

            DEFINE_SELECT(&)
            DEFINE_SELECT(const&)
            DEFINE_SELECT(&&)

#undef DEFINE_SELECT

        public:
            template <typename TFwd, typename... TFwds>
            constexpr linear_overloader(TFwd&& x, TFwds&&... xs) noexcept(
                std::is_nothrow_constructible_v<T, TFwd&&>&&
                    std::is_nothrow_constructible_v<U, TFwds&&...>)
                : T{FWD(x)}, _u{FWD(xs)...}
            {
            }
        };
    }

    template <typename... Ts>
    [[nodiscard]] constexpr auto linear_overload(Ts&&... xs) SCELTA_RETURNS(
        impl::linear_overloader<std::remove_reference_t<Ts>...>{FWD(xs)...})
}
