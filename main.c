#include <stdio.h>
#include "board.h"
#include "input.h"
#include "moves.h" 



int main() {
    GameState state;
    init_board(&state);
    
    state.current_player = BLACK;

    printf("Welcome to Checkers!\n");

    while (state.red_count > 0 && state.black_count > 0) {
        render_board(&state);

        Move m = get_player_move(&state);

        apply_move(&state, m); 


        if (state.current_player == BLACK) {
            state.current_player = RED;
        } else {
            state.current_player = BLACK;
        }
    }

    render_board(&state);
    if (state.red_count == 0) {
        printf("Black wins!\n");
    } else {
        printf("Red wins!\n");
    }

    return 0;
}