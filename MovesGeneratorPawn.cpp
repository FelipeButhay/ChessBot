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
		int row  = sqIndx / 8;
		int file = sqIndx % 8;
		
		std::string MoveStr = "----";
		MoveStr[0] = file + '0';
		MoveStr[1] = row  + '0' - 1;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}


	// double push
	U64 DoublePushMoves = (PawnsBitboard << 16) & Empty & (Empty << 8) & RanksMasks[3];
	if (CheckingPieces.size() == 1) DoublePushMoves &= CheckingPieces[0].BlockingBitboard;

	while (DoublePushMoves != 0) {
		int sqIndx = IterLSB(DoublePushMoves);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "----";
		MoveStr[0] = file + '0';
		MoveStr[1] = row  + '0' - 2;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}

	// captures
	U64 RightCaptures = (PawnsBitboard << 9) & (BlackPiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[7] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) RightCaptures &= CheckingPieces[0].BlockingBitboard;

	while (RightCaptures != 0) {
		int sqIndx = IterLSB(RightCaptures);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "----";
		MoveStr[0] = file + '0' - 1;
		MoveStr[1] = row  + '0' - 1;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}

	U64 LeftCaptures  = (PawnsBitboard << 7) & (BlackPiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[7] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) LeftCaptures &= CheckingPieces[0].BlockingBitboard;

	while (LeftCaptures != 0) {
		int sqIndx = IterLSB(LeftCaptures);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "----";
		MoveStr[0] = file + '0' + 1;
		MoveStr[1] = row  + '0' - 1;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}

	// PROMOTIONS
	// single push
	U64 SinglePushMovesProm = (PawnsBitboard << 8)  & Empty & RanksMasks[7];
	if (CheckingPieces.size() == 1) SinglePushMovesProm &= CheckingPieces[0].BlockingBitboard;

	while (SinglePushMovesProm != 0) {
		int sqIndx = IterLSB(SinglePushMovesProm);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "---";
		MoveStr[0] = file + '0';
		MoveStr[1] = row  + '0' - 1;
		MoveStr[2] = file + '0';

		this->PossibleMoves += MoveStr + "N" + " ";
		this->PossibleMoves += MoveStr + "B" + " ";
		this->PossibleMoves += MoveStr + "R" + " ";
		this->PossibleMoves += MoveStr + "Q" + " ";
	}

	// captures
	U64 RightCapturesProm = (PawnsBitboard << 9) & BlackPiecesOccupied & RanksMasks[7] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) RightCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (RightCapturesProm != 0) {
		int sqIndx = IterLSB(RightCapturesProm);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "---";
		MoveStr[0] = file + '0' - 1;
		MoveStr[1] = row  + '0' - 1;
		MoveStr[2] = file + '0';

		this->PossibleMoves += MoveStr + "N" + " ";
		this->PossibleMoves += MoveStr + "B" + " ";
		this->PossibleMoves += MoveStr + "R" + " ";
		this->PossibleMoves += MoveStr + "Q" + " ";
	}

	U64 LeftCapturesProm  = (PawnsBitboard << 7) & BlackPiecesOccupied & RanksMasks[7] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) LeftCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (LeftCapturesProm != 0) {
		int sqIndx = IterLSB(LeftCapturesProm);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "---";
		MoveStr[0] = file + '0' + 1;
		MoveStr[1] = row  + '0' - 1;
		MoveStr[2] = file + '0';

		this->PossibleMoves += MoveStr + "N" + " ";
		this->PossibleMoves += MoveStr + "B" + " ";
		this->PossibleMoves += MoveStr + "R" + " ";
		this->PossibleMoves += MoveStr + "Q" + " ";
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
			int EndRow = sqIndx / 8;
			int EndFile = sqIndx % 8;

			std::string MoveStr = "---";
			MoveStr[0] = file    + '0';
			MoveStr[1] = row     + '0';
			MoveStr[2] = EndFile + '0';

			this->PossibleMoves += MoveStr + "N" + " ";
			this->PossibleMoves += MoveStr + "B" + " ";
			this->PossibleMoves += MoveStr + "R" + " ";
			this->PossibleMoves += MoveStr + "Q" + " ";
		}

		while (PawnMovements != 0) {
			int sqIndx = IterLSB(PawnMovements);
			int EndRow = sqIndx / 8;
			int EndFile = sqIndx % 8;

			std::string MoveStr = "----";
			MoveStr[0] = file    + '0';
			MoveStr[1] = row     + '0';
			MoveStr[2] = EndFile + '0';
			MoveStr[3] = EndRow  + '0';
			
			this->PossibleMoves += MoveStr + " ";
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
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "----";
		MoveStr[0] = file + '0';
		MoveStr[1] = row  + '0' + 1;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}

	// double push
	U64 DoublePushMoves = (PawnsBitboard >> 16) & Empty & (Empty >> 8) & RanksMasks[4];
	if (CheckingPieces.size() == 1) DoublePushMoves &= CheckingPieces[0].BlockingBitboard;

	while (DoublePushMoves != 0) {
		int sqIndx = IterLSB(DoublePushMoves);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "----";
		MoveStr[0] = file + '0';
		MoveStr[1] = row  + '0' + 2;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}

	// captures
	U64 RightCaptures = (PawnsBitboard >> 9) & (WhitePiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[0] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) RightCaptures &= CheckingPieces[0].BlockingBitboard;

	while (RightCaptures != 0) {
		int sqIndx = IterLSB(RightCaptures);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "----";
		MoveStr[0] = file + '0' + 1;
		MoveStr[1] = row  + '0' + 1;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}

	U64 LeftCaptures  = (PawnsBitboard >> 7) & (WhitePiecesOccupied | BoardVariables.EnPassantMask) & ~RanksMasks[0] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) LeftCaptures &= CheckingPieces[0].BlockingBitboard;

	while (LeftCaptures != 0) {
		int sqIndx = IterLSB(LeftCaptures);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "----";
		MoveStr[0] = file + '0' - 1;
		MoveStr[1] = row  + '0' + 1;
		MoveStr[2] = file + '0';
		MoveStr[3] = row  + '0';
		this->PossibleMoves += MoveStr + " ";
	}

	// PROMOTIONS
	// single push
	U64 SinglePushMovesProm = (PawnsBitboard >> 8)  & Empty & RanksMasks[0];
	if (CheckingPieces.size() == 1) SinglePushMovesProm &= CheckingPieces[0].BlockingBitboard;

	while (SinglePushMovesProm != 0) {
		int sqIndx = IterLSB(SinglePushMovesProm);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "---";
		MoveStr[0] = file + '0';
		MoveStr[1] = row  + '0' + 1;
		MoveStr[2] = file + '0';

		this->PossibleMoves += MoveStr + "N" + " ";
		this->PossibleMoves += MoveStr + "B" + " ";
		this->PossibleMoves += MoveStr + "R" + " ";
		this->PossibleMoves += MoveStr + "Q" + " ";
	}

	// captures
	U64 RightCapturesProm = (PawnsBitboard >> 9) & WhitePiecesOccupied & RanksMasks[0] & ~FilesMasks[7];
	if (CheckingPieces.size() == 1) RightCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (RightCapturesProm != 0) {
		int sqIndx = IterLSB(RightCapturesProm);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "---";
		MoveStr[0] = file + '0' + 1;
		MoveStr[1] = row  + '0' + 1;
		MoveStr[2] = file + '0';

		this->PossibleMoves += MoveStr + "N" + " ";
		this->PossibleMoves += MoveStr + "B" + " ";
		this->PossibleMoves += MoveStr + "R" + " ";
		this->PossibleMoves += MoveStr + "Q" + " ";
	}

	U64 LeftCapturesProm  = (PawnsBitboard >> 7) & WhitePiecesOccupied & RanksMasks[0] & ~FilesMasks[0];
	if (CheckingPieces.size() == 1) LeftCapturesProm &= CheckingPieces[0].BlockingBitboard;

	while (LeftCapturesProm != 0) {
		int sqIndx = IterLSB(LeftCapturesProm);
		int row = sqIndx / 8;
		int file = sqIndx % 8;

		std::string MoveStr = "---";
		MoveStr[0] = file + '0' - 1;
		MoveStr[1] = row  + '0' + 1;
		MoveStr[2] = file + '0';

		this->PossibleMoves += MoveStr + "N" + " ";
		this->PossibleMoves += MoveStr + "B" + " ";
		this->PossibleMoves += MoveStr + "R" + " ";
		this->PossibleMoves += MoveStr + "Q" + " ";
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

		while (PawnMovements != 0 && row == 6) {
			int sqIndx = IterLSB(PawnMovements);
			int EndRow = sqIndx / 8;
			int EndFile = sqIndx % 8;

			std::string MoveStr = "---";
			MoveStr[0] = file    + '0';
			MoveStr[1] = row     + '0';
			MoveStr[2] = EndFile + '0';

			this->PossibleMoves += MoveStr + "N" + " ";
			this->PossibleMoves += MoveStr + "B" + " ";
			this->PossibleMoves += MoveStr + "R" + " ";
			this->PossibleMoves += MoveStr + "Q" + " ";
		}

		while (PawnMovements != 0) {
			int sqIndx = IterLSB(PawnMovements);
			int EndRow = sqIndx / 8;
			int EndFile = sqIndx % 8;

			std::string MoveStr = "----";
			MoveStr[0] = file    + '0';
			MoveStr[1] = row     + '0';
			MoveStr[2] = EndFile + '0';
			MoveStr[3] = EndRow  + '0';

			this->PossibleMoves += MoveStr + " ";
		}
	}
}