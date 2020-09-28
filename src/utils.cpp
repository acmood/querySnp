//
//  utils.cpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#include "utils.hpp"
#include <string.h>

char* readData(const char* filepath){
    char* data;
    return data;
}

mmh3 getModOfHash(const char* s, const std::vector<int> &seeds, const uint64_t mod){
    int len = strlen(s);
    printf("len = %d\n", len);
    mmh3 ret;
    uint8_t *out = new uint8_t[16];
    for (int seed : seeds){
        uint64_t res = 0;
        MurmurHash3_x64_128(s, len, seed, out);
        for (int j = 15; j >= 0; j --){
            res = (((1ll * res) << 8) + out[j])%mod;
        }
        ret.push_back(res);
    }
    delete [] out;
    return ret;
}
