//
//  utils.hpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include "MurmurHash3.h"
#include <vector>

#define mmh3 std::vector<uint64_t>

char* readData(const char* filepath);
mmh3 getModOfHash(const char* s, const std::vector<int> &seed, const uint64_t mod);

#endif /* utils_hpp */
