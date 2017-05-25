// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <memory>
#include <scelta/recursive.hpp>
#include <scelta/recursive/experimental_match.hpp>
#include <scelta/support.hpp>
#include <vector>

namespace sr = scelta::recursive;
namespace ser = scelta::experimental::recursive;
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
            ser::match<void>([&](int x) { acc += x; })(
                             [&](auto recurse, r0& v) { for(auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 1);

            ser::match<void>([&](int x) { acc += x; })(
                             [&](auto recurse, const r0& v) { for(const auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 2);
        }

        {
            type v{r0{type{1}, type{1}}};
            int acc = 0;
            ser::match<void>([&](int x) { acc += x; })(
                             [&](auto recurse, r0& v) { for(auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 2);

            ser::match([&](int x) { acc += x; })(
                       [&](auto recurse, const r0& v) { for(const auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 4);

            ser::match([&](int x) { acc += x; })(
                       [&](auto recurse, const auto& v) { for(const auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 6);

/* TODO: ?
            ser::match([&](auto x) { acc += x; })(
                       [&](auto recurse, const r0& v) { for(const auto& x : v) recurse(x); })(v);

            EXPECT_EQ(acc, 8);
*/
        }

        {
            using nrv = Variant<int, char>;

            {
                nrv v0{0};
                EXPECT_EQ(ser::match([](int) { return 0; },
                                     [](char){ return 1; })(v0), 0);
            }

            {
                nrv v0{'a'};
                EXPECT_EQ(ser::match([](int) { return 0; },
                                     [](char){ return 1; })(v0), 1);
            }

            {
                nrv v0{0};
                EXPECT_EQ(ser::match([](auto) { return 0; },
                                     [](char){ return 1; })(v0), 0);
            }

            {
                nrv v0{'a'};
                EXPECT_EQ(ser::match([](std::vector<int>) { return 0; },
                                     [](auto){ return 1; })(v0), 1);
            }
        }
    }
};

TEST_MAIN()
{
    test::instantiate_with_all_variant_implementations<test_case>(
        [](auto x) { x.run(); });
}
