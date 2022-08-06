#ifndef MELEE_COMBAT_SYSTEM
#define MELEE_COMBAT_SYSTEM

#include <libtcod.h>
#include <stdio.h>
#include "external/flecs/flecs.h"
#include "external/klib/kstring.h"
#include "external/stb/stb_ds.h"
#include "components.h"
#include "io.h"

void melee_combat_system(ecs_iter_t* it);

#endif