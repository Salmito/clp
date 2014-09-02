#include "instance.h"
#include "marshal.h"
#include "scheduler.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifdef DEBUG

#endif 

static const struct luaL_Reg InstanceLibs[] = {
		{"io",luaopen_io},
		{"os",luaopen_os},
		{"table",luaopen_table},
		{"string",luaopen_string},
		{"math",luaopen_math},
		{"debug",luaopen_debug},
 	#if LUA_VERSION_NUM > 501
		{"coroutine",luaopen_coroutine},
 	#endif
		{NULL,NULL}
};

void lstage_initinstance(instance_t i) {
  	_DEBUG("Initiating instance %p\n",i);
	lua_State *L=i->L;
	lua_pushliteral(L,LSTAGE_INSTANCE_KEY);
	lua_pushlightuserdata(L,i);
	lua_settable(L, LUA_REGISTRYINDEX);	
	lua_pushcfunction(L,luaopen_base);
   lua_pcall(L,0,0,0);
   lua_pushcfunction(L,luaopen_package);
   lua_pcall(L,0,1,0);
   lua_getfield(L,-1,"preload");
	LUA_REGISTER(L,InstanceLibs);
   lua_pop(L,2);
//	luaL_openlibs(L);
	lua_pushliteral(L,STAGE_HANDLER_KEY);
	luaL_loadstring(L,"local a={...} "
							"local h=a[1].f "
							"a[1].e = a[1].e or require'debug'.traceback "
	                  "local i=a[2] "
	                  "local o=a[3] "
	                  "return require'coroutine'.wrap(function() while true do o:push(h(i:get())) end end)"
	                  );
	lua_pushcfunction(L,mar_decode);
	lua_pushlstring(L,i->stage->env,i->stage->env_len);
	lua_call(L,1,1);
	lua_pushvalue(L,-1);
	lua_setfield(L, LUA_REGISTRYINDEX,LSTAGE_ENV_KEY);
	lstage_pushchannel(L,i->stage->input);
	lstage_pushchannel(L,i->stage->output);
	lua_call(L,3,1);
	lua_settable(L, LUA_REGISTRYINDEX);
	lua_getfield(L, LUA_REGISTRYINDEX,LSTAGE_ENV_KEY);
	lua_getfield(L,-1,"e");
	lua_setfield(L,LUA_REGISTRYINDEX,LSTAGE_ERRORFUNCTION_KEY);
	lua_pop(L,1);
	i->flags=I_READY;
}

instance_t lstage_newinstance(stage_t s) {
   lua_State * L = luaL_newstate();
	instance_t i=malloc(sizeof(struct instance_s));
	i->L=L;
	i->stage=s;
	i->flags=I_CREATED;
	i->ev=NULL;
	lstage_pushinstance(i);
	return i;
}

void lstage_destroyinstance(instance_t i) {
   lua_close(i->L);
   if(i->ev) lstage_destroyevent(i->ev);
   free(i);
}
