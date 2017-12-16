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

struct cymk_image_view
{
};

struct rgb_image_view
{
};

template <typename T>
optional<T> simulate_operation()
{
    return success() ? optional<T>{T{}}
                     : scelta::nullopt;
}

optional<rgb_image_view> to_rgb(cymk_image_view)
{
    std::cout << "converting...\n";
    return simulate_operation<rgb_image_view>();
}

optional<rgb_image_view> crop_to_cat(rgb_image_view)
{
    std::cout << "cropping...\n";
    return simulate_operation<rgb_image_view>();
}

optional<rgb_image_view> add_bow_tie(rgb_image_view)
{
    std::cout << "adding bow tie...\n";
    return simulate_operation<rgb_image_view>();
}

optional<rgb_image_view> make_eyes_sparkle(rgb_image_view)
{
    std::cout << "making eyes sparkle...\n";
    return simulate_operation<rgb_image_view>();
}

rgb_image_view make_smaller(rgb_image_view)
{
    std::cout << "making smaller...\n";
    return {};
}

rgb_image_view add_rainbow(rgb_image_view)
{
    std::cout << "adding rainbow...\n";
    return {};
}

// clang-format off
optional<rgb_image_view> get_cute_cat(cymk_image_view img)
{
    using namespace scelta::infix;
    return to_rgb(img)
         | and_then(crop_to_cat)
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
