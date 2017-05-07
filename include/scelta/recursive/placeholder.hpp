// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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