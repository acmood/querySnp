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

#define PVV std::pair<std::vector<int>, std::vector< std::vector<Bitarray > > >

std::vector< std::vector<char*> > parserFileData(char *fileData){
    std::vector< std::vector<char*> > ret;
    return ret;
}

PVV checkRead(std::vector<char*> dataLine){
    std::vector<int> ansIdx;
    std::vector<std::vector<Bitarray> > readBit;
    return make_pair(ansIdx, readBit);
}

void checkSnp(std::vector<PVV> &first_bf_ans){
    
}

void doCheck(char *filepath){
    char* fileData = readData(filepath);
    // fileData -> ListData
    auto dataList = parserFileData(fileData);
    std::vector<PVV> first_bf_ans;
    // ListData -> PairData
    for(auto &dataLine: dataList){
        auto tmp = checkRead(dataLine);
        first_bf_ans.push_back(tmp);
    }
    //
    checkSnp(first_bf_ans);
}

void testMurmurHash3(){
    char x[33] = "GTCCACGAAGTTTGAGTGCTACATCTGAGGA";
    std::vector<int> seeds{0,1,2,3,4,5,6};
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
    doCheck("");
    return 0;
}
