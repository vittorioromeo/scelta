// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

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
                auto v = scelta::recursive::match<int>(
                                  [](auto, int) { return 0; },
                                  [](auto, char){ return 1; });

                auto vt = make(0);
                EXPECT_EQ(v(vt), 0);
            }

            {
                auto v = scelta::recursive::match<int>(
                                  [](auto, int) { return 0; },
                                  [](auto, char){ return 1; });

                auto vt = make('a');
                EXPECT_EQ(v(vt), 1);
            }
        }
    );
}
