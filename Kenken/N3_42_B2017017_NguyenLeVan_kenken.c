#include <stdio.h>
#define Rows 4
#define Cols 4
#define MaxLength 50
#define MaxBlocks 8
#define MAX_VALUE 5
#define EMPTY 0
#define AREA_SQUARE_SIZE 1
#define INF 999999


typedef struct{
    int x, y;
}Coord;
typedef struct{
    Coord data[MaxLength];
    int size;
}ListCoord;
void initListCoord(ListCoord *list){
    list->size=0;
}
void appendListCoord(ListCoord *list, Coord coord){
    list->data[list->size++]=coord;
}
typedef struct{
    int data [Rows*Cols][Rows*Cols];
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
typedef struct{
	int result;
	char opr;
	ListCoord listCoord;
}Block;

typedef struct{
	Block blocks[MaxBlocks];
	int cells[MaxBlocks][MaxBlocks];
	int nums_blocks[MaxBlocks][MaxBlocks];
	Constrains constrains;
	int NBlock;
}KenKen;

void updateListBlock(KenKen kenken,Block *block, int index){
	int i, j;
	initListCoord(&block->listCoord);
	for(i=0; i<Rows; i++)
		for(j=0; j<Cols; j++){
			if(kenken.nums_blocks[i][j]==index){
				Coord pos={i, j};
				appendListCoord(&block->listCoord, pos);
			}
		}
}
void initNumsBlocks(KenKen *kenken){
	int i, j;
	for(i=0; i<Rows; i++)
		for(j=0; j<Cols; j++)
			kenken->nums_blocks[i][j]=0;
}
void initBlocks(Block *block){
	block->opr='n';
	block->result=-9999;
	initListCoord(&block->listCoord);
}
void initKenKen(KenKen *kenken){
	int i, j;
	for(i=0; i<Rows; i++)
		for(j=0; j<Cols; j++)
			kenken->cells[i][j]=0;
	kenken->NBlock=0;
	initBlocks(kenken->blocks);
	initNumsBlocks(kenken);
	initConstrains(&kenken->constrains);
}

void printKenKen(KenKen kenken){
    int i, j,k;
    printf("KenKen Sudoku:\n");
    for(i=0; i<Rows; i++){
        if(i%AREA_SQUARE_SIZE==0)
			for (k = 1; k <= kenken.NBlock*2.2; k++)
				printf("-");
		printf("\n");
		for(j=0; j<Cols; j++){
            if(j%AREA_SQUARE_SIZE==0)
                printf("| ");
            printf("%d ", kenken.cells[i][j]);
        }
        printf("|\n");
    }
	for (k = 1; k <= kenken.NBlock * 2.2; k++)
		printf("-");
	printf("\n");
}
int getBlockNumber(Coord pos, Block block[])
{
	int i, j;
	for (i = 0; i < MaxBlocks; i++)
	{
		for (j = 0; j < block[i].listCoord.size; j++)
			if (pos.x == block[i].listCoord.data[j].x && pos.y == block[i].listCoord.data[j].y)
				return i;
	}
	return 0;
}
void initKenKenFromFile(KenKen *kenken){
	initKenKen(kenken);
	FILE *f = freopen("input1.txt", "r", stdin);
	int m, n;
	int i, j, num_opr, num;
	char op;
	scanf("%d %d", &m, &n);
	for(i=0; i<m; i++){
		for(j=0; j<n; j++){
			int x;
			scanf("%d", &x);
			kenken->nums_blocks[i][j]=x;
		}
	}
	// for(i=0; i<m; i++){
	// 	for(j=0; j<n; j++)
	// 		printf("%d ",kenken->nums_blocks[i][j]);
	// 	printf("\n");
	// }
    scanf("%d\n", &num_opr);
	kenken->NBlock=num_opr;
	for(i=0; i<num_opr; i++)
		updateListBlock(*kenken,&kenken->blocks[i] ,i+1);
    for(i=0; i<num_opr; i++){
    	scanf("%d %c", &num, &op);
    	kenken->blocks[i].result=num;
    	kenken->blocks[i].opr=op;
	}
	Coord pos;
	for(i=0; i<num_opr; i++){
		if(kenken->blocks[i].opr=='n'){
			int j;
			for (j = 0; j < kenken->blocks[i].listCoord.size; j++)
				pos = kenken->blocks[i].listCoord.data[j];
				kenken->cells[pos.x][pos.y]=kenken->blocks[i].result;
		}
	}
}
int isFilledSudoku(KenKen kenken)
{
	int i, j;
	for (i = 0; i < Rows; i++)
		for (j = 0; j < Cols; j++)
			if (kenken.cells[i][j] == EMPTY)
				return 0;
	return 1;
}

void spreadConstrainsFrom(Coord position, Constrains *constrains, ListCoord *changeds,KenKen kenken)
{
	int row = position.x, column = position.y;
	int i, j;
	// Tao rang buoc theo cot
	for (i = 0; i < Rows; i++) {
		if (i != row) {
			Coord pos = {i, column};
			if (addConstrain(constrains, position, pos))
				appendListCoord(changeds, pos);
		}
	}
	// Tao rang buoc theo hang
	for (i = 0; i < Cols; i++) {
		if (i != column) {
			Coord pos = {row, i};
			if (addConstrain(constrains, position, pos))
				appendListCoord(changeds, pos);
		}
	}
	//rang buoc theo khoi
	int indexBlock=getBlockNumber(position, kenken.blocks);
	for(i=0; i<kenken.blocks[indexBlock].listCoord.size; i++){
	 	Coord pos =kenken.blocks[indexBlock].listCoord.data[i];
	 	if(pos.x!=row && pos.y!=column)
	 		if(addConstrain(constrains, position, pos))
	  				appendListCoord(changeds, pos);
	}
}

typedef struct
{
	int Elements[MaxLength];
	int size;
}List;
void makeNullList(List *list)
{
	list->size = 0;
}
int emptyList(List list)
{
	return list.size == 0;
}
int fullList(List list)
{
	return list.size == MaxLength;
}
int elementAt(int p, List list)
{
	return list.Elements[p - 1];
}
void pushList(int x, int position, List *list)
{
	if (!fullList(*list))
	{
		int p;
		for (p = list->size; p >= position; p--)
		{
			list->Elements[p] = list->Elements[p - 1];
		}
		list->Elements[position - 1] = x;
		list->size++;
	}
	else
		printf("List is full");
}
void appendList(int x, List *list){
	if(list->size<MaxLength)
		list->Elements[list->size]=x;
		list->size++;
}
void pop(List *list){
	list->size--;
}
List getAvailableValues(Coord position, KenKen kenken)
{
	ListCoord posList = getConstrains(kenken.constrains, position);
	int availables[MAX_VALUE];
	int i;
	for (i = 1; i <MAX_VALUE; i++)
		availables[i] = 1;
	for (i = 0; i < posList.size; i++)
	{
		Coord pos = posList.data[i];
		if (kenken.cells[pos.x][pos.y] != EMPTY)
			availables[kenken.cells[pos.x][pos.y]] = 0;
	}
	List result;
	makeNullList(&result);
	for (i = 1; i < MAX_VALUE; i++)
		if (availables[i])
			appendList(i, &result);
	return result;
}
Coord getNextEmptyCell(KenKen kenken)
{
	int i, j;
	for (i = 0; i < Rows; i++)
		for (j = 0; j < Cols; j++)
		{
			Coord pos = {i, j};
			if (kenken.cells[i][j] == EMPTY)
				return pos;
		}
}
Coord getNextMinDomainCell(KenKen kenken)
{
	int minLength = INF;
	int i, j;
	Coord result;
	for (i = 0; i < Rows; i++)
		for (j = 0; j < Cols; j++)
		{
			if (kenken.cells[i][j] == EMPTY)
			{
				Coord pos = {i, j};
				int availablesLength = getAvailableValues(pos, kenken).size;
				if (availablesLength < minLength)
				{
					minLength = availablesLength;
					result = pos;
				}
			}
		}
	return result;
}
int exploredCounter = 0;


int caculator(List Values, char opr, int resultOfBlock){
	int result=0;
	int i, j;
	for (i = 0; i < Values.size; i++)
		for (j = i + 1; j < Values.size; j++)
			if (Values.Elements[i] < Values.Elements[j])
			{
				int temp = Values.Elements[i];
				Values.Elements[i] = Values.Elements[j];
				Values.Elements[j] = temp;
			}
	if (opr == '+')
	{
		for (i = 0; i < Values.size; i++)
			result += Values.Elements[i];
	}
	else if(opr=='-'){
		result=Values.Elements[0];
		for(i=1; i<Values.size; i++)
			result-=Values.Elements[i];
	}
	else if (opr == '*')
	{
		result=1;
		for (i = 0; i < Values.size; i++)
			result *= Values.Elements[i];
	}
	else if (opr =='/')
	{
		result=Values.Elements[0];
		for (i =1; i <Values.size; i++)
			result /= Values.Elements[i];
	}
	if (result != resultOfBlock)
		return 0;
	return 1;
}
List Values[MaxBlocks];
int sudokuBackTracking(KenKen *kenken)
{
// 	printKenKen(*kenken);
// 	printf("Explored: %d\n", exploredCounter);
	if (isFilledSudoku(*kenken))
		return 1;
	Coord position = getNextMinDomainCell(*kenken);
	List availables = getAvailableValues(position, *kenken);
	int indexBlock = getBlockNumber(position, kenken->blocks);
	int sizeBlock=kenken->blocks[indexBlock].listCoord.size;

	int j;
	for (j = 0; j < availables.size; j++)
	{
		int value = availables.Elements[j];
		kenken->cells[position.x][position.y] = value;
		appendList(value, &Values[indexBlock]);
		exploredCounter++;

		if (Values[indexBlock].size == sizeBlock)
			if (!caculator(Values[indexBlock], kenken->blocks[indexBlock].opr, kenken->blocks[indexBlock].result))
			{
				pop(&Values[indexBlock]);
				kenken->cells[position.x][position.y] = EMPTY;
				continue;
			}
		if (sudokuBackTracking(kenken))
			return 1;
		pop(&Values[indexBlock]);
		kenken->cells[position.x][position.y] = EMPTY;
	}
	return 0;
}
void printListCoord(ListCoord list){
	int i;
	for(i=0; i<list.size; i++){
		Coord pos=list.data[i];
		printf("(%d %d)\n", pos.x, pos.y);
	}
}
KenKen solveSudoku(KenKen kenken)
{
	int i, j;
	initConstrains(&kenken.constrains);
	for (i = 0; i < Rows; i++)
	{
		for (j = 0; j < Cols; j++){
			ListCoord history;
			initListCoord(&history);
			Coord pos = {i, j};
			spreadConstrainsFrom(pos, &kenken.constrains, &history, kenken);
		}
	}
	exploredCounter = 0;
	
	for(i=0; i<MaxBlocks; i++){
		makeNullList(&Values[i]);	
	}
	if (sudokuBackTracking(&kenken))
		printf("Solved\n");
	else
		printf("Can not solve\n");
	printf("Explored %d states\n", exploredCounter);
	return kenken;
}
int main(){
	KenKen kenken;
	initKenKenFromFile(&kenken);
	printKenKen(kenken);
	KenKen result=solveSudoku(kenken);
	printKenKen(result);
    return 0;
}
