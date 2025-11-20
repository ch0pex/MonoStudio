from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class MonoGameRecipe(ConanFile):
    name = "MonoGame"
    version = "0.1"
    package_type = "application"

    # Optional metadata
    license = "MIT License"
    author = "Álvaro Cabrera Barrio"
    url = ""
    description = "MonoGame Studio games dependencies"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe
    exports_sources = "CMakeLists.txt", "src/*"

    default_options = {
        "reflect-cpp/*:with_toml": True,
    }

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.user_presets_path = ""
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def requirements(self):
        self.requires("vulkan-loader/[>=1]")
        self.requires("glfw/[>=3]")
        self.requires("boost/[>=1]")
        self.requires("reflect-cpp/[>=0]")
        self.requires("quill/[>=10]")

    def build_requirements(self):
        self.test_requires("doctest/2.4.11")
