#include "./test_utils.hpp"
#include <scelta/support.hpp>
#include <scelta/utils.hpp>

namespace test
{
    namespace impl
    {
        template <typename T>
        struct type_w_t
        {
            using type = T;
        };
    }

    template <typename... Ts>
    constexpr impl::type_w_t<Ts...> type_w{};

    namespace impl
    {
        template <typename... Ts>
        struct alternatives_t
        {
        };

        template <typename>
        struct unpack_alternatives;

        template <typename... Ts>
        struct unpack_alternatives<alternatives_t<Ts...>>
        {
            template <template <typename...> class T>
            using apply = T<Ts...>;
        };

        template <typename T>
        struct maker_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) const
                SCELTA_RETURNS(T(FWD(xs)...))
        };
    }

    template <typename... Ts>
    constexpr impl::alternatives_t<Ts...> alternatives{};

    template <typename TAlternatives, typename TF>
    void with_all_variant_implementations(TAlternatives, TF&& f)
    {
        using helper = impl::unpack_alternatives<TAlternatives>;

#if defined(SCELTA_SUPPORT_STD_VARIANT)
        using v_std = typename helper::template apply<std::variant>;
        f(impl::maker_t<v_std>{});
#endif

#if defined(SCELTA_SUPPORT_BOOST_VARIANT)
        using v_boost = typename helper::template apply<boost::variant>;
        f(impl::maker_t<v_boost>{});
#endif

#if defined(SCELTA_SUPPORT_EGGS_VARIANT)
        using v_eggs = typename helper::template apply<eggs::variant>;
        f(impl::maker_t<v_eggs>{});
#endif

        // using v_typesafe = typename helper::template
        // apply<type_safe::variant>;

        // TODO:
        // f(impl::maker_t<v_typesafe>{});
    }
}