#pragma once
#include <string>
#include <cmath>
#include <cstring>
#include <immintrin.h>
#include <iostream>

#include "raylib.h"

bool mouseInRect(int posx, int posy, int width, int height);

bool mouseInCirc(int posx, int posy, int radius);

char* strToChar(std::string& str);

std::string numToStr(float num);

std::string intToStr(int num);

int IterLSB(unsigned long long int& Bitboard);

using U64 = unsigned long long int;
U64 reverse(U64 b);

void PrintU64(U64 b);