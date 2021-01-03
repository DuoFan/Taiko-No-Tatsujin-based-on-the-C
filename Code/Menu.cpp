#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include"Menu.h"
#include"Map.h"

#define Dot 10
#define Percent 11
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
int current = 0;
int musicAddress_Length;
int* Chosen = (int*)malloc(sizeof(int));

char* temp_Address;
char Input;

bool isChooseFirst = true;
bool isEndFirst = true;
bool isAuto = false;
static bool isCursorMove = true;

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
void PlayMusicRepeat(char*,int);
void PlayMusicOnce(char*, int);
void Draw_Auto();
void Draw_Hit300();
void Draw_Hit100();
void Draw_HitMiss();
void Draw_End_Combo();
void Draw_End_Accuarcy();
void DrawRank();

void StartMenu() {

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
		if (_kbhit())
			Status = 1;
		FlushBatchDraw();
}

void ChooseMenu() {
	
	int i,music_Length,composer_Length;
		
	putimage(0, 0, &Map[current].img);
	if (isAuto)
		Draw_Auto();
	settextcolor(WHITE);
	settextstyle(20, 10, "楷体");
	for (i = 0; i < Number_Map; i++) {
		music_Length = GetLength(Map[i].music_Name);
		composer_Length = GetLength(Map[i].composer);
		outtextxy(1024 / 2 - (music_Length + composer_Length + 4) / 2 * 10, 
			50 + 50 * i,Map[i].music_Name);
		outtextxy(1024 / 2 + (music_Length - composer_Length - 4) / 2 * 10 + 20,
			50 + 50 * i, Map[i].composer);
		outtextxy(1024 / 2 + (music_Length + composer_Length - 4) / 2 * 10 + 30,
			50 + 50 * i, Map[i].difficulty);
	}

	music_Length = GetLength(Map[current].music_Name);
	composer_Length = GetLength(Map[current].composer);
	Cursor.x = 1024 / 2 - (music_Length + composer_Length + 4) / 2 * 10 - 47;
	Cursor.y = 45 + 50 * current;
	putimage(Cursor.x, Cursor.y, &Cursor.mask, NOTSRCERASE);
	putimage(Cursor.x, Cursor.y, &Cursor.img, SRCINVERT);
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
		case 72:
			if (current > 0) {
				current--;
				isCursorMove = true;
			}
			break;
		case 80:
			if (current < Number_Map - 1) {
				current++;
				isCursorMove = true;
			}
			break;
		case 13:
			if (!isChooseFirst) {
				if (!isEndFirst) {
					int i = 1;
				}
				*Chosen = current;
				mciSendString("close tmpmusic", NULL, 0, NULL);
				Status = 2;
			}
			break;
		case 32:
			isAuto = !isAuto;
			break;
		}
	}
	if(isChooseFirst)
	isChooseFirst = !isChooseFirst;
	FlushBatchDraw();
}

void EndMenu() {

	putimage(0, 0, &EndBackGround);
	Draw_Hit300();
	Draw_Hit100();
	Draw_HitMiss();
	Draw_End_Combo();
	Draw_End_Accuarcy();
	DrawRank();
	if (_kbhit() && !isEndFirst) {
		Input = _getch();
		if (Input == 13) {
			Status = 1;
			isChooseFirst = true;
		}
	}
	if (isEndFirst)
		isEndFirst = !isEndFirst;
	FlushBatchDraw();
}

int GetLength(char* target) {

	int length = 0;
	while (target[length] != '\0' && target[length] != '\n')
		length++;
	return length;
}

void PlayMusicRepeat(char* fileName,int number) {

	mciSendString("close tmpmusic", NULL, 0, NULL);
	char cmdString[100] = "open ";
	temp_Address = CutAddress(fileName, number);
	strcat(cmdString,temp_Address);
	strcat(cmdString, " alias tmpmusic");
	mciSendString(cmdString, NULL, 0, NULL);
	mciSendString("play tmpmusic repeat", NULL, 0, NULL);
}

void PlayMusicOnce(char* fileName, int number) {

	mciSendString("close tmpmusic", NULL, 0, NULL);
	char cmdString[100] = "open ";
	temp_Address = CutAddress(fileName, number);
	strcat(cmdString, temp_Address);
	strcat(cmdString, " alias tmpmusic");
	mciSendString(cmdString, NULL, 0, NULL);
	mciSendString("play tmpmusic", NULL, 0, NULL);
}

char* CutAddress(char* fileName, int number) {

	char* temp = (char*)malloc(sizeof(char) * number);
	for (int i = 0; i < number; i++)
		sprintf(&temp[i], "%c", fileName[i]);
	return temp;
}

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