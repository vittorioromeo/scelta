// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include <scelta/meta/forward_like.hpp>

int test_int;
float test_float;

SA_TYPE((std::forward<int>(int{})), (int&&));
SA_TYPE((std::forward<int&&>(int{})), (int&&));
SA_TYPE((std::forward<int>((int&&)(1))), (int&&));
SA_TYPE((std::forward<int&&>((int&&)(1))), (int&&));

SA_TYPE((scelta::meta::forward_like<int>(float{})), (float&&));
SA_TYPE((scelta::meta::forward_like<int&&>(float{})), (float&&));
SA_TYPE((scelta::meta::forward_like<int>((float&&)(1))), (float&&));
SA_TYPE((scelta::meta::forward_like<int&&>((float&&)(1))), (float&&));

SA_TYPE((std::forward<int&>(test_int)), (int&));
SA_TYPE((scelta::meta::forward_like<int&>(test_float)), (float&));

SA_TYPE((std::forward<int>(test_int)), (int&&));
SA_TYPE((std::forward<int&&>(test_int)), (int&&));

SA_TYPE((scelta::meta::forward_like<int>(test_float)), (float&&));
SA_TYPE((scelta::meta::forward_like<int&&>(test_float)), (float&&));

template <int>
struct nocopy
{
    nocopy() = default;
    nocopy(const nocopy&) = delete;
    nocopy(nocopy&&) = default;
};

template <typename T>
void g(T)
{
}

template <typename T0, typename T1>
void f(T0&& x0, T1&& x1)
{
    g(std::forward<T0>(x0));
    g(std::forward<T1>(x1));
    g(scelta::meta::forward_like<T0>(x1));
    g(scelta::meta::forward_like<T1>(x0));
}

TEST_MAIN()
{
    f(nocopy<0>{}, nocopy<1>{});

    return 0;
}
