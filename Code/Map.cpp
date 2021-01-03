#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"Map.h"

_Map Map[Number_Map]{ {"..\\Music\\POCARIKORCF\\Map.txt"},{"..\\Music\\CrescentMoon\\Map.txt"},{"..\\Music\\Pandemonium\\Map.txt"},
{"..\\Music\\YuugennoRan\\Map.txt"},{"..\\Music\\V^3\\Map.txt"}, };//谱面数组，存放所有谱面文件的相对地址

int imageAddress_Length;//图片地址长度
int noteAddress_Length;//note文件地址长度

extern char* temp_Address;
extern char* CutAddress(char*,int);//去掉字符串的休止符
extern int GetLength(char*);//得到当前字符串去掉休止符的长度

void ReadMap(int i)  //读取第i号铺面
{
	if (i == Number_Map)//当i等于谱面数时退出
		return;
	else {
		FILE* map,*note;//打开map文件
		map = fopen(Map[i].map_Address, "r");
		//从map文件读取对应的信息并存入对应的变量中
		fgets(Map[i].music_Address, 50, map);
		fgets(Map[i].music_Name, 50, map);
		fgets(Map[i].composer, 20, map);
		fgets(Map[i].note_Address, 50, map);
		fgets(Map[i].image_Address, 50, map);
		fgets(Map[i].difficulty, 5, map);
		fscanf(map, "%d %d", &Map[i].note_Number, &Map[i].timing_Number);
		
		//去掉difficulty的休止符，使之能正确输出
		*Map[i].difficulty = *CutAddress(Map[i].difficulty, 4);
		
		//去掉note文件地址的休止符，使之能正确打开
		noteAddress_Length = GetLength(Map[i].note_Address);
		temp_Address = CutAddress(Map[i].note_Address, noteAddress_Length);
		note = fopen(temp_Address,"r");
		
		//通过读取到的音符数来分配该谱面的note数组空间大小
		Map[i].note = (_Note *)malloc(Map[i].note_Number * sizeof(_Note));
		for (int j = 0; j < Map[i].note_Number; j++) {
			if (Map[i].note + j == NULL) {
				closegraph();
				printf("分配内存失败");
			}
			fscanf(note, "%f %d", &((Map[i].note[j]).timing), &((Map[i].note[j]).color));
		}
		
		//通过读取到的时间点数来分配该谱面的timing数组空间大小
		Map[i].timing = (_Timing*)malloc(Map[i].timing_Number * sizeof(_Timing));
		for (int j = 0; j < Map[i].timing_Number; j++) {
			if (Map[i].note + j == NULL) {
				closegraph();
				printf("分配内存失败");
			}
			fscanf(map, "%f %f %f", &((Map[i].timing[j]).value), &((Map[i].timing[j]).bpm)
				, &((Map[i].timing[j]).factor));
		}
		
		//去掉image文件地址的休止符，使之能正确被读取
		imageAddress_Length = GetLength(Map[i].image_Address);
		temp_Address = CutAddress(Map[i].image_Address, imageAddress_Length);
		loadimage(&Map[i].img, temp_Address);
		
		//关闭文件
		fclose(map);
		fclose(note);
		
		//读下一号谱面
		ReadMap(i + 1);
	}
}

