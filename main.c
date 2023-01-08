#include <stdio.h>
#include <stdlib.h>
#include "wav_format.h"

int main(int argc, char **argv)
{
	Wav *wav_file_struct = NULL;
	int header_status;
	
	wav_file_struct = create_wav();
	header_status = 0;
	
	if (wav_file_struct == NULL) {
		fprintf(stderr, "Malloc error, can't create wav structure !\n");
		exit(1);
	}

	header_status = load_header("file.wav", wav_file_struct);
	if (header_status == 1) {
		fprintf(stderr, "Error opening file file.wav\n");
		exit(1);
	}
	else if (header_status == 2) {
		fprintf(stderr, "Header file is corrupted !\n");
		exit(1);
	}
	Destroy_wav(wav_file_struct);
}
