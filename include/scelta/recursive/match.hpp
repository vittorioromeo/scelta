// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/y_combinator.hpp"
#include "../utils/fwd.hpp"
#include "../recursive/visit.hpp"
#include "./visit.hpp"

namespace scelta::recursive
{
    namespace impl
    {
        template <typename Return, typename... Fs>
        constexpr auto make_recursive_visitor(Fs&&... fs)
        {
            // clang-format off
            return meta::y_combinator(
                // Recurse on this lambda via Y combinator to pass it back
                // as part of the bound `recurse` argument.
                [o = overload(FWD(fs)...)](auto self, auto&&... xs) mutable
                    -> Return
                {
                    // Invoke the overload...
                    return o(

                        // Passing a visitor that invokes `self` to the
                        // `recurse` argument.
                        [&self](auto&&... vs) -> Return {
                            return ::scelta::recursive::visit<Return>(
                                self, FWD(vs)...);
                        },

                        // Forwarding the rest of the arguments.
                        FWD(xs)...);
                }
            );
            // clang-format on
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

// Note: I've spent countless hours trying to get the return types deduced and
// trying to add a `noexcept` clause to recursive visitation/matching.
// Unfortunately, due to the fact that lambdas do not have an explicit return
// type (i.e. they return `auto` - the compiler does not attempt to produce a
// return type with the body of the lambda) and due to other complications I've
// decided to scrap the idea and have the user explicitly specify the desired
// return type and avoid `noexcept` altogether.
