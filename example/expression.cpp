// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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
    using recr_expr = std::tuple<num, op, std::unique_ptr<_>>;

    using builder = sr::builder<example::variant<num, recr_expr<_>>>;

    using type = sr::type<builder>;
    using resolved_recr_expr = sr::resolve<builder, recr_expr<_>>;
}

using base_expr = impl::type;
using recr_expr = impl::resolved_recr_expr;

template <typename Op, typename Rest>
auto recr(num x, Op op, Rest&& rest)
{
    return std::make_unique<recr_expr>(x, op, FWD(rest));
}

int main()
{
}