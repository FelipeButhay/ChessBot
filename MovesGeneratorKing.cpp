#include "ChessEngine.h"

void Engine::MovesGeneratorKing() {
	U64 KingBitboard = BoardVariables.Turn ? WhitePieces[K] : BlackPieces[K];
	while (KingBitboard != 0) {
		U64 KingMovements = 0;
		int KingPosIndx = IterLSB(KingBitboard);

		if (KingPosIndx >= 9) KingMovements = kingMask << (KingPosIndx - 9);
		else                  KingMovements = kingMask >> (9 - KingPosIndx);

		if (KingPosIndx % 8 < 4) KingMovements &= ~FilesMasks[7];
		else                     KingMovements &= ~FilesMasks[0];

		KingMovements &= BoardVariables.Turn ? ~WhitePiecesOccupied : ~BlackPiecesOccupied;
		U64 Unsafe = MovesGeneratorUnsafeSq();

		KingMovements &= ~Unsafe;
		while (KingMovements != 0) {
			int sqIndx = IterLSB(KingMovements);

			std::string MoveStr = "----";
			MoveStr[0] = KingPosIndx % 8 + '0';
			MoveStr[1] = KingPosIndx / 8 + '0';
			MoveStr[2] = sqIndx % 8 + '0';
			MoveStr[3] = sqIndx / 8 + '0';
			this->PossibleMoves += MoveStr + " ";
		}

		if ((Unsafe & KingBitboard) == 0) {
			if (BoardVariables.Turn) {
				// wk
				if (BoardVariables.CastleWK && (WhitePieces[R] & (1ULL << 7)) && 
					(Occupied & CastlingEmptyWKMask) == 0 && (Unsafe & CastlingSafe_WKMask) == 0) {

					this->PossibleMoves += "40O2";

				} else
				// wq
				if (BoardVariables.CastleWQ && (WhitePieces[R] & (1ULL << 0)) &&
					(Occupied & CastlingEmptyWQMask) == 0 && (Unsafe & CastlingSafe_WQMask) == 0) {

					this->PossibleMoves += "40O3";

				}
			} else {
				// bk
				if (BoardVariables.CastleBK && (BlackPieces[R] & (1ULL << 63)) &&
					(Occupied & CastlingEmptyBKMask) == 0 && (Unsafe & CastlingSafe_BKMask) == 0) {

					this->PossibleMoves += "47O2";

				} else
				// bq
				if (BoardVariables.CastleBQ && (BlackPieces[R] & (1ULL << 56)) &&
					(Occupied & CastlingEmptyBQMask) == 0 && (Unsafe & CastlingSafe_BQMask) == 0) {

					this->PossibleMoves += "47O3";

				}
			}
		}
	}
}

U64 Engine::MovesGeneratorUnsafeSq() {
	// delete king pos
	U64 KingSave;
	if (BoardVariables.Turn) {
		KingSave = WhitePieces[K];
		WhitePieces[K] = 0;
	} else {
		KingSave = BlackPieces[K];
		BlackPieces[K] = 0;
	}

	this->WhitePiecesOccupied = WhitePieces[0] | WhitePieces[1] | WhitePieces[2] | WhitePieces[3] | WhitePieces[4] | WhitePieces[5];
	this->BlackPiecesOccupied = BlackPieces[0] | BlackPieces[1] | BlackPieces[2] | BlackPieces[3] | BlackPieces[4] | BlackPieces[5];
	this->Occupied = WhitePiecesOccupied | BlackPiecesOccupied;
	this->Empty = ~Occupied;

	// define unsafe
	U64 Unsafe = 0;

	// PAWN UNSAFE
	// left capture
	Unsafe |= !BoardVariables.Turn ? (WhitePieces[P] << 7) & ~FilesMasks[7] : (BlackPieces[P] >> 7) & ~FilesMasks[0];
	// right capture
	Unsafe |= !BoardVariables.Turn ? (WhitePieces[P] << 9) & ~FilesMasks[0] : (BlackPieces[P] >> 9) & ~FilesMasks[7];

	// KNIGHT UNSAFE
	U64 KnightsBitboard = !BoardVariables.Turn ? WhitePieces[N] : BlackPieces[N];
	while (KnightsBitboard != 0) {
		U64 KnightMovements = 0;
		int KnightPosIndx = IterLSB(KnightsBitboard);

		if (KnightPosIndx >= 18) KnightMovements = knightMask << (KnightPosIndx - 18);
		else                     KnightMovements = knightMask >> (18 - KnightPosIndx);

		if (KnightPosIndx % 8 < 4) KnightMovements &= ~(FilesMasks[6] | FilesMasks[7]);
		else                       KnightMovements &= ~(FilesMasks[0] | FilesMasks[1]);

		Unsafe |= KnightMovements;
	}

	// BISHOP UNSAFE 
	U64 BishopsBitboard = !BoardVariables.Turn ? WhitePieces[B] : BlackPieces[B];
	while (BishopsBitboard != 0) {
		int BishopPosIndx = IterLSB(BishopsBitboard);
		int file = BishopPosIndx % 8;
		int row = BishopPosIndx / 8;

		int d = row - file + 7;
		int ad = row + file;

		U64 s = 1ULL << BishopPosIndx;

		U64 d0 = (Occupied & DiagonalMasks[d]) - 2*s;
		U64 d1 = reverse(reverse(Occupied & DiagonalMasks[d]) - 2*reverse(s));
		U64 Diagonal = (d0 ^ d1) & DiagonalMasks[d];

		U64 a0 = (Occupied & AntiDiagonalMasks[ad]) - 2*s;
		U64 a1 = reverse(reverse(Occupied & AntiDiagonalMasks[ad]) - 2*reverse(s));
		U64 AntiDiagonal = (a0 ^ a1) & AntiDiagonalMasks[ad];

		Unsafe |= (Diagonal | AntiDiagonal);
	}

	// ROOK UNSAFE
	U64 RooksBitboard = !BoardVariables.Turn ? WhitePieces[R] : BlackPieces[R];
	while (RooksBitboard != 0) {
		int RookPosIndx = IterLSB(RooksBitboard);
		int file = RookPosIndx % 8;
		int row = RookPosIndx / 8;

		U64 s = 1ULL << RookPosIndx;

		U64 h0 = (Occupied & RanksMasks[row]) - 2*s;
		U64 h1 = reverse(reverse(Occupied & RanksMasks[row]) - 2*reverse(s));
		U64 Horizontal = (h0 ^ h1) & RanksMasks[row];

		U64 v0 = (Occupied & FilesMasks[file]) - 2*s;
		U64 v1 = reverse(reverse(Occupied & FilesMasks[file]) - 2*reverse(s));
		U64 Vertical = (v0 ^ v1) & FilesMasks[file];

		Unsafe |= (Horizontal | Vertical);
	}

	// QUEEN UNSAFE
	U64 QueensBitboard = !BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q];
	while (QueensBitboard != 0) {
		int BishopPosIndx = IterLSB(QueensBitboard);
		int file = BishopPosIndx % 8;
		int row = BishopPosIndx / 8;

		int d = row - file + 7;
		int ad = row + file;

		U64 s = 1ULL << BishopPosIndx;

		U64 h0 = (Occupied & RanksMasks[row]) - 2*s;
		U64 h1 = reverse(reverse(Occupied & RanksMasks[row]) - 2*reverse(s));
		U64 Horizontal = (h0 ^ h1) & RanksMasks[row];

		U64 v0 = (Occupied & FilesMasks[file]) - 2*s;
		U64 v1 = reverse(reverse(Occupied & FilesMasks[file]) - 2*reverse(s));
		U64 Vertical = (v0 ^ v1) & FilesMasks[file];

		U64 d0 = (Occupied & DiagonalMasks[d]) - 2*s;
		U64 d1 = reverse(reverse(Occupied & DiagonalMasks[d]) - 2*reverse(s));
		U64 Diagonal = (d0 ^ d1) & DiagonalMasks[d];

		U64 a0 = (Occupied & AntiDiagonalMasks[ad]) - 2*s;
		U64 a1 = reverse(reverse(Occupied & AntiDiagonalMasks[ad]) - 2*reverse(s));
		U64 AntiDiagonal = (a0 ^ a1) & AntiDiagonalMasks[ad];

		Unsafe |= (Horizontal | Vertical | Diagonal | AntiDiagonal);
	}

	// KING UNSAFE
	U64 KingBitboard = !BoardVariables.Turn ? WhitePieces[K] : BlackPieces[K];
	while (KingBitboard != 0) {
		U64 KingMovements = 0;
		int KingPosIndx = IterLSB(KingBitboard);

		if (KingPosIndx >= 9) KingMovements = kingMask << (KingPosIndx - 9);
		else                  KingMovements = kingMask >> (9 - KingPosIndx);

		if (KingPosIndx % 8 < 4) KingMovements &= ~FilesMasks[7];
		else                     KingMovements &= ~FilesMasks[0];

		Unsafe |= KingMovements;
	}

	// add king pos again
	if (BoardVariables.Turn) {
		WhitePieces[K] = KingSave;
	} else {
		BlackPieces[K] = KingSave;
	}

	this->WhitePiecesOccupied = WhitePieces[0] | WhitePieces[1] | WhitePieces[2] | WhitePieces[3] | WhitePieces[4] | WhitePieces[5];
	this->BlackPiecesOccupied = BlackPieces[0] | BlackPieces[1] | BlackPieces[2] | BlackPieces[3] | BlackPieces[4] | BlackPieces[5];
	this->Occupied = WhitePiecesOccupied | BlackPiecesOccupied;
	this->Empty = ~Occupied;

	return Unsafe;
}