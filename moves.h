#ifndef MOVES_H
#define MOVES_H

#include "board.h"


Move get_full_move(const GameState *state, Move m);

void apply_move(GameState *state, Move m);

#endif 

int must_player_jump(const GameState *state, int player_color);
int can_piece_jump(const GameState *state, int r, int c);
int can_piece_move(const GameState *state, int r, int c);