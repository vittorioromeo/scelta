from conans import ConanFile, tools, CMake
import os

class SceltaConan(ConanFile):
    name = "scelta"
    version = "0.1"
    url = "https://github.com/SuperV1234/scelta.git"
    build_policy = "missing"
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        self.run("git clone https://github.com/SuperV1234/scelta.git")
        self.run("cd scelta && git checkout conan_support && git submodule update --init")

    def build(self):
        cmake = CMake(self)
        self.run('cmake %s/scelta %s' % (self.source_folder, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.hpp", dst="include", src="scelta/include")

    def package_info(self):
        self.info.header_only()
