#include "ChessEngine.h"

void Engine::MovesGeneratorBishop() {
	U64 BishopsBitboard = BoardVariables.Turn ? WhitePieces[B] : BlackPieces[B];
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

		U64 BishopMovements = Diagonal | AntiDiagonal;
		BishopMovements &= ~(BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);
		while (BishopMovements != 0) {
			int sqIndx = IterLSB(BishopMovements);

			std::string MoveStr = "----";
			MoveStr[0] = file + '0';
			MoveStr[1] = row  + '0';
			MoveStr[2] = sqIndx % 8 + '0';
			MoveStr[3] = sqIndx / 8  + '0';
			this->PossibleMoves += MoveStr + " ";
		}
	}
}