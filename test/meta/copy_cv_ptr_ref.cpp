// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test_utils.hpp"
#include <scelta/meta/copy_cv_ptr_ref.hpp>

struct A
{
};
struct B
{
};

TEST_MAIN()
{
    using namespace scelta::meta;

    // Sanity check.
    SA_SAME((A), (A));

    SA_SAME((copy_cv_ptr_ref_t<A, A>), (A));
    SA_SAME((copy_cv_ptr_ref_t<A*, A>), (A*));
    SA_SAME((copy_cv_ptr_ref_t<A&, A>), (A&));

    SA_SAME((copy_cv_ptr_ref_t<A, B>), (B));
    SA_SAME((copy_cv_ptr_ref_t<A*, B>), (B*));
    SA_SAME((copy_cv_ptr_ref_t<A&, B>), (B&));

    SA_SAME((copy_cv_ptr_ref_t<A, const B&&>), (B));
    SA_SAME((copy_cv_ptr_ref_t<A*, const B&&>), (B*));
    SA_SAME((copy_cv_ptr_ref_t<A&, const B&&>), (B&));

    SA_SAME((copy_cv_ptr_ref_t<A, B>), (B));
    SA_SAME((copy_cv_ptr_ref_t<A const, B>), (B const));
    SA_SAME((copy_cv_ptr_ref_t<volatile A, B>), (volatile B));
    SA_SAME((copy_cv_ptr_ref_t<volatile A const, B>), (volatile B const));
    SA_SAME((copy_cv_ptr_ref_t<A&, B>), (B&));
    SA_SAME((copy_cv_ptr_ref_t<A const&, B>), (B const&));
    SA_SAME((copy_cv_ptr_ref_t<volatile A&, B>), (volatile B&));
    SA_SAME((copy_cv_ptr_ref_t<volatile A const&, B>), (volatile B const&));
    SA_SAME((copy_cv_ptr_ref_t<A&&, B>), (B &&));
    SA_SAME((copy_cv_ptr_ref_t<A const&&, B>), (B const&&));
    SA_SAME((copy_cv_ptr_ref_t<volatile A&&, B>), (volatile B &&));
    SA_SAME((copy_cv_ptr_ref_t<volatile A const&&, B>), (volatile B const&&));
}
