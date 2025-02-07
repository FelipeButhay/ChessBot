#include "ChessEngine.h"

void Engine::MovesGeneratorPawn() {
	if (BoardVariables.Turn) {
		this->MovesGeneratorWhitePawn();
	} else {
		this->MovesGeneratorBlackPawn();
	}
}

void Engine::MovesGeneratorWhitePawn() {
	// single push
	U64 PawnsBitboard = WhitePieces[P];

	U64 SinglePushMoves = (PawnsBitboard << 8)  & Empty & ~RanksMasks[7];
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

void Engine::MovesGeneratorBlackPawn() {
	U64 PawnsBitboard = BlackPieces[P];

	// single push
	U64 SinglePushMoves = (PawnsBitboard >> 8)  & Empty & ~RanksMasks[0];
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