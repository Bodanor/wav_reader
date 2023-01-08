#include "wav_format.h"

/* 
Positions   Sample Value         Description
1 - 4       "RIFF"               Marks the file as a riff file. Characters are each 1. byte long.
5 - 8       File size (integer)  Size of the overall file - 8 bytes, in bytes (32-bit integer). Typically, you'd fill this in after creation.
9 -12       "WAVE"               File Type Header. For our purposes, it always equals "WAVE".
13-16       "fmt "               Format chunk marker. Includes trailing null
17-20       16                   Length of format data as listed above
21-22       1                    Type of format (1 is PCM) - 2 byte integer
23-24       2                    Number of Channels - 2 byte integer
25-28       44100                Sample Rate - 32 bit integer. Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz.
29-32       176400               (Sample Rate * BitsPerSample * Channels) / 8.
33-34       4                    (BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo
35-36       16                   Bits per sample
37-40       "data"               "data" chunk header. Marks the beginning of the data section.
41-44       File size (data)     Size of the data section, i.e. file size - 44 bytes header.
*/

/* We use packed so that the compiler doesn't optimize the structure as we absolutely want it ti be 44 bytes long + 8 bytes for the data pointer */
struct __attribute__((__packed__)) WAV_t
{
	u_int8_t chunkID[4];
	u_int32_t chunkSize;
	u_int8_t Format[4];
	u_int8_t subChunk1_ID[4];
	u_int32_t subchunk1_lenght;
	u_int16_t audio_format;
	u_int16_t channels_numbers;
	u_int32_t sample_rate;
	u_int32_t bytes_rate;
	u_int16_t block_align;
	u_int16_t bits_per_sample;
	u_int8_t subchunk2_ID[4];
	u_int32_t subChunk2_lenght;
	u_int8_t *data; // Its going to be allocated when we load the actual data
};

static u_int8_t load_header(FILE *file_dst, Wav *src);
static u_int8_t load_data_section(FILE *file_dst, Wav *src);

Wav *create_wav(void)
{
	Wav *tmp = (Wav*)malloc(sizeof(Wav));
	if (tmp == NULL)
		return NULL;

	tmp->data = NULL;
	return tmp;
}

void Destroy_wav(Wav *src)
{
	if (src != NULL) {
		if (src->data != NULL)
			free (src->data);
		free (src);
	}

}

u_int8_t load_header(FILE *file_dst, Wav *src)
{
	
	if (fread(src, HEADER_SIZE, 1, file_dst) != 1)
		return 1;

	return 0;
}

void print_header(Wav *src)
{
	u_int8_t i;
	
	printf("ChunkID : ");
	for (i = 0; i < sizeof(src->chunkID); i++)
		putchar(src->chunkID[i]);

	putchar('\n');
	printf("File Size : %u\n", src->chunkSize);

	printf("File Type Header : ");
	for (i = 0; i < sizeof(src->Format); i++)
		putchar(src->Format[i]);
	putchar('\n');

	printf("Format chunk format : ");
	for (i = 0; i < sizeof(src->subChunk1_ID); i++)
		putchar(src->subChunk1_ID[i]);
	putchar('\n');

	printf("Format chunk size : %u\n", src->subchunk1_lenght);
	printf("Type of format : %hu ", src->audio_format);
	if (src->audio_format == 1)
		printf("PCM uncompressed\n");
	else
		printf("Compressed\n");
	printf("Numbers of channels : %hu\n", src->channels_numbers);
	printf("Sample Rate : %u\n", src->sample_rate);
	printf("Bytes rate : %u\n", src->bytes_rate);
	printf("Number of bytes for one sample : %hu\n", src->block_align);
	printf("Bits per sample : %hu\n", src->bits_per_sample);

	printf("SubChunk2 : ");
	for (i = 0; i < sizeof(src->subChunk1_ID); i++)
		putchar(src->subchunk2_ID[i]);
	putchar('\n');

	printf("Bytes in data section : %u\n", src->subChunk2_lenght);
}

u_int8_t load_wav_file(const char * filename, Wav *src)
{

	assert(filename != NULL && src != NULL);

	FILE *f_ptr;
	int ret_satus;

	ret_satus = 0;
	
	f_ptr = fopen(filename, "rb");
	if (f_ptr == NULL)
		return 1;

	ret_satus = load_header(f_ptr, src);
	if (ret_satus == 1)
		return 2; /* Header is corrupted or could not be load */

	src->data = (u_int8_t*)malloc(sizeof(u_int8_t)*src->subChunk2_lenght);
	if (src->data == NULL)
		return 3;

	ret_satus = load_data_section(f_ptr, src);
	if (ret_satus == 1)
		return 4; /* Data section is corrupted */
	
	
	fclose(f_ptr);
	return 0;
	
}

u_int8_t load_data_section(FILE *f_ptr, Wav *src)
{
	if (fread(src->data, src->subChunk2_lenght, 1, f_ptr) != 1)
		return 1;
	
	return 0;

}
