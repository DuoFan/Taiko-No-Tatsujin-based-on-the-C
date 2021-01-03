#pragma once
#pragma comment(lib,"Winmm.lib")

#include<graphics.h>

#define Width 0
#define Height 1

typedef struct _Setting {

	int resolution[2]{ 1024,768 }; 
	IMAGE backGround;
}_Setting;

typedef struct _Cursor {

	int x;
	int y;
	IMAGE mask;
	IMAGE img;
}_Cursor;

typedef struct _Score_Number {

	int x;
	int y = 290;
	int value;
}_Score_Number;

typedef struct _ScoreBoard {

	int combo;
	int combo_Length;
	int max_Combo;
	int max_Combo_Length;
	int next_Change;
	int current_Changed;
	int accuracy_Length;
	int hit300;
	int hit300_Length;
	int hit100;
	int hit100_Length;
	int hitMiss;
	int hitMiss_Length;
	int Rank;
	float max_Weight = 100;
	float half_Weight = 50;
	float miss_Weight = 0;
	float accuracy;
	bool accuracy_Break;
	_Score_Number* number;
}_ScoreBoard;

void StartMenu();
void ChooseMenu();
void EndMenu();
void PauseMenu();