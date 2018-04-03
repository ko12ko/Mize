//���⣺�Թ�
//���ߣ���ʷ��
//ʱ�䣺2014-2-2
//�汾��v3.0
//״̬�����
/*
������
���ý��棺ѡ���ѶȻ��˳�����20*20���е�40*40������60*60��
��Ϸ���棺w-�ϣ�a-��s-�£�d-�ң�i-���룬o-���棬q-����
��#��-ǽ���� ��-·����d��-�����š�s��-��㣬��e��-�յ㣬��0��-��ǰλ��
*/

#include<iostream>
#include<fstream>
#include<cstring>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<queue>

using namespace std;

//��Ϸ��ر���
#define MAP_MAX_LENGTH 80	//��ͼ��󳤶�
#define MAP_MAX_WIDTH 80	//��ͼ�����
#define FILENAME_MAX_LENGTH 512	//�ļ�����󳤶�
#define MODE_EASY 20		//��ģʽ
#define MODE_MIDDLE 40		//�е�ģʽ
#define MODE_HARD 60		//����ģʽ
#define CHAR_START 's'		//��ʼλ���ַ�
#define CHAR_END 'e'		//�յ�λ���ַ�
#define CHAR_NOW '@'		//��ǰλ���ַ�
#define CHAR_FLY_DOOR 'd'	//������
#define CHAR_WALL '#'		//ǽ
#define CHAR_WALL_FILE '#'	//ǽ���ļ���
#define CHAR_ROAD ' '		//·
#define CHAR_ROAD_FILE '.'	//·���ļ���
#define CHAR_UP 'w'			//��
#define CHAR_LEFT 'a'		//��
#define CHAR_DOWN 's'		//��
#define CHAR_RIGHT 'd'		//��
#define CHAR_IN 'i'			//����
#define CHAR_SAVE 'o'		//����
#define CHAR_QUIT 'q'		//�˳�

struct Position			//λ��
{
	int x;
	int y;
	bool operator==(Position pos)
	{
		return (x == pos.x) && (y == pos.y);
	}
};

//��Ϸ������Ϣ
char map[MAP_MAX_WIDTH][MAP_MAX_LENGTH];					//��ͼ
int map_flag[MAP_MAX_WIDTH][MAP_MAX_LENGTH];				//��Ǹ�����
int map_grids_number[MAP_MAX_WIDTH][MAP_MAX_LENGTH];		//�����ӱ��
Position first_grid[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//��һ������λ��
int sumOfBlocks;												//�������
bool canArriveBlocks[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//�ɴ���������
int gridsOfBlock[MAP_MAX_WIDTH * MAP_MAX_LENGTH];				//������ĸ�������
int flyDoorsOfBlock[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//������Ĵ������� 
int sumOfAllGrids;												//���п��߸�����
int sumOfZeroBlocks;											//û�пո��ӵ���������
queue<Position>Q;												//��ɴ�����ʱ�ĸ�������
vector<Position>V[MAP_MAX_WIDTH * MAP_MAX_LENGTH];			//ÿ����������и���λ��
int map_length, map_width;										//��ͼ���ȺͿ��
int direction[4][2] = { { -1,0 },{ 0,-1 },{ 1,0 },{ 0,1 } };		//��������
Position flyTo[MAP_MAX_WIDTH][MAP_MAX_LENGTH];				//����

															//��Ϸʱ��Ϣ
Position first, last, now;										//��ʼλ�ã�����λ�ã���ǰλ��
int step;														//�ڼ���
bool isWinner;													//�Ƿ�ʤ��


																///////////////////////////////////////��ʾģ��////////////////////////////////////////

																//���ܣ���ʾ��ͼ
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

//���ܣ���ʾѡ�����
void view_option()
{
	system("cls");
	printf("��Ϸ���֣��Թ�\n");
	printf("���ߣ���ʷ��\n");
	printf("�汾��v3.0\n");
	printf("��Ϸ������%c-�ϣ�%c-��%c-�£�%c-�ң�%c-���룬%c-���棬%c-����\n", CHAR_UP, CHAR_LEFT, CHAR_DOWN, CHAR_RIGHT, CHAR_IN, CHAR_SAVE, CHAR_QUIT);
	printf("��Ϸ��־��%c-ǽ��%c-·��%c-�����ţ�%c-��㣬%c-�յ㣬%c-��ǰλ��\n", CHAR_WALL, CHAR_ROAD, CHAR_FLY_DOOR, CHAR_START, CHAR_END, CHAR_NOW);
	printf("1.�Ѷ�ѡ��\n");
	printf("2.��ȡ\n");
	printf("0.�˳�\n");
	printf("��ѡ��");
}

//���ܣ���ʾ�Ѷ�ѡ�����
void view_difficult()
{
	system("cls");
	printf("�Ѷ�ѡ��\n");
	printf("1.�򵥣�%d*%d��\n", MODE_EASY, MODE_EASY);
	printf("2.�еȣ�%d*%d��\n", MODE_MIDDLE, MODE_MIDDLE);
	printf("3.���ѣ�%d*%d��\n", MODE_HARD, MODE_HARD);
	printf("0.����\n");
}

//���ܣ���Ϸ��ʼ����
void view_game()
{
	system("cls");
	printf("step: %d\n", step);
	view_show_map();
	printf("start: %d %d\n", first.x, first.y);
	printf("end: %d %d\n", last.x, last.y);
	printf("now: %d %d\n", now.x, now.y);
	printf("���������ı��ߣ�");
}

//���ܣ���Ϸ�������
void view_result()
{
	if (isWinner) printf("You are winner!( step = %d )\n", step);
	else printf("You are lost!\n");
}

//���ܣ�������Ϸ����
void view_save()
{
	printf("�Ƿ񱣴棿��y/n����");
}

//���ܣ�������Ϸ���ֽ���
void view_save_name()
{
	printf("�����ļ����֣�");
}

//���ܣ��Ƿ񸲸ǽ���
void view_save_cover()
{
	printf("�Ƿ񸲸ǣ���y/n����");
}

//���ܣ�����ɹ�����
void view_save_success()
{
	printf("����ɹ���\n");
	system("pause");
}

//���ܣ�����ʧ�ܽ���
void view_save_fail()
{
	printf("����ʧ�ܣ�\n");
	system("pause");
}

//���ܣ���ȡ��Ϸ����
void view_load()
{
	system("cls");
	printf("�Ƿ��ȡ����y/n����");
}

//���ܣ���ȡ��Ϸ���ֽ���
void view_load_name()
{
	printf("��ȡ�ļ����֣�");
}

//���ܣ�������ɵ�ͼ�ɹ�
void view_difficult_success()
{
	printf("������ɵ�ͼ�ɹ�!\n");
	system("pause");
}

//���ܣ�������ɵ�ͼʧ��
void view_difficult_fail()
{
	printf("������ɵ�ͼʧ��!\n");
	system("pause");
}

//���ܣ���ȡ�ɹ�����
void view_load_success()
{
	printf("��ȡ�ɹ�!\n");
	system("pause");
}

//���ܣ���ȡʧ�ܽ���
void view_load_fail()
{
	printf("��ȡʧ��!\n");
	system("pause");
}

//���ܣ��Ҳ����ļ�����
void view_cannot_find_file()
{
	printf("�Ҳ������ļ���\n");
	system("pause");
}

///////////////////////////////////////����ģ��////////////////////////////////////////

//�ж�posλ���Ƿ��ڵ�ͼ��
//����ֵ��true - pos�ڵ�ͼ�⣬ false - pos�ڵ�ͼ��
bool outOfMap(Position pos)
{
	if (pos.x >= 0 && pos.x < map_width && pos.y >= 0 && pos.y < map_length) return false;
	else return true;
}

//�����ڵ�ͼ��,����·��λ��
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

/////////////////////////////////����ģ��////////////////////////////////////////////

//���ܣ�����
//����ֵ��true-���棬false-����
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

//���ܣ���ȡ
//����ֵ��true-��ȡ��false-����
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

//���ܣ��Ѷ�ѡ��
//����ֵ��true-�Ѷ�������ɣ�false-����
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

/////////////////////////////////һ��ģ��////////////////////////////////////////////

//���ܣ�ѡ��
//����ֵ��1-�Ѷ����ã�2-��ȡ��0-�˳�����
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

//���ܣ��Ѷ�ѡ��ĳ�ʼ��
//����ֵ����
void init_difficult()
{
	Position pos_temp1, pos_temp2;
	int number;
	int temp1, temp2;
	//���������ͼ
	srand((unsigned)time(NULL));
	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_length; j++)
		{
			map[i][j] = rand() % 2 ? CHAR_WALL : CHAR_ROAD;
		}
	}
	//��ʼ������
	while (!Q.empty()) Q.pop();
	for (int i = 0; i < map_width * map_length; i++) V[i].clear();
	memset(map_flag, 0, sizeof(map_flag));
	memset(gridsOfBlock, 0, sizeof(gridsOfBlock));
	memset(map_grids_number, 0, sizeof(map_grids_number));
	memset(first_grid, -1, sizeof(first_grid));
	sumOfAllGrids = 0;
	sumOfBlocks = 0;
	//����ֻ��1�����ӵ�����
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
	//����������ͼ���ռ�����
	for (int i = 0; i < map_width; i++)
	{
		for (int j = 0; j < map_length; j++)
		{
			int k;
			if (map[i][j] == CHAR_WALL || map_flag[i][j] != 0) continue;
			//ÿ����������һ
			sumOfBlocks++;
			//����map_flag����,V��������
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
			//����first_grid���飬map_grids_number���飬gridsOfBlock���飬sumOfAllGrids������ÿ������һ������ĸ�������
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
	//��ʼ������
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
	//�Ƚ���һ��������Ϊ�ܵ��������ٽ������������һ��һ�����ӵ��ɵ�������
	canArriveBlocks[1] = true;
	for (int i = 2; i <= sumOfBlocks; i++)
	{
		int k;
		//�ҵ������ܵ����һ����������һ�����λ��
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
		//�ҵ��ܵ����һ����������һ�����λ��
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
		//������λ����Ϊ�ɻ��ൽ��Ĵ�����
		map[pos_temp1.x][pos_temp1.y] = map[pos_temp2.x][pos_temp2.y] = CHAR_FLY_DOOR;
		flyTo[pos_temp1.x][pos_temp1.y] = pos_temp2;
		flyTo[pos_temp2.x][pos_temp2.y] = pos_temp1;
		flyDoorsOfBlock[map_flag[pos_temp1.x][pos_temp1.y]] ++;
		if (flyDoorsOfBlock[map_flag[pos_temp1.x][pos_temp1.y]] == gridsOfBlock[map_flag[pos_temp1.x][pos_temp1.y]]) sumOfZeroBlocks++;
		flyDoorsOfBlock[map_flag[pos_temp2.x][pos_temp2.y]] ++;
		if (flyDoorsOfBlock[map_flag[pos_temp2.x][pos_temp2.y]] == gridsOfBlock[map_flag[pos_temp2.x][pos_temp2.y]]) sumOfZeroBlocks++;
		canArriveBlocks[map_flag[pos_temp1.x][pos_temp1.y]] = true;
	}
	//������ʼλ�ã��յ�λ�ã���ǰλ��
	first = randomPosition();
	map[first.x][first.y] = CHAR_START;
	last = randomPosition();
	now = first;
	map[last.x][last.y] = CHAR_END;
}

//���ܣ���ȡ�ĳ�ʼ��
//����ֵ����
void init_load()
{
}

//���ܣ���ʼ��
//������choic��1-�Ѷ�ѡ��ĳ�ʼ����2-��ȡ�ĳ�ʼ��
//����ֵ����
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

//���ܣ���ʼ��Ϸ
//����ֵ����
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
		case CHAR_UP:										//������
			pos_temp1.x = now.x + direction[0][0];
			pos_temp1.y = now.y + direction[0][1];
			break;
		case CHAR_LEFT:										//������
			pos_temp1.x = now.x + direction[1][0];
			pos_temp1.y = now.y + direction[1][1];
			break;
		case CHAR_DOWN:										//������
			pos_temp1.x = now.x + direction[2][0];
			pos_temp1.y = now.y + direction[2][1];
			break;
		case CHAR_RIGHT:									//������
			pos_temp1.x = now.x + direction[3][0];
			pos_temp1.y = now.y + direction[3][1];
			break;
		case CHAR_IN:										//����
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
		case CHAR_SAVE:										//����
			if (save())
			{
				view_save_success();
			}
			else
			{
				view_save_fail();
			}
			continue;
		default:											//��������
			continue;
		}
		if (!gameOver && !outOfMap(pos_temp1) && map[pos_temp1.x][pos_temp1.y] != CHAR_WALL)			//����һ��λ���ƶ�
		{
			now = pos_temp1;
			step++;
		}
	} while (!gameOver);
}

//���ܣ���Ϸ���
//����ֵ����
void result()
{
	view_result();
	system("pause");
}

///////////////////////////////////������///////////////////////////////////////////

//������
int main()
{
	int choic;
	do
	{
		choic = option();			//ѡ�����		����ֵ��1-�Ѷ�ѡ��ģʽ��2-��ȡģʽ��0-�˳�
		if (!choic) break;
		init(choic);				//��ʼ��
		game();						//��Ϸ��ʼ
		result();					//��Ϸ���
	} while (1);

	return 0;
}