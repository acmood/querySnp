//
//  mredis.hpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#ifndef mredis_hpp
#define mredis_hpp

#include <stdio.h>
#include <vector>
#include "hiredis.h"

namespace redis{
struct RedisMgr{
    RedisMgr();
    ~RedisMgr();
    bool connect(const char* host, int port);
    bool disconnect();
    void set(const char *key, const uint8_t* value);
    bool select(int index);
    uint32_t getPipeLineWithPrefAndCols(const char* pref, const std::vector<uint64_t> &cols, std::vector<uint8_t*> &out);
    uint64_t get (const char *key, uint8_t* &ret);

private:
    redisContext* _connect;
    redisReply* _reply;

    
};
RedisMgr* instance(const char* host=nullptr, int port=6379, int dbIndex=-1);


}
#endif /* mredis_hpp */
