# scelta

> **C++17 zero-overhead syntactic sugar for `variant` and `optional`.**

[![build][badge.build]][build]
[![stability][badge.stability]][stability]
[![license][badge.license]][license]
[![gratipay][badge.gratipay]][gratipay]
[![conan][badge.conan]][conan]
![badge.cpp](https://img.shields.io/badge/c++-17-ff69b4.svg?style=flat-square)
[![on-wandbox][badge.on-wandbox]][on-wandbox]
[![on-godbolt][badge.on-godbolt]][on-godbolt]

[badge.build]: https://img.shields.io/travis/SuperV1234/scelta.svg?style=flat-square
[badge.stability]: https://img.shields.io/badge/stability-experimental-orange.svg?style=flat-square
[badge.license]: http://img.shields.io/badge/license-mit-blue.svg?style=flat-square
[badge.gratipay]: https://img.shields.io/gratipay/user/SuperV1234.svg?style=flat-square
[badge.conan]: https://img.shields.io/badge/conan.io-scelta%2F0.1-yellowgreen.svg?style=flat-square
[badge.on-wandbox]: https://img.shields.io/badge/on-wandbox-376666.svg?style=flat-square
[badge.on-godbolt]: https://img.shields.io/badge/on-godbolt-376666.svg?style=flat-square

[build]: https://travis-ci.org/SuperV1234/scelta
[stability]: http://github.com/badges/stability-badges
[license]: https://github.com/SuperV1234/scelta/blob/master/LICENSE
[gratipay]: https://gratipay.com/~SuperV1234/
[conan]: https://www.conan.io/source/scelta/0.1/SuperV1234/experimental
[on-wandbox]: https://wandbox.org/permlink/wSA55OCJz17k7Jtz
[on-godbolt]: https://godbolt.org/g/4sQtkM



## Table of Contents

* [Overview](#overview)
* [Installation/usage](#installationusage)
* [Documentation](#documentation)
* [Resources](#resources)



## Overview

`std::variant` and `std::optional` were introduced to C++17's Standard Library. They are **sum types** that can greatly improve *type safety* and *performance*.

However, there are some problems with them:

* The syntax of some common operations such as **visitation** is not as nice as it could be, and requires a significant amount of boilerplate.

* Defining and using recursive `variant` or `optional` types is not trivial and requires a lot of boilerplate.

* `std::optional` doesn't support visitation.

* The interface of `std::variant` and `std::optional` is different from some other commonly used ADT implementations - interoperability requires significant boilerplate.

**`scelta`** aims to fix all the aformenetioned problems by providing zero-overhead syntactic sugar that:

* Automatically detects and homogenizes all available `variant` and `optional` implementations, providing a single **implementation-independent** interface.

* Provides **"pattern matching"**-like syntax for visitation and recursive visitation which works both for `variant` and `optional`.

* Provides an intuitive **placeholder-based** recursive `variant` and `optional` type definition.


### Implementation independent

`scelta` detects and works out-of-the-box with:

* [`std::variant`](http://en.cppreference.com/w/cpp/utility/variant)
* [`boost::variant`](http://www.boost.org/doc/libs/1_64_0/doc/html/variant.html)
* [`mpark::variant`](https://github.com/mpark/variant)
* [`eggs::variant`](https://github.com/eggs-cpp/variant)
* [`type_safe::variant`](https://github.com/foonathan/type_safe)
* [`std::optional`](http://en.cppreference.com/w/cpp/utility/optional)
* [`boost::optional`](http://www.boost.org/doc/libs/1_64_0/libs/optional/doc/html/index.html)
* [`type_safe::optional`](https://github.com/foonathan/type_safe)


Other implementation can be easily adapted by providing specializations of the helper `traits` structs. PRs are welcome!

### Curried visitation syntax

`scelta` provides *curried*, `constexpr`-friendly, and SFINAE-friendly visitation utilities both for `variant` and `optional`. The final user syntax resembles *pattern matching*. Recursive data structures are supported.

```cpp
using shape = std::variant<circle, box>;

shape s0{circle{/*...*/}};
shape s1{box{/*...*/}};

// In place `match` visitation.
scelta::match([](circle, circle){ /* ... */ },
              [](circle, box)   { /* ... */ },
              [](box,    circle){ /* ... */ },
              [](box,    box)   { /* ... */ })(s0, s1);
```

The `match` function is intentionally *curried* in order to allow reuse of a particular visitor in a scope, even on different implementations of `variant`/`optional`.

```cpp
using boost_optstr = boost::optional<std::string>;
using std_optstr = std::optional<std::string>;

// Curried `match` usage.
auto print = scelta::match([](std::string s)    { cout << s;       },
                           [](scelta::nullopt_t){ cout << "empty"; });

boost_optstr s0{/*...*/};
std_optstr s1{/*...*/};

// Implementation-independent visitation.
print(s0);
print(s1);
```


### Recursive ADTs creation and visitation

Recursive `variant` and `optional` data structures can be easily created through the use of *placeholders*.

```cpp
namespace impl
{
    namespace sr = scelta::recursive;

    // `placeholder` and `builder` can be used to define recursive
    // sum types.
    using _ = sr::placeholder;
    using builder = sr::builder<std::variant<int, std::vector<_>>>;

    // `type` evaluates to the final recursive data structure type.
    using type = sr::type<builder>;

    // `resolve` completely evaluates one of the alternatives.
    // (In this case, even the `Allocator` template parameter is
    // resolved!)
    using vector_type = sr::resolve<builder, std::vector<_>>;
}

using int_tree = impl::type;
using int_tree_vector = impl::vector_type;
```

After defining recursive structures, *in place recursive visitation* is also possible.

```cpp
int_tree t0{/*...*/};

scelta::recursive::match<return_type>(
    // Base case.
    [](auto, int x){ cout << x; },

    // Recursive case.
    [](auto recurse, int_tree_vector v){ for(auto x : v) recurse(v); }
)(t0);
```

## Installation/usage

### Quick start

**`scelta`** is an *header-only* library. It is sufficient to include it.

```cpp
// main.cpp
#include <scelta.hpp>

int main() { return 0; }
```

```bash
g++ -std=c++1z main.cpp -Isome_path/scelta/include
```

### Running tests and examples

Tests can be easily built and run using [CMake](https://cmake.org/).

```bash
git clone https://github.com/SuperV1234/scelta && cd scelta
./init-repository.sh # get `vrm_cmake` dependency
mkdir build && cd build

cmake ..
make check # build and run tests

make example_error_handling # error handling via pattern matching
make example_expression     # recursive expression evaluation
```

All tests currently pass on `Arch Linux x64` with:

* `g++ (GCC) 8.0.0 20170514 (experimental)`

* `clang version 5.0.0 (trunk 303617)`

### Integration with existing project

1. Add this repository and [SuperV1234/vrm_cmake](https://github.com/SuperV1234/vrm_cmake) as submodules of your project, in subfolders inside `your_project/extlibs/`:

    ```bash
    git submodule add   https://github.com/SuperV1234/vrm_cmake.git   your_project/extlibs/vrm_cmake
    git submodule add   https://github.com/SuperV1234/scelta.git      your_project/extlibs/scelta
    ```

2. Include `vrm_cmake` in your project's `CMakeLists.txt` and look for the `scelta` extlib:

    ```cmake
    # Include `vrm_cmake`:
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/vrm_cmake/cmake/")
    include(vrm_cmake)

    # Find `scelta`:
    vrm_cmake_find_extlib(scelta)
    ```

## Documentation

### `scelta::visit`

Executes non-recursive visitation.

* Interface:

    ```cpp
    template <typename Visitor, typename... Visitables>
    constexpr /*deduced*/ visit(Visitor&& visitor, Visitables&&... visitables)
        noexcept(/*deduced*/);
    ```

    * `visitables...` must all be the same type. *(i.e. different implementations of variant/optional currently cannot be mixed together)*

    * `visitor` must be invocable with all the alternatives of the passed visitables.

* Examples:

    ```cpp
    struct visitor
    {
        auto operator()(int) { return 0; }
        auto operator()(char){ return 1; }
    };

    variant<int, char> v0{'a'};
    assert(
        scelta::visit(visitor{}, v0) == 1
    );
    ```

    ```cpp
    struct visitor
    {
        auto operator()(int)              { return 0; }
        auto operator()(scelta::nullopt_t){ return 1; }
    };

    optional<int> o0{0};
    assert(
        scelta::visit(visitor{}, o0) == 0
    );
    ```



### `scelta::match`

Executes non-recursive in-place visitation.

* Interface:

    ```cpp
    template <typename... FunctionObjects>
    constexpr /*deduced*/ match(FunctionObjects&&... functionObjects)
        noexcept(/*deduced*/)
    {
        return [o = overload(functionObjects...)](auto&&... visitables)
            noexcept(/*deduced*/)
            -> /*deduced*/
        {
            // ... perform visitation with `scelta::visit` ...
        };
    };
    ```

    * Invoking `match` takes a number of `functionObjects...` and returns a new function which takes a number of `visitables...`.

    * `visitables...` must all be the same type. *(i.e. different implementations of variant/optional currently cannot be mixed together)*

    * `o` must be invocable with all the alternatives of the passed visitables. *(i.e. the overload of all `functionObjects...` must produce an exhaustive visitor)*

* Examples:

    ```cpp
    variant<int, char> v0{'a'};
    assert(
        scelta::match([](int) { return 0; }
                      [](char){ return 1; })(v0) == 1
    );
    ```

    ```cpp
    optional<int> o0{0};
    assert(
        scelta::match([](int)              { return 0; }
                      [](scelta::nullopt_t){ return 1; })(o0) == 1
    );
    ```



### `scelta::recursive::builder`

Allows placeholder-based definition of recursive ADTs.

* Interface:

    ```cpp
    template <typename ADT>
    class builder;

    struct placeholder;

    template <typename Builder>
    using type = /* ... recursive ADT type wrapper ... */;

    template <typename Builder, typename T>
    using resolve = /* ... resolved ADT alternative ... */;
    ```

    * `builder` takes any ADT containing zero or more `placeholder` alternatives. *(i.e. both optional and variant)*

    * `placeholder` is replaced with the recursive ADT itself when using `type` or `resolve`.

    * `type` returns a wrapper around a fully-resolved recursive `ADT`.

    * `resolve` returns a fully-resolved alternative contained in `ADT`.

* Examples:

    ```cpp
    using _ = scelta::recursive::placeholder;
    using b = scelta::recursive::builder<variant<int, _*>>;

    using recursive_adt = scelta::recursive::type<b>;
    using ptr_alternative = scelta::recursive::resolve<b, _*>;

    recursive_adt v0{0};
    recursive_adt v1{&v0};
    ```

### `scelta::recursive::visit`

Executes recursive visitation.

* Interface:

    ```cpp
    template <typename Return, typename Visitor, typename... Visitables>
    constexpr Return visit(Visitor&& visitor, Visitables&&... visitables)
        noexcept(false);
    ```

    * Similar to `scelta::match`, but requires an explicit return type and is not `noexcept`-friendly.

    * The `operator()` overloads of `visitor...` must take one extra generic argument to receive the `recurse` helper.

* Examples:

    ```cpp
    using _ = scelta::recursive::placeholder;
    using b = scelta::recursive::builder<variant<int, std::vector<_>>>;

    using recursive_adt = scelta::recursive::type<b>;
    using rvec = scelta::recursive::resolve<b, std::vector<_>>;

    struct visitor
    {
        auto operator()(auto,         int x)  { /* base case */ },
        auto operator()(auto recurse, rvec& v){ for(auto& x : v) recurse(x); }
    };

    recursive_adt v0{rvec{recursive_adt{0}, recursive_adt{1}}};
    scelta::recursive::visit(visitor{}, v0};
    ```

### `scelta::recursive::match`

Executes recursive visitation.

* Interface:

    ```cpp
    template <typename Return, typename... FunctionObjects>
    constexpr auto match(FunctionObjects&&... functionObjects)
        noexcept(false)
    {
        return [o = overload(functionObjects...)](auto&&... visitables)
            noexcept(false)
            -> Return
        {
            // ... perform visitation with `scelta::recursive::visit` ...
        };
    };
    ```

    * Similar to `scelta::visit`, but requires an explicit return type and is not `noexcept`-friendly.

    * The passed `functionObjects...` must take one extra generic argument to receive the `recurse` helper.

* Examples:

    ```cpp
    using _ = scelta::recursive::placeholder;
    using b = scelta::recursive::builder<variant<int, std::vector<_>>>;

    using recursive_adt = scelta::recursive::type<b>;
    using rvec = scelta::recursive::resolve<b, std::vector<_>>;

    recursive_adt v0{rvec{recursive_adt{0}, recursive_adt{1}}};
    scelta::recursive::match(
        [](auto,         int x)  { /* base case */ },
        [](auto recurse, rvec& v){ for(auto& x : v) recurse(x); }
    )(v0);
    ```

### `scelta::experimental::match`

**(work-in-progress)**

Executes visitation *(both non-recursive and recursive)*. Attempts to deduce the return type from the base cases, optionally supports user-provided explicit return type.

* Interface:

    ```cpp
    template <typename Return = impl::deduce_t, typename... BaseCases>
    constexpr auto match(BaseCases&&... baseCases)
    {
        return [bco = overload(adapt(baseCases)...)](auto... xs)
        {
            if constexpr(are_visitables<decltype(xs)...>())
            {
                // ... perform visitation with `scelta::visit` ...
            }
            else
            {
                return [o = overload(bco, xs...)](auto&&... visitables)
                {
                    // ... perform visitation with `scelta::recursive_visit` ...
                };
            }
        };
    };
    ```

    * The first invocation of `scelta::experimental::match` takes one or more *base cases*. A base case is a function object with the same arity as the number of objects that will be visited.

    * The function returned by the first invocation takes either a number of *recursive cases* or a number of *visitables*.

        * Recursive cases are function objects with arity equal to the number of objects that will be visited plus one *(the +1 is for the `recurse` argument)*.

        * Visitables are variants or optionals. If visitables are passed here, non-recursive visitation will be performed immediately.

    * If recursive cases were passed, the last returned function takes any number of visitables. Recursive visitation will then be performed immediately.

* Examples:

    ```cpp
    variant<int, char> v0{'a'};
    assert(
        scelta::experimental::match([](int) { return 0; }
                                    [](char){ return 1; })(v0) == 1
    );
    ```

    ```cpp
    using _ = scelta::recursive::placeholder;
    using b = scelta::recursive::builder<variant<int, std::vector<_>>>;

    using recursive_adt = scelta::recursive::type<b>;
    using rvec = scelta::recursive::resolve<b, std::vector<_>>;

    recursive_adt v0{rvec{recursive_adt{0}, recursive_adt{1}}};
    scelta::experimental::match(
        [](int x){ /* base case */ }
    )(
        [](auto recurse, rvec& v){ for(auto& x : v) recurse(x); }
    )(v0);
    ```

## Resources

* ACCU 2017 talk: [**"Implementing variant Visitation Using Lambdas"**](https://www.youtube.com/watch?v=mqei4JJRQ7s)
