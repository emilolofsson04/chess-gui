#include <raylib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "structs.h"
#include "initialize.h"
#include "board.h"

Texture2D LoadPieceTexture(const char *path, int squareSize) {
    Image img = LoadImage(path);
    ImageResize(&img, squareSize, squareSize);

    Texture2D tex = LoadTextureFromImage(img);

    UnloadImage(img);   
    return tex;
}

struct pngs ReadPiecePngs(int squareSize) {
    struct pngs Pieces;

    Pieces.wPawn   = LoadPieceTexture("piecespng/Chess_plt60.png", squareSize);
    //Pieces.wKnight = LoadPieceTexture("piecespng/Chess_nlt60.png", squareSize); // Obsolete
    Pieces.wKnight = LoadPieceTexture("piecespng/wKnightCool.png", squareSize - 15*squareSize/120);
    Pieces.wBishop = LoadPieceTexture("piecespng/Chess_blt60.png", squareSize);
    Pieces.wRook   = LoadPieceTexture("piecespng/Chess_rlt60.png", squareSize);
    Pieces.wQueen  = LoadPieceTexture("piecespng/Chess_qlt60.png", squareSize);
    Pieces.wKing   = LoadPieceTexture("piecespng/Chess_klt60.png", squareSize);

    Pieces.bPawn   = LoadPieceTexture("piecespng/Chess_pdt60.png", squareSize);
    //Pieces.bKnight = LoadPieceTexture("piecespng/Chess_ndt60.png", squareSize); // Obsolete
    Pieces.bKnight = LoadPieceTexture("piecespng/bKnightCool.png", squareSize - 15*squareSize/120);
    Pieces.bBishop = LoadPieceTexture("piecespng/Chess_bdt60.png", squareSize);
    Pieces.bRook   = LoadPieceTexture("piecespng/Chess_rdt60.png", squareSize);
    Pieces.bQueen  = LoadPieceTexture("piecespng/Chess_qdt60.png", squareSize);
    Pieces.bKing   = LoadPieceTexture("piecespng/Chess_kdt60.png", squareSize);

    return Pieces;

}

void DrawPieceAt(Texture2D tex, int i, int k, int squareSize, int xoffset, int yoffset) {
    int posX = i * squareSize + xoffset + (squareSize / 2) - (tex.width / 2);
    int posY = (7 - k) * squareSize + yoffset + (squareSize / 2) - (tex.height / 2);
    DrawTexture(tex, posX, posY, WHITE);
}

Texture2D GetPieceTexture(char type, struct pngs Pieces) {
    switch (type) {
        case 'P': return Pieces.bPawn;   case 'p': return Pieces.wPawn;
        case 'N': return Pieces.bKnight; case 'n': return Pieces.wKnight;
        case 'B': return Pieces.bBishop; case 'b': return Pieces.wBishop;
        case 'R': return Pieces.bRook;   case 'r': return Pieces.wRook;
        case 'Q': return Pieces.bQueen;  case 'q': return Pieces.wQueen;
        case 'K': return Pieces.bKing;   case 'k': return Pieces.wKing;
        default:  return (Texture2D){ 0 };
    }
}
void DrawGameHistory(struct Gui* gui, char playedMovestrings[256][5], int totPlayed, struct pngs tinypngs, ChessTheme theme, int* scroll) {
    
    int startX = 2* gui->xoffset + gui->boardSize - 15; 
    int startY = gui->yoffset + gui->squareSize;
    int textStart = startY + 4;
    int textSize = 30;
    int widthX = gui->textboxSize - gui->xoffset;
    int rowHeight = (textSize + 12);
    int maxRows = 14;
    int bottomY = maxRows * rowHeight + startY;
    int posY;
    

    DrawRectangle(startX - 4, startY - 4, widthX + 8, bottomY - startY + 8, theme.panelBorder);
    DrawRectangle(startX, startY, widthX, bottomY - startY, theme.panelBg);
    
    int cutoff = (totPlayed - maxRows*2 > 0) ? (totPlayed - maxRows*2 + 1)/2 : 0;
    cutoff = 2*cutoff;
    int k = 0;
     
    Vector2 mouse = GetMousePosition();
        
    if (mouse.x > startX && mouse.x < startX + widthX && mouse.y > startY && mouse.y < (bottomY)) {
            *scroll -= (float)(GetMouseWheelMove() * 10);
    }

    int minScroll = -((totPlayed + 1)/2 - maxRows) * rowHeight;
    if (minScroll > 0)
        minScroll = 0;


    if (*scroll > 0)
        *scroll = 0;
    if (*scroll < minScroll)
        *scroll = minScroll;

    int shiftAmount = - *scroll;

    BeginScissorMode(startX, startY, widthX, bottomY - startY);
    while (k < totPlayed) {
        posY = textStart + (k - cutoff)/2*rowHeight + shiftAmount;
        DrawTexture(tinypngs.wPawn, startX + 5, posY + 3, WHITE);
        DrawText(TextFormat("%s", playedMovestrings[k]), startX +  tinypngs.wPawn.width + 5, posY + 6, 30, theme.textMuted);
            if (k + 1 < totPlayed) {
            DrawTexture(tinypngs.bPawn, startX + (widthX)/2 + 5, posY + 3, WHITE);
            DrawText(TextFormat("%s", playedMovestrings[k + 1]), startX + (widthX)/2 + tinypngs.bPawn.width + 5, posY + 6, 30, theme.textMuted);
            DrawLine(startX, posY + rowHeight, startX + widthX, posY + rowHeight, GRAY);
            DrawLine(startX, posY + rowHeight, startX + widthX, posY + rowHeight, GRAY);
        }
        k+= 2;
    }
    EndScissorMode();
}

void DrawEvalBar(struct Gui* gui, float evaluation, ChessTheme theme) {
    int boardSize = gui->boardSize;
    int evalbarX = gui->evalbarSize;
    int yoffset = gui->yoffset;
    int xoffset = gui->xoffset - 30;

    float advantage = (evaluation > 0) ? log(1 + evaluation) : log(1 - evaluation); 
    advantage = (advantage > 3) ? 3 : advantage;
    DrawRectangle(xoffset - 3 - 10 , yoffset - 3, evalbarX + 6, boardSize+6, theme.panelBorder);

    if (evaluation > 0) {
        DrawRectangle(xoffset - 10, yoffset, evalbarX, (1 - advantage/3) * boardSize/2, theme.boardDark);
        DrawRectangle(xoffset - 10, yoffset + (1 - advantage/3) * boardSize/2, evalbarX, boardSize - (1 - advantage/3) * boardSize/2, theme.boardLight);
    } 
    else {
        DrawRectangle(xoffset - 10, yoffset, evalbarX, (1 + advantage/3) * boardSize/2, theme.boardDark);
        DrawRectangle(xoffset - 10, yoffset + (1 + advantage/3) * boardSize/2, evalbarX, boardSize - (1 + advantage/3) * boardSize/2, theme.boardLight);
    }
}

void DrawGameText(struct Gui* gui, int ctp, int check, int checkMate, int staleMate, ChessTheme theme) {
    int squareSize = gui->squareSize;
    int xoffset = gui->xoffset;
    int yoffset = gui->yoffset;
    int textBoxSize = gui->textboxSize;
    int boardSize = gui->boardSize;
        if (checkMate) {
            DrawText("Game Over - Checkmate!", xoffset + boardSize/2, yoffset + boardSize/2, 40, RED); 
        }
        else if (staleMate) {
            DrawText("Game Over - Stalemate!", xoffset + boardSize/2, yoffset + boardSize/2, 40, RED); 
        }
        else {
            if(ctp) {
                if (check) {
                    DrawText("White is in check", xoffset + 20 , boardSize + yoffset + 6, 20, theme.textMuted);     
                }
                else {
                    DrawText("White to move", xoffset + 20 , boardSize + yoffset + 6, 20, theme.textMuted);     
                }
            }
            else {
                if (check) {
                    DrawText("Black is in check", xoffset + 20 , boardSize + yoffset + 6, 20, theme.textMuted);     
                }
                else {
                    DrawText("Black to move", xoffset + 20 , boardSize + yoffset + 6, 20, theme.textMuted);     
                }
            }
        }
}

void DrawBoard(struct Gui* gui, ChessTheme theme) {
   
    // Draw the board
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            Color squareColor = ((i + k) % 2 == 0) ? theme.boardLight : theme.boardDark;
            DrawRectangle(i * gui->squareSize + gui->xoffset, k * gui->squareSize + gui->yoffset, gui->squareSize, gui->squareSize, squareColor);
        }
    }
}
void DrawPieces(char board[8][8], struct pngs Pieces, struct Gui* gui, ChessTheme theme) {
 
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            if (board[k][i] != '.') {
                Texture2D tex = GetPieceTexture(board[k][i], Pieces);
                DrawPieceAt(tex, i, k, gui->squareSize, gui->xoffset, gui->yoffset);
            }
        }
    }
}

void DrawHighlights(struct Gui* gui, int Clicked, int startFile, int startRank, ChessTheme theme) {
    if (Clicked) {
        int cx = gui->xoffset + gui->squareSize * startFile + gui->squareSize / 2;
        int cy = gui->yoffset + startRank * gui->squareSize + gui->squareSize / 2;
        float radius = gui->squareSize / 2.0f - 10.0f;
        DrawRing((Vector2){(float)cx, (float)cy}, radius - 5.0f, radius, 0.0f, 360.0f, 0, GRAY);
    }
}
void DrawPromotion(char board[8][8], struct pngs Pieces, struct Gui* gui, int Clicked, int startFile, int startRank, struct move inputMove, int* pChoice, int ctp, ChessTheme theme) {

    int squareSize = gui->squareSize;
    int xoffset = gui->xoffset;
    int yoffset = gui->yoffset;
    
 
    if (*pChoice) {
        int targetFile = inputMove.startFile;
        int targetRank = inputMove.startRank;

        float yShift = (ctp) ? 1.0f : -1.0f;
        int boxX = xoffset + (targetFile - 1) * squareSize;
        int boxY = yoffset + (8 - targetRank + yShift) * squareSize;

        DrawRectangle(boxX - 2, boxY - 2, 4 * squareSize + 4, squareSize + 4, theme.panelBorder);
        DrawRectangle(boxX, boxY, 4 * squareSize, squareSize, theme.panelBg);

        Texture2D options[4] = {
            (ctp) ? Pieces.wKnight : Pieces.bKnight,
            (ctp) ? Pieces.wBishop : Pieces.bBishop,
            (ctp) ? Pieces.wRook   : Pieces.bRook,
            (ctp) ? Pieces.wQueen  : Pieces.bQueen
        };

        for (int step = 0; step < 4; step++) {
            int itemX = boxX + (step * squareSize) + (squareSize / 2) - (options[step].width / 2);
            int itemY = boxY + (squareSize / 2) - (options[step].height / 2);
            DrawTexture(options[step], itemX, itemY, WHITE);
        }
    }
}
void DrawCapturedPiece(int squareSize, int* posx, int posy, char type, struct pngs CapturedPiecesPng, int Captured, ChessTheme theme)  {

        Texture2D tex = GetPieceTexture(type, CapturedPiecesPng);
        int copies = (Captured > 3) ? 3 : Captured;
        for (int i = 0; i < copies; i++) {
            DrawTexture(tex, *posx , posy, WHITE);
            *posx = *posx + 20;
        }
        *posx = *posx + (tex.width - 60) ;

}

void DrawCapturedPieces(struct Gui* gui, struct Pieces CapturedPieces, struct pngs CapturedPiecesPng, ChessTheme theme) {
    // White captured Pieces
    int squareSize = gui->squareSize;
    int posX = gui->xoffset*2 + gui->boardSize - 10; 
    int posY = gui->yoffset + 10;
    int* posx = &posX;
    int* posy = &posY;
    
    char wPieces[5] = {'q','r','b','n','p'};
    int wCaptured[5] = {CapturedPieces.whiteQueens, CapturedPieces.whiteRooks, CapturedPieces.whiteBishops, CapturedPieces.whiteKnights, CapturedPieces.whitePawns};

    for (int k = 0; k < 5; k++) {
        if (wCaptured[k]) {
            DrawCapturedPiece(squareSize, posx, posY, wPieces[k], CapturedPiecesPng, wCaptured[k], theme);
        }
    }

    char bPieces[5] = {'Q','R','B','N','P'};
    int bCaptured[5] = {CapturedPieces.blackQueens, CapturedPieces.blackRooks, CapturedPieces.blackBishops, CapturedPieces.blackKnights, CapturedPieces.blackPawns};

    posY = gui->yoffset + gui->boardSize -10 - gui->squareSize/3*2;
    posX = gui->xoffset*2 + gui->boardSize -10;
    for (int k = 0; k < 5; k++) {
        if (bCaptured[k]) {
            DrawCapturedPiece(squareSize, posx, posY, bPieces[k], CapturedPiecesPng, bCaptured[k], theme);
        }
    }
}

struct move InputCheck(int* alreadyClicked, int ctp, int* sFile, int* sRank, struct Gui* gui, struct move CurrentMove, int* pChoice) {
    int squareSize = gui->squareSize;
    int xoffset = gui->xoffset;
    int yoffset = gui->yoffset;
        Vector2 mouse = GetMousePosition();
        int tcol;
        int trow;

        struct move playedMove;
        playedMove.colour = -1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (*pChoice) {
                int targetFile = CurrentMove.startFile;
                int targetRank = CurrentMove.startRank;


                float yShift = (ctp) ? 1.0f : -1.0f;
                int boxX = xoffset + (targetFile - 1) * squareSize;
                int boxY = yoffset + (8 - targetRank + yShift) * squareSize;
                
                if (mouse.x >= boxX && mouse.y >= boxY && mouse.x < boxX + 4* squareSize && mouse.y < boxY + squareSize) {

                    tcol = (mouse.x - boxX) / squareSize;
                    switch (tcol) {
                        case 0:
                            playedMove.promotionPiece = 3;
                            break;
                        case 1:
                            playedMove.promotionPiece = 2;
                            break;
                        case 2:
                            playedMove.promotionPiece = 1;
                            break;
                        case 3:
                            playedMove.promotionPiece = 0;
                            break;
                    }
                    (*pChoice) = 0;
                    playedMove.startRank = CurrentMove.startRank;
                    playedMove.startFile = CurrentMove.startFile;
                    playedMove.targetRank = CurrentMove.targetRank;
                    playedMove.targetFile = CurrentMove.targetFile;
                    playedMove.colour = ctp;
                }

            }
            else { 
                if (mouse.x >= xoffset && mouse.y >= yoffset && mouse.x < (xoffset + 8*squareSize) && mouse.y < (yoffset + 8*squareSize)) {
                    if (*alreadyClicked) {    
                        tcol = (mouse.x - xoffset)  / squareSize; 
                        trow = (mouse.y - yoffset) / squareSize;
                       
                        *alreadyClicked = 0;
                        playedMove.startRank = 8 - *sRank;
                        playedMove.startFile = *sFile + 1;
                        playedMove.targetRank = 8 - trow;
                        playedMove.targetFile = tcol + 1;
                        playedMove.colour = ctp;
                        playedMove.promotionPiece = -1;
                    }
                    else {
                        *sFile = (mouse.x - xoffset)  / squareSize;
                        *sRank = (mouse.y - yoffset) / squareSize; 
                        *alreadyClicked = 1;
                    }
                }
            }
        }
        return playedMove;
}
