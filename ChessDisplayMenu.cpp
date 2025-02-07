#include "ChessDisplay.h"

void Display::LoopMenu() {
	if (IsKeyPressed(KEY_SPACE)) {
		if (GetWindowPosition().x == 0 && GetWindowPosition().y == 0) {
			SetWindowPosition(100, 100);
		} else {
			SetWindowPosition(0, 0);
		}
	}

	if (IsMouseButtonPressed(0)) {
		if (mouseInRect(2*u, 6*u, MeasureText("Local Game", 1.5*u), 1.5*u)) {
			this->stage = 2;

			this->BoardVariables = engine.GetBoardVariables();
			this->WhitePieces = engine.GetBitboards(true);
			this->BlackPieces = engine.GetBitboards(false);

			this->WaitingForPromotion = false;
			this->PromSubString = "";
		}

		if (mouseInRect(2*u, 9*u, MeasureText("Online Game (coming soon...)", 1.5*u), 1.5*u)) {
			
		}

		if (mouseInRect(2*u, 12*u, MeasureText("Online Game (coming soon...)", 1.5*u), 1.5*u)) {

		}

		if (mouseInRect(2*u, 15*u, MeasureText("Settings", 1.5*u), 1.5*u)) {
			this->stage = 1;
		}
	}
}

void Display::DrawMenu() {
	ClearBackground(BACKGROUND);

	// TITULO
	DrawText("Chess", u*1.25, u*1.25, 3.5*u, BLACK);
	DrawText("Chess", u     , u     , 3.5*u, WHITE);

	//SUB TITULOS
	DrawText("Local Game"          , 2*u, 6*u , 1.5*u,
		mouseInRect(2*u, 6*u , MeasureText("Local Game"          , 1.5*u), 1.5*u) ? GRAY : WHITE);

	DrawText("Bot (coming soon...)", 2*u, 9*u , 1.5*u,
		mouseInRect(2*u, 9*u , MeasureText("Bot (coming soon...)", 1.5*u), 1.5*u) ? GRAY : WHITE);

	DrawText("Online Game (coming soon...)", 2*u, 12*u, 1.5*u,
		mouseInRect(2*u, 12*u, MeasureText("Online Game (coming soon...)", 1.5*u), 1.5*u) ? GRAY : WHITE);

	DrawText("Settings"            , 2*u, 15*u, 1.5*u,
		mouseInRect(2*u, 15*u, MeasureText("Settings"            , 1.5*u), 1.5*u) ? GRAY : WHITE);
}