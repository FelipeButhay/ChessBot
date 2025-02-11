#include "ChessEngine.h"

void Engine::GenerateCheckingPieces() {
	CheckingPieces.clear();

	U64 KingBitboard = BoardVariables.Turn ? WhitePieces[K] : BlackPieces[K];
	int KingPos = _tzcnt_u64(KingBitboard);

	int file = KingPos % 8;
	int row = KingPos / 8;

	U64 s = 1ULL << KingPos;
	


	// >> PAWNS <<
	U64 KingIfPawnCaptures = 0;

	// left capture
	KingIfPawnCaptures |= BoardVariables.Turn ? (KingBitboard << 7) & BlackPieces[P] & ~FilesMasks[7] :
												(KingBitboard >> 7) & WhitePieces[P] & ~FilesMasks[0];
	// right capture
	KingIfPawnCaptures |= BoardVariables.Turn ? (KingBitboard << 9) & BlackPieces[P] & ~FilesMasks[0] :
												(KingBitboard >> 9) & WhitePieces[P] & ~FilesMasks[7];
	


	// >> KNIGHTS <<
	U64 KingIfKnightCaptures = 0;
	if (KingPos >= 18) KingIfKnightCaptures = knightMask << (KingPos - 18);
	else               KingIfKnightCaptures = knightMask >> (18 - KingPos);

	if (KingPos % 8 < 4) KingIfKnightCaptures &= ~(FilesMasks[6] | FilesMasks[7]);
	else                 KingIfKnightCaptures &= ~(FilesMasks[0] | FilesMasks[1]);

	KingIfKnightCaptures &= !BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied;



	// >> BISHOP <<
	int d = row - file + 7;
	int ad = row + file;

	U64 d0 = (Occupied & DiagonalMasks[d]) - 2*s;
	U64 d1 = reverse(reverse(Occupied & DiagonalMasks[d]) - 2*reverse(s));
	U64 Diagonal = (d0 ^ d1) & DiagonalMasks[d];

	U64 a0 = (Occupied & AntiDiagonalMasks[ad]) - 2*s;
	U64 a1 = reverse(reverse(Occupied & AntiDiagonalMasks[ad]) - 2*reverse(s));
	U64 AntiDiagonal = (a0 ^ a1) & AntiDiagonalMasks[ad];

	U64 KingIfBishopMoves = Diagonal | AntiDiagonal;
	U64 KingIfBishopCaptures = KingIfBishopMoves & (!BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);



	// >> ROOKS << 
	U64 h0 = (Occupied & RanksMasks[row]) - 2*s;
	U64 h1 = reverse(reverse(Occupied & RanksMasks[row]) - 2*reverse(s));
	U64 Horizontal = (h0 ^ h1) & RanksMasks[row];

	U64 v0 = (Occupied & FilesMasks[file]) - 2*s;
	U64 v1 = reverse(reverse(Occupied & FilesMasks[file]) - 2*reverse(s));
	U64 Vertical = (v0 ^ v1) & FilesMasks[file];

	U64 KingIfRookMoves = Horizontal | Vertical;
	U64 KingIfRookCaptures = KingIfRookMoves & (!BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);



	// pawn appends
	while (KingIfPawnCaptures != 0) {
		short CheckingPiecePos = IterLSB(KingIfPawnCaptures);
		CheckingPiece PieceData = { 'P', CheckingPiecePos, 1ULL << CheckingPiecePos };
		CheckingPieces.push_back(PieceData);
	}

	// knights append
	while (KingIfKnightCaptures != 0) {
		short CheckingPiecePos = IterLSB(KingIfKnightCaptures);
		CheckingPiece PieceData = { 'N', CheckingPiecePos, 1ULL << CheckingPiecePos };
		CheckingPieces.push_back(PieceData);
	}

	// bishops append
	while (KingIfBishopCaptures != 0) {
		short CheckingPiecePos = IterLSB(KingIfBishopCaptures);

		// LINE OF SIGHT
		int fileC = CheckingPiecePos % 8;
		int rowC = CheckingPiecePos / 8;

		int dc = row - file + 7;
		int adc = row + file;

		U64 sc = 1ULL << CheckingPiecePos;

		U64 d0c = (Occupied & DiagonalMasks[dc]) - 2*sc;
		U64 d1c = reverse(reverse(Occupied & DiagonalMasks[dc]) - 2*reverse(sc));
		U64 DiagonalC = (d0c ^ d1c) & DiagonalMasks[dc];

		U64 a0c = (Occupied & AntiDiagonalMasks[adc]) - 2*sc;
		U64 a1c = reverse(reverse(Occupied & AntiDiagonalMasks[adc]) - 2*reverse(sc));
		U64 AntiDiagonalC = (a0c ^ a1c) & AntiDiagonalMasks[adc];

		U64 BishopMovements = DiagonalC | AntiDiagonalC;
		U64 LineOfSight = BishopMovements & KingIfBishopMoves;
		LineOfSight |= 1ULL << CheckingPiecePos;

		// APPEND
		CheckingPiece PieceData = { 'B', CheckingPiecePos, LineOfSight };
		CheckingPieces.push_back(PieceData);
	}

	// rooks append
	while (KingIfRookCaptures != 0) {
		short CheckingPiecePos = IterLSB(KingIfRookCaptures);

		// LINE OF SIGHT
		int fileC = CheckingPiecePos % 8;
		int rowC = CheckingPiecePos / 8;

		U64 sc = 1ULL << CheckingPiecePos;

		U64 h0c = (Occupied & RanksMasks[rowC]) - 2*sc;
		U64 h1c = reverse(reverse(Occupied & RanksMasks[rowC]) - 2*reverse(sc));
		U64 HorizontalC = (h0c ^ h1c) & RanksMasks[rowC];

		U64 v0c = (Occupied & FilesMasks[fileC]) - 2*sc;
		U64 v1c = reverse(reverse(Occupied & FilesMasks[fileC]) - 2*reverse(sc));
		U64 VerticalC = (v0c ^ v1c) & FilesMasks[fileC];

		U64 RookMovements = HorizontalC | VerticalC;
		U64 LineOfSight = RookMovements & KingIfRookMoves;
		LineOfSight |= 1ULL << CheckingPiecePos;

		// APPEND
		CheckingPiece PieceData = { 'R', CheckingPiecePos, LineOfSight };
		CheckingPieces.push_back(PieceData);
	}
}