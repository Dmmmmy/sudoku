#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<iostream> 
#include<algorithm>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<math.h> 
#include<cstring>
using namespace std;


int n = 0; //要生成的数独终局个数 
char a[10][10]; //数独矩阵
char vis[3][10][10];
bool Found_Ans = false;
bool Is_First_Task = true;

int init[9] = { 3, 1, 2, 4, 5, 6, 7, 8, 9 };//第1行。第1个数字固定，第2-9个数字从小到大布置

//均通过第1行的各列右移，4-6行、7-9行各6次变换
int row_trans_0[3] = { 0, 3, 6 };//生成1-3行的右移列数
int row_trans_1[6][3] = { {1, 4, 7}, {1, 7, 4}, {4, 1, 7}, {4, 7, 1}, {7, 1, 4}, {7, 4, 1} };//生成4-6行的右移列数
int row_trans_2[6][3] = { {2, 5, 8}, {2, 8, 5}, {5, 2, 8}, {5, 8, 2}, {8, 2, 5}, {8, 5, 2} };//生成7-9行的右移列数

static char buffer[165000000];//缓冲字符
static int pos;//缓冲字符串的当前写入位置

void TranslateRow(int trans_1, int trans_2, bool flag)//trans_1为row_trans_1的第一维位置，trans_2为row_trans_2的第一维位置
{
	for (int i = 0; i < 3; i++)//写入1-3行至buffer中
	{
		int trans = 9 - row_trans_0[i];
		for (int j = 0; j < 8; j++)
		{
			buffer[pos] = init[(j + trans) % 9] + '0';
			pos += 2;
		}
		buffer[pos++] = init[(8 + trans) % 9] + '0';
		buffer[pos++] = '\n';
	}
	for (int i = 3; i < 6; i++)//写入4-6行至buffer中
	{
		int trans = 9 - row_trans_1[trans_1][i - 3];
		for (int j = 0; j < 8; j++)
		{
			buffer[pos] = init[(j + trans) % 9] + '0';
			pos += 2;
		}
		buffer[pos++] = init[(8 + trans) % 9] + '0';
		buffer[pos++] = '\n';
	}
	for (int i = 6; i < 9; i++)//写入7-9行至buffer中
	{
		int trans = 9 - row_trans_2[trans_2][i - 6];
		for (int j = 0; j < 8; j++)
		{
			buffer[pos] = init[(j + trans) % 9] + '0';
			pos += 2;
		}
		buffer[pos++] = init[(8 + trans) % 9] + '0';
		buffer[pos++] = '\n';
	}
	if (flag == false)
		buffer[pos++] = '\n';
}

void CreateSudoku(int n)//需生成的终局个数
{
	ofstream out("sudoku.txt");//打开文件
	memset(buffer, ' ', sizeof(buffer));
	pos = 0;
	int num = 0;//已生成的终局个数
	do
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				num += 1;
				if (num < n)//生成终局个数未达标，终局之间换行
				{
					TranslateRow(i, j, false);
				}
				else//生成终局个数已达标，写入文件并退出
				{
					TranslateRow(i, j, true);
					out.write(buffer, pos - 1);
					out.clear();
					out.close();
					return;
				}
			}
		}
	} while (next_permutation(init + 1, init + 9));//从init第二个数字开始全排列
}



void Set_Vis(int r, int c, int num)
{
	vis[0][r][num] = 1;
	vis[1][c][num] = 1;
	vis[2][r / 3 * 3 + c / 3][num] = 1;
}

void Reset_Vis(int r, int c, int num)
{
	vis[0][r][num] = 0;
	vis[1][c][num] = 0;
	vis[2][r / 3 * 3 + c / 3][num] = 0;
}

bool Check_Vis(int r, int c, int num) //检查可不可以访问
{
	if (vis[0][r][num] == 0
		&& vis[1][c][num] == 0
		&& vis[2][r / 3 * 3 + c / 3][num] == 0)
		return true;
	else
		return false;
}

void Solve_Sudoku(int r, int c)
{
	while (a[r][c] != '0') //找到一个空的数独位置 
	{
		if (c < 8)
			c++;
		else //再来一轮 
		{
			c = 0;
			r++;
		}
		if (r == 9)	//找到了一个答案即9x9数独中没有0，那就是找到了一个解答 
		{
			Found_Ans = true;
			return;
		}
	}

	bool Can_Search = false; //标记回溯算法中当前结点是否可以搜索
	for (int i = 1; i <= 9; i++)
	{
		if (Check_Vis(r, c, i))
		{
			Can_Search = true; //标记可以搜索 
			Set_Vis(r, c, i); //当前结点搜索过 
			a[r][c] = i + '0';
			Solve_Sudoku(r, c);
			if (Found_Ans) //剪枝 
				return;
			Can_Search = false;
			Reset_Vis(r, c, i);
			a[r][c] = '0';
		}
	}
}

void Print()
{
	FILE* fp;
	//ofstream OutputFileSolveSudoku("solve.txt",ios::app);
	int pos = 0;
	if (Is_First_Task == true) //如果是第一个问题，就在求解之前输出一行空行 
	{
		fp = fopen("solve.txt", "w");
	}
	else
		fp = fopen("solve.txt", "a+");
	if (Is_First_Task == false)
		fprintf(fp, "\n");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			char x = a[i][j];
			fputc(x, fp);
			if (j != 9)
			{
				fprintf(fp, " ");
			}
		}
		fprintf(fp, "\n");//存入换行符
	}
	fclose(fp);
	if (Is_First_Task == true)
		Is_First_Task = false;
}



int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Please input THREE parameters!");
		return 0;
	}
	int start = clock();
	char file[100];
	strcpy(file, argv[2]);
	FILE* fp1 = fopen(file, "r");
	if (strcmp(argv[1], "-s") == 0 && fp1 == NULL)
	{
		printf("非法输入!\n");
		return 0;
	}
	if (argc == 3 && strcmp(argv[1], "-c") == 0)
	{
		int len = strlen(argv[2]); //将命令行字符串转换为整数 
		for (int i = 0; i < len; i++)
		{
			if (argv[2][i] >= '0' && argv[2][i] <= '9')
			{
				int value = argv[2][i] - '0';
				for (int j = 1; j <= len - 1; j++)
					value *= 10;
				n += value;
			}
			else
			{
				printf("Please input a legal number!\n");
				return 0;
			}
		}
		if (n > 1000000 || n <= 0)
		{
			printf("Please input a number from 1 to 1000000!\n");
			return 0;
		}
		CreateSudoku(n);
	}
	else if (argc == 3 && strcmp(argv[1], "-s") == 0)
	{
		//char filename[50];
		printf("%s\n", argv[2]);
		//strcpy(filename, argv[2]);
		FILE* fp1 = fopen(argv[2], "r");
		int count = 0;
		char buf[110];
		while (fgets(buf, 20, fp1))
		{
			if (strcmp(buf, "\n") == 0) //遇空行跳过 
				continue;
			for (int i = 0; i <= 16; i++)
			{
				if (i % 2 == 0)
				{
					a[count][i / 2] = buf[i];
					Set_Vis(count, i / 2, buf[i] - '0');
				}
			}
			count++;
			if (count == 9)
			{
				Found_Ans = false;
				Solve_Sudoku(0, 0); //回溯解数独 
				Print();
				memset(vis, 0, sizeof(vis));
				count = 0;
			}
		}
	}
	else
		printf("非法输入!\n");
	int end = clock();
	printf("time: %d ms\n", end - start);
	return 0;
}
