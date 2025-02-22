#include "ChessEngine.h"

void Engine::PerftTesting() {
	for (int n = 0; n <= 10; n++) {
		auto inicio = std::chrono::high_resolution_clock::now();

		unsigned long int NMovements = PerftGeneration(n);

		auto fin = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> duracion = fin - inicio;
		std::cout << "Se genero el paso: " << n << " se generaron: " << NMovements << " en: " << duracion.count() << " ms" << std::endl;
		//std::cout << " COPY 1 TIME: " << Generation1Time << " ms " << std::endl;
		//std::cout << " MOVE 2 TIME: " << Generation2Time << " ms " << std::endl;
		//std::cout << " UNDO 3 TIME: " << Generation3Time << " ms " << std::endl;
	}
}

// 	double MoveTime, SavingTime, UndoTime;

unsigned long int Engine::PerftGeneration(const int Depth) {
	//std::cout << Depth << " ";
	if (Depth <= 0) return 1;

	auto CopyStart = std::chrono::high_resolution_clock::now();

	BoardState BoardData = { 
		WhitePieces, 
		BlackPieces, 
		
		WhitePiecesOccupied, 
		BlackPiecesOccupied, 
		Occupied, 
		Empty, 
		
		BoardVariables, 
		
		PossibleMoves, 
		
		GameState
	};

	this->BoardHistory.push_back(BoardData);

	const std::vector<MoveData> LocalPossibleMoves = PossibleMoves;

	auto CopyEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> CopyTime = CopyEnd - CopyStart;
	Generation1Time += CopyTime.count();

	unsigned long int nodes = 0;
	for (int m = 0; m < LocalPossibleMoves.size(); m++) {

		auto MoveStart = std::chrono::high_resolution_clock::now();

		MoveData Move = LocalPossibleMoves[m];
		this->Move(Move);

		auto MoveEnd = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> MoveTime = MoveEnd - MoveStart;
		Generation2Time += MoveTime.count();



		nodes += PerftGeneration(Depth - 1);



		auto UndoStart = std::chrono::high_resolution_clock::now();

		this->UnMove();

		auto UndoEnd = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> UndoTime = UndoEnd - UndoStart;
		Generation3Time += UndoTime.count();
	}

	this->BoardHistory.pop_back();

	return nodes;
}
