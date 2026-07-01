#pragma once

#include <string>
#include <unordered_map>
#include <variant>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class LuaRuntime {
public:
    struct LuaFunctionRef {
        int ref;
    };

    using LuaValue = std::variant<
        std::monostate,
        double,
        int,
        bool,
        std::string,
        LuaFunctionRef
    >;

    LuaRuntime();
    ~LuaRuntime();

    void execute(const std::string& code);
    LuaValue eval(const std::string& expr);

    std::unordered_map<std::string, LuaValue> globals();

    LuaValue callRef(int ref);
    LuaValue toValue(int index);

private:
    lua_State* L;
};
