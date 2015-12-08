/*
 * Copyright (c) 2015 Micro Systems Marc Balmer, CH-5073 Gipf-Oberfrick.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * "nanomsg" is a trademark of Martin Sustrik.  Used with permission.
 */

/* nanomsg for Lua */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string.h>
#include <stdlib.h>

#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>

#include "luananomsg.h"

static int
luann_socket(lua_State *L)
{
	int *s;

	s = lua_newuserdata(L, sizeof(int));
	*s = nn_socket(luaL_checkint(L, -3), luaL_checkint(L, -2));
	luaL_getmetatable(L, NN_SOCKET_METATABLE);
	lua_setmetatable(L, -2);
	return 1;
}

static int
luann_close(lua_State *L)
{
	int *s;

	s = luaL_checkudata(L, -1, NN_SOCKET_METATABLE);
	if (*s != -1) {
		lua_pushboolean(L, nn_close(*s) == 0);
		*s = -1;
	}
	return 1;
}

static int
luann_setsockopt(lua_State *L)
{
	int *s, level, option, rc, int_optval;
	const char *str_optval;
	size_t optvallen;

	s = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	level = luaL_checkinteger(L, 2);
	option = luaL_checkinteger(L, 3);

	switch (option) {
	case NN_SOCKET_NAME:
	case NN_SUB_SUBSCRIBE:
	case NN_SUB_UNSUBSCRIBE:
		str_optval = luaL_checklstring(L, 4, &optvallen);
		rc = nn_setsockopt(*s, level, option, str_optval, optvallen);
		break;
	default:
		int_optval = luaL_checkinteger(L, 4);
		rc = nn_setsockopt(*s, level, option, &int_optval, sizeof(int));
	}

	lua_pushboolean(L, rc == 0);
	return 1;
}

static int
luann_getsockopt(lua_State *L)
{
	int *s, level, option, rc, int_optval;
	const char *str_optval;
	size_t optvallen;

	s = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	level = luaL_checkinteger(L, 2);
	option = luaL_checkinteger(L, 3);

	switch (option) {
	case NN_SOCKET_NAME:
	case NN_SUB_SUBSCRIBE:
	case NN_SUB_UNSUBSCRIBE:
		str_optval = luaL_checklstring(L, 4, &optvallen);
		rc = nn_getsockopt(*s, level, option, &str_optval, &optvallen);
		lua_pushboolean(L, rc == 0);
		if (!rc)
			lua_pushlstring(L, str_optval, optvallen);
		else
			lua_pushnil(L);
		break;
	default:
		int_optval = luaL_checkinteger(L, 4);
		rc = nn_getsockopt(*s, level, option, &int_optval, &optvallen);
		lua_pushboolean(L, rc == 0);
		if (!rc)
			lua_pushinteger(L, int_optval);
		else
			lua_pushnil(L);
	}
	return 2;
}

static int
luann_bind(lua_State *L)
{
	int *s, rc;

	s = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	rc = nn_bind(*s, luaL_checkstring(L, 2));

	if (rc == -1)
		lua_pushnil(L);
	else
		lua_pushinteger(L, rc);
	return 1;
}

static int
luann_connect(lua_State *L)
{
	int *s, rc;

	s = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	rc = nn_connect(*s, luaL_checkstring(L, 2));

	if (rc < 0)
		lua_pushnil(L);
	else
		lua_pushinteger(L, rc);
	return 1;
}

static int
luann_shutdown(lua_State *L)
{
	int *s, how;

	s = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	how = luaL_checkinteger(L, 2);

	lua_pushboolean(L, nn_shutdown(*s, how) == 0);
	return 1;
}

static int
luann_send(lua_State *L)
{
	int *s, flags, nbytes;
	size_t len;
	const char *buf;

	flags = lua_gettop(L) == 3 ? lua_tointeger(L, 3) : 0;
	s = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	buf = lua_tolstring(L, 2, &len);

	nbytes = nn_send(*s, buf, len, flags);
	if (nbytes == -1)
		lua_pushnil(L);
	else
		lua_pushinteger(L, nbytes);
	return 1;
}

static int
luann_recv(lua_State *L)
{
	int *s, flags, nbytes;
	size_t len;
	char *buf;

	s = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	len = luaL_checkinteger(L, 2);
	flags = lua_gettop(L) == 3 ? lua_tointeger(L, 3) : 0;

	buf = malloc(len + 1);
	if (buf == NULL)
		return luaL_error(L, "memory error");

	nbytes = nn_recv(*s, buf, len, flags);
	buf[len] = '\0';

	if (nbytes == -1) {
		lua_pushnil(L);
		lua_pushinteger(L, 0);
	} else {
		buf[nbytes] = '\0';
		lua_pushstring(L, buf);
		lua_pushinteger(L, nbytes);
	}
	free(buf);
	return 2;
}

static int
luann_errno(lua_State *L)
{
	lua_pushinteger(L, nn_errno());
	return 1;
}

static int
luann_strerror(lua_State *L)
{
	lua_pushstring(L, nn_strerror(errno));
	return 1;
}

static int
luann_device(lua_State *L)
{
	int *s1, *s2;

	s1 = luaL_checkudata(L, 1, NN_SOCKET_METATABLE);
	s2 = luaL_checkudata(L, 2, NN_SOCKET_METATABLE);

	lua_pushboolean(L, nn_device(*s1, *s2) == 0);
	return 1;
}

static int
luann_term(lua_State *L)
{
	nn_term();
	return 0;
}

static void
luann_set_info(lua_State *L)
{
	lua_pushliteral(L, "_COPYRIGHT");
	lua_pushliteral(L, "Copyright (C) 2015 by "
	    "micro systems marc balmer");
	lua_settable(L, -3);
	lua_pushliteral(L, "_DESCRIPTION");
	lua_pushliteral(L, "nanomsg for Lua");
	lua_settable(L, -3);
	lua_pushliteral(L, "_VERSION");
	lua_pushliteral(L, "nanomsg 1.0.0");
	lua_settable(L, -3);
}
int
luaopen_nanomsg(lua_State *L)
{
	struct luaL_Reg functions[] = {
		{ "socket",		luann_socket },
		{ "errno",		luann_errno },
		{ "strerror",		luann_strerror },
		{ "device",		luann_device },
		{ "term",		luann_term },
		{ NULL,			NULL }
	};
	struct luaL_Reg socket_methods[] = {
		{ "close",		luann_close },
		{ "setsockopt",		luann_setsockopt },
		{ "getsockopt",		luann_getsockopt },
		{ "bind",		luann_bind },
		{ "connect",		luann_connect },
		{ "shutdown",		luann_shutdown },
		{ "send",		luann_send },
		{ "recv",		luann_recv },
		{ NULL,			NULL }
	};
	int n;

#if LUA_VERSION_NUM >= 502
	luaL_newlib(L, functions);
#else
	luaL_register(L, "nanomsg", functions);
#endif
	if (luaL_newmetatable(L, NN_SOCKET_METATABLE)) {
#if LUA_VERSION_NUM >= 502
		luaL_setfuncs(L, socket_methods, 0);
#else
		luaL_register(L, NULL, socket_methods);
#endif
		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, luann_close);
		lua_settable(L, -3);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);

		lua_pushliteral(L, "__metatable");
		lua_pushliteral(L, "must not access this metatable");
		lua_settable(L, -3);
	}
	lua_pop(L, 1);

        for (n = 0; n < num_nn_int(); n++) {
                lua_pushinteger(L, nn_int[n].value);
                lua_setfield(L, -2, nn_int[n].name);
        };
        luann_set_info(L);
	return 1;
}
