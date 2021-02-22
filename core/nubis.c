#pragma once
#ifndef NUBIS_H
 #include "nubis.h"
#endif
#ifndef NUBIS_C
int sread(void *buf, int elementSize, int elementsCount, void *stream, int streamSize){
    if(NUBIS_PTR.stream != buf){
        NUBIS_PTR.stream = buf;
        NUBIS_PTR.pos = 0;
    }
    char *_buf = (char*)buf;
    char *_stream = (char*)stream;
    int i = 0;
    for(; i < (elementsCount*elementSize) && NUBIS_PTR.pos < streamSize; i++){
        _buf[i] = _stream[NUBIS_PTR.pos]; 
        NUBIS_PTR.pos++;
    }
    return i;
}
int swrite(void *buf, int elementSize, int elementsCount, char *stream){
    //not ready yet
}
int sseek(void *stream, int offset, int origin, int streamSize){
    if(NUBIS_PTR.stream != stream){
        NUBIS_PTR.stream = stream;
        NUBIS_PTR.pos = 0;
    }
    if(origin == 1)
        NUBIS_PTR.pos+=offset;
    else if(origin == 0){
        NUBIS_PTR.pos = offset;
    }else if(origin == 3)
        NUBIS_PTR.pos = streamSize+offset;
    if(NUBIS_PTR.pos < 0) return -1;
    else return 0;
}

struct NCStream makeStream(char *stream, int _len){
    struct NCStream this;
    this.stream = stream;
    this.STREAM_MAX = _len;
    this.pos = 0;
    if(_len == -1){
        //Dunamic!
    }else{
        
    }
    return this;
}

int csread(void *buf, int elementSize, int elementsCount, struct NCStream stream){
    char *_buf = (char*)buf;
    char *_stream = (char*)stream.stream;
    int i = 0;
    for(; i < (elementsCount*elementSize) && stream.pos < stream.STREAM_MAX; i++){
        _buf[i] = _stream[(stream.pos)]; 
        stream.pos++;
    }
    return i;
}
int cswrite(void *buf, int elementSize, int elementsCount, struct NCStream stream){
    
}
int csseek(struct NCStream stream, int offset, int origin){
    if(origin == 1)
        stream.pos+=offset;
    else if(origin == 0){
        stream.pos = offset;
    }else if(origin == 3)
        stream.pos = stream.STREAM_MAX+offset;
    if(stream.pos < 0 || stream.pos >= stream.STREAM_MAX) return -1;
    else return 0;
}
char *strvp(const char *str){
    char *buf = (char*)malloc(strlen(str));
    strcpy(buf, str);
    return buf;
}
#endif