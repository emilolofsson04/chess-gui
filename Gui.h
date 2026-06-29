#ifndef GUI_H
#define GUI_H

struct pngs ReadPiecePngs(int squareSize);
void DrawGameHistory(struct Gui* gui, char playedMovestrings[256][5], int totPlayed, struct pngs tinypngs, ChessTheme theme, int* scroll);
void DrawEvalBar(struct Gui* gui, float evaluation, ChessTheme theme);
void DrawGameText(struct Gui* gui, int ctp, int check, int checkMate, int staleMate, ChessTheme theme); 
void DrawBoard(struct Gui* gui, ChessTheme theme);
void DrawPieces(char board[8][8], struct pngs Pieces, struct Gui* gui, ChessTheme theme);
void DrawHighlights(struct Gui* gui, int Clicked, int startFile, int startRank, ChessTheme theme) ;
void DrawPromotion(char board[8][8], struct pngs Pieces, struct Gui* gui, int Clicked, int startFile, int startRank, struct move inputMove, int* pChoice, int ctp, ChessTheme theme) ;
void DrawCapturedPieces(struct Gui* gui, struct Pieces CapturedPieces, struct pngs CapturedPiecesPng, ChessTheme theme); 

struct move InputCheck(int* alreadyClicked, int ctp, int* sFile, int* sRank, struct Gui* gui, struct move CurrentMove, int* pChoice);

#endif
