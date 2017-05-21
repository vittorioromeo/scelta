// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/support.hpp>

TEST_MAIN()
{
    test::with_all_variant_implementations(   //
        test::alternatives<int, float, char>, //
        [](auto make)                         //
        {
            {
                auto v = make(1);
                (void)v;
            }

            {
                auto v = make(1.0f);
                (void)v;
            }

            {
                auto v = make('a');
                (void)v;
            }
        });
}
