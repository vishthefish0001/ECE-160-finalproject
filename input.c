#include <stdio.h>
#include <stdlib.h>
#include "input.h" 

static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Move get_player_move(const GameState *state) {
    Move m;
    int valid = 0;
    const char *name = (state->current_player == RED) ? "Red" : "Black";

    int valid_pieces[64][2]; 
    int num_pieces = 0;

    // Scan the board to find which pieces are allowed to move
    int must_jump = must_player_jump(state, state->current_player);

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (state->board[r][c].color == state->current_player) {
                if (must_jump) { 
                    if (can_piece_jump(state, r, c)) {
                        valid_pieces[num_pieces][0] = r;
                        valid_pieces[num_pieces][1] = c;
                        num_pieces++;
                    }
                } else { 
                    if (can_piece_move(state, r, c)) {
                        valid_pieces[num_pieces][0] = r;
                        valid_pieces[num_pieces][1] = c;
                        num_pieces++;
                    }
                }
            }
        }
    }

    if (num_pieces == 0) {
        printf("%s has no legal moves! Game over.\n", name);
        exit(0);
    }

    // Display the menu and get input
    while (!valid) {
        printf("%s's turn. Available pieces to move:\n", name);
        
        for (int i = 0; i < num_pieces; i++) {
            int pr = valid_pieces[i][0];
            int pc = valid_pieces[i][1];
            int is_king = state->board[pr][pc].is_king;

            char col_letter = 'A' + pc;
            int row_number = 8 - pr;

            if (is_king) {
                printf("  %d) King at %c%d\n", i + 1, col_letter, row_number);
            } else {
                printf("  %d) Piece at %c%d\n", i + 1, col_letter, row_number);
            }
        }

        int choice;
        printf("Enter the number of the piece to select: ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_pieces) {
            if (feof(stdin)) exit(1);
            printf("  Error: invalid choice.\n\n");
            flush_stdin();
            continue;
        }
        flush_stdin();

        m.from_row = valid_pieces[choice - 1][0];
        m.from_col = valid_pieces[choice - 1][1];

        // Ask for destination
        char dest_col;
        int dest_row;
        printf("Enter destination (e.g., C4): ");
        if (scanf(" %c%d", &dest_col, &dest_row) != 2) {
            if (feof(stdin)) exit(1);
            printf("  Error: invalid format.\n\n");
            flush_stdin();
            continue;
        }
        flush_stdin();

        // Manual case conversion
        if (dest_col >= 'a' && dest_col <= 'z') {
            dest_col = dest_col - 'a' + 'A'; 
        }

        m.to_col = dest_col - 'A';
        m.to_row = 8 - dest_row;

        if (m.to_col < 0 || m.to_col >= BOARD_SIZE || m.to_row < 0 || m.to_row >= BOARD_SIZE) {
            printf("  Error: coordinates out of bounds.\n\n");
            continue;
        }

        Move full = get_full_move(state, m);
        if (full.jumped_row == -2) {
            printf("  Error: illegal destination. Try again.\n\n");
            continue;
        }

        m = full;
        valid = 1;
    }

    return m;
}
