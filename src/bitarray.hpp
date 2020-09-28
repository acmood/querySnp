//
//  bitarray.hpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#ifndef bitarray_hpp
#define bitarray_hpp

#include <stdio.h>
#include <vector>
#include <string.h>
//todo
struct Bitarray{
    std::vector<uint64_t> search();
    Bitarray();
    ~Bitarray();
    Bitarray(uint64_t len);
    Bitarray(uint8_t *_arr, uint64_t len);
    Bitarray operator & (const Bitarray &b);
    void operator &= (const Bitarray &b);
    void set(uint64_t pos, int value);
    uint8_t get(uint64_t pos);
    uint8_t operator [] (const uint64_t pos){
        return this->get(pos);
    }
    void full(int value);
    uint64_t arr_len;
    uint64_t len;
    uint8_t *arr;
};


Bitarray getAnd(std::vector<Bitarray>& bitarrays);

#endif /* bitarray_hpp */
