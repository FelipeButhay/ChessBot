#include "raylib.h"
#include "ChessDisplay.h"

Display::Display(Engine& engine) : engine(engine) {
    this->resolution = 0;
    this->sx = difResolutions[resolution%8][0];
    this->sy = difResolutions[resolution%8][1];
    this->u = sy*0.05f;

    InitWindow(sx, sy, "Chess");
    SetTargetFPS(60);

    this->stage = 0;

    // CARGO LAS PIEZAS BLANCAS EN EN ARRAY
    this->WhitePiecesTexture[0] = LoadTexture("sprites/white-pawn.png");
    this->WhitePiecesTexture[1] = LoadTexture("sprites/white-knight.png");
    this->WhitePiecesTexture[2] = LoadTexture("sprites/white-bishop.png");
    this->WhitePiecesTexture[3] = LoadTexture("sprites/white-rook.png");
    this->WhitePiecesTexture[4] = LoadTexture("sprites/white-queen.png");
    this->WhitePiecesTexture[5] = LoadTexture("sprites/white-king.png");
    // CARGO LAS PIEZAS NEGRAS EN EL ARRAY
    this->BlackPiecesTexture[0] = LoadTexture("sprites/black-pawn.png");
    this->BlackPiecesTexture[1] = LoadTexture("sprites/black-knight.png");
    this->BlackPiecesTexture[2] = LoadTexture("sprites/black-bishop.png");
    this->BlackPiecesTexture[3] = LoadTexture("sprites/black-rook.png");
    this->BlackPiecesTexture[4] = LoadTexture("sprites/black-queen.png");
    this->BlackPiecesTexture[5] = LoadTexture("sprites/black-king.png");
}

void Display::Loop() {
    switch (stage) {
        case 0: 
            LoopMenu(); break;
        case 1: 
            LoopSettings(); break;
        case 2:
        case 3: 
            LoopGame(); break;
    }
}

void Display::Draw() {
    switch (stage) {
        case 0: 
            DrawMenu(); break;
        case 1: 
            DrawSettings(); break;
        case 2:
        case 3: 
            DrawGame(); break;
    }
}

void Display::MovesStrToVec(std::string& MovesStr) {
    // MOVE NOTATION SF,SR,EF,ER
    // PROM NOTATION SF,SR,EF,PieceType
    // CAST NOTATION SF,SR,'O', 2/3
    int sqIndx = 0;

    for (int i = 0; i < MovesStr.length(); i += 5) {
        std::string SubString = "";
        bool ommit = false;

        // chequear prom
        if (MovesStr[i + 3] >= 'A') {
            sqIndx = 56 + MovesStr[i + 2] - '0';
            SubString = MovesStr.substr(i, 3) + "P";
            for (int j = 0; j < PossibleMovesForSelectedPieceVec.size(); j++) {
                if (PossibleMovesForSelectedPieceVec[j].Move4Char == SubString) { 
                    ommit = true; 
                    break;
                }
            }
        }
        // chequear castle
        else if (MovesStr[i + 2] == 'O') {
            int startIndx = (MovesStr[i] - '0') + (MovesStr[i + 1] - '0') * 8;
            if (MovesStr[i + 3] == '2') sqIndx = startIndx + 2;
            if (MovesStr[i + 3] == '3') sqIndx = startIndx - 2;
            SubString = MovesStr.substr(i, 4);
        } 
        // common moves
        else {
            sqIndx = (MovesStr[i + 2] - '0') + (MovesStr[i + 3] - '0') * 8;
            SubString = MovesStr.substr(i, 4);
        }

        MoveForDisplay MoveStruct = { SubString, sqIndx };
        if (!ommit) PossibleMovesForSelectedPieceVec.push_back(MoveStruct);
    }

    std::vector<MoveForDisplay>* arr = &PossibleMovesForSelectedPieceVec;

    for (int i = 1; i<(*arr).size(); i++) {
        for (int j = i; j > 0; j--) {
            if ((*arr)[j].sqIndx < (*arr)[j-1].sqIndx) {
                std::swap((*arr)[j], (*arr)[j-1]);
            } else {
                break;
            }
        }
    }
}