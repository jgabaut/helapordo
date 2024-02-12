#ifndef TURNOP_H_
#define TURNOP_H_

#include "game_utils.h"
#include "game_debug.h"
#include "game_fight.h"
#include "saves.h"

/**
 * Call function associated with the passed turnOption_OP.
 * @param op The turnOption_OP to execute.
 * @param args Pointer to turnOP_args object.
 * @param kls The Koliseo used for allocations.
 */
OP_res turnOP(turnOption_OP op, turnOP_args * args, Koliseo * kls,
              Koliseo_Temp * t_kls);
#endif // TURNOP_H_
