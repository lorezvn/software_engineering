#ifndef con2redis_h
#define con2redis_h

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

extern "C" {
#include <hiredis/hiredis.h>
}


#define dbg_log(fmt, ... )			\
    do { \
        fprintf(stderr, "%s:%d : " fmt "\n", __FILE__, __LINE__,__VA_ARGS__); \
    } while (0);


#define dbg_abort(fmt, ...) \
    do { \
        dbg_log(fmt, __VA_ARGS__); exit(-1); \
    } while (0)


#define RedisCommand(fmt, ...)			\
  (redisReply*) redisCommand(fmt, __VA_ARGS__)


// Verifica del tipo di risposta Redis
void assertReplyType(redisContext *c, redisReply *r, int type);

// Verifica della validità della risposta Redis
void assertReply(redisContext *c, redisReply *r);

// Stampa del contenuto di una risposta Redis
void dumpReply(redisReply *r, int indent);

// Inizializzazione di uno stream Redis
void initStreams(redisContext *c, const char *stream);

void ReadStreamReply(redisReply *r, int indent, int readstatus);

// Lettura del numero di stream presenti nella risposta Redis
int ReadNumStreams(redisReply *r);

// Lettura del nome di uno stream da una risposta Redis
int ReadStreamName(redisReply *r, char *streamname, long unsigned int k);

// Lettura del numero di messaggi in uno stream
int ReadStreamNumMsg(redisReply *r, long unsigned int streamnum);

// Lettura dell'ID di un messaggio in uno stream
int ReadStreamNumMsgID(redisReply *r, long unsigned int streamnum, int msgnum, char *msgid);

// Lettura del numero di valori in un messaggio di uno stream
int ReadStreamMsgNumVal(redisReply *r, long unsigned int streamnum, int msgnum);

// Lettura di un valore specifico in un messaggio di uno stream
int ReadStreamMsgVal(redisReply *r, long unsigned int streamnum, int msgnum, int entry, char *fval);

// Stampa i tipi di risposta Redis
void print_reply_types();

#endif
