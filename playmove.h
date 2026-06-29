#ifndef PLAYMOVE_H
#define PLAYMOVE_H


struct move translate(char move[6], bool* ctp);
void capturedPiecesUpdates(struct move playedMove, struct Pieces* CapturedPieces, struct GameState* gs);
void moveupdates(struct move legalMoves[256], int* totMov, int rank, int file, int pInd, int pcolour, int targetRank, int targetFile, int capture, char type, int movetype);
void playmove(struct move playedMove[1], struct GameState* gs);
#endif

