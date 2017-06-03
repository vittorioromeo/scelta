from conans import ConanFile, CMake
import os

channel = os.getenv("CONAN_CHANNEL", "experimental")
username = os.getenv("CONAN_USERNAME", "SuperV1234")

class HelloTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "scelta/0.1@%s/%s" % (username, channel)
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        os.chdir("../../../")

        if not os.path.exists("build"):
            os.mkdir("build")

        os.chdir("build")
        self.run("cmake .. %s" % cmake.command_line)
        self.run("cmake --build . %s" % cmake.build_config)
        self.run("make check -j8")

    def test(self):
        ()
