from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import os

PREFIX = "/data/data/com.termux/files/usr"

ext_modules = [
    Pybind11Extension(
        "pylua",
        [
            "src/LuaRuntime.cpp",
            "src/bindings.cpp",
        ],
        include_dirs=[
            "include",
            f"{PREFIX}/include/lua5.5",
        ],
        libraries=["lua5.5"],
        language="c++",
    ),
]

setup(
    name="pylua",
    version="0.1.0",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
