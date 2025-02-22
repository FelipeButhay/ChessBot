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

void Engine::Move(MoveData& Move) {
    this->GameMoves.push_back(Move);

    this->PossibleMoves.clear();

    if (BoardVariables.Turn) this->BoardVariables.N50MovesRule++;

    short int FinalSq  = (Move.Data & EndMask);
    short int StartSq  = (Move.Data & StartMask) >> 6;

    U64 FinalBitBoard = 1ULL << FinalSq;
    U64 StartBitBoard = 1ULL << StartSq;

    short int MoveFlag = (Move.Data & FlagMask) >> 12;

    short int PieceTypeMoved = -1;
    //std::cout << "Turno: " << BoardVariables.Turn << '\n';
    if (BoardVariables.Turn) {
        for (int k = 0; k<6; k++) {
            if ((WhitePieces[k] & StartBitBoard) != 0) {
                this->WhitePieces[k] &= ~StartBitBoard;
                PieceTypeMoved = k;
                break;
            }
        }
    } else {
        for (int k = 0; k<6; k++) {
            if ((BlackPieces[k] & StartBitBoard) != 0) {
                this->BlackPieces[k] &= ~StartBitBoard;
                PieceTypeMoved = k;
                break;
            }
        }
    }

    if (PieceTypeMoved == P) this->BoardVariables.N50MovesRule = 0;

    // escribe la pieza movida
    if (BoardVariables.Turn) this->WhitePieces[PieceTypeMoved] |= FinalBitBoard;
    else                     this->BlackPieces[PieceTypeMoved] |= FinalBitBoard;

    //bora cualquier pieza en el lugar donde moviste la tuya
    if (BoardVariables.Turn) {
        for (int k = 0; k<6; k++) {
            if ((BlackPieces[k] & FinalBitBoard) != 0) {
                this->BlackPieces[k] &= ~FinalBitBoard;

                this->BoardVariables.N50MovesRule = 0;
                break;
            }
        }
    } else {
        for (int k = 0; k<6; k++) {
            if ((WhitePieces[k] & FinalBitBoard) != 0) {
                this->WhitePieces[k] &= ~FinalBitBoard;

                this->BoardVariables.N50MovesRule = 0;
                break;
            }
        }
    }

    // Reglas para el enroque
    switch (StartSq) {
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

    // EN PASSANT 
    if (MoveFlag == FLAG_EN_PASSANT) {
        if (!BoardVariables.Turn) this->WhitePieces[P] &= ~(BoardVariables.EnPassantMask << 8);
        else                      this->BlackPieces[P] &= ~(BoardVariables.EnPassantMask >> 8);
    }

    // DOUBLE PUSH
    if (MoveFlag == FLAG_PAWN_DOUBLE_PUSH) {
        this->BoardVariables.EnPassantMask = BoardVariables.Turn ? StartBitBoard << 8 : StartBitBoard >> 8;
    } else {
        this->BoardVariables.EnPassantMask = 0;
    }
    
    // chequear castle
    if        (MoveFlag == FLAG_O2 &&  BoardVariables.Turn) { //enroque wk
        this->WhitePieces[R] &= ~(1ULL << 7);
        this->WhitePieces[R] |=  (1ULL << 5);

        this->BoardVariables.CastleWK = false;
        this->BoardVariables.CastleWQ = false;

    } else if (MoveFlag == FLAG_O3 &&  BoardVariables.Turn) { //enroque wqhttps://www.youtube.com/watch?v=zrh938pD38U
        this->WhitePieces[R] &= ~(1ULL << 0);
        this->WhitePieces[R] |=  (1ULL << 3);

        this->BoardVariables.CastleWK = false;
        this->BoardVariables.CastleWQ = false;

    } else if (MoveFlag == FLAG_O2 && !BoardVariables.Turn) { //enroque bk
        this->BlackPieces[R] &= ~(1ULL << 63);
        this->BlackPieces[R] |=  (1ULL << 61);

        this->BoardVariables.CastleBK = false;
        this->BoardVariables.CastleBQ = false;

    } else if (MoveFlag == FLAG_O3 && !BoardVariables.Turn) { //enroque bq
        this->BlackPieces[R] &= ~(1ULL << 56);
        this->BlackPieces[R] |=  (1ULL << 59);

        this->BoardVariables.CastleBK = false;
        this->BoardVariables.CastleBQ = false;
    }


    if (MoveFlag == FLAG_PROM_N || MoveFlag == FLAG_PROM_B || MoveFlag == FLAG_PROM_R || MoveFlag == FLAG_PROM_Q) {

            if (BoardVariables.Turn) this->WhitePieces[MoveFlag - FLAG_PROM_N + N] |= FinalBitBoard;
            else                     this->BlackPieces[MoveFlag - FLAG_PROM_N + N] |= FinalBitBoard;
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

void Engine::UnMove() {
    this->GameMoves.pop_back();

    BoardState BoardData = BoardHistory.back();

    this->WhitePieces = BoardData.WhitePieces;
    this->BlackPieces = BoardData.BlackPieces;

    this->WhitePiecesOccupied = BoardData.WhitePiecesOccupied;
    this->BlackPiecesOccupied = BoardData.BlackPiecesOccupied;
    this->Occupied = BoardData.Occupied;
    this->Empty = BoardData.Empty;

    this->BoardVariables = BoardData.BoardVariables;

    this->PossibleMoves = BoardData.PossibleMoves;

    this->GameState = BoardData.GameState;
}

void Engine::GenerateMoveStr() {

    this->GenerateCheckingPieces();
    this->GeneratePinnedPieces();

    if (CheckingPieces.size() < 2) {
        this->MovesGeneratorPawn();
        this->MovesGeneratorKnight();
        this->MovesGeneratorBishop();
        this->MovesGeneratorRook();
        this->MovesGeneratorQueen();
    }

    this->MovesGeneratorKing();

    // mate
    if (PossibleMoves.size() == 0 && CheckingPieces.size() != 0) {
        this->GameState = 1;
    }
    // ahogado
    if (PossibleMoves.size() == 0 && CheckingPieces.size() == 0) {
        this->GameState = 2;
    }

    if (GameState != 0) this->PossibleMoves.clear();

    this->CheckingPieces.clear();
    this->PinnedPieces.fill({ 0 });
    this->PinnedPiecesBitBoard = 0;
    
    //std::cout << PossibleMoves << '\n';
}

void Engine::FilterMoveString(std::vector<MoveData>& PossibleMovesFiltered, unsigned short filterSq) {
    U16 StartSquareBits = filterSq << 6;

    for (int i = 0; i < PossibleMoves.size(); i ++) {
        if (StartSquareBits == (PossibleMoves[i].Data & StartMask)) {
            PossibleMovesFiltered.push_back(PossibleMoves[i]);
        }
    }
}

Board Engine::GetBoardVariables() {
    return BoardVariables;
}

std::array<U64, 6> Engine::GetBitboards(bool color) {
    return (color ? WhitePieces : BlackPieces);
}

int Engine::GetGameResult() {
    return GameState;
}