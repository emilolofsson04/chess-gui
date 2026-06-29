#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <raylib.h>

struct piece {
    char type;
    bool colour;
    int rank;
    int file;
    int alive;
    int hasBeenMoved;
    int value;
};

struct move {
    int startRank;
    int startFile;
    int targetRank;
    int targetFile;
    int pieceIndex;
    int pieceType;
    int colour;
    int capture;
    int capturedIndex;
    int moveType;
    int promotionPiece;
};     

struct GameState {
    char Board[8][8];
    struct piece whitePieces[16];
    struct piece blackPieces[16];
    int ctp;

};

struct pngs {
    Texture2D wPawn;
    Texture2D wKnight;
    Texture2D wBishop;
    Texture2D wRook;
    Texture2D wQueen;
    Texture2D wKing;

    Texture2D bPawn;
    Texture2D bKnight;
    Texture2D bBishop;
    Texture2D bRook;
    Texture2D bQueen;
    Texture2D bKing;
};

struct Pieces {
    int whitePawns;
    int blackPawns;
    int whiteKnights;
    int blackKnights;
    int whiteBishops;
    int blackBishops;
    int whiteRooks;
    int blackRooks;
    int whiteQueens;
    int blackQueens;
};


typedef struct {
    Color boardLight;
    Color boardDark;
    Color panelBg;
    Color panelBorder;
    Color textMuted;
    Color accentCheck;
    Color WINDOW;
} ChessTheme;

struct Gui {
    int squareSize;
    int xoffset;
    int yoffset;
    int evalbarSize;
    int evalbarY;
    int textboxSize;
    int boardSize;
    int x;
    int y;
};

#endif

