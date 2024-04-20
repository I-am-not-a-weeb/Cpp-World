#pragma once

#include <utility>
#include <boost/functional/hash.hpp>>

struct pair_hash
{
	template <typename T1, typename T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, p.first);
		boost::hash_combine(seed, p.second);
		return seed;
	}
};