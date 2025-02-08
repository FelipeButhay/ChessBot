#include "ChessDisplay.h"

int main(void) {
	// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Engine engine(FEN);
	Display display(engine);

	while (!WindowShouldClose()) {
		display.Loop();

		BeginDrawing();

			display.Draw();

		EndDrawing();
	}

	return 0;
}