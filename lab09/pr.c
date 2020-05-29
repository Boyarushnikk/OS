#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    const char* name = argv[1];
    int byte = atoi(argv[2]);

    FILE* file = fopen(name, "r");
    if (file == NULL)
    {
        perror("cant open file");
        return -1;
    }
    int data[byte];
    int res = fread(data, sizeof(int), byte, file);

    printf("bytes read:  %i\n", res);
    for (size_t i = 0; i < res; ++i)
    {
        printf("random num: %i (0x%x)\n", data[i], data[i]);
    }
    fclose(file);
    return 0;
}