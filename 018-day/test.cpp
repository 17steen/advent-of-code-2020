#include <stdio.h>
int main()
{
    char buffer[255] = {};
    fgets(buffer, 255, stdin);

    fputs(buffer, stdout);
    fflush(stdout);
    return 0;
}