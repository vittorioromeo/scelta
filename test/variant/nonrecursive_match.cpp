#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/nonrecursive/match.hpp>

TEST_MAIN()
{
    using namespace test;

    // clang-format off
    with_all_variant_implementations(   //
        alternatives<int, float, char>, //
        [](auto make)                   //
        {
            {
                auto v = make(1);
                scelta::nonrecursive::match(                          //
                    [](int x) { EXPECT_OP(x, ==, 1); }, //
                    [](float) { EXPECT(false); },       //
                    [](char)  { EXPECT(false); })(v);
            }

            {
                auto v = make(2.f);
                scelta::nonrecursive::match(                              //
                    [](int)     { EXPECT(false); },         //
                    [](float x) { EXPECT_OP(x, ==, 2.f); }, //
                    [](char)    { EXPECT(false); })(v);
            }

            {
                auto v = make('c');
                scelta::nonrecursive::match(                     //
                    [](int)    { EXPECT(false); }, //
                    [](float)  { EXPECT(false); }, //
                    [](char x) { EXPECT_OP(x, ==, 'c'); })(v);
            }


            {
                int a;
                int b;

                auto v = make(1);
                EXPECT_EQ(&a,                            //
                    &scelta::nonrecursive::match(                      //
                        [&](int)  -> int& { return a; }, //
                        [&](auto) -> int& { return b; })(v));
            }
        });
    // clang-format on
}
