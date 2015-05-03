#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <lualib.h>
#include <luaconf.h>
#include <lauxlib.h>
#include <lua.h>

// references
// http://www.lua.org/pil/contents.html#P4
// http://luabinaries.sourceforge.net/

// how to define function to lua
static int l_custom_print(lua_State* L) 
{
	int n = lua_gettop(L);

	for (int i = 1; i <= n; i++) 
	{
		if (lua_isstring(L, i)) 
		{
			printf("lua: %s\n", lua_tostring(L, i));
		}
		else if (lua_isnumber(L, i)) 
		{
			printf("lua: %f\n", lua_tonumber(L, i));
		}
		else if (lua_isinteger(L, i)) 
		{
			printf("lua: %d\n", lua_tointeger(L, i));
		}
		else 
		{
			printf("lua: unknow");
		}
	}

	return 0; // number of result
}

// how to call lua function
void l_callme(lua_State* L, int timestamp)
{
	// determine lua function
	int t = lua_getglobal(L, "callme");

	// check "callme" type
	if (t == LUA_TFUNCTION)
	{
		// push argument to stack and make lua call
		lua_pushinteger(L, timestamp);
		
		// make call
		if (lua_pcall(L, 1, 1, 0))
		{
			// process function result
			if (lua_isinteger(L, 1))
			{
				printf("received: %d\n", lua_tointeger(L, -1));
			}
			else
			{
				printf("received: unexpected");
			}
		}
		// failed
		else 
		{
			printf("%s\n", lua_tostring(L, -1)); // print error
		}

		lua_pop(L, 1); // pop function result
	}
	else
	{
		lua_pop(L, 1); // pop function

		printf("Not found or not a function: callme.\n");
	}
}

// entry point
int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR buff[256];
	int error;

	// initialize lua vm
	lua_State *L = luaL_newstate();
	
	if (L == NULL) {
		printf("Cannot create Lua runtime.\n");
		return 0;
	}

	// open default libraries
	luaL_openlibs(L);

	// override lua print function
	lua_register(L, "print", l_custom_print);
	
	// create global variable STARTTIME
	int t = time();	
	lua_pushinteger(L, t);
	lua_setglobal(L, "STARTTIME");	

	// read from stdin while press CTRL^Z
	while (fgets(buff, sizeof(buff), stdin) != NULL) 
	{

		// execute lua function from c
		if (strcmp(buff, "callme\n") == 0) 
		{
			l_callme(L, time());
		}
		// exit from application
		else if (strcmp(buff, "exit\n") == 0) 
		{
			break;
		}
		// execute as lua script
		else 
		{
			error = luaL_dostring(L, buff);
			if (error) {
				fprintf(stderr, "%s", lua_tostring(L, -1));
				lua_pop(L, 1);  /* pop error message from the stack */
			}
		}
	}

	lua_close(L);
	return 0;
}

