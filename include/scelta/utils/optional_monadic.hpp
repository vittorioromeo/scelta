// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../traits/adt/is_visitable.hpp"
#include "../traits/adt/valid.hpp"
#include "./access_optional.hpp"
#include "./assert.hpp"
#include "./nullopt.hpp"
#include "./optional_utils.hpp"
#include "./returns.hpp"

// TODO: remove?
namespace scelta::detail
{
    template <typename Optional>
    struct change_item_type;

    template <template <typename...> typename Optional, typename T,
        typename... Ts>
    struct change_item_type<Optional<T, Ts...>>
    {
        template <typename U>
        using apply = Optional<U, Ts...>;
    };

    template <typename Optional, typename T>
    using change_item_type_t = typename change_item_type<Optional>::template apply<T>;

    template <typename Optional>
    using optional_item_type_t =
        decltype(impl::access_optional(std::declval<Optional>()));

    template <typename Optional, typename F>
    using unwrap_optional_invoke_result_t =
        std::invoke_result_t<F, optional_item_type_t<Optional>>;

    template <typename Optional, typename F>
    using map_result_t = change_item_type_t<std::decay_t<Optional>,
        unwrap_optional_invoke_result_t<Optional, F>>;
}

namespace scelta
{
    // clang-format off
    /// @brief Returns `f(*o)` if `o` is set, `f_def()` otherwise.
    /// @details Signatures:
    /// * `o: Optional<T>`
    /// * `f_def: () -> U`
    /// * `f: (T) -> U`
    template <typename Optional, typename FD, typename F>
    constexpr auto map_or_else(Optional&& o, FD&& f_def, F&& f)
    SCELTA_RETURNS(
        is_nullopt(o) ? make_like<Optional>(FWD(f_def)())
                      : make_like<Optional>(FWD(f)(impl::access_optional(FWD(o))))
    )

    /// @brief Returns `f(*o)` if `o` is set, `def` otherwise.
    /// @details Signatures:
    /// * `o: Optional<T>`
    /// * `def: U`
    /// * `f: (T) -> U`
    template <typename Optional, typename T, typename F>
    constexpr auto map_or(Optional&& o, T&& def, F&& f)
    SCELTA_RETURNS(
        is_nullopt(o) ? make_like<Optional>(FWD(def))
                      : make_like<Optional>(FWD(f)(impl::access_optional(FWD(o))))
    )

    /// @brief Returns `Optional{f(*o)}` if `o` is set, an empty
    /// optional otherwise.
    /// @details Signatures:
    /// * `o: Optional<T>`
    /// * `f: (T) -> U`
    template <typename Optional, typename F>
    constexpr auto map(Optional&& o, F&& f)
    SCELTA_RETURNS(
         is_nullopt(o) ? make_unset_like<Optional, decltype(FWD(f)(impl::access_optional(FWD(o))))>()
                       : make_like<Optional>(FWD(f)(impl::access_optional(FWD(o))))
    )

    /// @brief Returns `f(*o)` if `o` is set, an empty `Optional`
    /// otherwise.
    /// @details Signatures:
    /// * `o: Optional<T>`
    /// * `f: (T) -> Optional<U>`
    template <typename Optional, typename F>
    constexpr auto and_then(Optional&& o, F&& f)
    SCELTA_RETURNS(
        is_nullopt(o) ? std::decay_t<decltype(FWD(f)(impl::access_optional(FWD(o))))>{}
                      : FWD(f)(impl::access_optional(FWD(o)))
    )

    /// @brief Returns `ob` if `o` is set, an empty `OptB` otherwise.
    /// @details Signatures:
    /// * `o: Optional<T>`
    /// * `ob: Optional<U>`
    template <typename Optional, typename OptB>
    constexpr auto and_(Optional&& o, OptB&& ob)
    SCELTA_RETURNS(
        is_nullopt(o) ? std::decay_t<OptB>{} : FWD(ob)
    )

    /// @brief Returns `o` if `o` is set, `f()` otherwise.
    /// @details Signatures:
    /// * `o: Optional<T>`
    /// * `f: () -> Optional<U>`
    template <typename Optional, typename F>
    constexpr auto or_else(Optional&& o, F&& f)
    SCELTA_RETURNS(
        is_nullopt(o) ? FWD(f)() : FWD(o)
    )

    /// @brief Returns `o` if `o` is set, `def` otherwise.
    /// @details Signatures:
    /// * `o: Optional<T>`
    /// * `def: Optional<U>`
    template <typename Optional, typename Def>
    constexpr auto or_(Optional&& o, Def&& def)
    SCELTA_RETURNS(
        is_nullopt(o) ? FWD(def) : FWD(o)
    )
    // clang-format on
}
