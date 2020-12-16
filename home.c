#include "list.h"
#include <sys/stat.h>
#include <sys/types.h>

void saveMatrixToFile(List *vertices[], int listSize, char *path);
void freeMatrix(List *vertices[], int n);
List **openSave(int *n, char *path);
void writeToFile(List **vertices, int m, char *path);
void readFromFile(char *path);
off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

int main(int argc, char *argv[])
{

    char *path = "./save.txt";
    int n = 5;
    // scanf("%d", &n);
    List **vertices; // = openSave(NULL, path);
    vertices = (List **)malloc(sizeof(List *) * n);
    for (size_t i = 0; i < n; i++)
    {
        vertices[i] = newList();
        addIntsToList2(vertices[i], 0);
    }

    if (!vertices)
    {

        ERR("malloc");
    }

    writeToFile(vertices, n, "save2");
    // saveMatrixToFile(vertices, n, path);
    // readFromFile("save2");
    freeMatrix(vertices, n);

    return 0;
}
void readFromFile(char *path)
{
    FILE *infile;
    List **vertices;

    infile = fopen(path, "r");
    if (infile == NULL)
    {
        ERR("fopen");
    }

    // read file contents till end of file
    fread(*vertices, sizeof(List *), 1, infile);

    // close file
    fclose(infile);
}

void writeToFile(List **vertices, int m, char *path)
{
    FILE *outfile;
    outfile = fopen(path, "w");
    if (outfile == NULL)
    {
        ERR("fopen");
    }

    fprintf(outfile, "%d;\n", m);
    // write struct to file
    for (int i = 0; i < m; i++)
    {
        fprintf(outfile, "%d:", i);
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

List **openSave(int *n, char *path)
{
    List **vertices;
    int fd;
    int m;
    int filesize = fsize(path);
    char *buff = (char *)malloc(sizeof(char) * filesize);

    if ((fd = open(path, O_RDONLY)) < 0)
        ERR("open");
    char text[3];

    //zmienic na bulk_read/obrone przed sygnalami
    if (read(fd, buff, filesize) < 0)
    {
        ERR("read");
    }
    char tmp[2];
    for (int i = 0; i < filesize; i++)
    {
        switch (buff[i])
        {
        case ';':
            m = atoi(text);
            vertices = (List **)malloc(sizeof(List *) * m);

            break;
        case '\n':
            printf("%s\n", text);
            strcpy(text, "");
            break;
        case ':':
            m = atoi(text);
            vertices[m] = newList();
            break;
        default:
            sprintf(tmp, "%c", buff[i]);
            strcat(text, tmp);
            break;
        }
    }

    // for (int i = 0; i < m; i++)
    // {
    //     if (read(fd, buff, 1) < 0)
    //     {
    //         ERR("read");
    //     }
    //     switch (buff[0])
    //     {
    //     case ':':

    //         break;
    //     case '\n':
    //         printf("%s\n", text);
    //         strcpy(text, "");
    //         break;
    //     default:
    //         strcpy(text, buff);
    //         break;
    //     }
    //     // vertices[i] = newList();
    // }
    free(buff);
    close(fd);
    return vertices;
}

void freeMatrix(List *vertices[], int n)
{
    for (int i = 0; i < n; i++)
    {
        freeList(vertices[i]);
    }
    free(vertices);
}

void saveMatrixToFile(List *vertices[], int listSize, char *path)
{
    int fd;
    if ((fd = open(path, O_CREAT | O_WRONLY)) < 0)
        ERR("open");
    int size;
    char text[20];
    char nodeVal[3];

    sprintf(text, "%d\n", listSize);
    if (write(fd, text, 2) < 0)
        ERR("write");

    for (int i = 0; i < listSize; i++)
    {
        sprintf(text, "%d: ", i);
        size = 0;
        //iterate through the list and append an index
        Node *p = vertices[i]->head;
        while (p)
        {
            sprintf(nodeVal, "%d ", *(int *)p->data);
            strcat(text, nodeVal);
            p = p->next;
        }

        strcat(text, "\n");
        while (text[size])
        {
            size++;
        }
        if (write(fd, text, size) < 0)
            ERR("write");
    }
    close(fd);
}