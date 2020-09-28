//
//  utils.cpp
//  data_process_c__
//
//  Created by acmood on 2020/9/27.
//

#include "utils.hpp"
#include <string.h>

char* readData(const char* filepath){
    char* data;
    return data;
}

mmh3 getModOfHash(const char* s, const std::vector<int> &seeds, const uint64_t mod){
    int len = strlen(s);
    printf("len = %d\n", len);
    mmh3 ret;
    uint8_t *out = new uint8_t[16];
    for (int seed : seeds){
        uint64_t res = 0;
        MurmurHash3_x64_128(s, len, seed, out);
        for (int j = 15; j >= 0; j --){
            res = (((1ll * res) << 8) + out[j])%mod;
        }
        ret.push_back(res);
    }
    delete [] out;
    return ret;
}

char* int2str(int x){
    std::vector<int> vret;
    while(x){
        vret.push_back(x%10);
        x /= 10;
    }
    char *ret = new char[vret.size()+1];
    for (int i = 0; i < vret.size(); i ++){
        ret[vret.size()-i-1] = '0'+(vret[i]-0);
    }
    ret[vret.size()] = '\0';
    return ret;
}

template <typename T>
T max(T a, T b){
    return a>b?a:b;
}

uint32_t maxInList(uint32_t *count, int len){
    if (!count){
        return 0;
        printf ("ERROR maxInList count is null");
    }
    uint32_t mx = count[0];
    for(int i = 0; i < len; i ++){
        mx = max(mx, count[i]);
    }
    return mx;
}

////多字符串连接函数
size_t strcat2(char **dst_out, ...)
{
    size_t len = 0, len_sub;
    va_list argp;
    char *src;
    char *dst = NULL, *dst_p;
    *dst_out = NULL;
    va_start(argp, dst_out);
    for (;;)
    {
        if ((src = va_arg(argp, char *)) == NULL) break;
        len += strlen(src);
    }
    va_end(argp);
    if (len == 0) return 0;
    dst = (char *)malloc(sizeof(char) * (len + 1));
    if (dst == NULL) return -1;
    dst_p = dst;
    va_start(argp, dst_out);
    for (;;)
    {
        if ((src = va_arg(argp, char *)) == NULL) break;
        len_sub = strlen(src);
        memcpy(dst_p, src, len_sub);
        dst_p += len_sub;
    }
    va_end(argp);
    *dst_p = '\0';
    *dst_out = dst;
    return len;
}

////读取基因序列
char* get_read_string(char *file_name)
{
    FILE *fp;
    char *buffer;
    fp = fopen(file_name,"r");
    fseek(fp, 0, SEEK_END);
    int64_t length = ftell(fp);
    buffer = (char *)malloc((length + 1) * sizeof(char));
    rewind(fp);
    length = fread(buffer,sizeof(char),length,fp);
    return buffer;
}
