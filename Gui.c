
#include <raylib.h>
#include <stdio.h>
#include <ctype.h>

#include "structs.h"
#include "initialize.h"
#include "board.h"

Texture2D LoadPieceTexture(const char *path) {
    Image img = LoadImage(path);
    ImageResize(&img, 120, 120);

    Texture2D tex = LoadTextureFromImage(img);

    UnloadImage(img);   
    return tex;
}

struct pngs ReadPiecePngs() {
    struct pngs Pieces;

    Pieces.wPawn   = LoadPieceTexture("piecespng/Chess_plt60.png");
    Pieces.wKnight = LoadPieceTexture("piecespng/Chess_nlt60.png");
    Pieces.wBishop = LoadPieceTexture("piecespng/Chess_blt60.png");
    Pieces.wRook   = LoadPieceTexture("piecespng/Chess_rlt60.png");
    Pieces.wQueen  = LoadPieceTexture("piecespng/Chess_qlt60.png");
    Pieces.wKing   = LoadPieceTexture("piecespng/Chess_klt60.png");

    Pieces.bPawn   = LoadPieceTexture("piecespng/Chess_pdt60.png");
    Pieces.bKnight = LoadPieceTexture("piecespng/Chess_ndt60.png");
    Pieces.bBishop = LoadPieceTexture("piecespng/Chess_bdt60.png");
    Pieces.bRook   = LoadPieceTexture("piecespng/Chess_rdt60.png");
    Pieces.bQueen  = LoadPieceTexture("piecespng/Chess_qdt60.png");
    Pieces.bKing   = LoadPieceTexture("piecespng/Chess_kdt60.png");

    return Pieces;

}

void DrawBoard(char board[8][8], struct pngs Pieces) {
        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 8; k++) {
                if ((i%2 == 0 && k%2 == 0) || (i%2 == 1 && k%2 == 1)) {
                    DrawRectangle(i*120, k*120, 120, 120, WHITE);
                }
                else {
                    DrawRectangle(i*120, k*120, 120, 120, GREEN);
                }
            }
        }
        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 8; k++) {
                if (board[k][i] != '.') {
                    if (isupper(board[k][i])) {
                        if (board[k][i] == 'P') {
                            DrawTexture(Pieces.bPawn, i*120 + 60 - Pieces.bPawn.width/2, 840 -k*120 + 60 - Pieces.bPawn.height/2, WHITE);
                        }
                        if (board[k][i] == 'B') {
                            DrawTexture(Pieces.bBishop, i*120 + 60 - Pieces.bBishop.width/2, 840 -k*120 + 60 - Pieces.bBishop.height/2, WHITE);
                        }
                        if (board[k][i] == 'R') {
                            DrawTexture(Pieces.bRook, i*120 + 60 - Pieces.bRook.width/2, 840 -k*120 + 60 - Pieces.bRook.height/2, WHITE);
                        }
                        if (board[k][i] == 'Q') {
                            DrawTexture(Pieces.bQueen, i*120 + 60 - Pieces.bQueen.width/2, 840 -k*120 + 60 - Pieces.bQueen.height/2, WHITE);
                        }
                        if (board[k][i] == 'K') {
                            DrawTexture(Pieces.bKing, i*120 + 60 - Pieces.bKing.width/2, 840 -k*120 + 60 - Pieces.bKing.height/2, WHITE);
                        }
                        if (board[k][i] == 'N') {
                            DrawTexture(Pieces.bKnight, i*120 + 60 - Pieces.bKnight.width/2, 840 -k*120 + 60 - Pieces.bKnight.height/2, WHITE);
                        }
                    }
                    else {
                        if (board[k][i] == 'p') {
                            DrawTexture(Pieces.wPawn, i*120 + 60 - Pieces.wPawn.width/2, 840 -k*120 + 60 - Pieces.wPawn.height/2, WHITE);
                        }
                        if (board[k][i] == 'b') {
                            DrawTexture(Pieces.wBishop, i*120 + 60 - Pieces.wBishop.width/2, 840 -k*120 + 60 - Pieces.wBishop.height/2, WHITE);
                        }
                        if (board[k][i] == 'r') {
                            DrawTexture(Pieces.wRook, i*120 + 60 - Pieces.wRook.width/2, 840 -k*120 + 60 - Pieces.wRook.height/2, WHITE);
                        }
                        if (board[k][i] == 'q') {
                            DrawTexture(Pieces.wQueen, i*120 + 60 - Pieces.wQueen.width/2, 840 -k*120 + 60 - Pieces.wQueen.height/2, WHITE);
                        }
                        if (board[k][i] == 'k') {
                            DrawTexture(Pieces.wKing, i*120 + 60 - Pieces.wKing.width/2, 840 -k*120 + 60 - Pieces.wKing.height/2, WHITE);
                        }
                        if (board[k][i] == 'n') {
                            DrawTexture(Pieces.wKnight, i*120 + 60 - Pieces.wKnight.width/2, 840 -k*120 + 60 - Pieces.wKnight.height/2, WHITE);
                        }

                    }
                }
            }
        }
} 
struct move InputCheck(int* alreadyClicked, bool* ctp, int* sFile, int* sRank) {
        Vector2 mouse = GetMousePosition();
        int tcol;
        int trow;

        struct move playedMove;
        playedMove.colour = -1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (*alreadyClicked) {    
                tcol = mouse.x / 120; 
                trow = mouse.y / 120;
               
                printf("Move target: rank %d, file %d\n", 8 - trow , tcol + 1);
                *alreadyClicked = 0;
                playedMove.startRank = 8 - *sRank;
                playedMove.startFile = *sFile + 1;
                playedMove.targetRank = 8 - trow;
                playedMove.targetFile = tcol + 1;
                playedMove.colour = *ctp;
                playedMove.promotionPiece = -1;
                }
            else {
                *sFile = mouse.x / 120;
                *sRank = mouse.y / 120; 
                *alreadyClicked = 1;
                printf("Move starting: rank %d, file %d\n", 8 - *sRank , *sFile + 1);
            }
        }
        return playedMove;
}

