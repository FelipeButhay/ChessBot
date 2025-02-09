#include "ChessEngine.h"

void Engine::MovesGeneratorQueen() {
	U64 QueensBitboard = BoardVariables.Turn ? WhitePieces[Q] : BlackPieces[Q];
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

		U64 QueenMovements = Horizontal | Vertical | Diagonal | AntiDiagonal;
		QueenMovements &= ~(BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);
		if (CheckingPieces.size() == 1) QueenMovements &= CheckingPieces[0].BlockingBitboard;

		while (QueenMovements != 0) {
			int sqIndx = IterLSB(QueenMovements);

			std::string MoveStr = "----";
			MoveStr[0] = file + '0';
			MoveStr[1] = row  + '0';
			MoveStr[2] = sqIndx % 8 + '0';
			MoveStr[3] = sqIndx / 8  + '0';
			this->PossibleMoves += MoveStr + " ";
		}
	}
}