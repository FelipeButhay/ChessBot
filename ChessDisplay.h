#include "raylib.h"
#include "ChessEngine.h"
#include "Tools.h"

#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <thread>
#include <atomic>

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
	bool ReadyToDrawAnimation = false;
	std::atomic<bool> LoadingAnimation = false;
	std::thread LoadingThread;

	std::vector<std::vector<std::string>> Files;
	std::vector<std::string> FirstFile;
	Font MonoFont;

	unsigned short resolution;
	std::array<std::array<short, 2>, 8> difResolutions = { {
		{  900,  500 }, { 1280,  720 }, { 1366,  768 }, { 1536,  864 },
		{ 1600,  900 }, { 1920, 1080 }, { 2560, 1440 }, { 3840, 2160 }
	} };

	short SelectedPieceSq = -1;
	std::vector<MoveForDisplay> PossibleMovesForSelectedPieceVec;
	std::string PossibleMovesForSelectedPieceStr = "";

	std::array<Texture2D, 6> WhitePiecesTexture;
	std::array<Texture2D, 6> BlackPiecesTexture;

	std::array<U64, 6> WhitePieces = { 0 };
	std::array<U64, 6> BlackPieces = { 0 };
	Board BoardVariables = { 0 };
	short GameState = 0;

	bool WaitingForPromotion = false;
	std::string PromSubString;

	Engine& engine;

	void LoopMenu();
	void LoopGame();
	void LoopSettings();

	void DrawMenu();
	void DrawGame();
	void DrawSettings();

	void MovesStrToVec(std::string& MovesStr);

	void LoadAnimation(std::vector<std::vector<std::string>>* AnimationFiles);

	public:

	Display(Engine& engine);

	void Loop();
	void Draw();

	void Unload();
};