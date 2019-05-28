/* SDSLib, A C dynamic strings library
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __SDS_H
#define __SDS_H

// 分配最大的内存空间是1M
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

typedef char *sds;

struct sdshdr {
    unsigned int len;   // 字符串的长度
    unsigned int free;  // 申请空间中剩余的长度
    char buf[];         // 字符串的存储位置
};

/*
 * sds 是char *的别名
 * 内联函数，用于返回字符串的长度，不包括剩余空间长度
 * TODO: 这块有点迷惑
 */
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

/*
 * 返回字符串中剩余空间
 */
static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);    // 根据给定长度，创建一个sds
sds sdsnew(const char *init);   // 根据给定的字符串，创建一个sds
sds sdsempty(void); // 创建一个空sds
size_t sdslen(const sds s); // 返回字符串的长度
sds sdsdup(const sds s);    // 存储字符串
void sdsfree(sds s);    // 删除字符串
size_t sdsavail(const sds s);   // 返回剩余空间
sds sdsgrowzero(sds s, size_t len); // 用空字符扩展到指定长度
// 字符串拼接
sds sdscatlen(sds s, const void *t, size_t len);
sds sdscat(sds s, const char *t);
sds sdscatsds(sds s, const sds t);
// 字符串拷贝相关操作
sds sdscpylen(sds s, const char *t, size_t len);
sds sdscpy(sds s, const char *t);

sds sdscatvprintf(sds s, const char *fmt, va_list ap);
#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds sdscatprintf(sds s, const char *fmt, ...);
#endif

sds sdscatfmt(sds s, char const *fmt, ...); // 
sds sdstrim(sds s, const char *cset);   // 去掉首尾字符
void sdsrange(sds s, int start, int end);   // 
void sdsupdatelen(sds s);   // 更新字符串
void sdsclear(sds s);   // 清空字符串
int sdscmp(const sds s1, const sds s2); // 比较字符串
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);  // 切分字符串
void sdsfreesplitres(sds *tokens, int count);   
void sdstolower(sds s); // 字符串转小写
void sdstoupper(sds s); // 字符串转大写
sds sdsfromlonglong(long long value);   // 将长整形转化成字符串
sds sdscatrepr(sds s, const char *p, size_t len);   
sds *sdssplitargs(const char *line, int *argc);
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);
sds sdsjoin(char **argv, int argc, char *sep);  // 合并字符串

/* Low level functions exposed to the user API */
sds sdsMakeRoomFor(sds s, size_t addlen);
void sdsIncrLen(sds s, int incr);   // 
sds sdsRemoveFreeSpace(sds s);  // 释放空间
size_t sdsAllocSize(sds s);

#endif
