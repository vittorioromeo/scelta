#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <memory>
#include <scelta/recursive.hpp>
#include <scelta/support.hpp>

namespace sr = scelta::recursive;
using _ = sr::placeholder;

template <template <typename...> typename Variant>
struct test_case
{
    void run()
    {
        {
            using builder = sr::builder<Variant<int>>;
            using type = sr::type<builder>;
            static_assert(std::is_base_of_v<Variant<int>, type>);
        }

        {
            using builder = sr::builder<Variant<int, _*>>;
            using type = sr::type<builder>;
            using r0 = sr::resolve<builder, _*>;

            type v0{0};
            (void)v0;

            type v1{&v0};
            (void)v1;
        }
    }
};

TEST_MAIN()
{
    test::instantiate_with_all_variant_implementations<test_case>(
        [](auto x) { x.run(); });
}