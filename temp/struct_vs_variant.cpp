#include <utility>
#include <type_traits>

template <typename... Fs>
struct overloader : Fs...
{
    template <typename... FFwds>
    overloader(FFwds&&... fs) : Fs{std::forward<FFwds>(fs)}...
    {
    }

    using Fs::operator()...;
};

template <typename... Fs>
auto overload(Fs&&... fs)
{
    return overloader<std::decay_t<Fs>...>{std::forward<Fs>(fs)...};
}

struct a { void foo() { } };
struct b { void bar() { } };
struct c { void bar() { } };

struct s0
{
    auto operator()(a x) const { x.foo(); }
};

struct s1
{
    template <typename T>
    auto operator()(T x) const { x.bar(); }
};

auto l0 = [](a x)   { x.foo(); };
auto l1 = [](auto x){ x.bar(); }; // clang++ OK
                                  // g++     ERROR

int main()
{
    auto os = overload(s0{}, s1{});
    auto ol = overload(l0, l1);

    os(a{});
    ol(a{});
}