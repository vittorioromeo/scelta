// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include <scelta/support/eggs.hpp>

TEST_MAIN()
{
#if defined(SCELTA_SUPPORT_EGGS_VARIANT)
    static_assert(scelta::support::eggs);
#else
    static_assert(!scelta::support::eggs);
#endif
}
