#pragma once
#include<graphics.h>
#define Number_Map 5

typedef struct _Timing {

	float value;
	float bpm;
	float factor;
}_Timing;

typedef struct _Note {

	float start;
	float timing;
	float speed;
	float offset;
	bool act;
	unsigned int color;

}_Note;

typedef struct _Map { 

	const char* map_Address;
	int timing_Number;
	int note_Number; 
	char music_Address[50];
	char music_Name[50];
	char composer[20];
	char note_Address[50];
	char image_Address[50];
	char difficulty[5];
	_Note* note;
	_Timing* timing;
	IMAGE img;
}_Map;

void ReadMap(int);