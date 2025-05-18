from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.build import check_min_cppstd

class ilc_deltaRecipe(ConanFile):
    name = "ilc-delta"
    version = "1.0"
    package_type = "application"

    # Optional metadata
    license = "MIT License"
    author = "Gary Haussmann <gjhaussmann@gmail.com>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Incremental lambda calculus library and examples"
    topics = ("template", "reflection", "incremental lambda calculus")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def requirements(self):
        self.requires("immer/0.8.1")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, "20")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    
