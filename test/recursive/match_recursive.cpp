// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <memory>
#include <scelta/recursive.hpp>
#include <scelta/support.hpp>
#include <vector>

namespace sr = scelta::recursive;
using _ = sr::placeholder;

template <template <typename...> typename Variant>
struct test_case
{
    void run()
    {
        using builder = sr::builder<Variant<int, std::vector<_>>>;
        using type = sr::type<builder>;
        using r0 = sr::resolve<builder, std::vector<_>>;

        type v0{0};
        (void)v0;

        type v1{v0};
        (void)v1;

        type v2{r0{}};
        (void)v2;

        type v3{r0{v2}};
        (void)v3;

        type v4{r0{v3, type{0}}};
        (void)v4;

        {
            type v{1};
            int acc = 0;
            sr::match<void>([&](auto, int x) { acc += x; },
                            [&](auto recurse, r0& v) { for(auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 1);

            sr::match<void>([&](auto, int x) { acc += x; },
                            [&](auto recurse, const r0& v) { for(const auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 2);
        }

        {
            type v{r0{type{1}, type{1}}};
            int acc = 0;
            sr::match<void>([&](auto, int x) { acc += x; },
                            [&](auto recurse, r0& v) { for(auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 2);

            sr::match<void>([&](auto, int x) { acc += x; },
                            [&](auto recurse, const r0& v) { for(const auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 4);
        }
    }
};

TEST_MAIN()
{
    test::instantiate_with_all_variant_implementations<test_case>(
        [](auto x) { x.run(); });
}
