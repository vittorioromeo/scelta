#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <memory>
#include <scelta/recursive.hpp>
#include <scelta/support.hpp>

TEST_MAIN()
{
    #if 0
    using namespace test;

    // clang-format off
    with_all_variant_implementations(
        alternatives<int, char>,
        [](auto make)
        {
            {
auto v = scelta::recursive::match([](auto, int) { return 0; },
                                             [](auto, char){ return 1; });

                EXPECT_EQ(v
                    (make(0)), 0);
            }

            {
auto v = scelta::recursive::match([](auto, int) { return 0; },
                                             [](auto, char){ return 1; });

                EXPECT_EQ(v
                    (make('a')), 1);
            }
        }
    );
    #endif
}