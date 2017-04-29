// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "copy_cv_ptr_ref.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    namespace impl
    {
        template <typename T>
        using identity_check = T;

        template <typename, typename T>
        using identity_replace = T;

        template <                                            //
            template <typename> typename OnCheck,             //
            template <typename, typename> typename OnReplace, //
            typename Before, typename After,                  //
            typename T                                        //
            >
        struct apply : std::conditional<                       //
                           std::is_same_v<OnCheck<T>, Before>, //
                           OnReplace<T, After>,                //
                           T>
        {
        };

        template <                                            //
            template <typename> typename OnCheck,             //
            template <typename, typename> typename OnReplace, //
            typename Before, typename After,                  //
            template <typename...> typename T, typename... Ts //
            >
        struct apply<OnCheck, OnReplace, Before, After, T<Ts...>>
        {
            using type = T<
                typename apply<OnCheck, OnReplace, Before, After, Ts>::type...>;
        };

        template <                                           //
            template <typename> typename OnCheck,            //
            template <typename, typename> typename OnReplace //
            >
        struct replace_all_impl
        {
            template <typename Before, typename After, typename T>
            using apply_t =
                typename apply<OnCheck, OnReplace, Before, After, T>::type;
        };

        using identity_replacer =
            replace_all_impl<identity_check, identity_replace>;

        using copy_cv_ref_ptr_replacer =
            replace_all_impl<remove_cv_ref_ptr_t, copy_cv_ptr_ref_t>;
    }

    template <typename Before, typename After, typename T>
    using replace_all_t = impl::identity_replacer::apply_t<Before, After, T>;

    template <typename Before, typename After, typename T>
    using replace_all_copy_cv_ptr_ref_t =
        impl::copy_cv_ref_ptr_replacer::apply_t<Before, After, T>;
}