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

#define PRODUCE_ERROR_TODO(str)                \
    do                                         \
    {                                          \
        [](auto x) constexpr                   \
        {                                      \
            static_assert(decltype(x){}, str); \
        }                                      \
        (std::false_type{});                   \
    } while(false)

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
        return ::scelta::visit(FWD(visitor),
            static_cast<meta::copy_cv_ptr_ref_t<Variants&&,
                impl::original_type_t<std::decay_t<Variants>>>>(variants)...);
    }

    namespace impl
    {
        template <typename Return, typename Visitor>
        struct recursor
        {
            Visitor& _visitor;

            constexpr recursor(Visitor& visitor) noexcept : _visitor{visitor}
            {
            }

            template <typename... Ts>
            constexpr Return operator()(Ts&&... xs)
            {
                return ::scelta::recursive::visit<Return>(_visitor, FWD(xs)...);
            }
        };

        template <typename Return, typename F>
        struct recursive_visitor : F
        {
            using this_type = recursive_visitor<Return, F>;
            using recursor_type = recursor<Return, this_type>;

            template <typename FFwd>
            constexpr recursive_visitor(FFwd&& f) noexcept(noexcept(F{FWD(f)}))
                : F{FWD(f)}
            {
            }

            template <typename... Ts>
            constexpr Return operator()(Ts&&... xs)
            {
                return static_cast<F&>(*this)(recursor_type{*this}, FWD(xs)...);
            }
        };
    }


    namespace impl
    {
        template <typename Return, typename... Fs>
        constexpr auto make_recursive_visitor(Fs&&... fs)
        {
            return meta::y_combinator([o = overload(FWD(fs)...)](
                auto self, auto&&... xs) mutable->Return {
                return o(
                    [&self](auto&&... vs) -> Return {
                        return ::scelta::recursive::visit<Return>(
                            self, FWD(vs)...);
                    },
                    FWD(xs)...);
            });
        }
    }

    template <typename Return, typename... Fs>
    constexpr auto match(Fs&&... fs)
    {
        return [rv = impl::make_recursive_visitor<Return>(FWD(fs)...)](
            auto&&... vs) mutable->Return
        {
            return ::scelta::recursive::visit<Return>(rv, FWD(vs)...);
        };
    }
}