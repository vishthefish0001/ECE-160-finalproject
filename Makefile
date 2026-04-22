all:
	gcc -Wall main.c board.c input.c moves.c -o checkers

clean:
	rm -f checkers