#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/support.hpp>
#include <scelta/utils.hpp>

TEST_MAIN()
{
    using namespace test;

    using null = scelta::nullopt_t;

    // clang-format off
    with_all_optional_implementations<int>( //
        [](auto make)                       //
        {
            auto make_other = [](auto x)
            {
                return ::scelta::make_like<decltype(make())>(x);
            };

            // map_or_else, set
            {
                auto f_def = [&]{ return 42; };
                auto f = [](int x){ return x + 1; };
                auto o = make(0);
                EXPECT_TRUE(scelta::map_or_else(o, f_def, f) == 1);
                EXPECT_TRUE((o | scelta::infix::map_or_else(f_def, f)) == 1);
            }

            // map_or_else, unset
            {
                auto f_def = [&]{ return 42; };
                auto f = [](int x){ return x + 1; };
                auto o = make();
                EXPECT_TRUE(scelta::map_or_else(o, f_def, f) == 42);
                EXPECT_TRUE((o | scelta::infix::map_or_else(f_def, f)) == 42);
            }

            // map_or_else with a different type
            {
                auto f_def = [&]{ return 'b'; };
                auto f = [](int){ return 'a'; };
                auto o = make(0);
                EXPECT_TRUE(scelta::map_or_else(o, f_def, f) == make_other('a'));
                EXPECT_TRUE((o | scelta::infix::map_or_else(f_def, f)) == make_other('a'));
            }

            // map_or, set
            {
                auto f = [](int x){ return x + 1; };
                auto o = make(0);
                EXPECT_TRUE(scelta::map_or(o, 42, f) == 1);
                EXPECT_TRUE((o | scelta::infix::map_or(42, f)) == 1);
            }

            // map_or, unset
            {
                auto f = [](int x){ return x + 1; };
                auto o = make();
                EXPECT_TRUE(scelta::map_or(o, 42, f) == 42);
                EXPECT_TRUE((o | scelta::infix::map_or(42, f)) == 42);
            }

            // map_or with a different type
            {
                auto f = [](int){ return 'a'; };
                auto o = make(0);
                EXPECT_TRUE(scelta::map_or(o, 'x', f) == make_other('a'));
                EXPECT_TRUE((o | scelta::infix::map_or('x', f)) == make_other('a'));
            }

            // map, set
            {
                auto f = [](int x){ return x + 1; };
                auto o = make(0);
                EXPECT_TRUE(scelta::map(o, f) == make(1));
                EXPECT_TRUE((o | scelta::infix::map(f)) == make(1));
            }

            // map, unset
            {
                auto f = [](int x){ return x + 1; };
                auto o = make();
                EXPECT_TRUE(scelta::map(o, f) == make());
                EXPECT_TRUE((o | scelta::infix::map(f)) == make());
            }

            // map to a different type
            {
                auto o = make(0);
                auto f = [](int){ return 'a'; };
                EXPECT_TRUE(scelta::map(o, f) == make_other('a'));
            }

            // and_then, set
            {
                auto f = [&](int x){ return make(x + 1); };
                auto o = make(0);
                EXPECT_TRUE(scelta::and_then(o, f) == make(1));
                EXPECT_TRUE((o | scelta::infix::and_then(f)) == make(1));
            }

            // and_then, unset
            {
                auto f = [&](int x){ return make(x + 1); };
                auto o = make();
                EXPECT_TRUE(scelta::and_then(o, f) == make());
                EXPECT_TRUE((o | scelta::infix::and_then(f)) == make());
            }

            // and_then with a different type
            {
                auto f = [&](int){ return make_other('a'); };
                auto o = make(0);
                EXPECT_TRUE(scelta::and_then(o, f) == make_other('a'));
                EXPECT_TRUE((o | scelta::infix::and_then(f)) == make_other('a'));
            }

            // and_, set
            {
                auto a = make(10);
                auto b = make_other('a');
                EXPECT_TRUE(scelta::and_(a, b) == b);
                EXPECT_TRUE((a | scelta::infix::and_(b)) == make_other('a'));
            }

            // and_, unset
            {
                auto a = make();
                auto b = make_other('a');
                EXPECT_TRUE(scelta::and_(a, b) == decltype(b){});
                EXPECT_TRUE((a | scelta::infix::and_(b)) == decltype(b){});
            }

            // or_else, set
            {
                auto a = make(10);
                auto f = []{ return 42; };
                EXPECT_TRUE(scelta::or_else(a, f) == a);
                EXPECT_TRUE((a | scelta::infix::or_else(f)) == a);
            }

            // or_else, unset
            {
                auto a = make();
                auto f = []{ return 42; };
                EXPECT_TRUE(scelta::or_else(a, f) == f());
                EXPECT_TRUE((a | scelta::infix::or_else(f)) == f());
            }

            // or_, set
            {
                auto a = make(10);
                EXPECT_TRUE(scelta::or_(a, 42) == a);
                EXPECT_TRUE((a | scelta::infix::or_(42)) == a);
            }

            // or_, unset
            {
                auto a = make();
                EXPECT_TRUE(scelta::or_(a, 42) == 42);
                EXPECT_TRUE((a | scelta::infix::or_(42)) == 42);
            }
        });
    // clang-format on
}
