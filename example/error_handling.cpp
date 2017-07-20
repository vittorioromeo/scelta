// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./example_utils.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace io_outcome
{
    struct success
    {
        std::string _contents;
    };

    struct not_found
    {
    };

    // ...more outcome models...
}

using io_result = example::variant<io_outcome::success, io_outcome::not_found>;

io_result read_file(const std::string& path)
{
    std::ifstream ifs(path);

    if(!ifs.is_open())
    {
        return io_outcome::not_found{};
    }

    // ...more outcome handling...

    io_outcome::success result;
    result._contents.assign(
        std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

    return result;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr << "Please enter a file path." << std::endl;
        return 1;
    }

    return scelta::experimental::recursive::match(
        [](const io_outcome::success& x) { std::cout << x._contents << std::endl;        return 0; },
        [](const io_outcome::not_found&) { std::cerr << "File not found\n" << std::endl; return 1; })
        (read_file(argv[1]));
}
