//
//  mredis.cpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#include "utils.hpp"
#include "mredis.hpp"
#include "hiredis.h"
#include <string>
#include <string.h>
#include <algorithm>
namespace redis{
RedisMgr::RedisMgr(){}


uint64_t RedisMgr::get(const char *key, uint8_t* &ret)
{
    this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key);
    
    if (NULL == this->_reply->str)
    {
        printf("Redis key not find %s\n", key);
        freeReplyObject(this->_reply);
        return 0;
    }
    std::string str = this->_reply->str;
    uint64_t lenstr = this->_reply->len;
    freeReplyObject(this->_reply);
    char *cstr = nullptr;
    string2uint8(str, lenstr, ret);
    return lenstr;
}

uint32_t RedisMgr::getPipeLineWithPrefAndCols(const char* pref, const std::vector<uint64_t> &cols, std::vector<uint8_t*> &out){
    int lenpref = 0;
    for(;pref[lenpref]; lenpref ++);
    lenpref += 4;//"get pref%d"
    char *cmd=nullptr;
    int lastLenCol = 0;
    uint32_t outLen = 0;
    for (auto col:cols){
        char *strCol = int2str(col);
        int lenCol = 0;
        for (;strCol[lenCol]; lenCol ++);
        if (lenCol != lastLenCol){
            if (cmd){
                delete [] cmd;
                cmd = nullptr;
            }
            cmd = new char[lenpref+lenCol+1];
            strcpy(cmd, "get ");
            strcpy(cmd+4, pref);
            strcpy(cmd+lenpref, strCol);
        }
        redisAppendCommand(this->_connect, cmd);
    }
    for (uint64_t i = 0; i < cols.size(); i++) {
        int type = -1;
        redisReply *reply = 0;
        int replyStatus = redisGetReply(_connect, (void **) &reply);
        if (replyStatus == REDIS_OK && reply != NULL) {
            type = reply->type;
            if (reply->str != NULL) {
                uint8_t* ret = nullptr;
                string2uint8(reply->str, reply->len, ret);
                outLen = std::max(outLen, (uint32_t)reply->len);
                out.push_back(ret);
            }
        } else {
            printf("pipeline_process error i :%llu cmd : %llu", i, cols[i]);
        }
        freeReplyObject(reply);
    }
    return outLen;
}


bool RedisMgr::select(int index){
    this->_reply = (redisReply*)redisCommand(this->_connect, "SELECT %d", index);
    return true;
}


void RedisMgr::set(const char *key, const uint8_t* value)
{
    redisCommand(this->_connect, "SET %s %s", key, value);
}


RedisMgr::~RedisMgr()
{
    redisFree(this->_connect);
    this->_connect = nullptr;
    this->_reply = nullptr;
}


bool RedisMgr::disconnect(){
    redisFree(this->_connect);
    this->_connect = nullptr;
    this->_reply = nullptr;
}


bool RedisMgr::connect(const char* host, int port)
{
    this->_connect = redisConnect(host, port);
    if(this->_connect != nullptr && this->_connect->err)
    {
        printf("connect error: %s\n", this->_connect->errstr);
        return 0;
    }
    return 1;
}


RedisMgr *redisMgr;
RedisMgr* instance(const char* ip, int port, int dbIndex){
    if(redisMgr == nullptr){
	    printf("ip, port, db %s, %d, %d", ip, port, dbIndex);
        redisMgr = new RedisMgr();
        if(!redisMgr->connect(ip, port))
        {
            printf("connect error!\n");
            return 0;
        }
        if (dbIndex != -1){
            redisMgr->select(dbIndex);
	    printf("db is %d", dbIndex);
        }
    }
    return redisMgr;
}
}
