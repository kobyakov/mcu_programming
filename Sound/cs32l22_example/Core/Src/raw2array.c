#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

const size_t maxSize = 500000;



int main(int argc, char** argv)
{
    if (argc != 2)
    {
        perror("usage: raw2array <raw_file>\n");
        exit(EXIT_FAILURE);
    }

    const char* rawAudioFileName  = argv[1];
    const char* extension = ".c";

    const size_t cArrayFileNameLen = strlen(rawAudioFileName)+strlen(extension)+1;
    char* cArrayFileName = (char *)malloc(cArrayFileNameLen);
    snprintf(cArrayFileName, cArrayFileNameLen, "%s%s",rawAudioFileName, extension);

    FILE *rawAudioFile;
    FILE *cArrayFile;

    if ((rawAudioFile = fopen(rawAudioFileName, "rb")) == NULL)
    {
        perror("can't open raw audio file\n");
        exit(EXIT_FAILURE);
    }

    if ((cArrayFile = fopen(cArrayFileName, "w")) == NULL)
    {
        perror("can't open c array file\n");
        exit(EXIT_FAILURE);
    }
    free(cArrayFileName);

    fseek(rawAudioFile, 0, SEEK_END);
    const size_t fileLen = ftell(rawAudioFile);
    rewind(rawAudioFile);

    const size_t bytesToRead = maxSize < fileLen ? maxSize : fileLen;
    printf("bytes to read: %lu\n", bytesToRead);
    /*
    uint16_t chunk;
    for (size_t i = 0; i < bytesToRead; i=i+2)
    {
        fread(&chunk, 2, 1, rawAudioFile);
        fprintf(cArrayFile, "0x%04x, ", chunk);
    }
    */
    uint8_t chunk;
    for (size_t i = 0; i < bytesToRead; i++)
    {
        fread(&chunk, 1, 1, rawAudioFile);
        fprintf(cArrayFile, "0x%02x, ", chunk);
    }

    fclose(rawAudioFile);
    fclose(cArrayFile);

    exit(EXIT_SUCCESS);
}
