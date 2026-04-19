#include <stdio.h>
#include <stdlib.h>
#include "input.h"

static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // clears leftover newline so the next scanf doesnt read it as empty input
}

Move get_player_move(const GameState *state) {
    Move m;
    int  from, to;
    int  valid = 0;

    const char *name = (state->current_player == RED) ? "Red" : "Black";

    while (!valid) {
        printf("%s's turn. Enter move (e.g. 32 43): ", name);

        if (scanf("%d %d", &from, &to) != 2) { // scanf returns how many items it read, if not 2 the format was wrong
            if (feof(stdin)) {
                printf("\nInput ended.\n");
                exit(1);
            }
            printf("  Error: invalid format. Use two numbers like '32 43'.\n");
            flush_stdin();
            continue;
        }
        flush_stdin();

        m.from_row = from / 10; // split 2-digit number into row and col, so "32" becomes row=3 col=2
        m.from_col = from % 10;
        m.to_row   = to   / 10;
        m.to_col   = to   % 10;
        m.jumped_row = -1;
        m.jumped_col = -1;

        if (m.from_row < 0 || m.from_row >= BOARD_SIZE ||
            m.from_col < 0 || m.from_col >= BOARD_SIZE ||
            m.to_row   < 0 || m.to_row   >= BOARD_SIZE ||
            m.to_col   < 0 || m.to_col   >= BOARD_SIZE) {
            printf("  Error: coordinates out of bounds (0-7 only).\n"); // check bounds before anything else
            continue;
        }

        if (state->board[m.from_row][m.from_col].color != state->current_player) {
            printf("  Error: that piece does not belong to you.\n"); // cant move the opponents piece
            continue;
        }

        Move full = get_full_move(state, m);
        if (full.jumped_row == -2) { // -2 means the move wasnt in the legal list, also catches mandatory jump violations
            printf("  Error: illegal move. Remember — jumps are mandatory.\n");
            continue;
        }

        m     = full; // replace partial move with full move so jumped coords are included
        valid = 1;
    }

    return m;
}
