#include <stdio.h>
#include "board.h"

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

    printf("\n      A   B   C   D   E   F   G   H\n");

    for (r = 0; r < BOARD_SIZE; r++) {
        printf("    +---+---+---+---+---+---+---+---+\n");
        printf("  %d |", 8 - r); // Rows go 8 down to 1

        for (c = 0; c < BOARD_SIZE; c++) {
            Piece p = state->board[r][c];
            if (p.color == EMPTY) {
                printf("   |");
            } else if (p.color == RED) {
                printf(" %c |", p.is_king ? 'R' : 'r');
            } else {
                printf(" %c |", p.is_king ? 'B' : 'b');
            }
        }
        printf("\n");
    }
    printf("    +---+---+---+---+---+---+---+---+\n");
    printf("\n  Red: %d pieces   Black: %d pieces\n\n", state->red_count, state->black_count);
}
