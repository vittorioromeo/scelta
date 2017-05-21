// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include <scelta/meta/y_combinator.hpp>

TEST_MAIN()
{
    auto fact = scelta::meta::y_combinator([](auto self, int n) -> int {
        if(n == 0)
        {
            return 1;
        }

        return n * self(n - 1);
    });

    static_assert(fact(3) == 3 * 2 * 1);
}
