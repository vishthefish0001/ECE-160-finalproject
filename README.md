# ECE-160-finalproject
Checkers in C

Setup: 12 pieces each, placed on dark squares only.

Movement: Standard pieces move diagonally forward one square.

Jumping: If you can jump an opponent's piece, you must do it.

Multi-Jumps: If you land and another jump is available, you must keep jumping in the same turn.

Kings: Reach the back row to get "crowned." Kings can move and jump forward or backward.

Winning: Capture all opponent pieces or block them so they have no legal moves.

      A   B   C   D   E   F   G   H
    +---+---+---+---+---+---+---+---+
  8 |   | r |   | r |   | r |   | r |
    +---+---+---+---+---+---+---+---+
  7 | r |   | r |   | r |   | r |   |
    +---+---+---+---+---+---+---+---+
  6 |   | r |   | r |   | r |   | r |
    +---+---+---+---+---+---+---+---+
  5 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  4 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 | b |   | b |   | b |   | b |   |
    +---+---+---+---+---+---+---+---+
  2 |   | b |   | b |   | b |   | b |
    +---+---+---+---+---+---+---+---+
  1 | b |   | b |   | b |   | b |   |
    +---+---+---+---+---+---+---+---+

b: Black standard piece (moves forward only)

r: Red standard piece (moves forward only)

B: Black King (moves forward and backward)

R: Red King (moves forward and backward)