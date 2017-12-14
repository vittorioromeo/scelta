// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./example_utils.hpp"
#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

bool success()
{
    return std::uniform_real_distribution<float>{0.0f, 1.0f}(gen) > 0.25f;
}

template <typename T>
using optional = example::optional<T>;

struct image_view
{
};

optional<image_view> simulate_operation()
{
    return success() ? optional<image_view>{image_view{}}
                     : scelta::nullopt;
}

optional<image_view> crop_to_cat(image_view)
{
    std::cout << "cropping...\n";
    return simulate_operation();
}

optional<image_view> add_bow_tie(image_view)
{
    std::cout << "adding bow tie...\n";
    return simulate_operation();
}

optional<image_view> make_eyes_sparkle(image_view)
{
    std::cout << "making eyes sparkle...\n";
    return simulate_operation();
}

image_view make_smaller(image_view)
{
    std::cout << "making smaller...\n";
    return {};
}

image_view add_rainbow(image_view)
{
    std::cout << "adding rainbow...\n";
    return {};
}

// clang-format off
optional<image_view> get_cute_cat(image_view img)
{
    using namespace scelta::infix;
    return crop_to_cat(img)
         | and_then(add_bow_tie)
         | and_then(make_eyes_sparkle)
         | map(make_smaller)
         | map(add_rainbow);
}
// clang-format on

int main()
{
    get_cute_cat({});
}
