#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <memory>
#include <scelta/recursive.hpp>
#include <scelta/support.hpp>

TEST_MAIN()
{
    using namespace test;

    // clang-format off
    with_all_variant_implementations(
        alternatives<int, char>,
        [](auto make)
        {
            {
                auto o = scelta::overload([](auto, int)  { return 0; },
                                          [](auto, char) { return 1; });

                auto v = scelta::recursive::impl::recursive_visitor<decltype(o)>(
                    std::move(o));

                EXPECT_EQ(v(0), 0);
                EXPECT_EQ(v('a'), 1);
            }

            {
                auto o = scelta::overload([](auto, int)           { return 0; },
                                          [&](auto recurse, char) { return recurse(make(0)); });

                auto v = scelta::recursive::impl::recursive_visitor<decltype(o)>(
                    std::move(o));
                
                EXPECT_EQ(v(0), 0);
                EXPECT_EQ(v('a'), 0);
            }
        }
    );
}