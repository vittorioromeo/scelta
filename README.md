# scelta

C++17 zero-overhead syntactic sugar for `variant` and `optional`.

## Overview

With the introduction of `std::variant` and `std::optional` to C++17's Standard Library, hopefully *sum types* will become commonly used *vocabulary types* that improve type safety and efficiency of many applications. Unfortunately their syntax is not as nice as it could be - `scelta` aims to fix that by providing zero-overhead implementation-independent syntactic sugar.

### Implementation independent

`scelta` detects and works out-of-the-box with `std::variant`, `std::optional`, `boost::variant`, `boost::optional`, and `eggs::variant`. 

Other implementation can be easily adapted by providing an *homogenizer* helper `struct`. PRs are welcome!

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
    // resolved.)
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

TODO

## Documentation

TODO
