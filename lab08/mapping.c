#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char* input_name =  argv[1];
    const char* output_name = argv[2];
    int input = open(input_name, O_RDONLY, 0600);
    int output = open(output_name, O_CREAT | O_RDWR, 0600);
    if (input < 0 || output < 0)
    {
        perror("error of opening map file");
        return -1;
    }
    struct stat* t_stat;
    fstat(input, t_stat);
    int datasize = t_stat->st_size;
    char* mapdata = (char *)mmap(NULL, datasize, PROT_READ, MAP_SHARED, input, 0);
    if (MAP_FAILED == mapdata)
    {
        perror("error with mmap");
        return -2;
    }
    lseek(output, 0, SEEK_SET);
    write(output, mapdata, datasize);
    ftruncate(output, datasize);
    close(input);
    close(output);
}