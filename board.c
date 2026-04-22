#include <stdio.h>
#include "board.h"
#include "moves.h"

void init_board(GameState *state) {
    int r, c;

    for (r = 0; r < BOARD_SIZE; r++) {
        for (c = 0; c < BOARD_SIZE; c++) {
            state->board[r][c].color   = EMPTY; // clear every square before placing pieces
            state->board[r][c].is_king = 0;
        }
    }

    for (r = 0; r < 3; r++) {
        for (c = 0; c < BOARD_SIZE; c++) {
            if ((r + c) % 2 == 1) { // pieces only go on dark squares where row+col is odd
                state->board[r][c].color   = RED;
                state->board[r][c].is_king = 0;
            }
        }
    }

    for (r = 5; r < BOARD_SIZE; r++) {
        for (c = 0; c < BOARD_SIZE; c++) {
            if ((r + c) % 2 == 1) {
                state->board[r][c].color   = BLACK;
                state->board[r][c].is_king = 0;
            }
        }
    }

    state->red_count     = 12; 
    state->black_count   = 12;
    state->history_count = 0;
}

void render_board(const GameState *state) {
    int r, c;

    int must_jump = must_player_jump(state, state->current_player);

    printf("\n      A   B   C   D   E   F   G   H\n");

    for (r = 0; r < BOARD_SIZE; r++) {
        printf("    +---+---+---+---+---+---+---+---+\n");
        printf("  %d |", 8 - r); 

        for (c = 0; c < BOARD_SIZE; c++) {
            Piece p = state->board[r][c];
            if (p.color == EMPTY) {
                printf("   |");
            } 
            else {
                int movable = 0;
                if (p.color == state->current_player) {
                    if (must_jump) {
                        if (can_piece_jump(state, r, c)) movable = 1;
                    } else {
                        if (can_piece_move(state, r, c)) movable = 1;
                    }
                }

                char piece_char;
                if (p.color == RED) {
                    piece_char = p.is_king ? 'R' : 'r';
                } else {
                    piece_char = p.is_king ? 'B' : 'b';
                }

                if (movable) {
                    printf(" \033[1m%c\033[0m |", piece_char);
                } else {
                    printf(" %c |", piece_char);
                }
            }
        }
        printf("\n");
    }
    printf("    +---+---+---+---+---+---+---+---+\n");
    printf("\n  Red: %d pieces   Black: %d pieces\n\n", state->red_count, state->black_count);
}
