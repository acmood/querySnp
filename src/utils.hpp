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

char* int2str(int x);
size_t strcat2(char **dst_out, ...);
uint32_t maxInList(uint32_t *count, int len);
char* get_read_string(const char *file_name);
void split(const char * str, const char c, std::vector<char*> &out);

void subStr(char* ret, const char * src, int l, int r);

void print(const char *s);
void print(std::vector<uint64_t> s);

#endif /* utils_hpp */
