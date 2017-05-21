// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./test_utils.hpp"
#include <scelta/utils/linear_overload.hpp>

TEST_MAIN()
{
    // nothing
    {
        auto f = scelta::linear_overload();
        (void)f;
    }

    // lvalue_0
    {
        auto l = [](int x) { return x + 1; };
        auto f = scelta::linear_overload(l);

        EXPECT_EQ(f(1), 2);
    }

    // lvalue_1
    {
        auto l0 = [](int) { return 0; };
        auto l1 = [](char) { return 1; };
        auto f = scelta::linear_overload(l0, l1);

        EXPECT_EQ(f(1), 0);
        EXPECT_EQ(f('a'), 0); // calls `int`!
    }

    // lvalue_makes_copy
    {
        auto l = [k = 0]() mutable
        {
            ++k;
            return k;
        };
        auto f = scelta::linear_overload(l);

        EXPECT_EQ(f(), 1);
        EXPECT_EQ(f(), 2);
        EXPECT_EQ(f(), 3);

        EXPECT_EQ(l(), 1);
    }

    // rvalue_0
    {
        auto f = scelta::linear_overload([](int x) { return x + 1; });

        EXPECT_EQ(f(1), 2);
    }

    // rvalue_1
    {
        auto f = scelta::linear_overload(
            [](int) { return 0; }, [](char) { return 1; });

        EXPECT_EQ(f(1), 0);
        EXPECT_EQ(f('a'), 0); // calls `int`!
    }

    // non-ref overload
    {
        auto l0([i = 0](float) mutable { return i++; });
        auto l1([i = 0](char) mutable { return i++; });

        auto f = scelta::linear_overload(l0, l1);

        EXPECT_EQ(f(0.f), 0);
        EXPECT_EQ(f(0.f), 1);
        EXPECT_EQ(f(0.f), 2);
        EXPECT_EQ(f('a'), 3); // calls float!
        EXPECT_EQ(f('a'), 4); // calls float!
        EXPECT_EQ(f('a'), 5); // calls float!

        EXPECT_EQ(l0(0.f), 0);
        EXPECT_EQ(l1('a'), 0);
    }

    struct a { void foo() { } };
    struct b { void bar() { } };
    struct c { void bar() { } };

    // random test 0
    {
        auto l0 = [](a){ return 0; };
        auto l1 = [](b){ return 1; };
        auto l2 = [](c){ return 2; };

        auto o = scelta::linear_overload(l0, l1, l2);

        EXPECT_EQ(o(a{}), 0);
        EXPECT_EQ(o(b{}), 1);
        EXPECT_EQ(o(c{}), 2);
    }

    // SFINAE 0
    {
        auto l0 = [](a){ return 0; };
        auto l1 = [](auto){ return 1; };
        auto l2 = [](auto){ return 2; };

        auto o = scelta::linear_overload(l0, l1, l2);

        EXPECT_EQ(o(a{}), 0);
        EXPECT_EQ(o(b{}), 1);
        EXPECT_EQ(o(c{}), 1);
    }

    // SFINAE 1
    {
        auto l0 = [](a){ return 0; };
        auto l1 = [](auto){ return 1; };
        auto l2 = [](auto x){ x.xxxINVALID(); return 2; };

        auto o = scelta::linear_overload(l0, l1, l2);

        EXPECT_EQ(o(a{}), 0);
        EXPECT_EQ(o(b{}), 1);
        EXPECT_EQ(o(c{}), 1);
    }
}
