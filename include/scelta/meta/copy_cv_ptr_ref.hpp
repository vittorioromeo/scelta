// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    /// @brief Removes cv-qualifiers and references from `T`.
    template <typename T>
    using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    /// @brief Removes cv-qualifiers, references, and pointers from `T`.
    template <typename T>
    using remove_cv_ref_ptr_t = std::remove_pointer_t<remove_cv_ref_t<T>>;

    /// @brief Applies `Source`'s lvalue reference to `T`.
    template <typename Source, typename T>
    using copy_lvalue_reference_t =
        std::conditional_t<std::is_lvalue_reference_v<Source>,
            std::add_lvalue_reference_t<T>, T>;

    /// @brief Applies `Source`'s rvalue reference to `T`.
    template <typename Source, typename T>
    using copy_rvalue_reference_t =
        std::conditional_t<std::is_rvalue_reference_v<Source>,
            std::add_rvalue_reference_t<T>, T>;

    /// @brief Applies `Source`'s reference to `T`.
    template <typename Source, typename T>
    using copy_reference_t =
        copy_lvalue_reference_t<Source, copy_rvalue_reference_t<Source, T>>;

    /// @brief Applies `Source`'s `const` to `T`.
    template <typename Source, typename T>
    using copy_const_t =
        std::conditional_t<std::is_const_v<Source>, std::add_const_t<T>, T>;

    /// @brief Applies `Source`'s `volatile` to `T`.
    template <typename Source, typename T>
    using copy_volatile_t = std::conditional_t<std::is_volatile_v<Source>,
        std::add_volatile_t<T>, T>;

    /// @brief Applies `Source`'s cv-qualifiers to `T`.
    template <typename Source, typename T>
    using copy_cv_t = copy_volatile_t<Source, copy_const_t<Source, T>>;

    /// @brief Applies `Source`'s pointer to `T`.
    template <typename Source, typename T>
    using copy_pointer_t =
        std::conditional_t<std::is_pointer_v<Source>, std::add_pointer_t<T>, T>;

    /// @brief Applies `Source`'s reference, pointer, and cv-qualifiers to `T`.
    template <typename Source, typename Destination>
    class copy_cv_ptr_ref
    {
    private:
        using S = std::remove_reference_t<Source>;
        using D = remove_cv_ref_ptr_t<Destination>;

        using U1 = copy_cv_t<S, D>;
        using U2 = copy_pointer_t<S, U1>;

    public:
        using type = copy_reference_t<Source, U2>;
    };

    /// @brief Type alias for `copy_cv_ptr_ref`.
    template <typename Source, typename Destination>
    using copy_cv_ptr_ref_t =
        typename copy_cv_ptr_ref<Source, Destination>::type;
}
