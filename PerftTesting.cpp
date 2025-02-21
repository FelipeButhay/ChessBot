#include "ChessEngine.h"

void Engine::PerftTesting() {
	for (int n = 0; n <= 10; n++) {
		auto inicio = std::chrono::high_resolution_clock::now();

		unsigned long int NMovements = PerftGeneration(n);

		auto fin = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> duracion = fin - inicio;
		std::cout << "Se genero el paso: " << n << " se generaron: " << NMovements << " en: " << duracion.count() << " ms" << std::endl;
		std::cout << " GENERATION 1 TIME: " << Generation1Time << " ms " << std::endl;
		std::cout << " GENERATION 2 TIME: " << Generation2Time << " ms " << std::endl;
		std::cout << " GENERATION 3 TIME: " << Generation3Time << " ms " << std::endl;
	}
}

// 	double MoveTime, SavingTime, UndoTime;

unsigned long int Engine::PerftGeneration(const int Depth) {
	//std::cout << Depth << " ";
	if (Depth <= 0) return 1;

	BoardState BoardData = { 
		WhitePieces, BlackPieces, WhitePiecesOccupied, BlackPiecesOccupied, Occupied, Empty, BoardVariables, PossibleMoves, GameState
	};
	this->BoardHistory.push_back(BoardData);

	const std::string LocalPossibleMoves = PossibleMoves;

	//if (LocalPossibleMoves.size() == 0) {
	//	std::cout << "ERROR, SIN MOVIMIENTOS, movimientos: " << GameMoves << '\n';
	//}

	unsigned long int nodes = 0;
	for (int m = 0; m < LocalPossibleMoves.size(); m += 5) {

		std::string Move4Char = LocalPossibleMoves.substr(m, 4);
		this->Move(Move4Char);

		nodes += PerftGeneration(Depth - 1);

		this->UnMove();
	}

	//if (Depth == 2) std::cout << nodes << '\n';
	//if (Depth == 1) std::cout << "    " << nodes << '\n';

	this->BoardHistory.pop_back();

	return nodes;
}
