// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include "../variant_test_utils.hpp"
#include <memory>
#include <vector>
#include <scelta/recursive.hpp>
#include <scelta/support.hpp>

namespace sr = scelta::recursive;
using _ = sr::placeholder;

template <template <typename...> typename Optional>
struct test_case
{
    void run()
    {
        {
            using builder = sr::builder<Optional<int>>;
            using type = sr::type<builder>;
            static_assert(std::is_base_of_v<Optional<int>, type>);
        }

        {
            using builder =
                sr::builder<Optional<std::vector<std::pair<int, _>>>>;
            using type = sr::type<builder>;
            using r0 = sr::resolve<builder, std::vector<std::pair<int, _>>>;

            type v0{type{}};
            (void)v0;

            type v1{r0{}};
            (void)v1;

            type v2{r0{std::make_pair(0, v0)}};
            (void)v2;
        }
    }
};

TEST_MAIN()
{
    test::instantiate_with_all_optional_implementations<test_case>(
        [](auto x) { x.run(); });
}
