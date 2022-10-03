#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Maxlength 100
typedef struct{
    int x, y;
}Coord;
typedef struct{
    Coord data[Maxlength];
    int size;
}ListCoord;
void initListCoord(ListCoord *list){
    list->size=0;
}
void appendListCoord(ListCoord *list, Coord coord){
    list->data[list->size++]=coord;
}
#define Rows 8
#define Cols 8
typedef struct{
    int data [Rows*Cols][Rows*Cols];// luu các c?p d?nh có ràng bu?c v?i nhau
    int n;
}Constrains;
void initConstrains(Constrains *constrains){
    int i,j;
    for(i=0; i<Rows*Cols; i++)
        for(j=0; j<Rows*Cols; j++)
            constrains->data[i][j]=0;
    constrains->n=Rows*Cols;
}
int indexOf(Coord coord){
    return coord.x*Rows+coord.y;
}
Coord positionOfVertex(int vertex){
    Coord coord;
    coord.x=vertex/Rows;
    coord.y=vertex%Cols;
    return coord;
}
int addConstrain(Constrains * constrains, Coord source, Coord target){
    int u=indexOf(source);
    int v=indexOf(target);
    if(constrains->data[u][v]==0){
        constrains->data[u][v]=1;
        constrains->data[v][u]=1;
        return 1;
    }
    return 0;
}
ListCoord getConstrains(Constrains constrains, Coord coord){
    int i;
    int v=indexOf(coord);
    ListCoord result;
    initListCoord(&result);
    for(i=0; i<constrains.n; i++){
        if(constrains.data[v][i]==1)
            appendListCoord(&result, positionOfVertex(i));
    }
    return result;
}
#define EMPTY 0
#define INF 999999

typedef struct{
    int cells[Rows][Cols];
    Constrains constrains;
}EightQueen;
void initEightQueen(EightQueen *sudoku){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            sudoku->cells[i][j]=EMPTY;
    initConstrains(&sudoku->constrains);
}
void initEightQueenWithValues(EightQueen *sudoku, int inputs[Rows][Cols]){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            sudoku->cells[i][j]=inputs[i][j];

    initConstrains(&sudoku->constrains);
}
void printEightQueen(EightQueen sudoku){
    int i, j;
    printf("EightQueen:\n");
    for (i = 0; i < Rows +Cols; i++)
        printf("-");
    printf("\n");
    printf("  ");
	for(i=1;i<=Rows;i++)
    	printf("%d ",i);
    printf("\n");
    for(i=0; i<Rows; i++){
    	printf("%d ",i+1);
        for(j=0; j<Cols; j++){
            printf("%d ", sudoku.cells[i][j]);
        }
        printf("\n");
    }
    for(i=0; i<Rows+Cols; i++)
        printf("-");
    printf("\n");
}
void printListCoord(ListCoord listcoord)
{
    int i;
    for (i = 0; i < listcoord.size; i++)
    {
        printf("dinh: %d\n", indexOf(listcoord.data[i]));
    }
}
void spreadConstrainsFrom(Coord position, Constrains *constrains, ListCoord *changeds){
    int row=position.x, column=position.y;
    int i,j;
    //ràng bu?c theo hàng
    for(i=0; i<Rows; i++){
        if(i!=row){
	       	Coord pos={i, column};
	        if(addConstrain(constrains, position,pos))
	            appendListCoord(changeds, pos);
		}     
    }
    //ràng bu?c theo c?t
    for(i=0; i<Cols; i++){
        if(i!=column){
	       	Coord pos={row, i};
	        if(addConstrain(constrains, position,pos))
	            appendListCoord(changeds, pos);
		}     
    }
    //rang buoc cheo tren trai
    for(i=row-1, j=column-1; i>=0 && j>=0; i--, j--){
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
    // rang buoc cheo duoi trai
    for (i = row+1, j = column-1; i < Rows && j >= 0; i++, j--)
    {
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
    // rang buoc cheo tren phai
    for (i = row-1, j = column+1; i >= 0 && j < Cols; i--, j++)
    {
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
    // rang buoc cheo duoi phai
    for (i = row+1, j = column+1; i < Rows && j <Cols; i++, j++)
    {
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
}
int exploredCounter=0;
int checkConstrains(EightQueen sudoku,Coord position){
    ListCoord list = getConstrains(sudoku.constrains, position);//danh sách các ô có ràng bu?c v?i position
    int k;
    for (k = 0; k < list.size; k++)
    {
        Coord pos = list.data[k];
        if (sudoku.cells[pos.x][pos.y] == 1)
            return 1;
    }
    return 0;
}
int count=0;
int sudokuBackTracking(EightQueen *sudoku, int column){	
    if(column==Cols)
        column=0;
    if(count==Cols)
        return 1;
    int i;
    for(i=0; i<Cols; i++){
        Coord position={i, column};
        if(checkConstrains(*sudoku, position)==0){
            sudoku->cells[i][column] = 1;
            exploredCounter++;
            count++;
            if(sudokuBackTracking(sudoku, column+1)){
                return 1;
            }
            sudoku->cells[i][column] = 0;
        }
    }
    count=0;
    return 0;
}
int random(int min , int max){
    return min + rand()%(max+1-min);
}
EightQueen solveEightQueen(EightQueen sudoku){
    int i , j;
    initConstrains(&sudoku.constrains);
    for(i=0; i<Rows; i++){
        for(j=0; j<Cols; j++){
                ListCoord history;
                initListCoord(&history);
                Coord pos = {i, j};
                spreadConstrainsFrom(pos, &sudoku.constrains, &history);
        }
    }
    exploredCounter=0;
    srand((int)time(0));
    if (sudokuBackTracking(&sudoku, random(0, Cols - 1)))
        printf("Solved\n");
    else printf("Can not solve\n");
    printf("Explored %d states\n", exploredCounter);
    return sudoku;
}
int input1[Rows][Cols];
int main()
{
    EightQueen sudoku;
    int i, j;
    for (i = 0; i < Rows; i++)
        for (j = 0; j < Cols; j++)
            input1[i][j] = 0;
    initEightQueenWithValues(&sudoku, input1);
    printEightQueen(sudoku);
    EightQueen  result=solveEightQueen(sudoku);
    printEightQueen(result);
    return 0;
}

