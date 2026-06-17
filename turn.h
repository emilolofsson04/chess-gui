#ifndef TURN_H
#define TURN_H

void turn(bool* ctp, bool* agame, char move[7], char board[8][8], struct piece wPieces[16], struct piece bPieces[16], int* iteration, struct move playedMoves[256], struct move Moves[256], int* totMov, struct move legalMoves[256], int* totLegMov);
 


#endif

