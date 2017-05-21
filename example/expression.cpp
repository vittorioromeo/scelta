// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./example_utils.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

// clang-format off
using num = int;
struct o_add { };
struct o_sub { };
struct o_mul { };
struct o_div { };
// clang-format on

using op = example::variant<o_add, o_sub, o_mul, o_div>;

namespace impl
{
    namespace sr = scelta::recursive;
    using _ = sr::placeholder;

    template <typename T>
    using recr_expr = std::tuple<num, op, _>;

    using builder =
        sr::builder<example::variant<num, std::unique_ptr<recr_expr<_>>>>;

    using type = sr::type<builder>;
    using resolved_recr_expr = sr::resolve<builder, recr_expr<_>>;
}

using expr = impl::type;
using recr_expr = impl::resolved_recr_expr;

template <typename Op, typename Rest>
auto recr(num x, Op o, Rest&& rest)
{
    return std::make_unique<recr_expr>(x, op{o}, FWD(rest));
}

int main()
{
    expr e = recr(2, o_mul{}, recr(5, o_add{}, recr(15, o_div{}, 3)));

    // clang-format off
    std::cout << scelta::recursive::match<num>(
        [](auto, const num& x)
        {
            return x;
        },
        [](auto recurse, const std::unique_ptr<recr_expr>& x)
        {
            const auto& lhs = std::get<0>(*x);
            const auto& op  = std::get<1>(*x);
            const auto& rhs = recurse(std::get<2>(*x));

            return scelta::match([&](o_add){ return lhs + rhs; },
                                 [&](o_sub){ return lhs - rhs; },
                                 [&](o_mul){ return lhs * rhs; },
                                 [&](o_div){ return lhs / rhs; })(op);
        })
        (e);
    // clang-format on
}
