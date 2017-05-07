// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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
                auto v = scelta::recursive::impl::make_recursive_visitor<int>(
                                  [](auto, int) { return 0; },
                                  [](auto, char){ return 1; });

                auto vt = make(0);
                EXPECT_EQ(scelta::recursive::visit<int>(v, vt), 0);
            }

            {
                auto v = scelta::recursive::impl::make_recursive_visitor<int>(
                                  [](auto, int) { return 0; },
                                  [](auto, char){ return 1; });

                auto vt = make('a');
                EXPECT_EQ(scelta::recursive::visit<int>(v, vt), 1);
            }
        }
    );
}