#pragma once
#include<graphics.h>
#define Number_Map 5

//时间点的结构定义
typedef struct _Timing {

	float value;//时间值，单位为毫秒
	float bpm;//beat per minute，歌曲速度
	float factor;//乘算因数，bpm * factor = 该时刻点真正的速度
}_Timing;

//音符的结构定义
typedef struct _Note {

	float start;//起始时间点
	float timing;//当前时间点
	float speed;//速度
	float offset;//延迟
	bool act;//行动状态
	unsigned int color;//颜色

}_Note;

//谱面的结构定义
typedef struct _Map { 

	const char* map_Address;//谱面文件地址
	int timing_Number;
	int note_Number; 
	char music_Address[50];//音乐文件地址
	char music_Name[50];
	char composer[20];
	char note_Address[50];//note文件地址
	char image_Address[50];//背景图文件地址
	char difficulty[5];//难度
	_Note* note;//音符数组
	_Timing* timing;//时间点数组
	IMAGE img;
}_Map;

void ReadMap(int);
