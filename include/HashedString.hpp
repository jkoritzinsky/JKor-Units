#pragma once
#include<cstddef>

// Taken from ideone.com/sj8vu
// performs a compile time recursive string hash using the djb2 algorithm explained here: http://w...content-available-to-author-only...u.ca/~oz/hash.html

constexpr unsigned long long hashStringRecursive(unsigned long long hash, const char* str)
{
	return (!*str ? hash :
		hashStringRecursive(((hash << 5) + hash) + *str, (str + 1)));
}

template<std::size_t N>
constexpr unsigned long long hashString(const char(&str)[N])
{
	return (!str ? 0 :
		hashStringRecursive(5381, str));
}