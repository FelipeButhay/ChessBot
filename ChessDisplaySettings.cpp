#include "ChessDisplay.h"

void Display::LoopSettings() {
	if (IsKeyPressed(KEY_SPACE)) {
		if (GetWindowPosition().x == 0 && GetWindowPosition().y == 0) {
			SetWindowPosition(100, 100);
		} else {
			SetWindowPosition(0, 0);
		}
	}

	if (IsMouseButtonPressed(0)) {
		if (mouseInRect(u, u*18, MeasureText("Return", u*4/3), u*4/3)) {
			this->stage = 0;
		}
		if (mouseInRect(u*14.00f, u*3.533f, u*2, u*2)) {
			this->resolution--;
		}
		if (mouseInRect(u*16.4f, u*3.533f, u*2, u*2)) {
			this->resolution++;
		}
		if (mouseInRect(u*18.8f, u*3.533f, u*3.2f, u*2)) {
			this->sx = difResolutions[resolution%8][0];
			this->sy = difResolutions[resolution%8][1];
			this->u = sy*0.05f;

			SetWindowSize(sx, sy);
		}
	}
}

void Display::DrawSettings() {
	ClearBackground(BACKGROUND); // gap*7 en x empiezan los true setting
	DrawText("Settings", u, u, u*2, WHITE);

	// ELEGIR RESOLUCION
	DrawText("Resolution", u*3, u*3.9f, u*4/3, WHITE);

	DrawRectangle(u*14.00f, u*3.533f, u*2, u*2, GRAY);
	DrawText("-", u*14.602f, u*3.58f, u*2, WHITE);

	DrawRectangle(u*16.4f, u*3.533f, u*2, u*2, GRAY);
	DrawText("+", u*16.9f, u*3.58f, u*2, WHITE);

	DrawRectangle(u*18.8f, u*3.533f, u*3.2f, u*2, GRAY);
	DrawText("Apply", u*19.0f, u*3.9f, u, WHITE);

	std::string resolutionStdStr = intToStr(difResolutions[resolution%8][0]) + (std::string)" x " + intToStr(difResolutions[resolution%8][1]);
	char* resolutionCharP = strToChar(resolutionStdStr);
	DrawText(resolutionCharP, u*23.0f, u*3.9f, u*4/3, WHITE);
	delete[] resolutionCharP;

	// VOLVER AL MENU O LA PARTIDA
	DrawText("Return", u, u*18, u*4/3,
		mouseInRect(u, u*18, MeasureText("Return", u*4/3), u*4/3) ? GRAY : WHITE);
}