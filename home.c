#include "list.h"
void saveMatrixToFile(List *vertices[], int listSize, char *path);
void freeMatrix(List *vertices[], int n);
void openSave(List *vertices[], int *n, char *path);
int main(int argc, char *argv[])
{

    char *path = "./save.txt";
    int n = 5;
    // scanf("%d", &n);
    List **vertices = (List **)malloc(sizeof(List *) * n);

    openSave(NULL, NULL, path);

    if (!vertices)
    {
        ERR("malloc");
    }

    for (int i = 0; i < n; i++)
    {
        vertices[i] = newList();
    }

    addIntsToList2(vertices[1], 2);
    addIntsToList2(vertices[1], 3);
    addIntsToList2(vertices[1], 0);

    saveMatrixToFile(vertices, n, path);

    freeMatrix(vertices, n);

    return 0;
}

void openSave(List *vertices[], int *n, char *path)
{
    int fd;
    int m;
    if ((fd = open(path, O_RDONLY)) < 0)
        ERR("open");
    char buff[20];
    // while (1)
    // {
    if (read(fd, buff, 3) < 0)
    {
        ERR("read");
    }
    m = atoi(buff);
    printf("%d\n", m);
    // }
    close(fd);
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