#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "external/flecs/flecs.h"
#include "external/klib/kstring.h"
#include "components.h"
#include "io.h"

void item_collection_system(ecs_iter_t* it);
void item_drop_system(ecs_iter_t* it);

#endif