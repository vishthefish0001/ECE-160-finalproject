#ifndef INPUT_H
#define INPUT_H

#include "board.h"
#include "moves.h"

//full turns are defined as player picking legal move and typing destination
Move get_player_move(const GameState *state);

//called during multi jump turns, piece is fixed at (r,c), so the player chooses the end position
Move get_continuation_jump(const GameState *state, int r, int c);

#endif 
