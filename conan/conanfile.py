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
    description = "MonoGame Studio dependencies"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe
    exports_sources = "CMakeLists.txt", "src/*"

    default_options = {
        "reflect-cpp/*:with_toml": True,
        "glfw/*:with_wayland": True,
        "glad/*:gl_version": "4.5",
        "glad/*:gl_profile": "core",
        "boost/*:without_cobalt": True,
        "boost/*:without_stacktrace": True,
        "boost/*:without_locale": True,
        "boost/*:with_stacktrace_backtrace": False
    }

    def layout(self):
        self.folders.root = ".."
        compiler_name = str(self.settings.compiler).lower()
        build_type = str(self.settings.build_type).lower()
        base_path = f"build/{compiler_name}/{build_type}"
        self.folders.build = base_path
        self.folders.generators = f"{base_path}/generators"
        self.folders.source = "."

    def generate(self):
        deps = CMakeDeps(self)
        deps.check_components_exist = False
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
        self.requires("vulkan-loader/1.4.313.0")
        self.requires("glfw/3.4")
        self.requires("boost/1.90.0")
        self.requires("reflect-cpp/0.23.0")
        self.requires("quill/11.0.2")
        self.requires("glm/1.0.1")
        self.requires("glad/2.0.8")
        self.requires("vulkan-memory-allocator/3.3.0")
        self.requires("plf_colony/7.39")

    def build_requirements(self):
        self.test_requires("doctest/2.4.11")
