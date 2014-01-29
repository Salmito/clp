/*Adapted from https://github.com/Tieske/Lua_library_template/*/

/*
** ===============================================================
** Leda is a parallel and concurrent framework for Lua.
** Copyright 2014: Tiago Salmito
** License MIT
** ===============================================================
*/

#ifndef stage_h
#define stage_h

typedef struct leda_Stage * stage_t;

#include "leda.h"
#include "lf_queue.h"

enum stage_flag_t {
	DESTROYED=0x01
};

struct leda_Stage {
	LFqueue_t instances;
	LFqueue_t event_queue;
	char * env;
	size_t env_len;
	volatile unsigned int flags;
   int stateful;
};

stage_t leda_tostage(lua_State *L, int i);

#endif
