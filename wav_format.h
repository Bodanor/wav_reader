#ifndef __WAV_FORMAT_H__
#define __WAV_FORMAT_H__

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>


#define HEADER_SIZE 44

typedef struct WAV_t Wav;

Wav *create_wav();
void Destroy_wav(Wav *);

void print_header(Wav *);
u_int8_t load_wav_file(const char *, Wav *);


#endif
