#pragma once
#include<cstddef>

namespace jkor {
    namespace units {
        namespace details {
            // Taken from ideone.com/sj8vu
            
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
        }
    }
}