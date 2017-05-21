# scelta

C++17 zero-overhead syntactic sugar for `variant` and `optional`.

[![build][badge.build]][build]
[![stability][badge.stability]][stability]
[![license][badge.license]][license]
[![gratipay][badge.gratipay]][gratipay]

[badge.build]: https://img.shields.io/travis/SuperV1234/scelta.svg
[badge.stability]: https://img.shields.io/badge/stability-experimental-orange.svg
[badge.license]: http://img.shields.io/badge/license-boost-blue.svg
[badge.gratipay]: https://img.shields.io/gratipay/user/SuperV1234.svg

[build]: https://travis-ci.org/SuperV1234/scelta
[stability]: http://github.com/badges/stability-badges
[license]: https://github.com/SuperV1234/scelta/blob/master/LICENSE
[gratipay]: https://gratipay.com/~SuperV1234/



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

`scelta` detects and works out-of-the-box with `std::variant`, `std::optional`, `boost::variant`, `mpark::variant`, `boost::optional`, and `eggs::variant`.

Other implementation can be easily adapted by providing specializations of the helper `traits` and `optional_traits` structs. PRs are welcome!

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

TODO
