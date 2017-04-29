// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../meta/replace_all.hpp"
#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include <type_traits>

// TODO: to own file
/*
namespace scelta::recursive::impl
{
    template <typename T, typename Derived>
    struct indirection : T
    {
        T _data;

        template <typename... Ts,
            typename = std::enable_if_t<(sizeof...(Ts) != 1) ||
                                        !std::disjunction_v<std::is_same<
                                            std::decay_t<Ts>, Derived>...>>>
        constexpr indirection(Ts&&... xs) noexcept(noexcept(T{FWD(xs)...}))
            : T{FWD(xs)...}
        {
        }
    };

    // clang-format off
    template <typename T>
    constexpr auto get_indirect(T&& x)
        SCELTA_RETURNS(
            meta::forward_like<T>(x._data)
        )
    // clang-format on
}*/

// TODO: to own file
namespace scelta::recursive
{
    struct placeholder
    {
        placeholder() = delete;

        placeholder(const placeholder&) = delete;
        placeholder(placeholder&&) = delete;

        placeholder& operator=(const placeholder&) = delete;
        placeholder& operator=(placeholder&&) = delete;
    };

    namespace impl
    {
        template <typename After, typename T>
        using replace_placeholders = //
            meta::replace_all_copy_cv_ptr_ref_t<placeholder, After, T>;
    }

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
        };
    };

    template <typename Builder>
    using type = typename Builder::type;

    template <typename Builder, typename T>
    using resolve = typename Builder::template resolve<T>;
}