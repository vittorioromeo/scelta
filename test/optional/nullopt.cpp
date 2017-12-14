#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/support.hpp>
#include <scelta/utils.hpp>

TEST_MAIN()
{
    using namespace test;

    // clang-format off
    with_all_optional_implementations<int>( //
        [](auto make)                       //
        {
            using o_type = decltype(make());
            static_assert(std::is_convertible_v<scelta::nullopt_t, o_type>);
            static_assert(!std::is_convertible_v<scelta::nullopt_t, int>);
        });
    // clang-format on
}
