#ifndef MONSTER_AI_SYSTEM_H
#define MONSTER_AI_SYSTEM_H

#include "external/flecs/flecs.h"
#include <stdio.h>

#include "components.h"
#include "io.h"

void monster_ai(ecs_iter_t* it);

#endif