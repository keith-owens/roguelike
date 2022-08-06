#ifndef DAMAGE_SYSTEM
#define DAMAGE_SYSTEM

#include "external/flecs/flecs.h"
#include "external/klib/kstring.h"
#include "external/stb/stb_ds.h"
#include "components.h"
#include "io.h"

void damage_system(ecs_iter_t* it);
void delete_the_dead(ecs_iter_t* it);

#endif