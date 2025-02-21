#pragma once

#include <string>
#include <array>
#include <iostream>
#include <vector>
#include <Chrono>

#include "Tools.h"

enum PiecesIndx { P, N, B, R, Q, K };

using U64 = unsigned long long int;

const std::array<U64, 8> RanksMasks = {
	0x00000000000000ff, 
	0x000000000000ff00, 
	0x0000000000ff0000, 
	0x00000000ff000000, 
	0x000000ff00000000, 
	0x0000ff0000000000, 
	0x00ff000000000000, 
	0xff00000000000000 
};

const std::array<U64, 8> FilesMasks = {
	0x0101010101010101, 
	0x0202020202020202, 
	0x0404040404040404, 
	0x0808080808080808,
	0x1010101010101010, 
	0x2020202020202020, 
	0x4040404040404040, 
	0x8080808080808080, 
};

const std::array<U64, 15> DiagonalMasks = {
	0x0000000000000080,
	0x0000000000008040,
	0x0000000000804020,
	0x0000000080402010,
	0x0000008040201008,
	0x0000804020100804,
	0x0080402010080402,
	0x8040201008040201,
	0x4020100804020100,
	0x2010080402010000,
	0x1008040201000000,
	0x0804020100000000,
	0x0402010000000000,
	0x0201000000000000,
	0x0100000000000000 
};

const std::array<U64, 15> AntiDiagonalMasks = {
	0x0000000000000001,
	0x0000000000000102,
	0x0000000000010204,
	0x0000000001020408,
	0x0000000102040810,
	0x0000010204081020,
	0x0001020408102040,
	0x0102040810204080,
	0x0204081020408000,
	0x0408102040800000,
	0x0810204080000000,
	0x1020408000000000,
	0x2040800000000000,
	0x4080000000000000,
	0x8000000000000000   
};

const U64 knightMask = 0xa1100110a;
const U64 kingMask = 0x70507;

const U64 CastlingSafe_WKMask = 0x0000000000000070;
const U64 CastlingEmptyWKMask = 0x0000000000000060;
								
const U64 CastlingSafe_WQMask = 0x000000000000001c;
const U64 CastlingEmptyWQMask = 0x000000000000000e;
								
const U64 CastlingSafe_BKMask = 0x7000000000000000;
const U64 CastlingEmptyBKMask = 0x6000000000000000;

const U64 CastlingSafe_BQMask = 0x1c00000000000000;
const U64 CastlingEmptyBQMask = 0x0e00000000000000;

typedef struct Board {
	bool Turn;

	bool CastleWK;
	bool CastleWQ;

	bool CastleBK;
	bool CastleBQ;

	U64 EnPassantMask;

	unsigned short int N50MovesRule;
	unsigned short int NMoves;
};

typedef struct CheckingPiece {
	char PieceType;
	short PosIndx;
	U64 BlockingBitboard;
};

typedef struct PinnedPiece {
	U64 MovementRay;
};

typedef struct BoardState {
	std::array<U64, 6> WhitePieces = { 0 };
	std::array<U64, 6> BlackPieces = { 0 };

	U64 WhitePiecesOccupied = 0;
	U64 BlackPiecesOccupied = 0;
	U64 Occupied = 0;
	U64 Empty = 0;

	Board BoardVariables;

	std::string PossibleMoves;

	int GameState;
};

class Engine {
	private:

	// 4 chars + space  
	std::string GameMoves = "";
	std::string PossibleMoves = "";

	std::array<U64, 6> WhitePieces = { 0 };
	std::array<U64, 6> BlackPieces = { 0 };

	std::vector<CheckingPiece> CheckingPieces;
	std::array<PinnedPiece, 64> PinnedPieces;
	U64 PinnedPiecesBitBoard;

	U64 WhitePiecesOccupied = 0;
	U64 BlackPiecesOccupied = 0;
	U64 Occupied = 0;
	U64 Empty = 0;

	// 0 playing, 1 mate, 2 stalemate, 3 repetition, 4 insufficient material, 5 50 moves rule
	short GameState = 0;

	Board BoardVariables;

	std::vector<BoardState> BoardHistory;

	double Generation1Time = 0, Generation2Time = 0, Generation3Time;

	void MovesGeneratorPawn();
	void MovesGeneratorKnight();
	void MovesGeneratorBishop();
	void MovesGeneratorRook();
	void MovesGeneratorQueen();
	void MovesGeneratorKing();

	void MovesGeneratorWhitePawn();
	void MovesGeneratorBlackPawn();

	void MovesGeneratorWhitePinnedPawn();
	void MovesGeneratorBlackPinnedPawn();

	void GenerateCheckingPieces();
	void GeneratePinnedPieces();

	unsigned long int PerftGeneration(int Depth);

	U64 MovesGeneratorUnsafeSq();

	public:
	Engine(const std::string FEN);

	void GenerateMoveStr();
	void Move(std::string& Move4Char);
	void UnMove();

	std::string FilterMoveString(unsigned short filterSq);

	std::string        GetGameString();
	Board              GetBoardVariables();
	std::array<U64, 6> GetBitboards(bool color);
	int                GetGameResult();

	void PerftTesting();
};