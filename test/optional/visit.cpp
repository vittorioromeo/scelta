#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/visitation.hpp>

TEST_MAIN()
{
    using namespace test;

    using null = scelta::nullopt_t;

    // clang-format off
    with_all_optional_implementations<int>( //
        [](auto make)                       //
        {
            {
                auto f = scelta::overload(  //
                    [](null) { return 0; }, //
                    [](int)  { return 1; });

                EXPECT_EQ(scelta::visit(f, make()),  0);
                EXPECT_EQ(scelta::visit(f, make(0)), 1);
            }

            {
                auto f = scelta::overload(        //
                    [](null, null) { return 0; }, //
                    [](null, int)  { return 1; }, //
                    [](int,  null) { return 2; }, //
                    [](int,  int)  { return 3; });

                EXPECT_EQ(scelta::visit(f, make(),  make()),  0);
                EXPECT_EQ(scelta::visit(f, make(),  make(0)), 1);
                EXPECT_EQ(scelta::visit(f, make(0), make()),  2);
                EXPECT_EQ(scelta::visit(f, make(0), make(0)), 3);
            }

            {
                int a = 0;
                int b = 0;
                auto f = scelta::overload(  //
                    [&](null) -> int& { return a; }, //
                    [&](int)  -> int& { return b; });

                EXPECT_EQ(&scelta::visit(f, make()),  &a);
                EXPECT_EQ(&scelta::visit(f, make(0)), &b);
            }
        });
    // clang-format on
}
