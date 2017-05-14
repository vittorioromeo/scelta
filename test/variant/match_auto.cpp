#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/visitation.hpp>

// clang-format off
struct a { int foo() { return 0; } };
struct b { int bar() { return 1; } };
struct c { int bar() { return 2; } };
// clang-format on

TEST_MAIN()
{
    using namespace test;

    // clang-format off
    with_all_variant_implementations(
        alternatives<a, b, c>,
        [](auto make)
        {
            {
                auto v = make(a{});
                scelta::match(
                    [](a x) {                return x.foo(); },
                    [](b x) { EXPECT(false); return x.bar(); },
                    [](c x) { EXPECT(false); return x.bar(); })(v);

                scelta::match(
                    [](a x)                         {                return x.foo(); },
                    [](auto x) -> decltype(x.bar()) { EXPECT(false); return x.bar(); })(v);
            }

            {
                auto v = make(b{});
                scelta::match(
                    [](a x) { EXPECT(false); return x.foo(); },
                    [](b x) {                return x.bar(); },
                    [](c x) { EXPECT(false); return x.bar(); })(v);

                scelta::match(
                    [](a x)                         { EXPECT(false); return x.foo(); },
                    [](auto x) -> decltype(x.bar()) {                return x.bar(); })(v);
                // TODO: match linearly?
            }
        });
    // clang-format on
}