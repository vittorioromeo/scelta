// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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
