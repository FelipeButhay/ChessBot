#include "ChessEngine.h"

void Engine::MovesGeneratorKnight() {
	U64 KnightsBitboard = BoardVariables.Turn ? WhitePieces[N] : BlackPieces[N];
	while (KnightsBitboard != 0) {
		U64 KnightMovements = 0;
		int KnightPosIndx = IterLSB(KnightsBitboard);

		if (KnightPosIndx >= 18) KnightMovements = knightMask << (KnightPosIndx - 18);
		else                     KnightMovements = knightMask >> (18 - KnightPosIndx);

		if (KnightPosIndx % 8 < 4) KnightMovements &= ~(FilesMasks[6] | FilesMasks[7]);
		else                       KnightMovements &= ~(FilesMasks[0] | FilesMasks[1]);

		KnightMovements &= BoardVariables.Turn ? ~WhitePiecesOccupied : ~BlackPiecesOccupied;
		while (KnightMovements != 0) {
			int sqIndx = IterLSB(KnightMovements);

			std::string MoveStr = "----";
			MoveStr[0] = KnightPosIndx % 8 + '0';
			MoveStr[1] = KnightPosIndx / 8 + '0';
			MoveStr[2] = sqIndx % 8 + '0';
			MoveStr[3] = sqIndx / 8  + '0';
			this->PossibleMoves += MoveStr + " ";
		}
	}
}