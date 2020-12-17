#include "list.h"
#include <string.h>

off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

void parseLine(char *line, List *list)
{
    char *saveptr;
    char *token = strtok_r(line, " ", &saveptr);
    while (token != NULL)
    {

        int roomNo = atoi(token);
        addIntsToList2(list, roomNo);
        token = strtok_r(NULL, " ", &saveptr);
    }
}

List **readSaveFile(const char *path, int *sizeOfArray)
{
    int filesize = fsize(path);
    char *buff = (char *)malloc(sizeof(char) * filesize);
    if (!buff)
        ERR("malloc");

    int fd;
    if ((fd = open(path, O_RDONLY)) < 0)
        ERR("open");

    //zmienic na bulk_read/obrone przed sygnalami
    if (read(fd, buff, filesize) < 0)
    {
        ERR("read");
    }

    char *saveptr;
    char *token = strtok_r(buff, "\n", &saveptr);

    *sizeOfArray = atoi(token);
    int m;
    List **vertices = (List **)malloc(sizeof(List *) * (*sizeOfArray));

    token = strtok_r(NULL, ":", &saveptr);
    while (token != NULL)
    {

        m = atoi(token);
        vertices[m] = newList();

        char *token2 = strtok_r(NULL, "\n", &saveptr);
        parseLine(token2, vertices[m]);

        token = strtok_r(NULL, ":", &saveptr);
    }
    free(buff);
    close(fd);

    return vertices;
}
void writeSaveFile(List **vertices, int m, const char *path)
{
    FILE *outfile;

    outfile = fopen(path, "w");
    if (outfile == NULL)
    {
        ERR("fopen");
    }

    fprintf(outfile, "%d\n", m);
    for (int i = 0; i < m; i++)
    {
        fprintf(outfile, "%d: ", i);
        Node *p = vertices[i]->head;
        while (p)
        {
            fprintf(outfile, "%d ", *(int *)(p->data));

            p = p->next;
        }
        fprintf(outfile, "\n");
    }

    // close file
    fclose(outfile);
}
