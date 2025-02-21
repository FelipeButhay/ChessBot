#include "ChessEngine.h"

void Engine::GeneratePinnedPieces() {
	U64 KingBitboard = BoardVariables.Turn ? WhitePieces[K] : BlackPieces[K];
	int KingPos = _tzcnt_u64(KingBitboard);

	int file = KingPos % 8;
	int row = KingPos / 8;

	int d = row - file + 7;
	int ad = row + file;

	U64 sk = 1ULL << KingPos;

	// >> KING AS BISHOP <<

	// DIAGONAL
	U64 d0k = (Occupied & DiagonalMasks[d]) - 2*sk;
	U64 d1k = reverse(reverse(Occupied & DiagonalMasks[d]) - 2*reverse(sk));
	U64 DiagonalK = (d0k ^ d1k) & DiagonalMasks[d];
	U64 PiecesSeenD = DiagonalK & (BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

	while (PiecesSeenD != 0) {
		int PiecePos = IterLSB(PiecesSeenD);
		U64 s = 1ULL << PiecePos;
	
		U64 d0p = (Occupied & DiagonalMasks[d]) - 2*s;
		U64 d1p = reverse(reverse(Occupied & DiagonalMasks[d]) - 2*reverse(s));
		U64 PieceDiagonal = (d0p ^ d1p) & DiagonalMasks[d];
		U64 SlidingEnemySeenD = PieceDiagonal & (!BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

		if (!BoardVariables.Turn ?
			(SlidingEnemySeenD & WhitePieces[B]) || (SlidingEnemySeenD & WhitePieces[Q]) : 
			(SlidingEnemySeenD & BlackPieces[B]) || (SlidingEnemySeenD & BlackPieces[Q]) ) {

			U64 Ray = PieceDiagonal & ~(BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);
			Ray = Ray & ~Occupied & DiagonalMasks[d];

			if        (SlidingEnemySeenD & (!BoardVariables.Turn ? WhitePieces[B] : BlackPieces[B])) {
				Ray |= SlidingEnemySeenD & (!BoardVariables.Turn ? WhitePieces[B] : BlackPieces[B]);

			} else if (SlidingEnemySeenD & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q])) {
				Ray |= SlidingEnemySeenD & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q]);
			}

			PinnedPiece PieceData = { Ray };
			this->PinnedPieces[PiecePos] = PieceData;
			this->PinnedPiecesBitBoard |= s;
		}
	}


	// ANTI DIAGONAL
	U64 a0k = (Occupied & AntiDiagonalMasks[ad]) - 2*sk;
	U64 a1k = reverse(reverse(Occupied & AntiDiagonalMasks[ad]) - 2*reverse(sk));
	U64 AntiDiagonalK = (a0k ^ a1k) & AntiDiagonalMasks[ad];
	U64 PiecesSeenAD = AntiDiagonalK & (BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

	while (PiecesSeenAD != 0) {
		int PiecePos = IterLSB(PiecesSeenAD);
		U64 s = 1ULL << PiecePos;

		U64 a0p = (Occupied & AntiDiagonalMasks[ad]) - 2*s;
		U64 a1p = reverse(reverse(Occupied & AntiDiagonalMasks[ad]) - 2*reverse(s));
		U64 PieceAntiDiagonal = (a0p ^ a1p) & AntiDiagonalMasks[ad];
		U64 SlidingEnemySeenAD = PieceAntiDiagonal & (!BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

		if (!BoardVariables.Turn ?
			(SlidingEnemySeenAD & WhitePieces[B]) || (SlidingEnemySeenAD & WhitePieces[Q]) :
			(SlidingEnemySeenAD & BlackPieces[B]) || (SlidingEnemySeenAD & BlackPieces[Q])) {

			U64 Ray = PieceAntiDiagonal & ~(BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);
			Ray = Ray & ~Occupied & AntiDiagonalMasks[ad];

			if        (SlidingEnemySeenAD & (!BoardVariables.Turn ? WhitePieces[B] : BlackPieces[B])) {
				Ray |= SlidingEnemySeenAD & (!BoardVariables.Turn ? WhitePieces[B] : BlackPieces[B]);

			} else if (SlidingEnemySeenAD & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q])) {
				Ray |= SlidingEnemySeenAD & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q]);
			}

			PinnedPiece PieceData = { Ray };
			this->PinnedPieces[PiecePos] = PieceData;
			this->PinnedPiecesBitBoard |= s;
		}
	}


	// >> KING AS ROOK << 
	// HORIZONTAL
	U64 h0k = (Occupied & RanksMasks[row]) - 2*sk;
	U64 h1k = reverse(reverse(Occupied & RanksMasks[row]) - 2*reverse(sk));
	U64 HorizontalK = (h0k ^ h1k) & RanksMasks[row];
	U64 PiecesSeenH = HorizontalK & (BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

	while (PiecesSeenH != 0) {
		int PiecePos = IterLSB(PiecesSeenH);
		U64 s = 1ULL << PiecePos;

		U64 h0p = (Occupied & RanksMasks[row]) - 2*s;
		U64 h1p = reverse(reverse(Occupied & RanksMasks[row]) - 2*reverse(s));
		U64 PieceHorizontal = (h0p ^ h1p) & RanksMasks[row];
		U64 SlidingEnemySeenH = PieceHorizontal & (!BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

		if (!BoardVariables.Turn ?
			(SlidingEnemySeenH & WhitePieces[R]) || (SlidingEnemySeenH & WhitePieces[Q]) :
			(SlidingEnemySeenH & BlackPieces[R]) || (SlidingEnemySeenH & BlackPieces[Q])) {

			U64 Ray = PieceHorizontal & ~(BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);
			Ray = Ray & ~Occupied & RanksMasks[row] ;

			if        (SlidingEnemySeenH & (!BoardVariables.Turn ? WhitePieces[R] : BlackPieces[R])) {
				Ray |= SlidingEnemySeenH & (!BoardVariables.Turn ? WhitePieces[R] : BlackPieces[R]);

			} else if (SlidingEnemySeenH & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q])) {
				Ray |= SlidingEnemySeenH & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q]);
			}

			PinnedPiece PieceData = { Ray };
			this->PinnedPieces[PiecePos] = PieceData;
			this->PinnedPiecesBitBoard |= s;
		}
	}


	// VERTICAL
	U64 v0k = (Occupied & FilesMasks[file]) - 2*sk;
	U64 v1k = reverse(reverse(Occupied & FilesMasks[file]) - 2*reverse(sk));
	U64 VerticalK = (v0k ^ v1k) & FilesMasks[file];
	U64 PiecesSeenV = VerticalK & (BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

	while (PiecesSeenV != 0) {
		int PiecePos = IterLSB(PiecesSeenV);
		U64 s = 1ULL << PiecePos;

		U64 v0p = (Occupied & FilesMasks[file]) - 2*s;
		U64 v1p = reverse(reverse(Occupied & FilesMasks[file]) - 2*reverse(s));
		U64 PieceVertical = (v0p ^ v1p) & FilesMasks[file];
		U64 SlidingEnemySeenV = PieceVertical & (!BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

		if (!BoardVariables.Turn ?
			(SlidingEnemySeenV & WhitePieces[R]) || (SlidingEnemySeenV & WhitePieces[Q]) :
			(SlidingEnemySeenV & BlackPieces[R]) || (SlidingEnemySeenV & BlackPieces[Q])) {

			U64 Ray = PieceVertical & ~(BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);
			Ray = Ray & ~Occupied & RanksMasks[row];

			if        (SlidingEnemySeenV & (!BoardVariables.Turn ? WhitePieces[R] : BlackPieces[R])) {
				Ray |= SlidingEnemySeenV & (!BoardVariables.Turn ? WhitePieces[R] : BlackPieces[R]);

			} else if (SlidingEnemySeenV & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q])) {
				Ray |= SlidingEnemySeenV & (!BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q]);
			}

			PinnedPiece PieceData = { Ray };
			this->PinnedPieces[PiecePos] = PieceData;
			this->PinnedPiecesBitBoard |= s;
		}
	}
}