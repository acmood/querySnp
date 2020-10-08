//
//  bitarray.cpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#include "bitarray.hpp"
#include <iostream>



Bitarray* getAnd(std::vector<Bitarray*>& bitarrays){
    if (bitarrays.size() == 0){
        Bitarray* ret = new Bitarray();
        return ret;
    }else{
        int len = bitarrays[0]->len;
        Bitarray* ret = new Bitarray(len);
        ret->full(1);
        for(auto bitarray:bitarrays){
            *ret &= *bitarray;
        }
        return ret;
    }
}

Bitarray* getAnd(std::vector<Bitarray*>& bitarrays, int start, int end){
    
    if (bitarrays.size() == 0){
        Bitarray* ret = new Bitarray();
        return ret;
    }else{
        int len = bitarrays[0]->len;
        Bitarray* ret = new Bitarray(len);
        ret->full(1);
        for(int i = start; i < end; i ++){
            *ret &= *bitarrays[i];
        }
        return ret;
    }
}

void Bitarray::full(int value){
    uint8_t v = value ? 0xff : 0x00;
    memset(arr, v, sizeof(uint8_t)*arr_len);
}


void Bitarray::operator &= (const Bitarray &b){
    for (uint64_t i = 0; i < this->arr_len; i ++){
        arr[i] = arr[i] & b.arr[i];
    }
}

Bitarray Bitarray::operator & (const Bitarray &b){
    Bitarray ret = Bitarray(len);
    for (uint64_t i = 0; i < this->arr_len; i ++){
        ret.arr[i] = arr[i] & b.arr[i];
    }
    return ret;
}

Bitarray::Bitarray(){
    len = 0;
    arr_len = 0;
}

Bitarray::Bitarray(uint64_t len){
    uint64_t N = (len-1)/8+1;
    this->len = len;
    arr = new uint8_t[N];
    this->arr_len = N;
    memset(arr, 0, sizeof(arr));
}

Bitarray::Bitarray(uint8_t *_arr, uint64_t len){
    arr = new uint8_t[len];
    memcpy(arr, _arr, len*sizeof(uint8_t));
    arr_len = len;
    len = arr_len << 3;
}


Bitarray::~Bitarray(){
    if (arr) {
        delete []this->arr;  // todo 这里析构报错
        arr = nullptr;
    }
    len = 0;
    arr_len = 0;
}

void Bitarray::set(uint64_t pos, int value){
    if (pos > len) return ;
    value = value ? 1 : 0;
    int idx = pos/8;
    int i =pos-(idx<<3);
    arr[idx] = ((arr[idx]>>(i+1)<<1) | value)<<i | (arr[idx]&((1<<i)-1));
}

uint8_t Bitarray::get(uint64_t pos){
    if (pos >= len) return 0;
    uint64_t idx = pos/8;
    return (arr[idx]>> (pos-idx*8)) & 1;
}

std::vector<uint64_t> Bitarray::search(){
    std::vector<uint64_t> ret;
    for(uint64_t i = 0; i < arr_len; i ++){
        for(int j = 0; j < 8; j ++){
            if (arr[i]&(1<<j)){
                ret.push_back(i*8+j);
            }
        }
    }
    return ret;
}
