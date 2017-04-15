// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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
