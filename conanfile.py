from conans import ConanFile, tools
import os

class SceltaConan(ConanFile):
    name = "scelta"
    version = "0.1"
    url = "https://github.com/SuperV1234/scelta.git"
    build_policy = "missing"

    def source(self):
        self.run("git clone https://github.com/SuperV1234/scelta.git")
        self.run("cd scelta && git checkout v0.1 && git submodule update --init")

    def package(self):
        self.copy("*.hpp", dst="include", src="scelta/include")
