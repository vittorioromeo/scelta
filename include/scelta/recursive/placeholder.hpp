// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/replace_all.hpp"

namespace scelta::recursive
{
    struct placeholder
    {
        placeholder() = delete;

        placeholder(const placeholder&) = delete;
        placeholder(placeholder&&) = delete;

        placeholder& operator=(const placeholder&) = delete;
        placeholder& operator=(placeholder&&) = delete;
    };

    namespace impl
    {
        template <typename After, typename T>
        using replace_placeholders = //
            meta::replace_all_copy_cv_ptr_ref_t<placeholder, After, T>;
    }
}
