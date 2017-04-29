// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// TODO: docs refactoring etc

#pragma once

#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    template <typename T>
    using remove_cv_ref_ptr_t =
        std::remove_pointer_t<std::remove_cv_t<std::remove_reference_t<T>>>;


    template <typename Source, typename T>
    using copy_lvalue_reference_t =
        std::conditional_t<std::is_lvalue_reference_v<Source>,
            std::add_lvalue_reference_t<T>, T>;

    template <typename Source, typename T>
    using copy_rvalue_reference_t =
        std::conditional_t<std::is_rvalue_reference_v<Source>,
            std::add_rvalue_reference_t<T>, T>;

    template <typename Source, typename T>
    using copy_reference_t =
        copy_lvalue_reference_t<Source, copy_rvalue_reference_t<Source, T>>;

    template <typename Source, typename T>
    using copy_const_t =
        std::conditional_t<std::is_const_v<Source>, std::add_const_t<T>, T>;

    template <typename Source, typename T>
    using copy_volatile_t = std::conditional_t<std::is_volatile_v<Source>,
        std::add_volatile_t<T>, T>;

    template <typename Source, typename T>
    using copy_pointer_t =
        std::conditional_t<std::is_pointer_v<Source>, std::add_pointer_t<T>, T>;

    template <typename Source, typename Destination>
    class copy_cv_ptr_ref
    {
    private:
        using S = std::remove_reference_t<Source>;
        using D = remove_cv_ref_ptr_t<Destination>;

        using U1 = copy_const_t<S, D>;
        using U2 = copy_volatile_t<S, U1>;
        using U3 = copy_pointer_t<S, U2>;
        using U4 = copy_reference_t<Source, U3>;

    public:
        using type = U4;
    };

    template <typename Source, typename Destination>
    using copy_cv_ptr_ref_t =
        typename copy_cv_ptr_ref<Source, Destination>::type;
}