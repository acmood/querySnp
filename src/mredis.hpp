//
//  mredis.hpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#ifndef mredis_hpp
#define mredis_hpp

#include <stdio.h>
#include "hiredis.h"


struct RedisMgr{
    RedisMgr();
    ~RedisMgr();
    bool connect(const char* host, int port);
    void set(const char *key, const uint8_t* value);
    
    uint8_t* get (const char *key);

private:
    redisContext* _connect;
    redisReply* _reply;

    
};
#endif /* mredis_hpp */
