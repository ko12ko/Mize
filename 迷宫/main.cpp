//主题：迷宫
//作者：纪史超
//时间：2014-2-2
//版本：v3.0
//状态：完成
/*
概述：
设置界面：选择难度或退出（简单20*20，中等40*40，困难60*60）
游戏界面：w-上，a-左，s-下，d-右，i-进入，o-保存，q-认输
‘#’-墙，‘ ’-路，‘d’-传送门‘s’-起点，‘e’-终点，‘0’-当前位置
*/

#include<iostream>
#include<fstream>
#include<cstring>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<queue>

using namespace std;

//游戏相关变量
#define MAP_MAX_LENGTH 80	//地图最大长度
#define MAP_MAX_WIDTH 80	//地图最大宽度
#define FILENAME_MAX_LENGTH 512	//文件名最大长度
#define MODE_EASY 20		//简单模式
#define MODE_MIDDLE 40		//中等模式
#define MODE_HARD 60		//困难模式
#define CHAR_START 's'		//起始位置字符
#define CHAR_END 'e'		//终点位置字符
#define CHAR_NOW '@'		//当前位置字符
#define CHAR_FLY_DOOR 'd'	//传送门
#define CHAR_WALL '#'		//墙
#define CHAR_WALL_FILE '#'	//墙（文件）
#define CHAR_ROAD ' '		//路
#define CHAR_ROAD_FILE '.'	//路（文件）
#define CHAR_UP 'w'			//上
#define CHAR_LEFT 'a'		//左
#define CHAR_DOWN 's'		//下
#define CHAR_RIGHT 'd'		//右
#define CHAR_IN 'i'			//进入
#define CHAR_SAVE 'o'		//保存
#define CHAR_QUIT 'q'		//退出

struct Position			//位置
{
	int x;
	int y;
	bool operator==(Position pos)
	{
		return (x == pos.x) && (y == pos.y);
	}
};

//游戏基本信息
char map[MAP_MAX_WIDTH][MAP_MAX_LENGTH];					//地图
int map_flag[MAP_MAX_WIDTH][MAP_MAX_LENGTH];				//标记各区域
int map_grids_number[MAP_MAX_WIDTH][MAP_MAX_LENGTH];		//各格子编号
Position first_grid[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//第一个格子位置
int sumOfBlocks;												//区域个数
bool canArriveBlocks[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//可达区域数组
int gridsOfBlock[MAP_MAX_WIDTH * MAP_MAX_LENGTH];				//该区域的格子总数
int flyDoorsOfBlock[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//该区域的传送门数 
int sumOfAllGrids;												//所有可走格子数
int sumOfZeroBlocks;											//没有空格子的区域总数
queue<Position>Q;												//求可达区域时的辅助队列
vector<Position>V[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//每个区域的所有格子位置
int map_length, map_width;										//地图长度和宽度
int direction[4][2] = { { -1,0 },{ 0,-1 },{ 1,0 },{ 0,1 } };		//方向数组
Position flyTo[MAP_MAX_WIDTH][MAP_MAX_LENGTH];				//传送

															//游戏时信息
Position first, last, now;										//起始位置，结束位置，当前位置
int step;														//第几步
bool isWinner;													//是否胜利


																///////////////////////////////////////显示模块////////////////////////////////////////

																//功能：显示地图
void view_show_map()
{
	printf("     ");
	for (int i = 0; i < map_length; i++) printf("%1d", i / 10);
	printf("\n");
	printf("     ");
	for (int i = 0; i < map_length; i++) printf("%1d", i % 10);
	printf("\n");
	printf("    ");
	for (int i = 0; i < map_length + 2; i++) printf("%c", CHAR_WALL);
	printf("\n");
	for (int i = 0; i < map_width; i++)
	{
		printf("%02d  %c", i, CHAR_WALL);
		for (int j = 0; j < map_length; j++)
		{
			if (now.x == i && now.y == j) printf("%c", CHAR_NOW);
			else printf("%c", map[i][j]);
		}
		printf("%c", CHAR_WALL);
		printf("\n");
	}
	printf("    ");
	for (int i = 0; i < map_length + 2; i++) printf("%c", CHAR_WALL);
	printf("\n");
}

//功能：显示选项界面
void view_option()
{
	system("cls");
	printf("游戏名字：迷宫\n");
	printf("作者：纪史超\n");
	printf("版本：v3.0\n");
	printf("游戏操作：%c-上，%c-左，%c-下，%c-右，%c-进入，%c-保存，%c-认输\n", CHAR_UP, CHAR_LEFT, CHAR_DOWN, CHAR_RIGHT, CHAR_IN, CHAR_SAVE, CHAR_QUIT);
	printf("游戏标志：%c-墙，%c-路，%c-传送门，%c-起点，%c-终点，%c-当前位置\n", CHAR_WALL, CHAR_ROAD, CHAR_FLY_DOOR, CHAR_START, CHAR_END, CHAR_NOW);
	printf("1.难度选择\n");
	printf("2.读取\n");
	printf("0.退出\n");
	printf("请选择：");
}

//功能：显示难度选择界面
void view_difficult()
{
	system("cls");
	printf("难度选择：\n");
	printf("1.简单（%d*%d）\n", MODE_EASY, MODE_EASY);
	printf("2.中等（%d*%d）\n", MODE_MIDDLE, MODE_MIDDLE);
	printf("3.困难（%d*%d）\n", MODE_HARD, MODE_HARD);
	printf("0.返回\n");
}

//功能：游戏开始界面
void view_game()
{
	system("cls");
	printf("step: %d\n", step);
	view_show_map();
	printf("start: %d %d\n", first.x, first.y);
	printf("end: %d %d\n", last.x, last.y);
	printf("now: %d %d\n", now.x, now.y);
	printf("请输入向哪边走：");
}

//功能：游戏结果界面
void view_result()
{
	if (isWinner) printf("You are winner!( step = %d )\n", step);
	else printf("You are lost!\n");
}

//功能：保存游戏界面
void view_save()
{
	printf("是否保存？（y/n）：");
}

//功能：保存游戏名字界面
void view_save_name()
{
	printf("保存文件名字：");
}

//功能：是否覆盖界面
void view_save_cover()
{
	printf("是否覆盖？（y/n）：");
}

//功能：保存成功界面
void view_save_success()
{
	printf("保存成功！\n");
	system("pause");
}

//功能：保存失败界面
void view_save_fail()
{
	printf("保存失败！\n");
	system("pause");
}

//功能：读取游戏界面
void view_load()
{
	system("cls");
	printf("是否读取？（y/n）：");
}

//功能：读取游戏名字界面
void view_load_name()
{
	printf("读取文件名字：");
}

//功能：随机生成地图成功
void view_difficult_success()
{
	printf("随机生成地图成功!\n");
	system("pause");
}

//功能：随机生成地图失败
void view_difficult_fail()
{
	printf("随机生成地图失败!\n");
	system("pause");
}

//功能：读取成功界面
void view_load_success()
{
	printf("读取成功!\n");
	system("pause");
}

//功能：读取失败界面
void view_load_fail()
{
	printf("读取失败!\n");
	system("pause");
}

//功能：找不到文件界面
void view_cannot_find_file()
{
	printf("找不到该文件！\n");
	system("pause");
}

///////////////////////////////////////其它模块////////////////////////////////////////

//判断pos位置是否在地图外
//返回值：true - pos在地图外， false - pos在地图内
bool outOfMap(Position pos)
{
	if (pos.x >= 0 && pos.x < map_width && pos.y >= 0 && pos.y < map_length) return false;
	else return true;
}

//产生在地图内,且是路的位置
Position randomPosition()
{
	Position pos;
	srand((unsigned)time(NULL));
	do
	{
		pos.x = rand() % map_width;
		pos.y = rand() % map_length;
	} while (map[pos.x][pos.y] != CHAR_ROAD);
	return pos;
}

/////////////////////////////////二级模块////////////////////////////////////////////

//功能：保存
//返回值：true-保存，false-返回
bool save()
{
	bool isQuit;
	bool isSave;
	char choic;
	char temp;
	char fileName[512];
	ofstream f, f_temp;
	isQuit = false;
	isSave = false;
	do
	{
		do
		{
			view_save();
			fflush(stdin);
			scanf_s("%c", &choic);
		} while (choic != 'y' && choic != 'n');
		switch (choic)
		{
		case 'y':
			view_save_name();
			fflush(stdin);
			scanf_s("%s", fileName);				// modify by jishichao on 2017.10.15
			f_temp.open(fileName, ios::out | ios::_Noreplace);
			if (!f_temp)
			{
				view_save_cover();
				do
				{
					fflush(stdin);
					scanf_s("%c", &temp);
				} while (temp != 'y' && temp != 'n');
				if (temp == 'n')
				{
					f_temp.close();
					isSave = false;
					isQuit = false;
					break;
				}
			}
			f_temp.close();
			f.open(fileName, ios::out);
			if (!f)
			{
				f.close();
				isSave = false;
				isQuit = false;
				break;
			}
			f << map_width << ' ' << map_length << endl;
			for (int i = 0; i < map_width; i++)
			{
				for (int j = 0; j < map_length; j++)
				{
					if (map[i][j] == CHAR_WALL)
					{
						f << CHAR_WALL_FILE;
					}
					else if (map[i][j] == CHAR_ROAD)
					{
						f << CHAR_ROAD_FILE;
					}
					else
					{
						f << map[i][j];
					}
				}
				f << endl;
			}
			for (int i = 0; i < map_width; i++)
			{
				for (int j = 0; j < map_length; j++)
				{
					f << flyTo[i][j].x << ' ' << flyTo[i][j].y << "  ";
				}
				f << endl;
			}
			f << first.x << ' ' << first.y << "  " << last.x << ' ' << last.y << "  " << now.x << ' ' << now.y << endl;
			f.close();
			isSave = true;
			isQuit = true;
			break;
		case 'n':
			isSave = false;
			isQuit = true;
			break;
		default:
			exit(1);
		}
	} while (!isQuit);
	return isSave;
}

//功能：读取
//返回值：true-读取，false-返回
bool load()
{
	bool isLoad;
	bool isQuit;
	char choic;
	char fileName[512];
	ifstream f;
	isLoad = false;
	isQuit = false;
	do
	{
		do
		{
			view_load();
			fflush(stdin);
			scanf_s("%c", &choic);
		} while (choic != 'y' && choic != 'n');
		switch (choic)
		{
		case 'y':
			view_load_name();
			fflush(stdin);
			scanf_s("%s", fileName);						// modify by jishichao on 2017.10.15
			f.open(fileName, ios::in);
			if (!f)
			{
				view_cannot_find_file();
				f.close();
				isLoad = false;
				isQuit = false;
				break;
			}
			f >> map_width >> map_length;
			for (int i = 0; i < map_width; i++)
			{
				for (int j = 0; j < map_length; j++)
				{
					f >> map[i][j];
					if (map[i][j] == CHAR_WALL_FILE)
					{
						map[i][j] = CHAR_WALL;
					}
					else if (map[i][j] == CHAR_ROAD_FILE)
					{
						map[i][j] = CHAR_ROAD;
					}
				}
			}
			for (int i = 0; i < map_width; i++)
			{
				for (int j = 0; j < map_length; j++)
				{
					f >> flyTo[i][j].x >> flyTo[i][j].y;
				}
			}
			f >> first.x >> first.y >> last.x >> last.y >> now.x >> now.y;
			f.close();
			isLoad = true;
			isQuit = true;
			break;
		case 'n':
			isLoad = false;
			isQuit = true;
			break;
		default:
			exit(1);
		}
	} while (!isQuit);
	return isLoad;
}

//功能：难度选择
//返回值：true-难度设置完成，false-返回
bool difficult()
{
	int choic;
	fflush(stdin);
	do
	{
		view_difficult();
		if (cin >> choic, !cin)
		{
			cin.clear();
			while (cin.get() != '\n');
			choic = -1;
		}
	} while (choic < 0 || choic > 3);
	switch (choic)
	{
	case 1:
		map_length = map_width = MODE_EASY;
		break;
	case 2:
		map_length = map_width = MODE_MIDDLE;
		break;
	case 3:
		map_length = map_width = MODE_HARD;
		break;
	case 0:
		return false;
	default:
		exit(1);
	}
	return true;
}

/////////////////////////////////一级模块////////////////////////////////////////////

//功能：选择
//返回值：1-难度设置，2-读取，0-退出程序
int option()
{
	bool isQuit;
	int choic;
	isQuit = false;
	do
	{
		fflush(stdin);
		do
		{
			view_option();
			if (cin >> choic, !cin)
			{
				cin.clear();
				while (cin.get() != '\n');
				choic = -1;
			}
		} while (choic < 0 || choic > 3);
		switch (choic)
		{
		case 1:
			if (difficult())
			{
				view_difficult_success();
				isQuit = true;
			}
			else
			{
				view_difficult_fail();
				isQuit = false;
			}
			break;
		case 2:
			if (load())
			{
				view_load_success();
				isQuit = true;
			}
			else
			{
				view_load_fail();
				isQuit = false;
			}
			break;
		case 0:
			isQuit = true;
			break;
		default:
			exit(1);
		}
	} while (!isQuit);
	return choic;
}

//功能：难度选择的初始化
//返回值：无
void init_difficult()
{
	Position pos_temp1, pos_temp2;
	int number;
	int temp1, temp2;
	//创建随机地图
	srand((unsigned)time(NULL));
	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_length; j++)
		{
			map[i][j] = rand() % 2 ? CHAR_WALL : CHAR_ROAD;
		}
	}
	//初始化数据
	while (!Q.empty()) Q.pop();
	for (int i = 0; i < map_width * map_length; i++) V[i].clear();
	memset(map_flag, 0, sizeof(map_flag));
	memset(gridsOfBlock, 0, sizeof(gridsOfBlock));
	memset(map_grids_number, 0, sizeof(map_grids_number));
	memset(first_grid, -1, sizeof(first_grid));
	sumOfAllGrids = 0;
	sumOfBlocks = 0;
	//消除只有1个格子的区域
	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_length; j++)
		{
			int k;
			if (map[i][j] == CHAR_WALL) continue;
			for (k = 0; k < 4; k++)
			{
				pos_temp1.x = i + direction[k][0];
				pos_temp1.y = j + direction[k][1];
				if (!outOfMap(pos_temp1) && map[pos_temp1.x][pos_temp1.y] == CHAR_ROAD)
				{
					break;
				}
			}
			if (k >= 4)
			{
				map[i][j] = CHAR_WALL;
			}
		}
	}
	//遍历整个地图，收集数据
	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_length; j++)
		{
			int k;
			if (map[i][j] == CHAR_WALL || map_flag[i][j] != 0) continue;
			//每次区域数加一
			sumOfBlocks++;
			//设置map_flag数组,V向量数组
			pos_temp1.x = i;
			pos_temp1.y = j;
			map_flag[pos_temp1.x][pos_temp1.y] = sumOfBlocks;
			V[sumOfBlocks].push_back(pos_temp1);
			Q.push(pos_temp1);
			while (!Q.empty())
			{
				pos_temp1 = Q.front();
				Q.pop();
				for (k = 0; k < 4; k++)
				{
					pos_temp2.x = pos_temp1.x + direction[k][0];
					pos_temp2.y = pos_temp1.y + direction[k][1];
					if (!outOfMap(pos_temp2) && map[pos_temp2.x][pos_temp2.y] == CHAR_ROAD && map_flag[pos_temp2.x][pos_temp2.y] == 0)
					{
						map_flag[pos_temp2.x][pos_temp2.y] = sumOfBlocks;
						V[sumOfBlocks].push_back(pos_temp2);
						Q.push(pos_temp2);
					}
				}
			}
			//设置first_grid数组，map_grids_number数组，gridsOfBlock数组，sumOfAllGrids变量（每次增加一个区域的格子数）
			number = 0;
			pos_temp1.x = i;
			pos_temp1.y = j;
			first_grid[sumOfBlocks] = pos_temp1;
			map_grids_number[pos_temp1.x][pos_temp1.y] = ++number;
			Q.push(pos_temp1);
			while (!Q.empty())
			{
				pos_temp1 = Q.front();
				Q.pop();
				for (k = 0; k < 4; k++)
				{
					pos_temp2.x = pos_temp1.x + direction[k][0];
					pos_temp2.y = pos_temp1.y + direction[k][1];
					if (!outOfMap(pos_temp2) && map[pos_temp2.x][pos_temp2.y] == CHAR_ROAD && map_grids_number[pos_temp2.x][pos_temp2.y] == 0)
					{
						map_grids_number[pos_temp2.x][pos_temp2.y] = ++number;
						Q.push(pos_temp2);
					}
				}
			}
			gridsOfBlock[sumOfBlocks] = number;
			sumOfAllGrids += gridsOfBlock[sumOfBlocks];
		}
	}
	//初始化数据
	pos_temp1.x = pos_temp1.y = -1;
	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_length; j++)
		{
			flyTo[i][j] = pos_temp1;
		}
	}
	memset(canArriveBlocks, false, sizeof(canArriveBlocks));
	memset(flyDoorsOfBlock, 0, sizeof(flyDoorsOfBlock));
	sumOfZeroBlocks = 0;
	now.x = now.y = -1;
	//先将第一个区域设为能到达区域，再将其它区域随机一个一个连接到可到达区域
	canArriveBlocks[1] = true;
	for (int i = 2; i <= sumOfBlocks; i++)
	{
		int k;
		//找到还不能到达的一个随机区域的一个随机位置
		temp1 = rand() % (sumOfBlocks - i + 1) + 1;
		k = 1;
		for (int j = 1; j <= sumOfBlocks; j++)
		{
			if (!canArriveBlocks[j])
			{
				if (k == temp1)
				{
					temp1 = j;
					break;
				}
				else k++;
			}
		}
		do
		{
			temp2 = rand() % gridsOfBlock[temp1] + 1;
			pos_temp1 = V[temp1][temp2 - 1];
		} while (map[pos_temp1.x][pos_temp1.y] != CHAR_ROAD);
		//找到能到达的一个随机区域的一个随机位置
		temp1 = rand() % (i - 1 - sumOfZeroBlocks) + 1;
		k = 1;
		for (int j = 1; j <= sumOfBlocks; j++)
		{
			if (canArriveBlocks[j] && gridsOfBlock[j] > flyDoorsOfBlock[j])
			{
				if (k == temp1)
				{
					temp1 = j;
					break;
				}
				else k++;
			}
		}
		temp2 = rand() % (gridsOfBlock[temp1] - flyDoorsOfBlock[temp1]) + 1;
		k = 1;
		for (int j = 1; j <= gridsOfBlock[temp1]; j++)
		{
			if (map[V[temp1][j - 1].x][V[temp1][j - 1].y] == CHAR_ROAD)
			{
				if (k == temp2)
				{
					temp2 = j;
					break;
				}
				else k++;
			}
		}
		pos_temp2 = V[temp1][temp2 - 1];
		//将两个位置设为可互相到达的传送门
		map[pos_temp1.x][pos_temp1.y] = map[pos_temp2.x][pos_temp2.y] = CHAR_FLY_DOOR;
		flyTo[pos_temp1.x][pos_temp1.y] = pos_temp2;
		flyTo[pos_temp2.x][pos_temp2.y] = pos_temp1;
		flyDoorsOfBlock[map_flag[pos_temp1.x][pos_temp1.y]] ++;
		if (flyDoorsOfBlock[map_flag[pos_temp1.x][pos_temp1.y]] == gridsOfBlock[map_flag[pos_temp1.x][pos_temp1.y]]) sumOfZeroBlocks++;
		flyDoorsOfBlock[map_flag[pos_temp2.x][pos_temp2.y]] ++;
		if (flyDoorsOfBlock[map_flag[pos_temp2.x][pos_temp2.y]] == gridsOfBlock[map_flag[pos_temp2.x][pos_temp2.y]]) sumOfZeroBlocks++;
		canArriveBlocks[map_flag[pos_temp1.x][pos_temp1.y]] = true;
	}
	//设置起始位置，终点位置，当前位置
	first = randomPosition();
	map[first.x][first.y] = CHAR_START;
	last = randomPosition();
	now = first;
	map[last.x][last.y] = CHAR_END;
}

//功能：读取的初始化
//返回值：无
void init_load()
{
}

//功能：初始化
//参数：choic：1-难度选择的初始化，2-读取的初始化
//返回值：无
void init(int choic)
{
	switch (choic)
	{
	case 1:
		init_difficult();
		break;
	case 2:
		init_load();
		break;
	default:
		exit(1);
	}
}

//功能：开始游戏
//返回值：无
void game()
{
	char dir;
	bool gameOver = false;
	Position pos_temp1;
	isWinner = false;
	step = 0;
	do
	{
		view_game();
		fflush(stdin);
		scanf_s("%c", &dir);
		if (dir == CHAR_QUIT) return;
		switch (dir)
		{
		case CHAR_UP:										//向上走
			pos_temp1.x = now.x + direction[0][0];
			pos_temp1.y = now.y + direction[0][1];
			break;
		case CHAR_LEFT:										//向左走
			pos_temp1.x = now.x + direction[1][0];
			pos_temp1.y = now.y + direction[1][1];
			break;
		case CHAR_DOWN:										//向下走
			pos_temp1.x = now.x + direction[2][0];
			pos_temp1.y = now.y + direction[2][1];
			break;
		case CHAR_RIGHT:									//向右走
			pos_temp1.x = now.x + direction[3][0];
			pos_temp1.y = now.y + direction[3][1];
			break;
		case CHAR_IN:										//进入
			if (now == last)
			{
				isWinner = true;
				gameOver = true;
			}
			else if (flyTo[now.x][now.y].x != -1 && flyTo[now.x][now.y].y != -1)
			{
				pos_temp1.x = flyTo[now.x][now.y].x;
				pos_temp1.y = flyTo[now.x][now.y].y;
			}
			else continue;
			break;
		case CHAR_SAVE:										//保存
			if (save())
			{
				view_save_success();
			}
			else
			{
				view_save_fail();
			}
			continue;
		default:											//输入有误
			continue;
		}
		if (!gameOver && !outOfMap(pos_temp1) && map[pos_temp1.x][pos_temp1.y] != CHAR_WALL)			//向下一个位置移动
		{
			now = pos_temp1;
			step++;
		}
	} while (!gameOver);
}

//功能：游戏结果
//返回值：无
void result()
{
	view_result();
	system("pause");
}

///////////////////////////////////主程序///////////////////////////////////////////

//主函数
int main()
{
	int choic;
	do
	{
		choic = option();			//选择界面		返回值：1-难度选择模式，2-读取模式，0-退出
		if (!choic) break;
		init(choic);				//初始化
		game();						//游戏开始
		result();					//游戏结果
	} while (1);

	return 0;
}