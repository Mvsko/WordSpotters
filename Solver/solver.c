#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Step 1: get grid from file
int gridDim(char *filename,size_t *height,size_t *width)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == 0)
    {
        printf("Error: The file %s does not exist\n",filename);
        return -1; //error
    }
    char buffer[100];
    *height = 0;
    *width = 0;
    while(fgets(buffer,100,fptr))
    {
        if (*height == 0)
        {
            *width = strlen(buffer);
        }
        else
        {
            if (*width != strlen(buffer) && (strlen(buffer) != 0))
            {
                printf("Error: The file %s does not contain a valid grid\n",filename);
                fclose(fptr);
                return -1; //error
            }
            
        }
        (*height)++;
    }
    if (strlen(buffer) == 0)
    {
        (*height)--;
    }
    fclose(fptr);
    if (*height < 5 || *width < 5)
    {
        printf("Error: The grid is too small (must be at least 5x5)");
        return -1; //error
    }
    
    return 0; //no error
}

char **initGrid(char *filename,size_t height)
{
    char **grid = malloc(height * sizeof(size_t));
    FILE *fptr = fopen(filename, "r");
    char buffer[100];
    printf("%ld\n",height);
    for (size_t i = 0; i < height; i++)
    {
        fgets(buffer,100,fptr);
        size_t len = strlen(buffer);
        grid[i] = malloc((len+1) * sizeof(char));
        for (size_t j = 0; j < len; j++)
        {
            grid[i][j] = toupper(buffer[j]);
        }
        grid[i][len] = 0;
    }
    if (fptr != NULL)
    {
        fclose(fptr);
    }
    return grid;
}

void freeGrid(char **grid,size_t height)
{
    for (size_t i = 0; i < height; i++)
    {
        free(grid[i]);
    }
    free(grid);
}

//Step 2: reverse function
void strReverse(char *strDest, char* str,size_t width)
{
    for (size_t i = 0; i < width; i++)
    {
        strDest[i] = str[width-i-1];
    }
    strDest[width] = 0;
}

//Step 3: search horizontal / vertical / diagonal * 2
int * searchHorizontal(char **grid,char *word,size_t height,size_t width,int * res)
{
    size_t len = strlen(word);
    for (size_t x = 0; x < height; x++)
    {
        res[1] = x;
        for (size_t y = 0; y < width-len+1; y++)
        {
            res[0] = y;
            size_t i = 0;
            while (i < len && grid[x][y+i] == word[i])
            {
                i++;
            }
            if (i == len)
            {
                res[3] = x;
                res[2] = y+i-1;
                return res;
            }
            
        }
        
    }
    res[0] = -1;
    return res;
}

int * searchVertical(char **grid,char *word,size_t height,size_t width,int * res)
{
    size_t len = strlen(word);
    for (size_t x = 0; x < height-len+1; x++)
    {
        res[1] = x;
        for (size_t y = 0; y < width; y++)
        {
            res[0] = y;
            size_t i = 0;
            while (i < len && grid[x+i][y] == word[i])
            {
                i++;
            }
            if (i == len)
            {
                res[3] = x+i-1;
                res[2] = y;
                return res;
            }
            
        }
        
    }
    res[0] = -1;
    return res;
}

int * searchDiagonal(char **grid,char *word,size_t height,size_t width,int * res)
{
    size_t len = strlen(word);
    for (size_t x = 0; x < height-len+1; x++)
    {
        res[1] = x;
        for (size_t y = 0; y < width-len+1; y++)
        {
            res[0] = y;
            size_t i = 0;
            while (i < len && grid[x+i][y+i] == word[i])
            {
                i++;
            }
            if (i == len)
            {
                res[3] = x+i-1;
                res[2] = y+i-1;
                return res;
            }
            
        }
        
    }
    res[0] = -1;
    return res;
}

int * searchDiagonal2(char **grid,char *word,size_t height,size_t width,int * res)
{
    size_t len = strlen(word);
    for (size_t x = 0; x < height-len+1; x++)
    {
        res[1] = x;
        for (size_t y = len-1; y < width; y++)
        {
            res[0] = y;
            size_t i = 0;
            while (i < len && grid[x+i][y-i] == word[i])
            {
                i++;
            }
            if (i == len)
            {
                res[3] = x+i-1;
                res[2] = y-i-1;
                return res;
            }
            
        }
        
    }
    res[0] = -1;
    return res;
}


//Step 4: main function
int main(int argc, char **argv)
{
    //Sanitizing arguments
    if (argc != 3)
    {
        printf("Invalid arguments. Correct usage: ./solver <file name> <word>\n");
        return EXIT_FAILURE;
    }

    //Initializing the grid
    size_t height = 0;
    size_t width = 0;
    int d = gridDim(argv[1],&height,&width);
    if (d == -1)
    {
        printf("%d An error occurred. Exiting...",d);
        return EXIT_FAILURE;
    }
    char **grid = initGrid(argv[1],height);
    int wordLen = strlen(argv[2]);
    char word[wordLen+1];
    for (int i = 0; i < wordLen; i++)
    {
        word[i] = toupper(argv[2][i]);
    }
    word[wordLen] = 0;
    //Searching the word from left to right
    int res[4];
    searchHorizontal(grid,word,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[0],res[1],res[2],res[3]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }
    searchVertical(grid,word,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[0],res[1],res[2],res[3]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }
    searchDiagonal(grid,word,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[0],res[1],res[2],res[3]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }
    searchDiagonal2(grid,word,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[0],res[1],res[2],res[3]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }

    //Searching the word from right to left
    char reversed[wordLen+1];
    strReverse(reversed,word,wordLen);
    searchHorizontal(grid,reversed,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[2],res[3],res[0],res[1]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }
    searchVertical(grid,reversed,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[2],res[3],res[0],res[1]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }
    searchDiagonal(grid,reversed,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[2],res[3],res[0],res[1]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }
    searchDiagonal2(grid,reversed,height,width,res);
    if (res[0] != -1)
    {
        printf("(%d,%d)(%d,%d)\n",res[2],res[3],res[0],res[1]);
        freeGrid(grid,height);
        return EXIT_SUCCESS;
    }
    printf("Not Found\n");
    freeGrid(grid,height);
    return EXIT_SUCCESS;
}