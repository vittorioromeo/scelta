// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./test_utils.hpp"

struct unprintable
{
    auto operator==(unprintable)
    {
        return true;
    }
};

TEST_MAIN()
{
    int a = 5;
    EXPECT_EQ(5, a);

    EXPECT_EQ(unprintable{}, unprintable{});
}
