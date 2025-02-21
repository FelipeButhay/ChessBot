#include "raylib.h"
#include "ChessDisplay.h"

void Display::LoopGame() {
	if (IsMouseButtonPressed(0)) {
		bool PieceMoved = false;

		short mx = GetMouseX();
		short my = GetMouseY();
		short ClickedSqIndx = (mx - 2*u)/(2*u) + (7 - (my - 2*u)/(2*u)) * 8;

		if (mouseInRect(2*u, u*18, MeasureText("Return", u), u)) {
			this->stage = 0;
		}

		// CHOOSE A PIECE TO PROMOTE
		if (mouseInRect(20*u, 4*u, 8*u, 2*u) && WaitingForPromotion) {
			int PiecePromotedIndx = (mx - 20*u)/(2*u);
			switch (PiecePromotedIndx){
				case 0: this->PromSubString[3] = 'N'; break;
				case 1: this->PromSubString[3] = 'B'; break;
				case 2: this->PromSubString[3] = 'R'; break;
				case 3: this->PromSubString[3] = 'Q'; break;
			}

			engine.Move(PromSubString);
			this->GameState = engine.GetGameResult();
			this->PromSubString = "";

			PieceMoved = true;

			this->BoardVariables = engine.GetBoardVariables();
			this->WhitePieces = engine.GetBitboards(true);
			this->BlackPieces = engine.GetBitboards(false);

			this->PossibleMovesForSelectedPieceVec.clear();

			this->WaitingForPromotion = false;
		}

		// PLAY A MOVE
		if (mouseInRect(2*u, 2*u, 16*u, 16*u) && !WaitingForPromotion) {
			
			for (int i = 0; i < PossibleMovesForSelectedPieceVec.size(); i++) {
				if (ClickedSqIndx == PossibleMovesForSelectedPieceVec[i].sqIndx) {
					// hago elegir al usuario una pieza para la promotion
					if (PossibleMovesForSelectedPieceVec[i].Move4Char[3] > '9') {
						this->WaitingForPromotion = true;
						this->PromSubString = PossibleMovesForSelectedPieceVec[i].Move4Char;
						break;
					}

					engine.Move(PossibleMovesForSelectedPieceVec[i].Move4Char);
					this->GameState = engine.GetGameResult();

					PieceMoved = true;

					this->BoardVariables = engine.GetBoardVariables();
					this->WhitePieces = engine.GetBitboards(true);
					this->BlackPieces = engine.GetBitboards(false);

					this->PossibleMovesForSelectedPieceVec.clear();
					break;
				}
			}
		}

		// SELECT A PIECE
		if (mouseInRect(2*u, 2*u, 16*u, 16*u) && !PieceMoved && !WaitingForPromotion) {

			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {

					if (BoardVariables.Turn) {
						for (int k = 0; k < 6; k++) {
							if ((WhitePieces[k] & (1ULL << ClickedSqIndx)) != 0) {
								this->SelectedPieceSq = ClickedSqIndx;
								break;
							} else {
								this->SelectedPieceSq = -1;
							}
						}
					} else {
						for (int k = 0; k < 6; k++) {
							if ((BlackPieces[k] & (1ULL << ClickedSqIndx)) != 0) {
								this->SelectedPieceSq = ClickedSqIndx;
								break;
							} else {
								this->SelectedPieceSq = -1;
							}
						}
					}
				}
			}

			this->PossibleMovesForSelectedPieceVec.clear();
			if (SelectedPieceSq == -1) {
				this->PossibleMovesForSelectedPieceStr = "";
			} else {
				this->PossibleMovesForSelectedPieceStr = engine.FilterMoveString(SelectedPieceSq);
				MovesStrToVec(PossibleMovesForSelectedPieceStr);
			}
		}
	}
}

void Display::DrawGame() {
	ClearBackground(BACKGROUND);

	// DRAW THE BOARD AND THE PIECES
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {

			int sqIndx = i + (7 - j) * 8;
			DrawRectangle(2*u + 2*u*i, 2*u + 2*u*j, 2*u, 2*u, (i + j) % 2 == 0 ? BOARD_BLACK : BOARD_WHITE);

			for (int k = 0; k < 6; k++) {
				if ((WhitePieces[k] & (1ULL << sqIndx)) != 0) {
					Vector2 PiecePos = { 2*u + 2*u*i, 2*u + 2*u*j };
					DrawTextureEx(WhitePiecesTexture[k], PiecePos, 0, 2*u/128.0f, WHITE);
				}
			}

			for (int k = 0; k < 6; k++) {
				if ((BlackPieces[k] & (1ULL << sqIndx)) != 0) {
					Vector2 PiecePos = { 2*u + 2*u*i, 2*u + 2*u*j };
					DrawTextureEx(BlackPiecesTexture[k], PiecePos, 0, 2*u/128.0f, WHITE);
				}
			}
		}
	}

	// DRAW THE POSSIBLE MOVES SQ
	for (int i = 0; i < PossibleMovesForSelectedPieceVec.size(); i++) {
		int file = PossibleMovesForSelectedPieceVec[i].sqIndx % 8;
		int row = PossibleMovesForSelectedPieceVec[i].sqIndx / 8;

		DrawRectangle(2*u + 2*u*file, 2*u + 2*u*(7 - row), 2*u, 2*u, BOARD_T_RED);
	}

	// DRAW THE PROMOTION MENU
	if (WaitingForPromotion && GameState == 0) {
		DrawText("Promotion!", 20*u, 2*u, 1.5*u, WHITE);
		for (int i = 0; i < 4; i++) {
			DrawRectangle(20*u + 2*u*i, 4*u, 2*u, 2*u, i % 2 == 0 ? BOARD_BLACK : BOARD_WHITE);

			Vector2 PiecePos = { 20*u + 2*u*i, 4*u };
			DrawTextureEx(BoardVariables.Turn ? WhitePiecesTexture[i + 1] : BlackPiecesTexture[i + 1], 
						  PiecePos, 0, 2*u/128.0f, WHITE);
		}
	}

	if (GameState == 1) {
		DrawText("Checkmate", 20*u, 2*u, 2*u, WHITE);

		if (!BoardVariables.Turn) {
			DrawText("White Wins", 20*u, 4*u, 1*u, GRAY);
		} else {
			DrawText("Black Wins", 20*u, 4*u, 1*u, GRAY);
		}
	}

	if (GameState == 2) {
		DrawText("Stalemate", 20*u, 2*u, 2*u, WHITE);
		DrawText("It's a Draw", 20*u, 4*u, 1*u, GRAY);
	}

	DrawText("Return", 2*u, u*18.5, u,
		mouseInRect(2*u, u*18, MeasureText("Return", u), u) ? GRAY : WHITE);

}