all:
	gcc -Wall main.c board.c input.c moves.c -o checkers.out

clean:
	rm -f checkers.out