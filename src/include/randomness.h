#pragma once
#include <random>


inline std::random_device rd;
inline std::mt19937 generator(rd());
inline std::uniform_int_distribution<int> distribution_2(0,1 );
inline std::uniform_int_distribution<int> distribution_100(1, 100);

inline auto roll_dice_2 = [] { return distribution_2(generator); };
inline auto roll_dice_100 = [] {return distribution_100(generator); };