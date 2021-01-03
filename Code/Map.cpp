#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"Map.h"

_Map Map[Number_Map]{ {"..\\Music\\POCARIKORCF\\Map.txt"},{"..\\Music\\CrescentMoon\\Map.txt"},{"..\\Music\\Pandemonium\\Map.txt"},
{"..\\Music\\YuugennoRan\\Map.txt"},{"..\\Music\\V^3\\Map.txt"}, };

int imageAddress_Length;
int noteAddress_Length;

extern char* temp_Address;
extern char* CutAddress(char*,int);
extern int GetLength(char*);

void ReadMap(int i)  //读取第i号铺面
{
	if (i == Number_Map)
		return;
	else {
		FILE* map,*note;
		map = fopen(Map[i].map_Address, "r");
		fgets(Map[i].music_Address, 50, map);
		fgets(Map[i].music_Name, 50, map);
		fgets(Map[i].composer, 20, map);
		fgets(Map[i].note_Address, 50, map);
		fgets(Map[i].image_Address, 50, map);
		fgets(Map[i].difficulty, 5, map);
		fscanf(map, "%d %d", &Map[i].note_Number, &Map[i].timing_Number);

		*Map[i].difficulty = *CutAddress(Map[i].difficulty, 4);
		noteAddress_Length = GetLength(Map[i].note_Address);
		temp_Address = CutAddress(Map[i].note_Address, noteAddress_Length);
		note = fopen(temp_Address,"r");

		Map[i].note = (_Note *)malloc(Map[i].note_Number * sizeof(_Note));
		for (int j = 0; j < Map[i].note_Number; j++) {
			if (Map[i].note + j == NULL) {
				closegraph();
				printf("分配内存失败");
			}
			fscanf(note, "%f %d", &((Map[i].note[j]).timing), &((Map[i].note[j]).color));
		}

		Map[i].timing = (_Timing*)malloc(Map[i].timing_Number * sizeof(_Timing));
		for (int j = 0; j < Map[i].timing_Number; j++) {
			if (Map[i].note + j == NULL) {
				closegraph();
				printf("分配内存失败");
			}
			fscanf(map, "%f %f %f", &((Map[i].timing[j]).value), &((Map[i].timing[j]).bpm)
				, &((Map[i].timing[j]).factor));
		}

		imageAddress_Length = GetLength(Map[i].image_Address);
		temp_Address = CutAddress(Map[i].image_Address, imageAddress_Length);
		loadimage(&Map[i].img, temp_Address);

		fclose(map);
		fclose(note);

		ReadMap(i + 1);
	}
}

