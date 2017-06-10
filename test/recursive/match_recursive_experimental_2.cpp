// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include "../tracked_object.hpp"
#include "../variant_test_utils.hpp"
#include <memory>
#include <scelta/recursive.hpp>
#include <scelta/recursive/experimental_match.hpp>
#include <scelta/support.hpp>
#include <vector>

namespace sr = scelta::recursive;
namespace ser = scelta::experimental::recursive;
using _ = sr::placeholder;

struct f
{
    template <typename... Ts>
    constexpr auto operator()(Ts&&...) const noexcept
    {
    }
};

template <template <typename...> typename Variant>
struct test_case
{
    using builder = sr::builder<Variant<int, std::vector<_>>>;
    using type = sr::type<builder>;
    using r0 = sr::resolve<builder, std::vector<_>>;

    using builder_ic = sr::builder<Variant<int, char, std::vector<_>>>;
    using type_ic = sr::type<builder_ic>;
    using r0_ic = sr::resolve<builder_ic, std::vector<_>>;


    void run()
    {
        // none
        {
            // type v0{0};
            // TODO?
        }

        // concrete single base case, no recursive cases
        {
            Variant<int> v0{0};
            auto m = ser::match([](int){ return 0; });
            EXPECT_EQ(m(v0), 0);
        }

        // concrete multiple base cases, no recursive cases
        {
            Variant<int, char> v0{0};
            Variant<int, char> v1{'a'};

            auto m = ser::match([](int) { return 0; },
                                [](char){ return 1; });

            EXPECT_EQ(m(v0), 0);
            EXPECT_EQ(m(v1), 1);
        }

        // auto single base case, no recursive cases
        {
            Variant<int, char> v0{0};
            Variant<int, char> v1{'a'};

            auto m = ser::match([](auto) { return 0; });

            EXPECT_EQ(m(v0), 0);
            EXPECT_EQ(m(v1), 0);
        }

        // auto multiple base cases, no recursive cases
        {
            Variant<int, char> v0{0};
            Variant<int, char> v1{'a'};

            auto m = ser::match([](auto x) -> std::enable_if_t<std::is_same_v<int, decltype(x)>, int> { return 0; },
                                [](auto x) -> std::enable_if_t<std::is_same_v<char, decltype(x)>, int> { return 1; });

            EXPECT_EQ(m(v0), 0);
            EXPECT_EQ(m(v1), 1);
        }

        // concrete single base case, single concrete recursive case
        {
            type v0{42};
            type v1{r0{1}};

            auto m = ser::match([](int x){ return x; })
                               ([](auto recurse, const r0& x){ return recurse(x[0]); });

            EXPECT_EQ(m(v0), 42);
            EXPECT_EQ(m(v1), 1);
        }

        // concrete multiple base cases, single concrete recursive case
        {
            type_ic v0{42};
            type_ic v1{'a'};
            type_ic v2{r0_ic{43}};
            type_ic v3{r0_ic{'b'}};

            auto m = ser::match([](int x) { return x; },
                                [](char x){ return (int)x; })
                               ([](auto recurse, const r0_ic& x){ return recurse(x[0]); });

            EXPECT_EQ(m(v0), 42);
            EXPECT_EQ(m(v1), (int)'a');
            EXPECT_EQ(m(v2), 43);
            EXPECT_EQ(m(v3), (int)'b');
        }

        // auto single base case, single concrete recursive case
        {
            type_ic v0{42};
            type_ic v1{'a'};
            type_ic v2{r0_ic{43}};
            type_ic v3{r0_ic{'b'}};

            auto m = ser::match([](auto x) -> decltype((int)x) { return (int)x; }) // fails without constraint because this is (auto, auto)
                               ([](auto recurse, const r0_ic& x){ return recurse(x[0]); });

            EXPECT_EQ(m(v0), 42);
            EXPECT_EQ(m(v1), (int)'a');
            EXPECT_EQ(m(v2), 43);
            EXPECT_EQ(m(v3), (int)'b');
        }

        // auto multiple base cases, single concrete recursive case
        {
            type_ic v0{42};
            type_ic v1{'a'};
            type_ic v2{r0_ic{43}};
            type_ic v3{r0_ic{'b'}};

            auto m = ser::match([](auto x) -> std::enable_if_t<std::is_same_v<decltype(x), int>, int>  { return x; },
                                [](auto x) -> std::enable_if_t<std::is_same_v<decltype(x), char>, int> { return x; })
                               ([](auto recurse, const r0_ic& x){ return recurse(x[0]); });

            EXPECT_EQ(m(v0), 42);
            EXPECT_EQ(m(v1), (int)'a');
            EXPECT_EQ(m(v2), 43);
            EXPECT_EQ(m(v3), (int)'b');
        }

        // value category testing (no recursive case)
        // make sure that currying and calling the curried match doesn't affect the visitors
        {
            Variant<int> v0{42};

            testing::check_operations([](auto& ctx)
            {
                auto m = ser::match(ctx.template make_tracked_object<f>("A"));
            }).expect_that("A").ctors(1)
                               .no_copies()
                               .moves(2)
                               .dtors(3);

            testing::check_operations([&](auto& ctx)
            {
                auto m = ser::match(ctx.template make_tracked_object<f>("A"));
                m(v0);
            }).expect_that("A").ctors(1)
                               .no_copies()
                               .moves(2)
                               .dtors(3);

            testing::check_operations([&](auto& ctx)
            {
                auto m = ser::match(ctx.template make_tracked_object<f>("A"));
                m(v0);
                m(v0);
                m(v0);
                m(v0);
            }).expect_that("A").ctors(1)
                               .no_copies()
                               .moves(2)
                               .dtors(3);
        }

        // value category testing (recursive case)
        {
            testing::check_operations([&](auto& ctx)
            {
                type v0{42};

                auto m = ser::match(ctx.template make_tracked_object<f>("A"))
                                   (ctx.template make_tracked_object<f>("B"));
            }).expect_that("A").ctors(1)
                               .no_copies()
                               .moves(5)
                               .dtors(6)
              .expect_that("B").ctors(1)
                               .no_copies()
                               .moves(2)
                               .dtors(3);

            testing::check_operations([&](auto& ctx)
            {
                type v0{42};

                auto m = ser::match(ctx.template make_tracked_object<f>("A"))
                                   (ctx.template make_tracked_object<f>("B"));

                m(v0);
            }).expect_that("A").ctors(1)
                               .no_copies()
                               .moves(5)
                               .dtors(6)
              .expect_that("B").ctors(1)
                               .no_copies()
                               .moves(2)
                               .dtors(3);

            testing::check_operations([&](auto& ctx)
            {
                type v0{42};

                auto m = ser::match(ctx.template make_tracked_object<f>("A"))
                                   (ctx.template make_tracked_object<f>("B"));

                m(v0);
                m(v0);
                m(v0);
                m(v0);
                m(v0);
            }).expect_that("A").ctors(1)
                               .no_copies()
                               .moves(5)
                               .dtors(6)
              .expect_that("B").ctors(1)
                               .no_copies()
                               .moves(2)
                               .dtors(3);
        }

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
                [&](auto recurse, r0& v) {
                    for(auto& x : v) recurse(x);
                })(v);

            EXPECT_EQ(acc, 1);

            ser::match<void>([&](int x) { acc += x; })(
                [&](auto recurse, const r0& v) {
                    for(const auto& x : v) recurse(x);
                })(v);

            EXPECT_EQ(acc, 2);
        }

        {
            type v{r0{type{1}, type{1}}};
            int acc = 0;
            ser::match<void>([&](int x) { acc += x; })(
                [&](auto recurse, r0& v) {
                    for(auto& x : v) recurse(x);
                })(v);

            EXPECT_EQ(acc, 2);

            ser::match([&](int x) { acc += x; })(
                [&](auto recurse, const r0& v) {
                    for(const auto& x : v) recurse(x);
                })(v);

            EXPECT_EQ(acc, 4);

            ser::match([&](int x) { acc += x; })(
                [&](auto recurse, const auto& v) {
                    for(const auto& x : v) recurse(x);
                })(v);

            EXPECT_EQ(acc, 6);

            /* TODO: ?
                        ser::match([&](auto x) { acc += x; })(
                                   [&](auto recurse, const r0& v) { for(const
               auto& x : v) recurse(x); })(v);

                        EXPECT_EQ(acc, 8);
            */
        }

        {
            using nrv = Variant<int, char>;

            {
                nrv v0{0};
                EXPECT_EQ(ser::match([](int) { return 0; },
                              [](char) { return 1; })(v0),
                    0);
            }

            {
                nrv v0{'a'};
                EXPECT_EQ(ser::match([](int) { return 0; },
                              [](char) { return 1; })(v0),
                    1);
            }

            {
                nrv v0{0};
                EXPECT_EQ(ser::match([](auto) { return 0; },
                              [](char) { return 1; })(v0),
                    0);
            }

            {
                nrv v0{'a'};
                EXPECT_EQ(ser::match([](std::vector<int>) { return 0; },
                              [](auto) { return 1; })(v0),
                    1);
            }
        }
    }
};

TEST_MAIN()
{
    test::instantiate_with_all_variant_implementations<test_case>(
        [](auto x) { x.run(); });
}
