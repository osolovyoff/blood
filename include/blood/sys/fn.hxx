#ifndef blood_lua_fn_hxx_
#define blood_lua_fn_hxx_

#include <iostream>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "fn_list.hxx"

static void functions_addition(lua_State* L)
{
	lua_register(L, "c_print", c_print);
	lua_register(L, "c_sin", c_sin);
	lua_register(L, "remove", remove);

	//static const luaL_Reg gSystemRegistry[] = 
	//{
	//	{"zero", zero},
	//	{"open", open},
	//	{"close", close},
	//	{"print", print},
	//	{"create", create},
	//	{"remove", remove},
	//	{"NULL", NULL}
	//};
}

#endif//blood_lua_fn_hxx_