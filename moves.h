#ifndef MOVES_H
#define MOVES_H

#include "board.h"

// functions — used by board.c (render) and input.c (menu) */
int must_player_jump(const GameState *state, int player_color); //used for getting piece highlights and moves in terminal
int can_piece_jump(const GameState *state, int r, int c);
int can_piece_move(const GameState *state, int r, int c);

// return a movs with jumped_row = -1 (simple), >=0 (jump), or -2 (illegal) move 
Move get_full_move(const GameState *state, Move m);

// Executes the move: moves piece, removes captured piece, crowns kings 
void apply_move(GameState *state, Move m);

#endif 
