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
        self.run('cmake "%s" %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def test(self):
        os.chdir("bin")
        self.run(".%sexample" % os.sep)
