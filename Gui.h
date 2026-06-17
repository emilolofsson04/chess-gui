#ifndef GUI_H
#define GUI_H

struct pngs ReadPiecePngs();
void DrawBoard(char board[8][8], struct pngs Pieces);
struct move InputCheck(int* alreadyClicked, bool* ctp, int* sFile, int* sRank);

#endif
