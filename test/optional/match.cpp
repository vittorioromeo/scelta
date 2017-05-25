#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/visitation.hpp>
#include <scelta/traits/adt/alternatives.hpp>

TEST_MAIN()
{
    using namespace test;

    using null = scelta::nullopt_t;

    // clang-format off
    with_all_optional_implementations<int>( //
        [](auto make)                       //
        {
            static_assert(std::is_same_v<
                scelta::traits::adt::nth_alternative<std::decay_t<decltype(make(0))>, 0>,
                int>);

            {
                auto f = scelta::match(     //
                    [](null) { return 0; }, //
                    [](int)  { return 1; });

                EXPECT_EQ(f(make()),  0);
                EXPECT_EQ(f(make(0)), 1);
            }

            {
                auto f = scelta::match(           //
                    [](null, null) { return 0; }, //
                    [](null, int)  { return 1; }, //
                    [](int,  null) { return 2; }, //
                    [](int,  int)  { return 3; });

                EXPECT_EQ(f(make(),  make()),  0);
                EXPECT_EQ(f(make(),  make(0)), 1);
                EXPECT_EQ(f(make(0), make()),  2);
                EXPECT_EQ(f(make(0), make(0)), 3);
            }
        });
    // clang-format on
}
