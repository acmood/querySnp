//
//  querySnp.cpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#include "querySnp.hpp"
#include "utils.hpp"
#include <vector>
#include "bitarray.hpp"
#include "mredis.hpp"
#include <string>
#include <algorithm>

#define M 46
std::vector<int> seeds{0,1,2,3,4,5,6};
#define SIZE1 47925291
#define SIZE2 47925291
#define KERM_CNT 120
#define HASH_CNT 10
#define k1 31
#define k2 15




void parserFileData(char *fileData, std::vector<char*> &line_list, std::vector<std::vector<char*> > &out){
    split(fileData, '\n', line_list);
    for(int i = 0; i < line_list.size(); i ++){
        std::vector<char*> char_list;
        split(line_list[i], ' ', char_list);
        out.push_back(char_list);
    }
}


Bitarray* get_kmer_bitarray_col(uint64_t col, std::vector<Bitarray*> kmerHash){
    /* """得到BF的一列""" */
    std::vector<int> lst;
    for (auto kmer : kmerHash){
        lst.push_back(kmer->get(col));
    }
    Bitarray* ret = new Bitarray(kmerHash.size());
    for (int i = 0; i < kmerHash.size(); i ++)
        ret->set(i, lst[i]);
    return ret;
}

void checkRead(const std::vector<char*> &dataLine, std::vector<int> &ansIdx, std::vector<Bitarray*> &readBit){
    uint32_t *count = new uint32_t[M];
    memset(count, 0, sizeof(int)*M);
    std::vector<Bitarray*> kmerHash;
    int kmerId = 0;
    std::vector<uint8_t*> getBytes;
    uint64_t lenCol = 0;
    for(auto kmer:dataLine){
        auto kid = getModOfHash(kmer, seeds, SIZE1);
        bool flag = false;
        int colCnt = 0;
        for (auto col:kid){
            char *pref=nullptr;
            strcat2(&pref, "bacteria", "_", int2str(col), "_",  int2str(k1), "kmer", nullptr);
            if(kmerId == KERM_CNT-1 and colCnt == HASH_CNT-1){
                flag = true;
            }
            colCnt += 1;
            uint8_t *byte;
            uint64_t t = redis::instance()->get(pref, byte);
            colCnt = std::max(t, lenCol);
            if (t > 0)
                getBytes.push_back(byte);
            else
                printf("[ERROR] get col str is None");
        }
        kmerId += 1;
    }
    std::vector<Bitarray*> bitarrayList;
    for(auto x:getBytes)
        bitarrayList.push_back(new Bitarray(x, lenCol));
    int st = 0;
    for(int i = 0; i < dataLine.size(); i ++){
        int ed = (i+1) * HASH_CNT;
    //         与运算确定kmer在哪些bf中存在
        Bitarray *k_bit = getAnd(bitarrayList, st, ed);
        kmerHash.push_back(k_bit);
    //         bf_id：kmer存在的bf id
        auto bf_id = k_bit->search();
    //     count:存储每个bf中检索到的kmer个数
        for (auto idd:bf_id)
            count[idd] += 1;
        st += HASH_CNT;
    //         后续需要修改为top k
    }

    int mx = maxInList(count, M);
    for (int i =0; i < M; i ++)
        if(count[i] == mx)
            ansIdx.push_back(i);
    //     read_bit：返回第一层bf的检索结果
    for (auto col:ansIdx)
        readBit.push_back(get_kmer_bitarray_col(col,kmerHash));
    delete [] count;
    kmerHash.clear();
    getBytes.clear();
    //         ans:存在率最高的bf id, read_bit：由第一层bf检测后的01序列
}


Bitarray* getBitarrayByColume(uint64_t col){
    redis::RedisMgr* r = redis::instance("127.0.0.1", 6379);
    char *key;
    strcat2(&key, "bacteria", "_", int2str(col), "_", int2str(k2), "kmer");
    uint8_t *res;
    r->get(key, res);
    int len = sizeof(res);
    return new Bitarray(res, len);
}


uint8_t check_hash_bf2(mmh3 hash_value, Bitarray* bit_arr){
    uint8_t ret = 1;
    for (auto col : hash_value){
        auto arr = (*bit_arr)[col];
        ret &= arr;
    }
    return ret;
}

int checkSnp(const std::vector<int> &first_bf_ansIdx, const std::vector<Bitarray*> &first_bf_ans_readBit, std::vector<char*> lineList, int idx){
    int fir_bf_id = first_bf_ansIdx[0];
    char* temp = lineList[idx];
    Bitarray* fir_bf_arr = first_bf_ans_readBit[0];
    Bitarray* sec_bf_arr = getBitarrayByColume(fir_bf_id);
    uint64_t i = 0;
    uint64_t ret = 0;
    uint64_t end_pos = 0;
    uint64_t lentemp = strlen(temp);
    while(i < KERM_CNT){
        if((*fir_bf_arr)[i] == 0){
            ret += 1;
            uint64_t snp_pos = i+k1;
            end_pos = snp_pos;
            i += (k1-1);
            bool flag1 = 1, flag2 = 1;
            if(snp_pos+k2 <= lentemp){
                end_pos += k2;
                char *t1;
                subStr(t1, temp, snp_pos, snp_pos+k2);
                mmh3 hash_value = getModOfHash(t1, seeds, SIZE2);
                flag1 = check_hash_bf2(hash_value,sec_bf_arr);
                if(flag1 == 0){
                    ret += 1;
                    i += k2;
                }
            }
        }
        i += 1;
    }
    while(end_pos < lentemp){
        if(end_pos+k2 > lentemp) break;
        char *t1;
        subStr(t1, temp, end_pos, end_pos+k2);
        mmh3 hash_value = getModOfHash(t1, seeds, SIZE2);
        bool flag1 = check_hash_bf2(hash_value,sec_bf_arr);
        if(!flag1)
            ret += 1;
        end_pos += k2;
    }
    return ret;
}

void doCheck(const char *filepath){
    char* fileData = readData(filepath);
    // fileData -> ListData
    std::vector<std::vector<char*> > dataList;
    std::vector<char*> lineList;
    parserFileData(fileData, lineList, dataList);

    std::vector<std::vector<int> > first_bf_ansIdx;
    std::vector<std::vector<Bitarray*> > first_bf_readBit;
    // ListData -> PairData
    for(auto &dataLine: dataList){
        std::vector<int> t_ans;
        std::vector<Bitarray*> t_readBit;
        checkRead(dataLine, t_ans, t_readBit);
        first_bf_ansIdx.push_back(t_ans);
        first_bf_readBit.push_back(t_readBit);
    }
    //
    std::vector<uint64_t> reads_snp;
    for(int i = 0; i < first_bf_readBit.size(); i ++){
        reads_snp.push_back(checkSnp(first_bf_ansIdx[i], first_bf_readBit[i], lineList, i));
    }
    print(reads_snp);
}

void testMurmurHash3(){
    char x[33] = "GTCCACGAAGTTTGAGTGCTACATCTGAGGA";
    const uint64_t mod = 47925291;
    auto ret = getModOfHash(x, seeds, mod);
    for(uint64_t v:ret)printf("%d-", v);
    printf("\n");
}

void testBitArray1(){
    Bitarray *a = new Bitarray(46);
    a->set(3, 1);
    a->set(4, 1);
    a->set(8, 1);
    Bitarray *b = new Bitarray(46);
    b->set(3, 1);
    b->set(5, 1);
    b->set(8, 1);
    std::vector<Bitarray*> vb;
    vb.push_back(a);
    vb.push_back(b);
    Bitarray* result_and = getAnd(vb);
    auto ret_search = result_and->search();
    for(uint64_t idx:ret_search){
        printf("%llu-", idx);
    }
    printf("\n");
}
void testBitArray2(){
    uint8_t *xx = new uint8_t[12];
    for(int i = 0; i < 12; i ++){
        xx[i] = i;
    }
    Bitarray* x = new Bitarray(xx, 12);
    auto x_search = x->search();

    for(uint64_t idx:x_search){
        printf("%llu-", idx);
    }
    printf("\n");
}

void testRedis(){
    redis::RedisMgr* r = redis::instance("127.0.0.1", 6379);
    char name[5] = "name";
    char andy[15] = "hello,Andy";
    uint8_t *andy8 = new uint8_t[sizeof(andy)];
    memcpy(andy8, andy, sizeof(andy));
    r->set(name, andy8);
    uint8_t* res;
    int len = r->get(name, res);
    printf("size of res is %d", len);
    char andy2[15];
    memcpy(andy2, res, len);
    printf("%s\n", andy2);
    delete r;
}


void testReadFile(){
    char x[15] = "testReadFile";
    std::vector<char*> ret;
    split(x, 'e', ret);
    for(int i = 0; i < ret.size(); i ++){
        print(ret[i]);
    }
}



int main(int argc, char** argv){
//    testMurmurHash3();
//    testBitArray1();
//    testBitArray2();
//    testRedis();
//    testReadFile();
//
//    char *k=nullptr;
//    strcat2(&k, int2str(1223), int2str(3421), nullptr);
//    printf("%s", k);
    doCheck("../data/reads40_45");
//    doCheck("/Users/acmood/Documents/querySnp/data/reads40_45");
    return 0;
}
