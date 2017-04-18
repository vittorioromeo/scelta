#include "../test_utils.hpp"
#include <memory>
#include <scelta/recursive.hpp>
#include <scelta/support.hpp>

TEST_MAIN()
{
    using _ = scelta::recursive::placeholder;

    {
        using builder = scelta::recursive::builder<std::variant<int>>;
        using type = scelta::recursive::type<builder>;
        static_assert(std::is_base_of_v<std::variant<int>, type>);
    }

    {
        using builder = scelta::recursive::builder<std::variant<int, _*>>;
        using type = scelta::recursive::type<builder>;
        using r0 = scelta::recursive::resolve<builder, _*>;

        type v0{0};
        (void)v0;

        type v1{&v0};
        (void)v1;
    }
}