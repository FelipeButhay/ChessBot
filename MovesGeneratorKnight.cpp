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
		if (CheckingPieces.size() == 1) KnightMovements &= CheckingPieces[0].BlockingBitboard;

		if ((PinnedPiecesBitBoard & (1ULL << KnightPosIndx)) != 0) {
			KnightMovements &= PinnedPieces[KnightPosIndx].MovementRay;
		}

		while (KnightMovements != 0) {
			int sqIndx = IterLSB(KnightMovements);

			MoveData Move = { 0 };
			Move.Data |= sqIndx;
			Move.Data |= KnightPosIndx << 6;
			this->PossibleMoves.push_back(Move);
		}
	}
}