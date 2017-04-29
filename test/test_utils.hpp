// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#ifndef FWD
#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#endif

#define TEST_CONST __attribute__((const))
#define TEST_MAIN(...) int TEST_CONST main(__VA_ARGS__)

#define TEST_IMPL_DEPARENS(...) __VA_ARGS__

#define SA_SAME(value, type) \
    static_assert(           \
        std::is_same<TEST_IMPL_DEPARENS value, TEST_IMPL_DEPARENS type>{}, "")

#define SA_NOT_SAME(value, type) \
    static_assert(           \
        !std::is_same<TEST_IMPL_DEPARENS value, TEST_IMPL_DEPARENS type>{}, "")

#define SA_TYPE(value, type) \
    static_assert(std::is_same<decltype value, TEST_IMPL_DEPARENS type>{}, "")

#define VRM_CORE_LIKELY(...) __builtin_expect(!!(__VA_ARGS__), 1)

namespace test_impl
{
    namespace impl
    {
        template <typename...>
        using void_t = void;

        template <typename T, typename = void>
        struct can_print_t : std::false_type
        {
        };

        template <typename T>
        struct can_print_t<T, void_t<decltype(std::declval<std::ostream&>()
                                              << std::declval<T>())>>
            : std::true_type
        {
        };

        template <typename T>
        constexpr can_print_t<T> can_print{};
    }

    namespace impl
    {
        inline auto& get_oss() noexcept
        {
            static std::ostringstream oss;
            return oss;
        }

        inline auto& clear_and_get_oss() noexcept
        {
            auto& oss(get_oss());
            oss.str("");
            return oss;
        }

        [[noreturn]] inline void fail() noexcept
        {
            std::cout << get_oss().str() << std::endl;
            std::terminate();
        }

        template <typename TStream>
        inline void output_line(TStream& s, int line)
        {
            s << "line: " << line << "\n";
        }

        template <typename TStream>
        inline void output_expr(TStream& s, const char* expr)
        {
            s << "expr: " << expr << "\n";
        }

        template <typename TStream, typename T>
        inline void output_result(
            TStream& s, const T& lhs_result, std::true_type)
        {
            s << "result: " << lhs_result << "\n";
        }

        template <typename TStream, typename T>
        inline void output_result(TStream&, const T&, std::false_type)
        {
        }

        template <typename TStream, typename T>
        inline void output_expected(TStream& s, const char* expected,
            const T& rhs_result, std::true_type)
        {
            s << "expected: " << expected << " -> " << rhs_result << "\n";
        }

        template <typename TStream, typename T>
        inline void output_expected(
            TStream&, const char*, const T&, std::false_type)
        {
        }

        template <typename TF>
        inline void do_test(bool x, TF&& f)
        {
            if(VRM_CORE_LIKELY(x)) return;

            auto& error(impl::clear_and_get_oss());
            f(error);
            impl::fail();
        }
    }

    template <typename T>
    inline void test_expr(
        int line, bool x, T&& lhs_result, const char* expr) noexcept
    {
        impl::do_test(x, [&](auto& s) {
            impl::output_line(s, line);
            impl::output_expr(s, expr);
            impl::output_result(s, lhs_result, impl::can_print<T>);
        });
    }

    template <typename TLhs, typename TRhs>
    inline void test_op(int line, bool x, TLhs&& lhs_result, TRhs&& rhs_result,
        const char* expr, const char* expected)
    {
        impl::do_test(x, [&](auto& s) {
            impl::output_line(s, line);
            impl::output_expr(s, expr);
            impl::output_result(s, lhs_result, impl::can_print<TLhs>);
            impl::output_expected(
                s, expected, rhs_result, impl::can_print<TRhs>);
        });
    }
}

#define EXPECT(expr)                                       \
    do                                                     \
    {                                                      \
        auto _t_x(expr);                                   \
                                                           \
        test_impl::test_expr(__LINE__, _t_x, expr, #expr); \
    } while(false)

#define EXPECT_OP(lhs, op, rhs)                                                \
    do                                                                         \
    {                                                                          \
        auto _t_xl(lhs);                                                       \
        auto _t_xr(rhs);                                                       \
                                                                               \
        auto _t_x((_t_xl) op (_t_xr));                                         \
                                                                               \
        test_impl::test_op(__LINE__, _t_x, _t_xl, _t_xr, #lhs #op #rhs, #rhs); \
    } while(false)

#define EXPECT_EQ(lhs, rhs) EXPECT_OP(lhs, ==, rhs)
#define EXPECT_NEQ(lhs, rhs) EXPECT_OP(lhs, !=, rhs)
#define EXPECT_LT(lhs, rhs) EXPECT_OP(lhs, <, rhs)
#define EXPECT_LTE(lhs, rhs) EXPECT_OP(lhs, <=, rhs)
#define EXPECT_GT(lhs, rhs) EXPECT_OP(lhs, >, rhs)
#define EXPECT_GTE(lhs, rhs) EXPECT_OP(lhs, >=, rhs)

#define EXPECT_TRUE(x) EXPECT_EQ(x, true)
#define EXPECT_FALSE(x) EXPECT_EQ(x, false)

#define SA_SAME_TYPE(T0, T1) static_assert(std::is_same<T0, T1>{})
#define SA_TYPE_IS(a, T) SA_SAME_TYPE(decltype(a), T)
#define SA_DECAY_TYPE_IS(a, T) SA_SAME_TYPE(std::decay_t<decltype(a)>, T)
