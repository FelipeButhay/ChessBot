#include "ChessEngine.h"

void Engine::MovesGeneratorRook() {
	U64 RooksBitboard = BoardVariables.Turn ? WhitePieces[R] : BlackPieces[R];
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

		U64 RookMovements = Horizontal | Vertical;
		RookMovements &= ~(BoardVariables.Turn ? WhitePiecesOccupied : BlackPiecesOccupied);

		while (RookMovements != 0) {
			int sqIndx = IterLSB(RookMovements);

			std::string MoveStr = "----";
			MoveStr[0] = file + '0';
			MoveStr[1] = row  + '0';
			MoveStr[2] = sqIndx % 8 + '0';
			MoveStr[3] = sqIndx / 8  + '0';
			this->PossibleMoves += MoveStr + " ";
		}
	}
}