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

#define M 46
std::vector<int> seeds{0,1,2,3,4,5,6};
#define SIZE1 47925291
#define SIZE2 47925291
#define KERM_CNT 120
#define HASH_CNT 10

std::vector< std::vector<char*> > parserFileData(char *fileData){
    std::vector< std::vector<char*> > ret;
    return ret;
}


Bitarray get_kmer_bitarray_col(uint64_t col, std::vector<Bitarray> kmerHash){
    /* """得到BF的一列""" */
    std::vector<int> lst;
    for (auto kmer : kmerHash){
        lst.push_back(kmer.get(col));
    }
    Bitarray ret = Bitarray(kmerHash.size());
    for (int i = 0; i < kmerHash.size(); i ++)
        ret.set(i, lst[i]);
    return ret;
}

void checkRead(std::vector<char*> &dataLine, std::vector<int> &ansIdx, std::vector<Bitarray> readBit){
    uint32_t *count = new uint32_t[M];
    memset(count, 0, sizeof(int)*M);
    std::vector<Bitarray> kmerHash;
    int kmerId = 0;
    std::vector<uint8_t*> getBytes;
    for(auto kmer:dataLine){
        auto kid = getModOfHash(kmer, seeds, SIZE1);
        bool flag = false;
        int colCnt = 0;
        for (auto col:kid){
            char *pref=NULL;
            strcat2(&pref, 'bacteria', '_I', int2str(col), '_',  int2str(k1), 'kmer', NULL);
            if(kmerId == KERM_CNT-1 and colCnt == HASH_CNT-1){
                flag = true;
            }
            colCnt += 1;
            auto byte = RedisMgr().get(pref);
            getBytes.push_back(byte);
        }
        kmerId += 1;
    }
    std::vector<Bitarray> bitarrayList;
    for(auto x:getBytes)
        bitarrayList.push_back(Bitarray(x, sizeof(x)));
    int st = 0;
    for(int i = 0; i < dataLine.size(); i ++){
        int ed = (i+1) * HASH_CNT;
    //         与运算确定kmer在哪些bf中存在
        Bitarray k_bit = getAnd(bitarrayList, st, ed);
        kmerHash.push_back(k_bit);
    //         bf_id：kmer存在的bf id
        auto bf_id = k_bit.search();
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
    //         ans:存在率最高的bf id, read_bit：由第一层bf检测后的01序列
}

void checkSnp(std::vector<std::vector<int> > &first_bf_ansIdx, std::vector<std::vector<Bitarray> >first_bf_ans_readBit){
    
}

void doCheck(char *filepath){
    char* fileData = readData(filepath);
    // fileData -> ListData
    auto dataList = parserFileData(fileData);
    std::vector<std::vector<int> > first_bf_ansIdx;
    std::vector<std::vector<Bitarray> > first_bf_readBit;
    // ListData -> PairData
    for(auto &dataLine: dataList){
        std::vector<int> t_ans;
        std::vector<Bitarray> t_readBit;
        checkRead(dataLine, t_ans, t_readBit);
        first_bf_ansIdx.push_back(t_ans);
        first_bf_readBit.push_back(t_readBit);
    }
    //
    checkSnp(first_bf_ansIdx, first_bf_readBit);
}

void testMurmurHash3(){
    char x[33] = "GTCCACGAAGTTTGAGTGCTACATCTGAGGA";
    const uint64_t mod = 47925291;
    auto ret = getModOfHash(x, seeds, mod);
    for(uint64_t v:ret)printf("%d-", v);
    printf("\n");
}

void testBitArray1(){
    Bitarray a = Bitarray(46);
    a.set(3, 1);
    a.set(4, 1);
    a.set(8, 1);
    Bitarray b = Bitarray(46);
    b.set(3, 1);
    b.set(5, 1);
    b.set(8, 1);
    std::vector<Bitarray> vb;
    vb.push_back(a);
    vb.push_back(b);
    Bitarray result_and = getAnd(vb);
    auto ret_search = result_and.search();
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
    Bitarray x = Bitarray(xx, 12);
    auto x_search = x.search();
    
    for(uint64_t idx:x_search){
        printf("%llu-", idx);
    }
    printf("\n");
}

void testRedis(){
    RedisMgr *r = new RedisMgr();
    if(!r->connect("127.0.0.1", 6379))
    {
        printf("connect error!\n");
        return 0;
    }
    char name[5] = "name";
    char andy[5] = "Andy";
    uint8_t *andy8 = new uint8_t[sizeof(andy)];
    memcpy(andy8, andy, sizeof(andy));
    r->set(name, andy8);
    auto res = r->get(name);
    memcpy(andy, res, sizeof(res));
    printf("%s\n", andy);
    delete r;
}

int main(int argc, char** argv){
    testMurmurHash3();
    testBitArray1();
    testBitArray2();
    testRedis();
    //char *k=NULL;
    //strcat2(&k, int2str(1223), int2str(3421), NULL);
    //printf("%s", k);
    //doCheck("");
    return 0;
}
