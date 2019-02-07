#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 64
#endif

void ErrorExit(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    int fdInput, fdOutput;
    char buffer[BUFFER_SIZE];
    mode_t filePermission;

    if (argc != 3)
    {
        ErrorExit("Wrong number of arguments\n");
    }

    fdInput = open(argv[1], O_RDONLY);
    if (fdInput == -1)
    {
        ErrorExit("Fail to open file %s\n", argv[1]);
    }


    filePermission = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    fdOutput = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, filePermission);
    if (fdOutput == -1)
    {
        ErrorExit("Fail to open file %s\n", argv[2]);
    }

    int numRead = 0;
    while (numRead = read(fdInput, buffer, BUFFER_SIZE))
    {
        if (numRead == -1)
        {
            ErrorExit("Fail to read\n");
        }

        int numWrite = write(fdOutput, buffer, numRead);
        if (numWrite != numRead)
        {
            ErrorExit("Fail to write\n");
        }
    }
    
    if (close(fdInput) == -1)
    {
        ErrorExit("Fail to close %s", argv[1]);
    }
    if (close(fdOutput) == -1)
    {
        ErrorExit("Fail to close %s", argv[2]);
    }

    exit(EXIT_SUCCESS);
}