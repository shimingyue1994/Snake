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
int status, sleeptime = 200;	//蛇前进状态，每次运行的时间间隔
snake* head, * food;//蛇头指针，食物指针
snake* q; //遍历蛇的节点时用到的变量
int endgamestatus = 0;//游戏结束的情况：撞到墙；咬到自己；主动退出游戏
HANDLE hOut; //控制台句柄

/*******函  数  声  明 *******/
void gotoxy(int x, int y); //设置光标位置
int color(int c);//设置文字颜色
void printsnake();//字符画 蛇（首页）
void welcometogame();//开始界面（首页）
void createMap();// 绘制地图
void scoreandtips();//游戏界面右侧的得分和小提示
void initsnake();//初始化蛇身，画蛇身
void createfood();//创建并随机出现食物
int biteself();//判断是否咬到自己
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
	fopen_s(&fp, "save.txt", "a+");
	//扫描文件，从输入流中读取数据，第二个参数是format（输出的格式） ；第三个参数是保存地址
	fscanf_s(fp, "%d", &HighScore);
	//关闭打开的文件
	fclose(fp);
}

/**
 * 储存最高分进文件
 */
void File_in()
{
	FILE* fp;
	fopen_s(&fp,"save.txt", "w+");       //以读写的方式建立一个名为save.txt的文件
	fprintf(fp, "%d", score);           //把分数写进文件中
	fclose(fp);                         //关闭文件
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



/// <summary>
/// 创建地图区域
/// </summary>
void createMap()
{
	int i, j;
	for (i = 0; i < 58; i += 2)//绘制上下边框
	{
		gotoxy(i, 0); //上边框
		color(5);
		printf("□");
		gotoxy(i, 26);
		printf("□");
	}

	for (i = 1; i < 26; i++) //绘制左右的边框
	{
		gotoxy(0, i);//绘制左边框
		printf("□");
		gotoxy(56, i);//绘制右边框
		printf("□");
	}
	//打印中间的网格
	for (i = 2; i < 56; i += 2) //横向
	{
		for (j = 1; j < 26; j++)//纵向
		{
			gotoxy(i, j);
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
	gotoxy(64, 4);
	color(11);
	printf("☆最高记录☆：%d", HighScore);
	//得分
	gotoxy(64, 8);
	color(14);
	printf("得分：%d", score);

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

/// <summary>
/// 初始化蛇身
/// </summary>
void initsnake() {
	//先初始化蛇尾，因为是头插法生成链表
	snake* tail;
	int i;
	//为蛇尾对象分配内存 malloc 返回的是一个void类型的指针，需要强转成我们需要的类型
	tail = (snake*)malloc(sizeof(snake));
	tail->x = 24;  //从 24,5处开始绘制蛇身
	tail->y = 5;
	tail->next = NULL;
	//创建链表
	for ( i = 1; i <= 4; i++)
	{
		head = (snake*)malloc(sizeof(snake));//初始化一个新的蛇头
		head->next = tail; //蛇头的下一位是蛇位
		head->x = 24 + i * 2; //设置新的一节的位置
		head->y = 5;
		tail = head; // 新的一节又变成蛇尾， 然后重复循环，下一个新的蛇头节会指向它
	}

	//利用链表将蛇身打印出来
	while (tail!=NULL)//从头往前遍历，上面循环将tail赋值为蛇头节
	{
		gotoxy(tail->x, tail->y);
		color(14);
		printf("★");       //输出蛇身，蛇身使用★组成
		tail = tail->next; //输入下一位
	}
}

/// <summary>
/// 随机创建食物
/// </summary>
void createfood() {
	snake* food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	//保证其为偶数，使得食物能与蛇头对其，然后食物会出现在网格线上
	while (food_1->x%2!=0) 
	{
		food_1->x = rand() % 52 + 2; //x坐标范围是 2~53
	}

	food_1->y = rand() % 24 + 1;// y的坐标范围是1，24
	q = head;
	//判断是否与蛇身重合
	while (q->next == NULL)
	{
		if (q->x == food_1->x&&q->y == food_1->y)
		{
			free(food_1); // 如果重合就释放食物指针指向的内存
			createfood();//并重新创建食物
		}
		q = q->next;
	}
	gotoxy(food_1->x,food_1->y);
	food = food_1;
	color(12);
	printf("●");           //输出食物
}

/// <summary>
/// 判断是否咬到自己 蛇头坐标和蛇身任何一个坐标重合，就说明咬到自己
/// </summary>
int biteself() 
{
	snake* self;
	self = head->next;
	while (self->next!=NULL)
	{
		if (self->x==head->x && self->y == head->y)
		{
			return 1;
		}
		self = self->next;
	}

	return 0;
}
/// <summary>
/// 判断是否撞到墙
/// </summary>
void cantcrosswall() 
{
	if (head->x == 0 || head->x == 56||head->y==0||head->y == 26)
	{
		endgamestatus = 1;
		endgame();
	}
}

/// <summary>
/// 设置加速
/// </summary>
void speedup() 
{
	if (sleeptime>=50) // 如果延迟大于50
	{
		sleeptime = sleeptime - 10;//延时减小 移动速度增加
		add = add + 2; //得分
	}
}
/// <summary>
/// 设置减速
/// </summary>
void speeddown() 
{
	if (sleeptime<350)
	{
		sleeptime = sleeptime + 30;
		add = add + 2;
		if (sleeptime==350)
		{
			add = 1; 
		}
	}
}

/// <summary>
/// 蛇自动移动
/// </summary>
void snakemove()
{
	snake* nexthead;//下一个要出现的位置
	cantcrosswall();//判断是否撞墙
	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U) //向上前进时
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		nexthead->next = head;
		head = nexthead;
		q = head;
		//如果下一个位置上有食物，则蛇身增长，增加一个头，原来的蛇身的节点位置不变
		if (nexthead->x == food->x && nexthead->y == food->y)
		{
			while (q!=NULL)
			{
				gotoxy(q->x, q->y);
				color(14);
				printf("★");//原来食物的位置，从●换成★
				q = q->next;//指向下一个，继续循环绘制
			}
			score = score + add;//吃掉一个食物，在总分上加上食物的分
			speedup();//加速
			createfood();//重新创建一个food
		}
		else // 如果没有食物，则蛇身不变，往前移动，需要清除原来的蛇尾，在前进方向增加蛇头，其他4节点不变
		{
			while (q->next->next!=NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("★");
				q = q->next;
			}
			//清除原来的尾部 ,将原来的倒数第二个的next变为NULL,设为尾部,释放最后一个占用的内存
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}

	if (status == D) //下
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		nexthead->next = head;
		head = nexthead;
		q = head;
		//如果下一个位置上有食物，则蛇身增长
		if (nexthead->x == food->x && nexthead->y == food->y)
		{
			while (q != NULL)
			{
				gotoxy(q->x, q->y);
				color(14);
				printf("★");//原来食物的位置，从●换成★
				q = q->next;//指向下一个，继续循环绘制
			}
			score = score + add;//吃掉一个食物，在总分上加上食物的分
			speedup();//加速
			createfood();//重新创建一个food
		}
		else // 如果没有食物，则蛇身不变，往前移动，需要清除原来的蛇尾
		{
			while (q->next->next != NULL)
			{
				gotoxy(q->x, q->y);
				color(14);
				printf("★");
				q = q->next;
			}
			//清除原来的尾部 ,将原来的倒数第二个的next变为NULL,设为尾部,释放最后一个占用的内存
			gotoxy(q->next->x, q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == L) //左
	{
		nexthead->x = head->x-2;
		nexthead->y = head->y;
		nexthead->next = head;
		head = nexthead;
		q = head;
		//如果下一个位置上有食物，则蛇身增长
		if (nexthead->x == food->x && nexthead->y == food->y)
		{
			while (q != NULL)
			{
				gotoxy(q->x, q->y);
				color(14);
				printf("★");//原来食物的位置，从●换成★
				q = q->next;//指向下一个，继续循环绘制
			}
			score = score + add;//吃掉一个食物，在总分上加上食物的分
			speedup();//加速
			createfood();//重新创建一个food
		}
		else // 如果没有食物，则蛇身不变，往前移动，需要清除原来的蛇尾
		{
			while (q->next->next != NULL)
			{
				gotoxy(q->x, q->y);
				color(14);
				printf("★");
				q = q->next;
			}
			//清除原来的尾部 ,将原来的倒数第二个的next变为NULL,设为尾部,释放最后一个占用的内存
			gotoxy(q->next->x, q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}

	if (status == R) //左
	{
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
		nexthead->next = head;
		head = nexthead;
		q = head;
		//如果下一个位置上有食物，则蛇身增长
		if (nexthead->x == food->x && nexthead->y == food->y)
		{
			while (q != NULL)
			{
				gotoxy(q->x, q->y);
				color(14);
				printf("★");//原来食物的位置，从●换成★
				q = q->next;//指向下一个，继续循环绘制
			}
			score = score + add;//吃掉一个食物，在总分上加上食物的分
			speedup();//加速
			createfood();//重新创建一个food
		}
		else // 如果没有食物，则蛇身不变，往前移动，需要清除原来的蛇尾
		{
			while (q->next->next != NULL)
			{
				gotoxy(q->x, q->y);
				color(14);
				printf("★");
				q = q->next;
			}
			//清除原来的尾部 ,将原来的倒数第二个的next变为NULL,设为尾部,释放最后一个占用的内存
			gotoxy(q->next->x, q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}
	//咬到自己
	if (biteself()==1)
	{
		endgamestatus = 2;
		endgame();
	}
}
/// <summary>
/// 控制键盘输入
/// 注意：蛇方向转变时不能往相反的方向，比如向上时，则不能向下掉头
/// </summary>
void keyboardControl() 
{
	status = R; // 初始蛇向右移动
	while (1) //不断接受按键输入
	{
		scoreandtips();//游戏界面右侧的得分和小提示
		//当按下向上键，并且此时蛇的行进方向不是下时，可以掉头
		if (GetAsyncKeyState(VK_UP)&&status != D)//GetAsyncKeyState 返回按键的状态，如果按下了这个键，则返回1
		{
			status = U;
		}
		//下
		if (GetAsyncKeyState(VK_DOWN) && status != U)
		{
			status = D;
		}
		//左
		if (GetAsyncKeyState(VK_LEFT) && status != R)
		{
			status = L;
		}
		//右
		if (GetAsyncKeyState(VK_RIGHT) && status != L)
		{
			status = R;
		}

		//按下空格键 则暂停
		if (GetAsyncKeyState(VK_SPACE))
		{
			while (1)
			{
				Sleep(300);
				if (GetAsyncKeyState(VK_SPACE))
				{
					break;
				}
			}
		}
		//按下ESC键 退出
		else if (GetAsyncKeyState(VK_ESCAPE)) 
		{
			endgamestatus = 3;
		}
		else if (GetAsyncKeyState(VK_F1))
		{
			speedup();
		}
		else if (GetAsyncKeyState(VK_F2))
		{
			speeddown();
		}
		Sleep(sleeptime);
		snakemove();
	}
}
/**
 * 失败界面
 */
void Lostdraw()
{
	system("cls");
	int i;
	gotoxy(45, 2);
	color(6);
	printf("\\\\\\|///");
	gotoxy(43, 3);
	printf("\\\\");
	gotoxy(47, 3);
	color(15);
	printf(".-.-");
	gotoxy(54, 3);
	color(6);
	printf("//");

	gotoxy(44, 4);
	color(14);
	printf("(");

	gotoxy(47, 4);
	color(15);
	printf(".@.@");

	gotoxy(54, 4);
	color(14);
	printf(")");

	gotoxy(17, 5);
	color(11);
	printf("+------------------------");

	gotoxy(35, 5);
	color(14);
	printf("oOOo");

	gotoxy(39, 5);
	color(11);
	printf("----------");

	gotoxy(48, 5);
	color(14);
	printf("(_)");

	gotoxy(51, 5);
	color(11);
	printf("----------");

	gotoxy(61, 5);
	color(14);
	printf("oOOo");

	gotoxy(65, 5);
	color(11);
	printf("-----------------+");

	for (i = 6; i <= 19; i++)        //竖边框
	{
		gotoxy(17, i);
		printf("|");
		gotoxy(82, i);
		printf("|");
	}

	gotoxy(17, 20);
	printf("+---------------------------------");

	gotoxy(52, 20);
	color(14);
	printf("☆☆☆〃");

	gotoxy(60, 20);
	color(11);
	printf("----------------------+");

}

/**
 * 结束游戏
 */
void endgame()
{
	system("cls");
	if (endgamestatus == 1)
	{

		Lostdraw();
		gotoxy(35, 9);
		color(12);
		printf("对不起，您撞到墙了。游戏结束！");
	}
	else if (endgamestatus == 2)
	{

		Lostdraw();
		gotoxy(35, 9);
		color(12);
		printf("对不起，您咬到自己了。游戏结束！");
	}
	else if (endgamestatus == 3)
	{
		Lostdraw();
		gotoxy(40, 9);
		color(12);
		printf("您已经结束了游戏。");
	}
	gotoxy(43, 12);
	color(13);
	printf("您的得分是 %d", score);

	if (score >= HighScore)
	{
		color(10);
		gotoxy(33, 16);
		printf("创纪录啦！最高分被你刷新啦，真棒！！！");
		File_in();              //把最高分写进文件
	}
	else
	{
		color(10);
		gotoxy(33, 16);
		printf("继续努力吧~ 你离最高分还差：%d", HighScore - score);
	}
	choose();
}

/**
 * 边框下面的分支选项
 */
void choose()
{
	int n;
	gotoxy(25, 23);
	color(12);
	printf("我要重新玩一局-------1");
	gotoxy(52, 23);
	printf("不玩了，退出吧-------2");
	gotoxy(46, 25);
	color(11);
	printf("选择：");
	scanf_s("%d", &n);
	switch (n)
	{
	case 1:
		system("cls");          //清屏
		score = 0;                //分数归零
		sleeptime = 200;			//设定初始速度
		add = 10;				//使add设定为初值，吃一个食物得分10，然后累加
		printsnake();           //返回欢迎界面
		welcometogame();
		break;
	case 2:
		exit(0);                //退出游戏
		break;
	default:
		gotoxy(35, 27);
		color(12);
		printf("※※您的输入有误，请重新输入※※");
		system("pause >nul");
		endgame();
		choose();
		break;
	}

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
		initsnake();
		createfood();
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
int main()
{
	system("mode con cols=100 lines=30");//设置控制台的宽高
	printsnake(); //绘制字符蛇
	welcometogame(); // 绘制输入选项菜单
	File_out();//读取文件信息
	keyboardControl(); //按键控制以及蛇的移动
	endgame();
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
