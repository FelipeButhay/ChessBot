#include "Tools.h"

bool mouseInRect(int posx, int posy, int width, int height) {
    int mx = GetMouseX(), my = GetMouseY();
    return (mx > posx && mx < posx+width) && (my > posy && my < posy+height);
}

bool mouseInCirc(int posx, int posy, int radius) {
    int mx = GetMouseX(), my = GetMouseY();
    float h = sqrt((posx-mx)*(posx-mx) + (posy-my)*(posy-my));
    return h < radius;
}

char* strToChar(std::string& str) {
    char* cstr = new char[str.length() + 1];

    for (int i = 0; i<(int)str.length(); i++) {
        cstr[i] = str[i];
    }
    cstr[str.length()] = '\0';

    return cstr;
}

// EJEMPLO DE PASAR STD::STR A CHAR*
// 
// std::string textStr = "...";
// char* textChar = strToChar(textStr);
// DrawText(textChar,...);
// delete[] textChar;

std::string numToStr(float num) {
    std::string str = std::to_string(num);
    return str;
}

std::string intToStr(int num) {
    std::string str = std::to_string(num);
    return str;
}

int IterLSB(unsigned long long int& Bitboard) {
    int LSB = _tzcnt_u64(Bitboard);
    Bitboard &= (Bitboard - 1);
    return LSB;
}

U64 reverse(U64 b) {
    b = ((b >>  1) & 0x5555555555555555) | ((b & 0x5555555555555555) <<  1);
    b = ((b >>  2) & 0x3333333333333333) | ((b & 0x3333333333333333) <<  2);
    b = ((b >>  4) & 0x0F0F0F0F0F0F0F0F) | ((b & 0x0F0F0F0F0F0F0F0F) <<  4);
    b = ((b >>  8) & 0x00FF00FF00FF00FF) | ((b & 0x00FF00FF00FF00FF) <<  8);
    b = ((b >> 16) & 0x0000FFFF0000FFFF) | ((b & 0x0000FFFF0000FFFF) << 16);
    b = ( b >> 32)                       | ( b                       << 32);
    return b;
}

void PrintU64(U64 b) {
    std::cout << "White Side";
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) std::cout << '\n';
        std::cout << ((b & (1ULL << i)) != 0) << " ";
    }
    std::cout << '\n' << "Black Side" << '\n';
}
