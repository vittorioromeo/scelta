// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include <iostream>
#include <scelta/support.hpp>
#include <scelta/utils/variant_homogenizer.hpp>

TEST_MAIN()
{
#if defined(SCELTA_SUPPORT_STD_VARIANT)
    std::cout << scelta::impl::variant_homogenizer<::std::variant>::name
              << "\n";
#endif

#if defined(SCELTA_SUPPORT_BOOST_VARIANT)
    std::cout << scelta::impl::variant_homogenizer<::boost::variant>::name
              << "\n";
#endif

#if defined(SCELTA_SUPPORT_EGGS_VARIANT)
    std::cout << scelta::impl::variant_homogenizer<::eggs::variant>::name
              << "\n";
#endif
}
