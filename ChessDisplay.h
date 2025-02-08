#include "raylib.h"
#include "ChessEngine.h"
#include "Tools.h"

#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <fstream>

#define BOARD_WHITE  CLITERAL(Color){ 241, 217, 192, 255 }
#define BOARD_BLACK  CLITERAL(Color){ 127,  65,  31, 255 }

#define BOARD_T_RED  CLITERAL(Color){ 230,  41,  55, 100 }
#define BOARD_T_BLUE CLITERAL(Color){   0, 121, 241, 100 }

#define BACKGROUND   CLITERAL(Color){  50,  50,  50, 255 }

typedef struct MoveForDisplay {
	std::string Move4Char;
	int sqIndx;
};

class Display {
	private:
	short sx, sy, u;

	// 0 menu, 1 settings, 2 chess local, 3 chess bot
	short stage;

	float AnimationFrame = 0;
	std::vector<std::vector<std::string>> Files;
	Font MonoFont;

	unsigned short resolution;
	short difResolutions[8][2] = { {  900,  500 }, { 1280,  720 }, { 1366,  768 }, { 1536,  864 },
								   { 1600,  900 }, { 1920, 1080 }, { 2560, 1440 }, { 3840, 2160 } };

	short SelectedPieceSq = -1;
	std::vector<MoveForDisplay> PossibleMovesForSelectedPieceVec;
	std::string PossibleMovesForSelectedPieceStr = "";

	std::array<Texture2D, 6> WhitePiecesTexture;
	std::array<Texture2D, 6> BlackPiecesTexture;

	std::array<U64, 6> WhitePieces = { 0 };
	std::array<U64, 6> BlackPieces = { 0 };
	Board BoardVariables;

	bool WaitingForPromotion;
	std::string PromSubString;

	Engine& engine;

	void LoopMenu();
	void LoopGame();
	void LoopSettings();

	void DrawMenu();
	void DrawGame();
	void DrawSettings();

	void MovesStrToVec(std::string& MovesStr);

	public:

	Display(Engine& engine);

	void Loop();
	void Draw();

	void Unload();
};