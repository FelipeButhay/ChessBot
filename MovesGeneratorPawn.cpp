#include "ChessEngine.h"

void Engine::MovesGeneratorPawn() {
	if (BoardVariables.Turn) {
		this->MovesGeneratorWhitePawn();
		this->MovesGeneratorWhitePinnedPawn();
	} else {
		this->MovesGeneratorBlackPawn();
		this->MovesGeneratorBlackPinnedPawn();
	}
}

void Engine::MovesGeneratorWhitePawn() {
	// single push
	U64 PawnsBitboard = WhitePieces[P] & ~PinnedPiecesBitBoard;

	U64 SinglePushMoves = (PawnsBitboard << 8)  & Empty & ~RanksMasks[7];
	
	if (CheckingPieces.size() == 1) SinglePushMoves &= CheckingPieces[0].BlockingBitboard;

	while (SinglePushMoves != 0) {
		int sqIndx = IterLSB(SinglePushMoves);

		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx - 8) << 6;
		this->PossibleMoves.push_back(Move);
	}

	// double push
	U64 DoublePushMoves = (PawnsBitboard << 16) & Empty & (Empty << 8) & RanksMasks[3];
	if (CheckingPieces.size() == 1) DoublePushMoves &= CheckingPieces[0].BlockingBitboard;

	while (DoublePushMoves != 0) {
		int sqIndx = IterLSB(DoublePushMoves);

		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx - 16) << 6;
		Move.Data |= FLAG_PAWN_DOUBLE_PUSH << 12;
		this->PossibleMoves.push_back(Move);
	}

	// captures
	U64 RightCaptures = (PawnsBitboard << 9) & (BlackPiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[7] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) RightCaptures &= CheckingPieces[0].BlockingBitboard;

	while (RightCaptures != 0) {
		int sqIndx = IterLSB(RightCaptures);

		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx - 9) << 6;
		this->PossibleMoves.push_back(Move);
	}

	U64 LeftCaptures  = (PawnsBitboard << 7) & (BlackPiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[7] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) LeftCaptures &= CheckingPieces[0].BlockingBitboard;

	while (LeftCaptures != 0) {
		int sqIndx = IterLSB(LeftCaptures);

		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx - 7) << 6;
		this->PossibleMoves.push_back(Move);
	}

	// PROMOTIONS
	// single push
	U64 SinglePushMovesProm = (PawnsBitboard << 8)  & Empty & RanksMasks[7];
	if (CheckingPieces.size() == 1) SinglePushMovesProm &= CheckingPieces[0].BlockingBitboard;

	while (SinglePushMovesProm != 0) {
		int sqIndx = IterLSB(SinglePushMovesProm);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx - 8) << 6;
		
		for (int i = 0; i < 4; i++) {
			Move.Data |= (FLAG_PROM_N + i) << 12;
			this->PossibleMoves.push_back(Move);
			Move.Data &= ~FlagMask;
		}
	}

	// captures
	U64 RightCapturesProm = (PawnsBitboard << 9) & BlackPiecesOccupied & RanksMasks[7] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) RightCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (RightCapturesProm != 0) {
		int sqIndx = IterLSB(RightCapturesProm);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx - 9) << 6;
		
		for (int i = 0; i < 4; i++) {
			Move.Data |= (FLAG_PROM_N + i) << 12;
			this->PossibleMoves.push_back(Move);
			Move.Data &= ~FlagMask;
		}
	}

	U64 LeftCapturesProm  = (PawnsBitboard << 7) & BlackPiecesOccupied & RanksMasks[7] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) LeftCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (LeftCapturesProm != 0) {
		int sqIndx = IterLSB(LeftCapturesProm);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx - 7) << 6;

		for (int i = 0; i < 4; i++) {
			Move.Data |= (FLAG_PROM_N + i) << 12;
			this->PossibleMoves.push_back(Move);
			Move.Data &= ~FlagMask;
		}
	}
}

void Engine::MovesGeneratorWhitePinnedPawn() {
	// single push
	U64 PinnedPawnsBitboard = WhitePieces[P] & PinnedPiecesBitBoard;

	while (PinnedPawnsBitboard != 0) {
		int PawnPosIndx = IterLSB(PinnedPawnsBitboard);
		int file = PawnPosIndx % 8;
		int row = PawnPosIndx / 8;

		U64 PawnBitboard = 1ULL << PawnPosIndx;
		U64 PawnMovements = 0;

		PawnMovements |= (PawnBitboard <<  8) & Empty;
		PawnMovements |= (PawnBitboard << 16) & Empty & (Empty << 8) & RanksMasks[3];
		PawnMovements |= (PawnBitboard << 9)  & (BlackPiecesOccupied | BoardVariables.EnPassantMask) & ~FilesMasks[0];
		PawnMovements |= (PawnBitboard << 7)  & (BlackPiecesOccupied | BoardVariables.EnPassantMask) & ~FilesMasks[7];

		if (CheckingPieces.size() == 1) PawnMovements &= CheckingPieces[0].BlockingBitboard;

		if ((PinnedPiecesBitBoard & (1ULL << PawnPosIndx)) != 0) {
			PawnMovements &= PinnedPieces[PawnPosIndx].MovementRay;
		}

		while (PawnMovements != 0 && row == 6) {
			int sqIndx = IterLSB(PawnMovements);

			MoveData Move = { 0 };
			Move.Data |= sqIndx;
			Move.Data |= PawnPosIndx << 6;

			for (int i = 0; i < 4; i++) {
				Move.Data |= (FLAG_PROM_N + i) << 12;
				this->PossibleMoves.push_back(Move);
				Move.Data &= ~FlagMask;
			}
		}

		while (PawnMovements != 0) {
			int sqIndx = IterLSB(PawnMovements);

			MoveData Move = { 0 };
			Move.Data |= sqIndx;
			Move.Data |= PawnPosIndx << 6;
			this->PossibleMoves.push_back(Move);
		}
	}
}


void Engine::MovesGeneratorBlackPawn() {
	U64 PawnsBitboard = BlackPieces[P] & ~PinnedPiecesBitBoard;

	// single push
	U64 SinglePushMoves = (PawnsBitboard >> 8)  & Empty & ~RanksMasks[0];
	if (CheckingPieces.size() == 1) SinglePushMoves &= CheckingPieces[0].BlockingBitboard;

	while (SinglePushMoves != 0) {
		int sqIndx = IterLSB(SinglePushMoves);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx + 8) << 6;
		this->PossibleMoves.push_back(Move);
	}

	// double push
	U64 DoublePushMoves = (PawnsBitboard >> 16) & Empty & (Empty >> 8) & RanksMasks[4];
	if (CheckingPieces.size() == 1) DoublePushMoves &= CheckingPieces[0].BlockingBitboard;

	while (DoublePushMoves != 0) {
		int sqIndx = IterLSB(DoublePushMoves);

		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx + 16) << 6;
		Move.Data |= FLAG_PAWN_DOUBLE_PUSH << 12;
		this->PossibleMoves.push_back(Move);
	}

	// captures
	U64 RightCaptures = (PawnsBitboard >> 9) & (WhitePiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[0] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) RightCaptures &= CheckingPieces[0].BlockingBitboard;

	while (RightCaptures != 0) {
		int sqIndx = IterLSB(RightCaptures);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx + 9) << 6;
		this->PossibleMoves.push_back(Move);
	}

	U64 LeftCaptures  = (PawnsBitboard >> 7) & (WhitePiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[0] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) LeftCaptures &= CheckingPieces[0].BlockingBitboard;

	while (LeftCaptures != 0) {
		int sqIndx = IterLSB(LeftCaptures);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx + 7) << 6;
		this->PossibleMoves.push_back(Move);
	}

	// PROMOTIONS
	// single push
	U64 SinglePushMovesProm = (PawnsBitboard >> 8)  & Empty & RanksMasks[0];
	if (CheckingPieces.size() == 1) SinglePushMovesProm &= CheckingPieces[0].BlockingBitboard;

	while (SinglePushMovesProm != 0) {
		int sqIndx = IterLSB(SinglePushMovesProm);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx + 8) << 6;
		
		for (int i = 0; i < 4; i++) {
			Move.Data |= (FLAG_PROM_N + i) << 12;
			this->PossibleMoves.push_back(Move);
			Move.Data &= ~FlagMask;
		}
	}

	// captures
	U64 RightCapturesProm = (PawnsBitboard >> 9) & WhitePiecesOccupied & RanksMasks[0] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) RightCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (RightCapturesProm != 0) {
		int sqIndx = IterLSB(RightCapturesProm);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx + 9) << 6;

		for (int i = 0; i < 4; i++) {
			Move.Data |= (FLAG_PROM_N + i) << 12;
			this->PossibleMoves.push_back(Move);
			Move.Data &= ~FlagMask;
		}
	}

	U64 LeftCapturesProm  = (PawnsBitboard >> 7) & WhitePiecesOccupied & RanksMasks[0] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) LeftCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (LeftCapturesProm != 0) {
		int sqIndx = IterLSB(LeftCapturesProm);
		
		MoveData Move = { 0 };
		Move.Data |= sqIndx;
		Move.Data |= (sqIndx + 7) << 6;

		for (int i = 0; i < 4; i++) {
			Move.Data |= (FLAG_PROM_N + i) << 12;
			this->PossibleMoves.push_back(Move);
			Move.Data &= ~FlagMask;
		}
	}
}

void Engine::MovesGeneratorBlackPinnedPawn() {
	// single push
	U64 PinnedPawnsBitboard = BlackPieces[P] & PinnedPiecesBitBoard;

	while (PinnedPawnsBitboard != 0) {
		int PawnPosIndx = IterLSB(PinnedPawnsBitboard);
		int file = PawnPosIndx % 8;
		int row = PawnPosIndx / 8;

		U64 PawnBitboard = 1ULL << PawnPosIndx;
		U64 PawnMovements = 0;

		PawnMovements |= (PawnBitboard  >> 8) & Empty;
		PawnMovements |= (PawnBitboard >> 16) & Empty & (Empty >> 8) & RanksMasks[4];
		PawnMovements |= (PawnBitboard  >> 9) & (WhitePiecesOccupied | BoardVariables.EnPassantMask) & ~FilesMasks[7];
		PawnMovements |= (PawnBitboard  >> 7) & (WhitePiecesOccupied | BoardVariables.EnPassantMask) & ~FilesMasks[0];

		if (CheckingPieces.size() == 1) PawnMovements &= CheckingPieces[0].BlockingBitboard;

		if ((PinnedPiecesBitBoard & (1ULL << PawnPosIndx)) != 0) {
			PawnMovements &= PinnedPieces[PawnPosIndx].MovementRay;
		}

		while (PawnMovements != 0 && row == 1) {
			int sqIndx = IterLSB(PawnMovements);

			MoveData Move = { 0 };
			Move.Data |= sqIndx;
			Move.Data |= PawnPosIndx << 6;

			for (int i = 0; i < 4; i++) {
				Move.Data |= (FLAG_PROM_N + i) << 12;
				this->PossibleMoves.push_back(Move);
				Move.Data &= ~FlagMask;
			}
		}

		while (PawnMovements != 0) {
			int sqIndx = IterLSB(PawnMovements);

			MoveData Move = { 0 };
			Move.Data |= sqIndx;
			Move.Data |= PawnPosIndx << 6;
			this->PossibleMoves.push_back(Move);
		}
	}
}