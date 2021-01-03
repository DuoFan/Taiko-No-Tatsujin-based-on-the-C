#pragma once
#pragma comment(lib,"Winmm.lib")

#include<graphics.h>

#define Width 0
#define Height 1

//初始设置的结构定义
typedef struct _Setting {

	int resolution[2]{ 1024,768 }; 
	IMAGE backGround;
}_Setting;

//光标的结构定义
typedef struct _Cursor {

	int x;
	int y;
	IMAGE mask;
	IMAGE img;
}_Cursor;

//分数的数字的结构定义
typedef struct _Score_Number {

	int x;
	int y = 290;
	int value;
}_Score_Number;

//分数板的结构定义
typedef struct _ScoreBoard {

	int combo;//当前连击数
	int combo_Length;//当前连击数长度
	int max_Combo;//最大连击数
	int max_Combo_Length;
	int next_Change;//下一次的连击数长度改变值
	int current_Changed;
	int accuracy_Length;//精确度长度
	int hit300;//满判定数量
	int hit300_Length;//满判定长度
	int hit100;//半判定
	int hit100_Length;
	int hitMiss;//漏键
	int hitMiss_Length;
	int Rank;//结算等级
	float max_Weight = 100;//满判定权重
	float half_Weight = 50;
	float miss_Weight = 0;
	float accuracy;
	bool accuracy_Break;//精确度破坏标志
	_Score_Number* number;//数字数组
}_ScoreBoard;

void StartMenu();//开始菜单
void ChooseMenu();//选取菜单
void EndMenu();//结算菜单
