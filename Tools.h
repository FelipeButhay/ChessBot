#pragma once
#include <string>
#include <cmath>
#include <cstring>
#include <immintrin.h>
#include <iostream>

#include "raylib.h"

using U64 = unsigned long long int;

bool mouseInRect(int posx, int posy, int width, int height);

bool mouseInCirc(int posx, int posy, int radius);

char* strToChar(std::string& str);

std::string numToStr(float num);

std::string intToStr(int num);

int IterLSB(U64& Bitboard);

U64 reverse(U64 b);

void PrintU64(U64 b);