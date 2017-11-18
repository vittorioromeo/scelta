#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <scelta/recursive/experimental_match.hpp>
#include <scelta/visitation.hpp>

// clang-format off
struct a { int foo() { return 0; } };
struct b { int bar() { return 1; } };
struct c { int bar() { return 2; } };
// clang-format on

struct test0
{
    auto operator()(a x) const { return x.foo(); }

    template <typename T>
    auto operator()(T x) const { return x.bar(); }
};

struct test1_a
{
    auto operator()(a x) const { return x.foo(); }
};

struct test1_b
{
    template <typename T>
    auto operator()(T x) const { return x.bar(); }
};

struct test1 : test1_a, test1_b
{
    using test1_a::operator();
    using test1_b::operator();
};

TEST_MAIN()
{
    {
        test0 t;
        t(a{});
        t(b{});
        t(c{});
    }

    {
        test1 t;
        t(a{});
        t(b{});
        t(c{});
    }

    using namespace test;

    // clang-format off
    with_all_variant_implementations(
        alternatives<a, b, c>,
        [](auto make)
        {
            {
                auto v = make(a{});
                scelta::experimental::recursive::match(
                    [](a x) {                return x.foo(); },
                    [](b x) { EXPECT(false); return x.bar(); },
                    [](c x) { EXPECT(false); return x.bar(); })(v);

                test0 t0;
                scelta::visit(t0, v);

                test1 t1;
                scelta::visit(t1, v);

                auto t2 = scelta::overload(test1_a{}, test1_b{});
                scelta::visit(t2, v);

                /* TODO: ??? see temp/struct_vs_variant.cpp, report bug?
                // auto t3_a = [](a x)    {                return x.foo(); };
                // auto t3_b = [](auto x) { EXPECT(false); return x.bar(); };
                // auto t3 = scelta::overload(t3_a, t3_b);
                // scelta::visit(t3, v);

                This is gcc bug 80767, see:
                * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80767
                * http://stackoverflow.com/questions/43982799
                */

                scelta::experimental::recursive::match(
                    [](a x)                         {                return x.foo(); },
                    [](auto x) -> decltype(x.bar()) { EXPECT(false); return x.bar(); })(v);
            }

            {
                auto v = make(b{});
                scelta::experimental::recursive::match(
                    [](a x) { EXPECT(false); return x.foo(); },
                    [](b x) {                return x.bar(); },
                    [](c x) { EXPECT(false); return x.bar(); })(v);

                scelta::experimental::recursive::match(
                    [](a x)                         { EXPECT(false); return x.foo(); },
                    [](auto x) -> decltype(x.bar()) {                return x.bar(); })(v);
                // TODO: match linearly?
            }
        });
    // clang-format on
}
