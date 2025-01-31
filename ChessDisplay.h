#include "raylib.h"
#include "ChessEngine.h"

class Display {
	private:
	short sx, sy, u, sq;
	short stage;
	

	void LoopMenu();
	void LoopGame();

	void DrawMenu();
	void DrawGame();

	public:

	Display();

	void Loop();
	void Draw();
};