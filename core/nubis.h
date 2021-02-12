#pragma once
#ifndef NUBIS_H
struct NCStream{
    void *stream;
    int pos, STREAM_MAX;
}NUBIS_PTR;
#endif
extern int sread(void *buf, int elementSize, int elementsCount, void *stream, int streamSize);
extern int swrite(void *buf, int elementSize, int elementsCount, char *stream);
extern int sseek(void *stream, int offset, int origin, int streamSize);

extern struct NCStream makeStream(char *stream, int _len);

extern int csread(void *buf, int elementSize, int elementsCount, struct NCStream stream);
extern int cswrite(void *buf, int elementSize, int elementsCount, struct NCStream stream);
extern int csseek(struct NCStream stream, int offset, int origin);

#ifndef NUBIS_C
 #include "./nubis.c"
#endif