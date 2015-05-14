/* This file is part of the ChessAInt project 2015 */
/**
 * @file
 * @todo handle error with a real stderr file
 * @todo create functions to open/close files
 * @todo parse uci strings
 */

#include "include/loopuci.h"
#include "include/uci.h"
#include "include/astar.h"

/**
 *  Choose to use astar (set to 0) or not (set to 1)
 *
 */
#define RANDOM_MOVE 0

/**
 *  Choose to use uci or not (if 1, astar_time is set to 5)
 *  else it is changed by using go x x astar_time x x x
 *
 */
#define USE_UCI 0

/**
 * receive a uci string and act accordingly
 *
 * @return 0 if the gui stop
 * @return 1 otherwise
 */
int uciLoop(FILE* log, char* buffer, Graph *graph) {
  char uciBuffer[5] = "zzzz";
  char* word = NULL;
  int astar_time = 5;
  receive(log, buffer);
  char* firstWord = getFirstWord(buffer);
  // strcmp second parameter
  // should end with \n if the word is the only one
  if (strcmp(firstWord, "uci\n") == 0) {
      send(log, "id name ChessAInt");
      send(log, "uciok");
  } else if (strcmp(firstWord, "isready\n") == 0) {
      send(log, "readyok");
  } else if (strcmp(firstWord, "ucinewgame\n") == 0) {
  } else if (strcmp(firstWord, "position") == 0) {
      word = getNextWord();  // "startpos"
      // put the root board in the "startpos" position
      initAGame(&graph->root);

      word = getNextWord();  // "moves"

      word = getNextWord();  // the first move

      while (getLastCharacter(word) != '\n') {
        play_move(uciToMove(word), &graph->root);
        word = getNextWord();
      }

      rmUCILastCharacter(word, uciBuffer);
      play_move(uciToMove(word), &graph->root);

  } else if (strcmp(firstWord, "go") == 0) {
      // ignore all these parameters but assume they exist
      word = getNextWord();  // "wtime"
      word = getNextWord();  // number
      word = getNextWord();  // "btime"
      if (!USE_UCI)
        astar_time = atoi(word);
      word = getNextWord();  // number
      word = getNextWord();  // "movestogo"
      word = getNextWord();  // number

      graph->current_node = graph->root;

      graph->current_node.activeColor = black;

      // empty stack
      while (stack_pop(&(graph->current_moves)) != -1) {}

      int bestmove = 0;

      if (RANDOM_MOVE) {
        movesGenerator(graph);
        bestmove = pickBestMove(&(graph->current_moves));
      } else {
        int depth = -1;
        int stop = 0; /* do not change */
        int nodes = -1; /* the max number of nodes -1 <=> +inf */
        int ret = -1; /* do not change */
        ret = astar(graph, 1000, depth, astar_time, nodes, &stop, &bestmove);
        printf("astar ret = %d\n", ret); /* to delete */
        if (ret == 32)
          return 0;
      }

      int a, b, c, d;
      stack_expand(&a, &b, &c, &d, bestmove);
      getUciString(a, b, c, d, uciBuffer);

      char bestmoveString[20];
      snprintf(bestmoveString, sizeof(bestmoveString),
               "bestmove %s", uciBuffer);

      send(log, bestmoveString);

  } else if (strcmp(firstWord, "quit\n") == 0) {
      return 0;
  } else {}
  return 1;
}

/**
 * Choose a random move from the stack generated by
 * generateMoves
 */
int pickBestMove(Stack *current_moves) {
  int move = 0;
  for (int i = 0, rd = rand() % stack_length(*current_moves) ; i < rd + 1 ; ++i) //NOLINT
    move = stack_pop(current_moves);
  return move;
}

