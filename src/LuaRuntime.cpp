#include "LuaRuntime.hpp"
#include <stdexcept>

LuaRuntime::LuaRuntime() {
    L = luaL_newstate();
    if (!L) throw std::runtime_error("Failed to create Lua state");
    luaL_openlibs(L);
}

LuaRuntime::~LuaRuntime() {
    if (L) lua_close(L);
}

void LuaRuntime::execute(const std::string& code) {
    if (luaL_dostring(L, code.c_str()) != LUA_OK) {
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw std::runtime_error(err);
    }
}

LuaRuntime::LuaValue LuaRuntime::eval(const std::string& expr) {
    std::string code = "return " + expr;

    if (luaL_dostring(L, code.c_str()) != LUA_OK) {
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw std::runtime_error(err);
    }

    LuaValue v = toValue(-1);
    lua_pop(L, 1);
    return v;
}

LuaRuntime::LuaValue LuaRuntime::toValue(int index) {

    if (lua_isnumber(L, index)) {
        return (double)lua_tonumber(L, index);
    }

    if (lua_isboolean(L, index)) {
        return (bool)lua_toboolean(L, index);
    }

    if (lua_isstring(L, index)) {
        return std::string(lua_tostring(L, index));
    }

    if (lua_isfunction(L, index)) {
        lua_pushvalue(L, index);
        int ref = luaL_ref(L, LUA_REGISTRYINDEX);
        return LuaFunctionRef{ref};
    }

    return std::monostate{};
}

LuaRuntime::LuaValue LuaRuntime::callRef(int ref) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        throw std::runtime_error("Invalid Lua function reference");
    }

    if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw std::runtime_error(err);
    }

    LuaValue result = toValue(-1);
    lua_pop(L, 1);
    return result;
}

std::unordered_map<std::string, LuaRuntime::LuaValue>
LuaRuntime::globals() {

    std::unordered_map<std::string, LuaValue> result;

    lua_pushglobaltable(L);
    lua_pushnil(L);

    while (lua_next(L, -2) != 0) {
        std::string key = lua_tostring(L, -2);
        result[key] = toValue(-1);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
    return result;
}
