// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./test_utils.hpp"
#include <iostream>
#include <map>
#include <string>

namespace testing
{
    namespace impl
    {
        auto& get_ostream() noexcept
        {
            return std::cout;
        }

        using obj_id = std::string;

        struct stats
        {
            int _ctors = 0;
            int _dtors = 0;
            int _copies = 0;
            int _moves = 0;
        };

        using stat_map = std::map<obj_id, stats>;

        class operation_context;

        template <typename T>
        class tracked_object : public T
        {
        private:
            operation_context* _ctx;
            obj_id _id;

            auto& get_stats();

            void inc_ctor() noexcept
            {
                ++(get_stats()._ctors);
            }

            void inc_dtor() noexcept
            {
                ++(get_stats()._dtors);
            }

            void inc_copy() noexcept
            {
                ++(get_stats()._copies);
            }

            void inc_move() noexcept
            {
                ++(get_stats()._moves);
            }


        public:
            template <typename... Ts>
            tracked_object(operation_context& ctx, obj_id id, Ts&&... xs)
                : _ctx{&ctx}, _id{std::move(id)}, T(FWD(xs)...)
            {
                get_ostream() << _id << "()\n";
                inc_ctor();
            }

            ~tracked_object()
            {
                get_ostream() << "~" << _id << "()\n";
                inc_dtor();
            }


            tracked_object(const tracked_object& rhs)
                : _ctx{rhs._ctx}, _id{rhs._id}, T{static_cast<const T&>(rhs)}
            {
                get_ostream() << _id << "(const" << _id << "&)\n";
                inc_copy();
            }

            tracked_object& operator=(const tracked_object& rhs)
            {
                _ctx = rhs._ctx;
                _id = rhs._id;
                static_cast<T&>(*this) = static_cast<T&>(rhs);

                get_ostream()
                    << "tracked_object& operator=(const " << _id << "&)\n";
                inc_copy();
                return *this;
            }

            tracked_object(tracked_object&& rhs)
                : _ctx{std::move(rhs._ctx)}, _id{rhs._id}, T{static_cast<T&&>(
                                                               rhs)}
            {
                get_ostream() << _id << "(" << _id << "&&)\n";
                inc_move();
            }

            tracked_object& operator=(tracked_object&& rhs)
            {
                _ctx = std::move(rhs._ctx);
                _id = std::move(rhs._id);
                static_cast<T&>(*this) = static_cast<T&&>(rhs);

                get_ostream() << _id << "& operator=(" << _id << "&&)\n";
                inc_move();
                return *this;
            }

            auto& get() noexcept
            {
                return static_cast<T&>(*this);
            }

            const auto& get() const noexcept
            {
                return static_cast<const T&>(*this);
            }
        };

        namespace obj
        {
            struct anything
            {
            };

            struct no_copy
            {
                no_copy(const no_copy&) = delete;
                no_copy& operator=(const no_copy&) = delete;
            };

            struct no_move
            {
                no_move(no_move&&) = delete;
                no_move& operator=(no_move&&) = delete;
            };

            struct no_copy_no_move : no_copy, no_move
            {
            };
        }

        class matcher;
        class matcher_branch;

        class matcher_branch_logic
        {
        private:
            matcher& _m;
            matcher_branch& _mb;
            const int& _v;

        public:
            matcher_branch_logic(matcher& m, matcher_branch& mb, const int& v)
                : _m{m}, _mb{mb}, _v{v}
            {
            }

            auto& exactly(int x)
            {
                get_ostream() << "exactly `" << x << "`\n"
                              << "(they are: `" << _v << "`)\n";

                EXPECT_OP(_v, ==, x);
                return _mb;
            }

            auto& more_than(int x)
            {
                get_ostream() << "more than `" << x << "`\n"
                              << "(they are: `" << _v << "`)\n";

                EXPECT_OP(_v, >, x);
                return _mb;
            }

            auto& less_than(int x)
            {
                get_ostream() << "less than `" << x << "`\n"
                              << "(they are: `" << _v << "`)\n";

                EXPECT_OP(_v, <, x);
                return _mb;
            }

            template <typename TF>
            auto& satisfies(TF&& f)
            {
                get_ostream() << "satisfying `f`\n";
                TEST_ASSERT(f(_v));
                return _mb;
            }
        };

        class matcher_branch
        {
        private:
            matcher& _m;
            const obj_id& _id;
            const stats& _s;

        public:
            matcher_branch(matcher& m, const obj_id& id, const stats& s)
                : _m{m}, _id{id}, _s{s}
            {
            }

#define DEFINE_INTERFACE_FOR(name, member)                                  \
    auto name() noexcept                                                    \
    {                                                                       \
        get_ostream() << "Checking " << _id << "'s " << #name << " to be "; \
        return matcher_branch_logic{_m, *this, _s.member};                  \
    }                                                                       \
                                                                            \
    auto name(int x) noexcept                                               \
    {                                                                       \
        return name().exactly(x);                                           \
    }                                                                       \
                                                                            \
    auto no_##name() noexcept                                               \
    {                                                                       \
        return name(0);                                                     \
    }

            DEFINE_INTERFACE_FOR(ctors, _ctors)
            DEFINE_INTERFACE_FOR(dtors, _dtors)
            DEFINE_INTERFACE_FOR(copies, _copies)
            DEFINE_INTERFACE_FOR(moves, _moves)

            auto expect_that(const obj_id& id);
        };

        class matcher
        {
        private:
            stat_map _stats;

        public:
            matcher(stat_map&& m) : _stats{std::move(m)}
            {
            }

            auto expect_that(const obj_id& id)
            {
                return matcher_branch{*this, id, _stats[id]};
            }
        };

        auto matcher_branch::expect_that(const obj_id& id)
        {
            return _m.expect_that(id);
        }

        class operation_context
        {
            template <typename>
            friend class tracked_object;

            friend auto make_matcher(operation_context&&);

            stat_map _stats;

        public:
            template <typename T, typename... Ts>
            auto make_tracked_object(obj_id id, Ts&&... xs)
            {
                return tracked_object<T>{*this, std::move(id), FWD(xs)...};
            }

        public:
#define DEFINE_MAKE_TRACKED_OBJECT(name, type)        \
    template <typename... Ts>                         \
    auto name(Ts&&... xs)                             \
    {                                                 \
        return make_tracked_object<type>(FWD(xs)...); \
    }

            DEFINE_MAKE_TRACKED_OBJECT(anything, obj::anything)
            DEFINE_MAKE_TRACKED_OBJECT(no_copy, obj::no_copy)
            DEFINE_MAKE_TRACKED_OBJECT(no_move, obj::no_move)
            DEFINE_MAKE_TRACKED_OBJECT(no_copy_no_move, obj::no_copy_no_move)

#undef DEFINE_MAKE_TRACKED_OBJECT
        };

        template <typename T>
        auto& tracked_object<T>::get_stats()
        {
            return _ctx->_stats[_id];
        }

        auto make_matcher(operation_context&& ctx)
        {
            return matcher{std::move(ctx._stats)};
        }
    }

    template <typename TF>
    auto check_operations(TF&& f)
    {
        impl::operation_context ctx;
        f(ctx);
        return impl::make_matcher(std::move(ctx));
    }
}
