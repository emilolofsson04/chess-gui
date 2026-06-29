#ifndef MOVEGEN_H
#define MOVEGEN_H

bool isFree(char board[8][8], int targetRank, int targetFile );
int promotionCheck(struct move inputMove, char board[8][8], struct move legalMoves[256], int* totLegMov);
bool pawnmove(int ctp, int* totMov, struct move legalMoves[256], int pInd, int pcolour, char type, int rank, int file, char board[8][8], int length,  int rdir, int fdir, int step, struct move* playedMoves);

bool legalMove(int ctp, int* totMov, struct move legalMoves[256], int pInd, int pcolour, char type, int rank, int file, char board[8][8], int length,  int rdir, int fdir, int step);

void AllMoves(int* totMov, struct move legalMoves[256], struct GameState* gs,  struct move* playedMoves);
struct move isMoveLegal(int* totMov, struct move inputMove, struct move legalMoves[256]);

#endif
