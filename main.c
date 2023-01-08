#include <stdio.h>
#include <stdlib.h>
#include "wav_format.h"

int main(int argc, char **argv)
{
	Wav *wav_file_struct = NULL;
	int load_status;
	
	wav_file_struct = create_wav();
	load_status = 0;
	
	if (wav_file_struct == NULL) {
		fprintf(stderr, "Malloc error, can't create wav structure !\n");
		exit(1);
	}

	load_status = load_wav_file("file2.wav", wav_file_struct);
	if (load_status == 1) {
		fprintf(stderr, "Error opening file file.wav\n");
		exit(1);
	}
	else if (load_status == 2) {
		fprintf(stderr, "Header file is corrupted !\n");
		exit(1);
	}

	print_header(wav_file_struct);
	Destroy_wav(wav_file_struct);
}
