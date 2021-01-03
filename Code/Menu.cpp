#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include"Menu.h"
#include"Map.h"

#define Dot 10 //点号(.)的代数
#define Percent 11 //百分号(%)的代数
//各个结算等级的代数
#define SS 0 
#define S 1
#define A 2
#define B 3
#define C 4
#define D 5


extern _Map Map[Number_Map];
extern _ScoreBoard ScoreBoard;
static int time = 0;
int Status = 0; //0为开始界面，1为选歌界面，2为游戏界面，3为结算界面
int current = 0;//当前的谱面号码
int musicAddress_Length;
int* Chosen = (int*)malloc(sizeof(int)); //分配一个int空间

char* temp_Address;//临时地址
char Input;//输入

bool isChooseFirst = true;//是否是第一次进入选曲界面
bool isEndFirst = true;//是否是第一次进入结算界面
bool isAuto = false;//是否开启自动验奏
static bool isCursorMove = true;//光标是否有移动

//相关设置和素材的变量定义
_Setting Setting;
IMAGE Press, Press_Mask;
IMAGE EndBackGround;
IMAGE number[12];
IMAGE number_Mask[12];
IMAGE rank[6];
IMAGE rank_Mask[6];
IMAGE Auto[2];
_Cursor Cursor;

int GetLength(char*);
char* CutAddress(char*, int);
void PlayMusicRepeat(char*,int);//重复播放音乐
void PlayMusicOnce(char*, int);//单次播放音乐
//相关素材的绘制
void Draw_Auto();
void Draw_Hit300();
void Draw_Hit100();
void Draw_HitMiss();
void Draw_End_Combo();
void Draw_End_Accuarcy();
void DrawRank();

void StartMenu() {
		
		//给定PressAnykey的闪烁周期
		if (time <= 2000)
			time++;
		if (time == 1)
			putimage(0, 0, &Setting.backGround);
		else if (time == 1000)
		{
			putimage(Setting.resolution[Width] / 2 - 100, Setting.resolution[Height] - 70,
				&Press_Mask, NOTSRCERASE);
			putimage(Setting.resolution[Width] / 2 - 100, Setting.resolution[Height] - 70,
				&Press, SRCINVERT);

		}
		else if (time == 2000)
			time = 0;
		if (_kbhit())//接收到任意键，进入选曲界面
			Status = 1;
		FlushBatchDraw();
}

void ChooseMenu() {
	
	int i,music_Length,composer_Length;
		
	putimage(0, 0, &Map[current].img);//绘制当前谱面的背景图
	if (isAuto)//开启自动时，绘制自动图标
		Draw_Auto();
	settextcolor(WHITE);
	settextstyle(20, 10, "楷体");
	//通过各自的长度，计算每张谱面的音乐,作曲家及难度的对应位置，并显示
	for (i = 0; i < Number_Map; i++) {
		music_Length = GetLength(Map[i].music_Name);
		composer_Length = GetLength(Map[i].composer);
		//1024为屏幕长度的像素数量，10为每个字符的宽度，50为每行的间距
		outtextxy(1024 / 2 - (music_Length + composer_Length + 4) / 2 * 10, 
			50 + 50 * i,Map[i].music_Name);
		outtextxy(1024 / 2 + (music_Length - composer_Length - 4) / 2 * 10 + 20,
			50 + 50 * i, Map[i].composer);
		outtextxy(1024 / 2 + (music_Length + composer_Length - 4) / 2 * 10 + 30,
			50 + 50 * i, Map[i].difficulty);
	}
	
	//得到当前谱面的音乐及作曲家长度，并将光标显示在其左边
	music_Length = GetLength(Map[current].music_Name);
	composer_Length = GetLength(Map[current].composer);
	Cursor.x = 1024 / 2 - (music_Length + composer_Length + 4) / 2 * 10 - 47;
	Cursor.y = 45 + 50 * current;
	putimage(Cursor.x, Cursor.y, &Cursor.mask, NOTSRCERASE);
	putimage(Cursor.x, Cursor.y, &Cursor.img, SRCINVERT);
	
	//当光标有移动时，关闭背景音乐，重复播放当前谱面的音乐
	if (isCursorMove == true) {
		mciSendString("close backmusic", NULL, 0, NULL);
		musicAddress_Length = GetLength(Map[current].music_Address) - 1;
		PlayMusicRepeat(Map[current].music_Address, musicAddress_Length);
		isCursorMove = false;
	}

	if (_kbhit()) {
		Input = _getch();
		switch (Input)
		{
		case 72://Down键，向下移动光标
			if (current > 0) {
				current--;
				isCursorMove = true;
			}
			break;
		case 80://Up键，向上移动光标
			if (current < Number_Map - 1) {
				current++;
				isCursorMove = true;
			}
			break;
		case 13://回车键，按下回车键进入游玩界面
			//通过isChooseFirst防止过度响应
			if (!isChooseFirst) {
				*Chosen = current;
				mciSendString("close tmpmusic", NULL, 0, NULL);
				Status = 2;
			}
			break;
		case 32://空格键，开关自动演奏
			isAuto = !isAuto;
			break;
		}
	}
	//防止过度响应后设置其为假
	if(isChooseFirst)
	isChooseFirst = !isChooseFirst;
	FlushBatchDraw();
}

void EndMenu() {
	
	//绘制结算图以及应该有的元素
	putimage(0, 0, &EndBackGround);
	Draw_Hit300();
	Draw_Hit100();
	Draw_HitMiss();
	Draw_End_Combo();
	Draw_End_Accuarcy();
	DrawRank();
	//当输入回车键后回到选曲界面，通过isEndFirst防止过度响应
	if (_kbhit() && !isEndFirst) {
		Input = _getch();
		if (Input == 13) {
			Status = 1;
			isChooseFirst = true;
		}
	}
	
	//防止过度响应后将其设置为假
	if (isEndFirst)
		isEndFirst = !isEndFirst;
	FlushBatchDraw();
}

int GetLength(char* target) {

	int length = 0;
	while (target[length] != '\0' && target[length] != '\n')//如果输入字符串的当前位不为休止符或换行符
		length++;//则长度加一
	return length;
}

void PlayMusicRepeat(char* fileName,int number) {
	
	//通过strcat将必要的字符串加到cmdString的末尾，并最终重复播放音乐
	mciSendString("close tmpmusic", NULL, 0, NULL);
	char cmdString[100] = "open ";
	temp_Address = CutAddress(fileName, number);
	strcat(cmdString,temp_Address);
	strcat(cmdString, " alias tmpmusic");
	mciSendString(cmdString, NULL, 0, NULL);
	mciSendString("play tmpmusic repeat", NULL, 0, NULL);
}

void PlayMusicOnce(char* fileName, int number) {
	
	//通过strcat将必要的字符串加到cmdString的末尾，并最终单次播放音乐
	mciSendString("close tmpmusic", NULL, 0, NULL);
	char cmdString[100] = "open ";
	temp_Address = CutAddress(fileName, number);
	strcat(cmdString, temp_Address);
	strcat(cmdString, " alias tmpmusic");
	mciSendString(cmdString, NULL, 0, NULL);
	mciSendString("play tmpmusic", NULL, 0, NULL);
}

char* CutAddress(char* fileName, int number) {
	
	//输入字符串后，返回一个去掉休止符的版本
	char* temp = (char*)malloc(sizeof(char) * number);
	for (int i = 0; i < number; i++)
		sprintf(&temp[i], "%c", fileName[i]);
	return temp;
}

//在各自的位置绘制各自的素材，用掩码将不必要的部分遮去
void Draw_Auto() {
	
	putimage(0, 708, &Auto[0], NOTSRCERASE);
	putimage(0, 708, &Auto[1], SRCINVERT);
}

void Draw_Hit300() {

	int temp_X, temp_Value, temp;
	for (int i = 0; i < ScoreBoard.hit300_Length; i++) {
		temp = ScoreBoard.hit300_Length - i - 1;
		temp_Value = (int)(ScoreBoard.hit300 / pow(10, temp)) % 10;
		temp_X = 87 + i * 40;
		putimage(temp_X, 165, &number_Mask[temp_Value], NOTSRCERASE);
		putimage(temp_X, 165, &number[temp_Value], SRCINVERT);
	}
}

void Draw_Hit100() {

	int temp_X, temp_Value, temp;
	for (int i = 0; i < ScoreBoard.hit100_Length; i++) {
		temp = ScoreBoard.hit100_Length - i - 1;
		temp_Value = (int)(ScoreBoard.hit100 / pow(10, temp)) % 10;
		temp_X = 87 + i * 40;
		putimage(temp_X, 269, &number_Mask[temp_Value], NOTSRCERASE);
		putimage(temp_X, 269, &number[temp_Value], SRCINVERT);
	}
}

void Draw_HitMiss() {

	int temp_X, temp_Value, temp;
	for (int i = 0; i < ScoreBoard.hitMiss_Length; i++) {
		temp = ScoreBoard.hitMiss_Length - i - 1;
		temp_Value = (int)(ScoreBoard.hitMiss / pow(10, temp)) % 10;
		temp_X = 454 + i * 40;
		putimage(temp_X, 371, &number_Mask[temp_Value], NOTSRCERASE);
		putimage(temp_X, 371, &number[temp_Value], SRCINVERT);
	}
}

void Draw_End_Combo() {

	int temp;
	int interval = 150 / (ScoreBoard.max_Combo_Length + 1);
	for (int i = 0; i < ScoreBoard.max_Combo_Length; i++) {
		temp = ScoreBoard.max_Combo_Length - i - 1;
		ScoreBoard.number[i].value = (int)(ScoreBoard.max_Combo / pow(10, temp)) % 10;
		ScoreBoard.number[i].x = 80 + (i + 1) * interval - 20;
		putimage(ScoreBoard.number[i].x, 510, &number_Mask[ScoreBoard.number[i].value], NOTSRCERASE);
		putimage(ScoreBoard.number[i].x, 510, &number[ScoreBoard.number[i].value], SRCINVERT);
	}
}

void Draw_End_Accuarcy() {

	int temp_X, temp_Value, temp;
	int tempAccuracy = ScoreBoard.accuracy * 100;
	for (int i = 0; i < ScoreBoard.accuracy_Length; i++) {
		if (i >= ScoreBoard.accuracy_Length - 2)
			temp_X = 550 - (ScoreBoard.accuracy_Length - i) * 40;
		else
			temp_X = 550 - 25 - (ScoreBoard.accuracy_Length - i) * 40;
		temp = ScoreBoard.accuracy_Length - i - 1;
		temp_Value = (int)(tempAccuracy / pow(10, temp)) % 10;
		putimage(temp_X, 510, &number_Mask[temp_Value], NOTSRCERASE);
		putimage(temp_X, 510, &number[temp_Value], SRCINVERT);
	}
	putimage(445, 510, &number_Mask[Dot], NOTSRCERASE);
	putimage(445, 510, &number[Dot], SRCINVERT);
	putimage(550, 510, &number_Mask[Percent], NOTSRCERASE);
	putimage(550, 510, &number[Percent], SRCINVERT);
}

void DrawRank() {

	switch (ScoreBoard.Rank) {
	case SS:
		putimage(650, 115, &rank_Mask[SS], NOTSRCERASE);
		putimage(650, 115, &rank[SS],SRCINVERT);
		break;
	case S:
		putimage(650, 115, &rank_Mask[S], NOTSRCERASE);
		putimage(650, 115, &rank[S], SRCINVERT);
		break;
	case A:
		putimage(650, 115, &rank_Mask[A], NOTSRCERASE);
		putimage(650, 115, &rank[A], SRCINVERT);
		break;
	case B:
		putimage(650, 115, &rank_Mask[B], NOTSRCERASE);
		putimage(650, 115, &rank[B], SRCINVERT);
		break;
	case C:
		putimage(650, 115, &rank_Mask[C], NOTSRCERASE);
		putimage(650, 115, &rank[C], SRCINVERT);
		break;
	case D:
		putimage(650, 115, &rank_Mask[D], NOTSRCERASE);
		putimage(650, 115, &rank[D], SRCINVERT);
		break;
	}

}
