#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "LuaRuntime.hpp"

namespace py = pybind11;

static py::object to_python(LuaRuntime &self, const LuaRuntime::LuaValue &val) {

    if (std::holds_alternative<std::monostate>(val))
        return py::none();

    if (std::holds_alternative<double>(val)) {
        double v = std::get<double>(val);
        if (v == (int)v) return py::int_((int)v);
        return py::float_(v);
    }

    if (std::holds_alternative<int>(val))
        return py::int_(std::get<int>(val));

    if (std::holds_alternative<bool>(val))
        return py::bool_(std::get<bool>(val));

    if (std::holds_alternative<std::string>(val))
        return py::str(std::get<std::string>(val));

    if (std::holds_alternative<LuaRuntime::LuaFunctionRef>(val)) {

        int ref = std::get<LuaRuntime::LuaFunctionRef>(val).ref;

        return py::cpp_function([&self, ref](py::args) -> py::object {
            return to_python(self, self.callRef(ref));
        });
    }

    return py::none();
}

PYBIND11_MODULE(pylua, m) {

    py::class_<LuaRuntime>(m, "LuaRuntime")
        .def(py::init<>())

        .def("execute",
            [](LuaRuntime &self, const std::string &code) {
                self.execute(code);
            }
        )

        .def("eval",
            [](LuaRuntime &self, const std::string &expr) {
                return to_python(self, self.eval(expr));
            }
        )

        .def("globals",
            [](LuaRuntime &self) -> py::dict {

                auto g = self.globals();
                py::dict out;

                for (auto &kv : g) {
                    out[py::str(kv.first)] = to_python(self, kv.second);
                }

                return out;
            }
        );
}
