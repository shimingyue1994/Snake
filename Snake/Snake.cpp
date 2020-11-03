// Snake.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<stdio.h>			//标准输入输出函数库
#include<time.h>			//用于获得随机数
#include<windows.h>			//控制dos界面
#include<stdlib.h>			//即standard library标志库头文件，里面定义了一些宏和通用工具函数
#include<conio.h>			//接收键盘输入输出

/*******宏  定  义*******/
#define U 1
#define D 2
#define L 3
#define R 4   //蛇的状态，U：上 ；D：下；L:左 R：右


/*******定  义  全  局  变  量 *******/
typedef struct  snake //蛇身的一个节点,链表结构
{
	int x;              //节点的x坐标
	int y;              //节点的y坐标
	struct snake* next; //蛇身的下一个节点
} snake;
int score = 0, add = 10; //  总得分和每次吃完食物后加的分数
int HighScore = 0;//最高分
snake* head, * food;//蛇头指针，食物指针
snake* q; //遍历蛇的节点时用到的变量
int endgamestatus = 0;//游戏结束的情况：撞到墙；咬到自己；主动退出游戏
HANDLE hOut; //控制台句柄

/*******函  数  声  明 *******/
void gotoxy(int x,int y); //设置光标位置
int color(int c);//设置文字颜色
void printsnake();//字符画 蛇（首页）
void welcometogame();//开始界面（首页）
void createMap();// 绘制地图
void scoreandtips();//游戏界面右侧的得分和小提示
void initsnake();//初始化蛇身，画蛇身
void createfood();//创建并随机出现食物
void biteself();//判断是否咬到自己
void cantcrosswall();//判断是否撞墙
void speedup();//加速
void speeddown();//减速
void snakemove();//控制蛇的前进方向
void keyboardControl();//控制键盘按键，根据按键做出相应的操作
void Lostdraw();//游戏结束界面
void endgame();//游戏结束
void choose();//游戏失败之后的选择
void File_out();//在文件中读取最高分
void File_in();//将最高分存入文件
void explationg();//游戏说明



/**
 * 设置光标位置
 */
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

/**
 * 文字颜色函数      此函数的局限性：1、只能Windows系统下使用   2、不能改变背景颜色
 */
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //更改文字颜色
	return 0;
}

/**
 * 在文件中读取最高分
 */
void File_out() 
{
	FILE* fp;
	//打开一个文件，第一个参数是文件路径，第二个参数是以何种方式打开，返回指向打开文件的指针
	fopen_s(&fp,"save.txt","a+");
	//扫描文件，从输入流中读取数据，第二个参数是format（输出的格式） ；第三个参数是保存地址
	fscanf_s(fp,"%d",&HighScore);
	//关闭打开的文件
	fclose(fp);
}
/*
*   字符画---蛇
*/
void printsnake()
{
	gotoxy(35, 1);
	color(6);
	printf("/^\\/^\\");      //蛇眼睛

	gotoxy(34, 2);
	printf("|__|  O|");      //蛇眼睛

	gotoxy(33, 2);
	color(2);
	printf("_");

	gotoxy(25, 3);
	color(12);
	printf("\\/");      	//蛇信

	gotoxy(31, 3);
	color(2);
	printf("/");

	gotoxy(37, 3);
	color(6);
	printf(" \\_/");     	//蛇眼睛

	gotoxy(41, 3);
	color(10);
	printf(" \\");

	gotoxy(26, 4);
	color(12);
	printf("\\____");   	//舌头

	gotoxy(32, 4);
	printf("_________/");

	gotoxy(31, 4);
	color(2);
	printf("|");

	gotoxy(43, 4);
	color(10);
	printf("\\");

	gotoxy(32, 5);
	color(2);
	printf("\\_______");    //蛇嘴

	gotoxy(44, 5);
	color(10);
	printf("\\");

	gotoxy(39, 6);
	printf("|     |                  \\");  //下面都是画蛇身

	gotoxy(38, 7);
	printf("/      /                   \\");

	gotoxy(37, 8);
	printf("/      /                    \\ \\");

	gotoxy(35, 9);
	printf("/      /                       \\ \\");

	gotoxy(34, 10);
	printf("/     /                          \\  \\");

	gotoxy(33, 11);
	printf("/     /             _----_         \\   \\");

	gotoxy(32, 12);
	printf("/     /           _-~      ~-_         |  |");

	gotoxy(31, 13);
	printf("(      (        _-~    _--_    ~-_     _/  |");

	gotoxy(32, 14);
	printf("\\     ~-____-~    _-~    ~-_    ~-_-~    /");

	gotoxy(33, 15);
	printf("~-_           _-~          ~-_       _-~");

	gotoxy(35, 16);
	printf("~--______-~                ~-___-~");
}

/**
 * 开始界面
 */
void welcometogame()
{
	int n;
	int i, j = 1;
	gotoxy(43, 18);
	color(11);
	printf("贪 吃 蛇 大 作 战");
	color(14);          			//黄色边框
	for (i = 20; i <= 26; i++)   	//输出上下边框┅
	{
		for (j = 27; j <= 74; j++)  //输出左右边框┇
		{
			gotoxy(j, i);
			if (i == 20 || i == 26)
			{
				printf("-");
			}
			else if (j == 27 || j == 74)
			{
				printf("|");
			}
		}
	}
	color(12);
	gotoxy(35, 22);
	printf("1.开始游戏");
	gotoxy(55, 22);
	printf("2.游戏说明");
	gotoxy(35, 24);
	printf("3.退出游戏");
	gotoxy(29, 27);
	color(3);
	printf("请选择[1 2 3]:[ ]\b\b");        //\b为退格，使得光标处于[]中间
	color(14);
	scanf_s("%d", &n);    		//输入选项
	switch (n)
	{
	case 1:
		system("cls");
		createMap();
		scoreandtips();
		break;
	case 2:
		break;
	case 3:
		exit(0);
		break;
	default:				//输入非1~3之间的选项
		color(12);
		gotoxy(40, 28);
		printf("请输入1~3之间的数!");
		_getch();			//输入任意键
		system("cls");		//清屏
		printsnake();//重新绘制开始界面
		welcometogame();
	}
}

/// <summary>
/// 创建地图
/// </summary>
void createMap() 
{
	int i, j;
	for ( i = 0; i < 58; i+=2)//绘制上下边框
	{
		gotoxy(i,0); //上边框
		color(5);
		printf("□");
		gotoxy(i,26);
		printf("□");
	}

	for (i = 1; i < 26; i++) //绘制左右的边框
	{
		gotoxy(0, i);//绘制左边框
		printf("□");
		gotoxy(56,i);//绘制右边框
		printf("□");
	}
	//打印中间的网格
	for ( i = 2; i < 56; i+=2) //横向
	{
		for ( j = 1; j < 26; j++)//纵向
		{
			gotoxy(i,j);
			color(3);
			printf("■");
		}
	}
}

/// <summary>
/// 游戏右侧的得分和小提示
/// </summary>
void scoreandtips()
{
	//最高得分
	File_out();// 调用File_out()函数，读取save.txt中的内容
	gotoxy(64,4);
	color(11);
	printf("☆最高记录☆：%d", HighScore);
	//得分
	gotoxy(64, 8);
	color(14);
	printf("得分：%d",score);

	//小提示
	color(13);
	gotoxy(73, 11);
	printf("小 提 示");

	//打印边框
	gotoxy(60, 13);
	color(6);
	printf("╬ ┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅ ╬");
	gotoxy(60, 25);
	printf("╬ ┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅ ╬");

	//边框中间的按键等提示文字
	color(3);
	gotoxy(64, 14);
	printf("每个食物得分：%d分", add);
	gotoxy(64, 16);
	printf("不能穿墙，不能咬到自己");
	gotoxy(64, 18);
	printf("用↑ ↓ ← →分别控制蛇的移动");
	gotoxy(64, 20);
	printf("F1 为加速，F2 为减速");
	gotoxy(64, 22);
	printf("space：暂停游戏");
	gotoxy(64, 24);
	printf("ESC ：退出游戏");
}


int main()
{
	system("mode con cols=100 lines=30");//设置控制台的宽高
	printsnake(); //绘制字符蛇
	welcometogame(); // 绘制输入选项菜单
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:  
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
