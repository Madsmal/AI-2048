# 2048-AI

2048-AI implements expectimax algorithm. It uses a efficient data structure to represent the game board and facilitate game operations. A great deal of work on game heuristics has improved the AI significantly.

Further improvement can be adjusting parameters in a machine learning approach, rather than hard coding all of them. It is not trivial for human to explore how to acheive pretty high score, for example, merging a 16384 tile. Hopefully, a more advanced AI may figure out the pattern.

performance:
    2048: 99% 
    4096: 89% 
    8192: 65% 
    16384: 4% 

average time for a move:
    30 ms 

To run the executable, go to the src directory and run ./main in command line.

To recompile all source files, go to the src directory and run make in command line.
