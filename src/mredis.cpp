//
//  mredis.cpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#include "mredis.hpp"
#include "hiredis.h"
#include <string>
#include <string.h>
namespace redis{
RedisMgr::RedisMgr(){}


uint64_t RedisMgr::get(const char *key, uint8_t* &ret)
{
    this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key);
    
    if (NULL == this->_reply->str)
    {
        freeReplyObject(this->_reply);
        return 0;
    }
    std::string str = this->_reply->str;
    freeReplyObject(this->_reply);
    uint64_t lenstr = str.size();
    char *cstr = new char[lenstr+1];
    strcpy(cstr, str.c_str());
    cstr[lenstr] = '\0';
    ret = new uint8_t[lenstr];
    memcpy(ret, cstr, lenstr);
    return lenstr;
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
        redisMgr = new RedisMgr();
        if(!redisMgr->connect(ip, port))
        {
            printf("connect error!\n");
            return 0;
        }
        if (dbIndex != -1){
            redisMgr->select(dbIndex);
        }
    }
    return redisMgr;
}
}
