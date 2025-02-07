#include "ChessEngine.h"

Engine::Engine(const std::string FEN) {
    // PARTE LA STRING FEN EN CADA UNO DE SUS 6 COMPONENTES
    std::string FENComponents[6];
    int j = 0, i;

    for (i = 0; i<(int)(FEN.length()); i++) {
        if (FEN[i] == ' ') {
            j++;
        } else {
            FENComponents[j] += FEN[i];
        }
    }

    // (1° comp) CARGA LA POSICION EN board[64]
    j = 56;
    for (i = 0; i<(int)(FENComponents[0].length()); i++) {
        if (FENComponents[0][i] == '/') {
            j -= 16;
        } else if (FENComponents[0][i] >= '1' && FENComponents[0][i] <= '8') {
            j += FENComponents[0][i] - '0';
        } else {
            switch (FENComponents[0][i]) {
                case 'P': this->WhitePieces[0] |= 1ULL << j; break;
                case 'N': this->WhitePieces[1] |= 1ULL << j; break;
                case 'B': this->WhitePieces[2] |= 1ULL << j; break;
                case 'R': this->WhitePieces[3] |= 1ULL << j; break;
                case 'Q': this->WhitePieces[4] |= 1ULL << j; break;
                case 'K': this->WhitePieces[5] |= 1ULL << j; break;

                case 'p': this->BlackPieces[0] |= 1ULL << j; break;
                case 'n': this->BlackPieces[1] |= 1ULL << j; break;
                case 'b': this->BlackPieces[2] |= 1ULL << j; break;
                case 'r': this->BlackPieces[3] |= 1ULL << j; break;
                case 'q': this->BlackPieces[4] |= 1ULL << j; break;
                case 'k': this->BlackPieces[5] |= 1ULL << j; break;
            }
            j++;
        }
    }

    this->WhitePiecesOccupied = WhitePieces[0] | WhitePieces[1] | WhitePieces[2] | WhitePieces[3] | WhitePieces[4] | WhitePieces[5];
    this->BlackPiecesOccupied = BlackPieces[0] | BlackPieces[1] | BlackPieces[2] | BlackPieces[3] | BlackPieces[4] | BlackPieces[5];
    this->Occupied = WhitePiecesOccupied | BlackPiecesOccupied;
    this->Empty = ~Occupied;

    // (2° comp) SACA EL TURNO DEL FEN
    switch (FENComponents[1][0]) {
        case 'w': this->BoardVariables.Turn = true;  break;
        case 'b': this->BoardVariables.Turn = false; break;
    }

    // (3° comp) DIRECCIONES DE ENROQUE DISPONIBLES
    for (i = 0; i < (int)FENComponents[2].length(); i++) {
        switch (FENComponents[2][i]) {
            case '-': break;
            case 'K': this->BoardVariables.CastleWK = true; break;
            case 'Q': this->BoardVariables.CastleWQ = true; break;
            case 'k': this->BoardVariables.CastleBK = true; break;
            case 'q': this->BoardVariables.CastleBQ = true; break;
        }
    }

    // (4° comp) CASILLA DE EN PASSANT
    if (FENComponents[3] == "-") {
        this->BoardVariables.EnPassantMask = 0;
    } else {
        // numero * 8 + caracter
        short EnPassantIndx = (FENComponents[3][1]-48)*8 + (FENComponents[3][0]-97);
        this->BoardVariables.EnPassantMask = 1ULL << EnPassantIndx;
    }

    // (5° comp) NUMERO DE MOVIMIENTO PARA LA REGLA DE LOS 50 MOVIMIENTOS
    this->BoardVariables.N50MovesRule = std::stoi(FENComponents[4]);

    // (6° comp) NUMERO DE MOVIMIENTO TOTALES
    this->BoardVariables.NMoves = std::stoi(FENComponents[5]);

    GenerateMoveStr();
}

void Engine::Move(std::string& Move4Char) {
    this->GameMoves += Move4Char + " ";
    this->PossibleMoves = "";

    int StartFile = Move4Char[0] - '0', StartRow = Move4Char[1] - '0';
    int StartSq = StartFile + StartRow*8;

    int PieceTypeMoved;
    if (BoardVariables.Turn) {
        for (int k = 0; k<6; k++) {
            if ((WhitePieces[k] & (1ULL << StartSq)) != 0) {
                this->WhitePieces[k] &= ~(1ULL << StartSq);
                PieceTypeMoved = k;
                break;
            }
        }
    } else {
        for (int k = 0; k<6; k++) {
            if ((BlackPieces[k] & (1ULL << StartSq)) != 0) {
                this->BlackPieces[k] &= ~(1ULL << StartSq);
                PieceTypeMoved = k;
                break;
            }
        }
    }

    // promotions
    if (Move4Char[3] >= 'A') {
        int FinalSq = (Move4Char[2] - '0') + 56;
        int PieceTypePromoted;

        switch ((char)Move4Char[3]){
            case 'N': PieceTypePromoted = 1; break;
            case 'B': PieceTypePromoted = 2; break;
            case 'R': PieceTypePromoted = 3; break;
            case 'Q': PieceTypePromoted = 4; break;
        }

        if (BoardVariables.Turn) this->WhitePieces[PieceTypePromoted] |= (1ULL << FinalSq);
        else                     this->BlackPieces[PieceTypePromoted] |= (1ULL << FinalSq);

        //bora cualquier pieza en el lugar donde moviste la tuya
        if (BoardVariables.Turn) {
            for (int k = 0; k<6; k++) {
                if ((BlackPieces[k] & (1ULL << FinalSq)) != 0) {
                    this->BlackPieces[k] &= ~(1ULL << FinalSq);
                    PieceTypeMoved = k;
                    break;
                }
            }
        } else {
            for (int k = 0; k<6; k++) {
                if ((WhitePieces[k] & (1ULL << FinalSq)) != 0) {
                    this->WhitePieces[k] &= ~(1ULL << FinalSq);
                    PieceTypeMoved = k;
                    break;
                }
            }
        }
    }
    // chequear castle
    else if (Move4Char[2] == 'O') {
        if        (Move4Char[3] == '2' &&  BoardVariables.Turn) { //enroque corto blanco

        } else if (Move4Char[3] == '2' && !BoardVariables.Turn) { //enroque corto negro

        } else if (Move4Char[3] == '3' &&  BoardVariables.Turn) { //enroque largo blanco

        } else if (Move4Char[3] == '3' && !BoardVariables.Turn) { //enroque largo negro

        }
    }
    // common moves
    else {
        int FinalSq = (Move4Char[2] - '0') + (Move4Char[3] - '0') * 8;

        // escribe la pieza movida
        if (BoardVariables.Turn) this->WhitePieces[PieceTypeMoved] |= (1ULL << FinalSq);
        else                     this->BlackPieces[PieceTypeMoved] |= (1ULL << FinalSq);

        //bora cualquier pieza en el lugar donde moviste la tuya
        if (BoardVariables.Turn) {
            for (int k = 0; k<6; k++) {
                if ((BlackPieces[k] & (1ULL << FinalSq)) != 0) {
                    this->BlackPieces[k] &= ~(1ULL << FinalSq);
                    break;
                }
            }
        } else {
            for (int k = 0; k<6; k++) {
                if ((WhitePieces[k] & (1ULL << FinalSq)) != 0) {
                    this->WhitePieces[k] &= ~(1ULL << FinalSq);
                    break;
                }
            }
        }

        // chequea de borrar el peon de en passant
        if ((BoardVariables.EnPassantMask & (1ULL << FinalSq)) != 0) {
            if (BoardVariables.Turn) {
                this->BlackPieces[P] &= ~(1ULL << (FinalSq - 8));
            } else {
                this->WhitePieces[P] &= ~(1ULL << (FinalSq + 8));
            }
        }

        // crea la EnPassantMask
        if (PieceTypeMoved == 0 && std::abs(StartSq - FinalSq) == 16) {
            int EnPassantSq = (StartSq + FinalSq)/2;
            this->BoardVariables.EnPassantMask = 1ULL << EnPassantSq;
        } else {
            this->BoardVariables.EnPassantMask = 0;
        }

        // Reglas para el enroque
        switch (StartSq){
            case 0:  this->BoardVariables.CastleWQ = false; break;
            case 7:  this->BoardVariables.CastleWK = false; break;
            case 56: this->BoardVariables.CastleBQ = false; break;
            case 63: this->BoardVariables.CastleBK = false; break;

            case 4:  this->BoardVariables.CastleWQ = false; 
                     this->BoardVariables.CastleWK = false; break;
            case 60: this->BoardVariables.CastleBQ = false; 
                     this->BoardVariables.CastleBK = false; break;
            default: break;
        }
    }

    this->WhitePiecesOccupied = WhitePieces[0] | WhitePieces[1] | WhitePieces[2] | WhitePieces[3] | WhitePieces[4] | WhitePieces[5];
    this->BlackPiecesOccupied = BlackPieces[0] | BlackPieces[1] | BlackPieces[2] | BlackPieces[3] | BlackPieces[4] | BlackPieces[5];
    this->Occupied = WhitePiecesOccupied | BlackPiecesOccupied;
    this->Empty = ~Occupied;
     
    // numero de movimientos
    if (!BoardVariables.Turn) BoardVariables.NMoves++;

    // cambia el turno
    this->BoardVariables.Turn = !BoardVariables.Turn;

    this->GenerateMoveStr();
}

void Engine::GenerateMoveStr() {
    this->MovesGeneratorPawn();
    this->MovesGeneratorKnight();
    this->MovesGeneratorBishop();
    this->MovesGeneratorRook();
    this->MovesGeneratorQueen();
    this->MovesGeneratorKing();

    std::cout << PossibleMoves << '\n';
}

std::string Engine::FilterMoveString(unsigned short filterSq) {
    std::string PossibleMovesFiltered = "";

    int StartRow = filterSq / 8;
    int StartFile = filterSq % 8;

    std::string StartSquareStr = "--";
    StartSquareStr[0] = StartFile + '0';
    StartSquareStr[1] = StartRow  + '0';

    for (int i = 0; i < PossibleMoves.length(); i += 5) {
        // MOVE NOTATION SF,SR,EF,ER
        // PROM NOTATION SF,SR,EF,PieceType
        // CAST NOTATION SF,SR,'O', 2/3

        if (StartSquareStr == PossibleMoves.substr(i, 2)) {
            PossibleMovesFiltered += PossibleMoves.substr(i, 5);
        }
    }

    return PossibleMovesFiltered;
}


Board Engine::GetBoardVariables() {
    return BoardVariables;
}

std::array<U64, 6> Engine::GetBitboards(bool color) {
    return (color ? WhitePieces : BlackPieces);
}