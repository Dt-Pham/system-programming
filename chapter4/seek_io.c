#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "tlpi_hdr.h"

int main(int argc, char **argv)
{
    int fd; // file descriptor
    int ap; // argument pointer
    int i;  // loop counter
    int numRead, numWritten;
    int offset;
    int cur;
    size_t len;

    char *buffer;

    if (argc < 3 || strcmp(argv[0], "--help") == 0)
    {
        usageErr("%s filename {r<bytes> | R<bytes> | s<offset> | w<string>} ", argv[0]);
    }

    mode_t filePerm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    fd = open(argv[1], O_CREAT | O_RDWR, filePerm);

    for (ap = 2; ap < argc; ap++)
    {
        switch (argv[ap][0])
        {
            case 'r':
            case 'R':
                len = getLong(argv[ap] + 1, GN_ANY_BASE, argv[ap]);
                buffer = malloc(len);

                if (buffer == NULL)
                {
                    errExit("malloc");
                }

                numRead = read(fd, buffer, len);
                if (numRead == -1)
                {
                    errExit("read");
                }

                printf("%s: ", argv[ap]);
                for (i = 0; i < numRead; i++)
                {
                    if (argv[ap][0] == 'r')
                    {
                        if (isprint(buffer[i]))
                            printf(" %c", buffer[i]);
                        else
                            printf(" ?");
                    }
                    else
                    {
                        printf(" %02x", buffer[i]);
                    }
                }
                printf("\n");
                free(buffer);
                break;
            case 'w': 
                len = strlen(argv[ap] + 1);
                numWritten = write(fd, argv[ap] + 1, len);

                if (numWritten != len)
                {
                    errExit("write");
                }
                printf("%s: wrote %zu bytes\n", argv[ap], len);
                break;

            case 's':
                offset = getInt(argv[ap] + 1, GN_ANY_BASE, argv[ap]);
                cur = lseek(fd, offset, SEEK_SET);

                if (cur == -1)
                {
                    errExit("lseek");
                }
                
                printf("%s: seek succeeded\n", argv[ap]);
                break;

            default:
                usageErr("use rRws prefix\n");
                break;
        }
    }

    exit(EXIT_SUCCESS);
}