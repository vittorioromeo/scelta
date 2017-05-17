#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/visitation.hpp>

struct a
{
};
struct b
{
};

#define MAKE_AND_EXPECT(t0, t1, expc)              \
    {                                              \
        auto v0 = make(t0{});                      \
        auto v1 = make(t1{});                      \
        EXPECT_EQ(scelta::visit(f, v0, v1), expc); \
    }

TEST_MAIN()
{
    using namespace test;

    with_all_variant_implementations( //
        alternatives<a, b>,           //
        [](auto make)                 //
        {
            {
                auto f = scelta::overload(  //
                    [](a, a) { return 0; }, //
                    [](a, b) { return 1; }, //
                    [](b, b) { return 2; }, //
                    [](b, a) { return 3; });

                MAKE_AND_EXPECT(a, a, 0);
                MAKE_AND_EXPECT(a, b, 1);
                MAKE_AND_EXPECT(b, b, 2);
                MAKE_AND_EXPECT(b, a, 3);
            }
        });
}
