//
//  mredis.cpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#include "mredis.hpp"
#include "hiredis.h"
#include <string>

namespace redis{
RedisMgr::RedisMgr(){
    
}


uint8_t* RedisMgr::get(const char *key)
{
    this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key);
    std::string str = this->_reply->str;
    freeReplyObject(this->_reply);
    const char *cstr = str.c_str();
    uint8_t *ret = new uint8_t[sizeof(cstr)];
    memcpy(ret, cstr, sizeof(cstr));
    return ret;
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


RedisMgr* instance(const char* ip, int port){
    if(redisMgr == nullptr){
        redisMgr = new RedisMgr();
        if(!redisMgr->connect(ip, port))
        {
            printf("connect error!\n");
            return 0;
        }
    }
    return redisMgr;
}
}
