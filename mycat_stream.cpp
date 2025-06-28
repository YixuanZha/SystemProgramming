#include <stdio.h>
#include <fcntl.h>

#define BUFSIZE 4096

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage: %s <filename>\n",argv[0]);
        return 1;
    }

    char* filename = argv[1];

    FILE* open_file = fopen(filename,"r");

    unsigned char buffer[BUFSIZE];
    size_t blocks_read;

    while((blocks_read = fread(buffer,1,BUFSIZE,open_file)) > 0)
    {
        if(fwrite(buffer,1,blocks_read,stdout) != blocks_read)
        {
            fprintf(stderr,"Error wrting to stdout\n");
            fclose(open_file);
            return 1;
        }
    }
    if(ferror(open_file))
    {
        fprintf(stderr,"Error reading from file\n");
    }

    fclose(open_file);
    return 0;
}