#ifndef _INSTANCE_H
#define _INSTANCE_H

typedef struct instance_s * instance_t;

#include "lstage.h"
#include "stage.h"
#include "event.h"

#include "lua.h"

#define LSTAGE_INSTANCE_KEY "lstage-instance-key"

enum instance_flag_t {
	CREATED=0x0,
	IDLE,
	READY,
	WAITING_IO,
	TIMEOUT_IO
};

struct instance_s {
   lua_State * L;
   stage_t stage;
   event_t ev;
   enum instance_flag_t flags;
   instance_t waiting;
   int args;
};

instance_t lstage_newinstance(stage_t s);
void lstage_initinstance(instance_t i);
void lstage_destroyinstance(instance_t i);
void lstage_putinstance(instance_t i);

#endif
