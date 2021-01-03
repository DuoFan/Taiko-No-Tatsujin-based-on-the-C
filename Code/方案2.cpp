#include<stdio.h>
#include<conio.h>
#include <time.h>
#include<math.h>
#include"Menu.h"
#include"Map.h"

#define Slider_Number 4
#define Dot 10
#define Percent 11

typedef struct _Slider {

	int width = 410;
	int speed = 2;
	int x;
	IMAGE hit;
	IMAGE fail;
}_Slider;

typedef struct _Bar {

	int y = 214;
	int bar_X = 181;
	int innerLeft_X = 31;
	int innerLeft_Y = y + 47;
	int innerRight_X = 31 + 54;
	int innerRight_Y = y + 47;
	int outerLeft_X = 16;
	int outerLeft_Y = y + 32;
	int outerRight_X = 15 + 70;
	int outerRight_Y = y + 32;
	IMAGE bar;
	IMAGE drum;
	IMAGE innerLeft;
	IMAGE innerLeft_mask;
	IMAGE innerRight;
	IMAGE innerRight_mask;
	IMAGE outerLeft;
	IMAGE outerLeft_mask;
	IMAGE outerRight;
	IMAGE outerRight_mask;
}_Bar;

typedef struct _Judge {

	int x = 181 + 50;
	int y = 214 + 50;
	int hit_great = 33;
	int hit_normal = 50;
	int hit_time;
	bool type;
	IMAGE point;
	IMAGE hit300;
	IMAGE hit300_Mask;
	IMAGE hit100;
	IMAGE hit100_Mask;
}_Judge;

typedef struct _Don {

	int x = 14;
	int y = 87;
	int next_Action;
	int next_Draw;
	float frequency;
	int status;
	IMAGE idle[12];
	IMAGE fail[4];
}_Don;

_Slider Slider[Slider_Number];
_Bar Bar;
_Judge Judge;
_Don Don;
_ScoreBoard ScoreBoard;

SYSTEMTIME current_Time;
SYSTEMTIME Start_Time;

IMAGE red;
IMAGE blue;
IMAGE normal;
extern IMAGE number[12];
extern IMAGE number_Mask[12];
extern IMAGE rank[6];
extern IMAGE rank_Mask[6];


extern _Setting Setting;
extern IMAGE Press, Press_Mask;
extern _Map Map[Number_Map];
extern _Cursor Cursor;
extern IMAGE EndBackGround;
extern IMAGE Auto[2];
extern int Status;
extern int* Chosen;
extern int musicAddress_Length;
extern char Input;
extern bool isAuto;

int Current = 0;
int current_Note;
int current_Timing;
int last_Slider;
int passed_Mileseconds;
int station;
int last_Input = 0;
int note_y = Judge.y + 13;
int drawHit;
int currentHit;
int nextInput;
int count_Down;
float note_Speed;

bool first = true;
bool isKeyDown = false;
bool canInput[4];
bool isPlay;
bool isEnd;

extern void PlayMusicOnce(char*, int);
extern int GetLength(char*);
extern char* CutAddress(char*, int);

int GetActualTime();

void Playing();
void Auto_Playing();
void Show();
void UpdateWithoutInput();
void Auto_Update();
void UpdateWithInput();

void StartUp();
void Load_BackGround();
void Load_Bar();
void Load_Judge();
void Load_Don();
void Load_Number();
void Load_Rank();
void Load_Cursor();
void Load_Auto();
void Load_End();

void First();
void Initialize_Slider();
void Initialize_Note();
void Search_Timing(int i);
void Initialize_Judge();
void Initialize_Key();
void Initialize_Don();
void Initialize_ScoreBoard();
void PlayMusic();

void Count_Down();
void ReleaseKey();
void Slider_Move();
void Act_Note();
void Note_Move();
void Put_Judge();
void Auto_Put_Judge();
void Update_Current_Timing();
void Update_Don_Frequency();
void Update_Accuarcy();
void Update_MaxCombo();
void ComboSetZero();
void Update_Don();
void GetComboLength();

void Draw_Slider();
void Draw_Bar();
void Draw_Combo();
void Draw_Accuarcy();
void Draw_Note();
void Draw_Don();
void DrawJudge(int);
void DrawHit(int);

void End();
void Get_Hit_Length();
void Get_Rank();


int main() {

	StartUp();
	while (1) {

		switch (Status)
		{
		case 0:
			StartMenu();
			break;
		case 1:
			ChooseMenu();
			break;
		case 2:
			if (!isAuto)
				Playing();
			else
				Auto_Playing();
			End();
			break;
		case 3:
			EndMenu();
			break;
		}
	}
}

int GetActualTime() {

	GetLocalTime(&current_Time);
	passed_Mileseconds = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
		- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
	return passed_Mileseconds;
}


void StartUp() {

	ReadMap(Current);
	mciSendString("open ..\\Music\\Setting\\BackGround\\ÓÄÐþ¥ÎÂÒ.mp3 alias backmusic", NULL, 0, NULL);
	mciSendString("play backmusic repeat", NULL, 0, NULL);

	initgraph(Setting.resolution[Width],
		Setting.resolution[Height]);

	Load_BackGround();
	Load_Bar();
	Load_Judge();
	Load_Don();
	Load_Number();
	Load_Rank();
	Load_Cursor();
	Load_Auto();
	Load_End();

	BeginBatchDraw();
}

void Load_BackGround() {

	loadimage(&Setting.backGround, "..\\Picture\\Begin\\background.jpeg");
	loadimage(&Press_Mask, "..\\Picture\\Begin\\PressMask.bmp");
	loadimage(&Press, "..\\Picture\\Begin\\Press.bmp");
}

void Load_Bar() {

	loadimage(&Bar.drum, "..\\Picture\\Bar\\drum.png");
	loadimage(&Bar.bar, "..\\Picture\\Bar\\bar.png");
	loadimage(&Bar.innerLeft, "..\\Picture\\Bar\\innerLeft.bmp");
	loadimage(&Bar.innerLeft_mask, "..\\Picture\\Bar\\innerLeft_mask.bmp");
	loadimage(&Bar.innerRight, "..\\Picture\\Bar\\innerRight.bmp");
	loadimage(&Bar.innerRight_mask, "..\\Picture\\Bar\\innerRight_mask.bmp");
	loadimage(&Bar.outerLeft, "..\\Picture\\Bar\\outerLeft.bmp");
	loadimage(&Bar.outerLeft_mask, "..\\Picture\\Bar\\outerLeft_mask.bmp");
	loadimage(&Bar.outerRight, "..\\Picture\\Bar\\outerRight.bmp");
	loadimage(&Bar.outerRight_mask, "..\\Picture\\Bar\\outerRight_mask.bmp");
}

void Load_Judge() {

	loadimage(&Judge.point, "..\\Picture\\Judge\\JudgePoint.png");
	loadimage(&Judge.hit300, "..\\Picture\\Judge\\hit300.bmp");
	loadimage(&Judge.hit300_Mask, "..\\Picture\\Judge\\hit300_mask.bmp");
	loadimage(&Judge.hit100, "..\\Picture\\Judge\\hit100.bmp");
	loadimage(&Judge.hit100_Mask, "..\\Picture\\Judge\\hit100_mask.bmp");
	loadimage(&red, "..\\Picture\\Circle\\Red.bmp");
	loadimage(&blue, "..\\Picture\\Circle\\Blue.bmp");
	loadimage(&normal, "..\\Picture\\Circle\\Small.bmp");
}

void Load_Don() {

	loadimage(&Don.idle[0], "..\\Picture\\Don\\Idle\\idle0_mask.bmp");
	loadimage(&Don.idle[1], "..\\Picture\\Don\\Idle\\idle0.bmp");
	loadimage(&Don.idle[2], "..\\Picture\\Don\\Idle\\idle1_mask.bmp");
	loadimage(&Don.idle[3], "..\\Picture\\Don\\Idle\\idle1.bmp");
	loadimage(&Don.idle[4], "..\\Picture\\Don\\Idle\\idle2_mask.bmp");
	loadimage(&Don.idle[5], "..\\Picture\\Don\\Idle\\idle2.bmp");
	loadimage(&Don.idle[6], "..\\Picture\\Don\\Idle\\idle3_mask.bmp");
	loadimage(&Don.idle[7], "..\\Picture\\Don\\Idle\\idle3.bmp");
	loadimage(&Don.idle[8], "..\\Picture\\Don\\Idle\\idle4_mask.bmp");
	loadimage(&Don.idle[9], "..\\Picture\\Don\\Idle\\idle4.bmp");
	loadimage(&Don.idle[10], "..\\Picture\\Don\\Idle\\idle5_mask.bmp");
	loadimage(&Don.idle[11], "..\\Picture\\Don\\Idle\\idle5.bmp");
	loadimage(&Don.fail[0], "..\\Picture\\Don\\Fail\\fail0_mask.bmp");
	loadimage(&Don.fail[1], "..\\Picture\\Don\\Fail\\fail0.bmp");
	loadimage(&Don.fail[2], "..\\Picture\\Don\\Fail\\fail1_mask.bmp");
	loadimage(&Don.fail[3], "..\\Picture\\Don\\Fail\\fail1.bmp");
}

void Load_Number() {

	loadimage(&number_Mask[0], "..\\Picture\\ScoreBoard\\score-0_mask.bmp");
	loadimage(&number[0], "..\\Picture\\ScoreBoard\\score-0.bmp");
	loadimage(&number_Mask[1], "..\\Picture\\ScoreBoard\\score-1_mask.bmp");
	loadimage(&number[1], "..\\Picture\\ScoreBoard\\score-1.bmp");
	loadimage(&number_Mask[2], "..\\Picture\\ScoreBoard\\score-2_mask.bmp");
	loadimage(&number[2], "..\\Picture\\ScoreBoard\\score-2.bmp");
	loadimage(&number_Mask[3], "..\\Picture\\ScoreBoard\\score-3_mask.bmp");
	loadimage(&number[3], "..\\Picture\\ScoreBoard\\score-3.bmp");
	loadimage(&number_Mask[4], "..\\Picture\\ScoreBoard\\score-4_mask.bmp");
	loadimage(&number[4], "..\\Picture\\ScoreBoard\\score-4.bmp");
	loadimage(&number_Mask[5], "..\\Picture\\ScoreBoard\\score-5_mask.bmp");
	loadimage(&number[5], "..\\Picture\\ScoreBoard\\score-5.bmp");
	loadimage(&number_Mask[6], "..\\Picture\\ScoreBoard\\score-6_mask.bmp");
	loadimage(&number[6], "..\\Picture\\ScoreBoard\\score-6.bmp");
	loadimage(&number_Mask[7], "..\\Picture\\ScoreBoard\\score-7_mask.bmp");
	loadimage(&number[7], "..\\Picture\\ScoreBoard\\score-7.bmp");
	loadimage(&number_Mask[8], "..\\Picture\\ScoreBoard\\score-8_mask.bmp");
	loadimage(&number[8], "..\\Picture\\ScoreBoard\\score-8.bmp");
	loadimage(&number_Mask[9], "..\\Picture\\ScoreBoard\\score-9_mask.bmp");
	loadimage(&number[9], "..\\Picture\\ScoreBoard\\score-9.bmp");
	loadimage(&number_Mask[Dot], "..\\Picture\\ScoreBoard\\score-dot_mask.bmp");
	loadimage(&number[Dot], "..\\Picture\\ScoreBoard\\score-dot.bmp");
	loadimage(&number_Mask[Percent], "..\\Picture\\ScoreBoard\\score-percent_mask.bmp");
	loadimage(&number[Percent], "..\\Picture\\ScoreBoard\\score-percent.bmp");
}

void Load_Rank() {

	loadimage(&rank_Mask[0], "..\\Picture\\End\\ranking-X_mask.bmp");
	loadimage(&rank[0], "..\\Picture\\End\\ranking-X.bmp");
	loadimage(&rank_Mask[1], "..\\Picture\\End\\ranking-S_mask.bmp");
	loadimage(&rank[1], "..\\Picture\\End\\ranking-S.bmp");
	loadimage(&rank_Mask[2], "..\\Picture\\End\\ranking-A_mask.bmp");
	loadimage(&rank[2], "..\\Picture\\End\\ranking-A.bmp");
	loadimage(&rank_Mask[3], "..\\Picture\\End\\ranking-B_mask.bmp");
	loadimage(&rank[3], "..\\Picture\\End\\ranking-B.bmp");
	loadimage(&rank_Mask[4], "..\\Picture\\End\\ranking-C_mask.bmp");
	loadimage(&rank[4], "..\\Picture\\End\\ranking-C.bmp");
	loadimage(&rank_Mask[5], "..\\Picture\\End\\ranking-D_mask.bmp");
	loadimage(&rank[5], "..\\Picture\\End\\ranking-D.bmp");
}

void Load_Cursor() {

	loadimage(&Cursor.mask, "..\\Picture\\Cursor\\Cursor_mask.bmp");
	loadimage(&Cursor.img, "..\\Picture\\Cursor\\Cursor.bmp");
}

void Load_Auto() {

	loadimage(&Auto[0], "..\\Picture\\Auto\\autoplay_mask.bmp");
	loadimage(&Auto[1], "..\\Picture\\Auto\\autoplay.bmp");
}

void Load_End() {

	loadimage(&EndBackGround, "..\\Picture\\End\\end.jpg");
}

void Playing() {

	if (first)
		First();
	if (_kbhit())
		UpdateWithInput();
	UpdateWithoutInput();
	Show();
}

void Auto_Playing() {

	if (first)
		First();
	Auto_Update();
	Show();
}

void Show() {

	Draw_Slider();
	Draw_Bar();
	Draw_Combo();
	Draw_Accuarcy();
	Draw_Note();
	Draw_Don();

	if (passed_Mileseconds - Judge.hit_time <= 100
		&& Judge.hit_time != 0)
		DrawJudge(Judge.type);

	if (passed_Mileseconds - last_Input <= 100)
		DrawHit(drawHit);

	FlushBatchDraw();
	//Sleep(1);
}

void UpdateWithoutInput() {

	if (!isPlay)
		Count_Down();
	ReleaseKey();
	Slider_Move();

	if (isPlay) {
		GetActualTime();
		Act_Note();
		Note_Move();
		Put_Judge();
		Update_MaxCombo();
		Update_Current_Timing();
		Update_Don();
	}

	if (ScoreBoard.current_Changed == ScoreBoard.next_Change)
		GetComboLength();
}

void Auto_Update() {

	if (!isPlay)
		Count_Down();
	Slider_Move();

	if (isPlay) {
		GetActualTime();
		Act_Note();
		Note_Move();
		Auto_Put_Judge();
		Update_MaxCombo();
		Update_Current_Timing();
		Update_Don();
	}

	
	if (ScoreBoard.current_Changed == ScoreBoard.next_Change)
		GetComboLength();
}

void UpdateWithInput() {

	if (GetAsyncKeyState(69) & 0x8000 && canInput[0]) {
		currentHit = 1;
		drawHit = 1;
		canInput[0] = false;
		GetLocalTime(&current_Time);
		last_Input = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
			- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
		PlaySound("..\\Music\\Setting\\Hit\\hitInner.wav", NULL, SND_ASYNC);
	}

	else if (GetAsyncKeyState(103) & 0x8000 && canInput[1]) {
		currentHit = 1;
		drawHit = 2;
		canInput[1] = false;
		GetLocalTime(&current_Time);
		last_Input = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
			- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
		PlaySound("..\\Music\\Setting\\Hit\\hitInner.wav", NULL, SND_ASYNC);
	}

	else if (GetAsyncKeyState(87) & 0x8000 && canInput[2]) {
		currentHit = 2;
		drawHit = 3;
		canInput[2] = false;
		GetLocalTime(&current_Time);
		last_Input = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
			- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
		PlaySound("..\\Music\\Setting\\Hit\\hitOuter.wav", NULL, SND_ASYNC);
	}

	else if (GetAsyncKeyState(104) & 0x8000 && canInput[3]) {
		currentHit = 2;
		drawHit = 4;
		canInput[3] = false;
		GetLocalTime(&current_Time);
		last_Input = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
			- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
		PlaySound("..\\Music\\Setting\\Hit\\hitOuter.wav", NULL, SND_ASYNC);
	}
}


void First() {

	Initialize_Slider();
	Initialize_Note();
	Initialize_Judge();
	Initialize_Key();
	Initialize_Don();
	Initialize_ScoreBoard();
	count_Down = 500;
	isEnd = false;
	first = false;
}

void Initialize_Slider() {

	for (int i = 0; i < Slider_Number; i++) {
		loadimage(&Slider[i].hit, "..\\Picture\\Slider\\taiko-slider.png");
		loadimage(&Slider[i].fail, "..\\Picture\\Slider\\taiko-slider-fail.png");
		Slider[i].x = 0 + i * Slider[i].width;
		if (i == Slider_Number - 1)
			last_Slider = Slider[i].x;
	}
}

void Initialize_Note() {

	int i;
	isPlay = false;
	note_Speed = 1;
	current_Note = 0;
	current_Timing = 0;
	float tempFactor;
	for (i = 0; i < Map[*Chosen].note_Number; i++) {
		Map[*Chosen].note[i].start = Map[*Chosen].note[i].timing;
		/*if (Map[*Chosen].note[i].start <= 1024)
			Map[*Chosen].note[i].act = true;
		else*/
			Map[*Chosen].note[i].act = false;
		Search_Timing(i);
		if (current_Timing >= Map[*Chosen].timing_Number - 1
			&& Map[*Chosen].note[i].start >= Map[*Chosen].timing[current_Timing].value) {
			tempFactor = ((Map[*Chosen].timing[current_Timing].bpm *
				Map[*Chosen].timing[current_Timing].factor) / 200);
			Map[*Chosen].note[i].speed = note_Speed * tempFactor;
			if(isAuto)
				Map[*Chosen].note[i].offset = 1065.5 / Map[*Chosen].note[i].speed;
			else
				Map[*Chosen].note[i].offset = 1024 / Map[*Chosen].note[i].speed;
		}
	}
	current_Timing = 0;
	station = 0;
}

void Search_Timing(int i) {

	if (current_Timing < Map[*Chosen].timing_Number - 1
		&& Map[*Chosen].note[i].start >= Map[*Chosen].timing[current_Timing].value
		&& Map[*Chosen].note[i].start < Map[*Chosen].timing[current_Timing + 1].value) {
		float tempFactor;
		tempFactor = ((Map[*Chosen].timing[current_Timing].bpm *
			Map[*Chosen].timing[current_Timing].factor) / 200);
		Map[*Chosen].note[i].speed = note_Speed * tempFactor;
		if(isAuto)
			Map[*Chosen].note[i].offset = 1065.5 / Map[*Chosen].note[i].speed;
		else
			Map[*Chosen].note[i].offset = 1024 / Map[*Chosen].note[i].speed;
		return;
	}
	else if (current_Timing >= Map[*Chosen].timing_Number - 1)
		return;
	else
		current_Timing++; 
	Search_Timing(i);
}

void Initialize_Judge() {

	Judge.hit_time = 0;
}

void Initialize_Key() {

	for (int i = 0; i < 4; i++)
		canInput[i] = true;
}

void Initialize_Don() {

	Don.next_Action = 0;
	Don.next_Draw = 0;
	Don.status = 0;
}

void Initialize_ScoreBoard() {

	ScoreBoard.combo = 0;
	ScoreBoard.current_Changed = 0;
	ScoreBoard.max_Combo = 0;
	ScoreBoard.next_Change = 10;
	ScoreBoard.combo_Length = 1;
	ScoreBoard.max_Combo_Length = 1;
	ScoreBoard.accuracy = 0;
	ScoreBoard.accuracy_Length = 4;
	ScoreBoard.number = (_Score_Number*)malloc(sizeof(ScoreBoard.number) * ScoreBoard.combo_Length);
	ScoreBoard.hit300 = 0;
	ScoreBoard.hit300_Length = 1;
	ScoreBoard.hit100 = 0;
	ScoreBoard.hit100_Length = 1;
	ScoreBoard.hitMiss = 0;
	ScoreBoard.hitMiss_Length = 1;
	ScoreBoard.accuracy_Break = false;
}

void PlayMusic() {

	musicAddress_Length = GetLength(Map[*Chosen].music_Address) - 1;
	PlayMusicOnce(Map[*Chosen].music_Address, musicAddress_Length);
	GetLocalTime(&Start_Time);
}


void Count_Down() {

	count_Down--;
	if (count_Down <= 0 && !isPlay) {
		PlayMusic();
		isPlay = true;
		
	}
}

void ReleaseKey() {

	if (!(GetAsyncKeyState(69) & 0x8000))
		canInput[0] = true;
	if (!(GetAsyncKeyState(103) & 0x8000))
		canInput[1] = true;
	if (!(GetAsyncKeyState(87) & 0x8000))
		canInput[2] = true;
	if (!(GetAsyncKeyState(104) & 0x8000))
		canInput[3] = true;
}

void Slider_Move() {

	for (int i = 0; i < Slider_Number; i++) {
		Slider[i].x -= Slider->speed;
		if (Slider[i].x + Slider->width < 0)
			Slider[i].x = last_Slider;
	}
}

void Act_Note() {

	for (int i = current_Note; i < Map[*Chosen].note_Number; i++) {
		if ((passed_Mileseconds >= Map[*Chosen].note[i].start - Map[*Chosen].note[i].offset)
			&& Map[*Chosen].note[i].act)
			continue;
		else if (passed_Mileseconds >= Map[*Chosen].note[i].start - Map[*Chosen].note[i].offset) {
			Map[*Chosen].note[i].timing = 1024;
			Map[*Chosen].note[i].act = true;
		}
		else
			break;
	}
}

void Note_Move() {

	if (passed_Mileseconds >= station) {
		station = passed_Mileseconds + 2;
		for (int i = current_Note; i < Map[*Chosen].note_Number; i++) {
			if (Map[*Chosen].note[i].act) {
				Map[*Chosen].note[i].timing -= 2 * Map[*Chosen].note[i].speed;
				if (Map[*Chosen].note[i].timing <= 181) {
					current_Note++;
				}
			}
			else
				break;
		}
	}
}

void Put_Judge() {

	if (current_Note == Map[*Chosen].note_Number && !isEnd) {
		if (Map[*Chosen].note[current_Note].timing <= Judge.hit_great &&
			Map[*Chosen].note[current_Note].timing >= -Judge.hit_great 
			&& currentHit == Map[*Chosen].note[current_Note].color) {
			currentHit = 0;
			ScoreBoard.combo++;
			GetLocalTime(&current_Time);
			Judge.hit_time = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
				- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
			Judge.type = true;
			Don.status = 0;
			ScoreBoard.hit300++;
			Update_Accuarcy();
			ScoreBoard.current_Changed++;
		}
		else if (Map[*Chosen].note[current_Note].timing <= Judge.hit_normal &&
			Map[*Chosen].note[current_Note].timing >= -Judge.hit_normal
			&& currentHit == Map[*Chosen].note[current_Note].color) {
			ScoreBoard.combo++;
			GetLocalTime(&current_Time);
			Judge.hit_time = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
				- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
			Judge.type = false;
			Don.status = 0;
			ScoreBoard.hit100++;
			Update_Accuarcy();
			ScoreBoard.current_Changed++;
		}
		else if (Map[*Chosen].note[current_Note].timing <= -190) {
			Don.status = -1;
			ScoreBoard.hitMiss++;
			Update_Accuarcy();
			Update_MaxCombo();
			ComboSetZero();
		}
	}
	if (current_Note < Map[*Chosen].note_Number) {
		if (Map[*Chosen].note[current_Note].timing <= 231 + Judge.hit_great &&
			Map[*Chosen].note[current_Note].timing >= 231 - Judge.hit_great
			&& currentHit == Map[*Chosen].note[current_Note].color) {
			ScoreBoard.combo++;
			GetLocalTime(&current_Time);
			Judge.hit_time = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
				- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
				current_Note++;
			Judge.type = true;
			Don.status = 0;
			ScoreBoard.hit300++;
			Update_Accuarcy();
			ScoreBoard.current_Changed++;
		}
		else if (Map[*Chosen].note[current_Note].timing <= 231 + Judge.hit_normal &&
			Map[*Chosen].note[current_Note].timing >= 231 - Judge.hit_normal
			&& currentHit == Map[*Chosen].note[current_Note].color) {
			ScoreBoard.combo++;
			GetLocalTime(&current_Time);
			Judge.hit_time = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
				- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
				current_Note++;
			Judge.type = false;
			Don.status = 0;
			ScoreBoard.hit100++;
			Update_Accuarcy();
			ScoreBoard.current_Changed++;
		}
		else if (Map[*Chosen].note[current_Note].timing <= 190) {
				current_Note++;
			Don.status = -1;
			ScoreBoard.hitMiss++;
			Update_Accuarcy();
			Update_MaxCombo();
			ComboSetZero();
		}
	}
	currentHit = 0;
}

void Auto_Put_Judge() {

	if (current_Note == Map[*Chosen].note_Number && !isEnd) {
		if (Map[*Chosen].note[current_Note].timing <= Judge.hit_great &&
			Map[*Chosen].note[current_Note].timing >= -Judge.hit_great) {
			ScoreBoard.combo++;
			GetLocalTime(&current_Time);
			Judge.hit_time = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
				- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
			Judge.type = true;
			if (Map[*Chosen].note[current_Note].color == 1)
				PlaySound("..\\Music\\Setting\\Hit\\hitInner.wav", NULL, SND_ASYNC);
			else
				PlaySound("..\\Music\\Setting\\Hit\\hitOuter.wav", NULL, SND_ASYNC);
			Don.status = 0;
			ScoreBoard.hit300++;
			Update_Accuarcy();
			ScoreBoard.current_Changed++;
		}
		else if (Map[*Chosen].note[current_Note].timing <= -190) {
			Don.status = -1;
			ScoreBoard.hitMiss++;
			Update_Accuarcy();
			Update_MaxCombo();
			ComboSetZero();
		}
	}
	else if(current_Note < Map[*Chosen].note_Number){
		if (Map[*Chosen].note[current_Note].timing <= 231 + Judge.hit_great &&
			Map[*Chosen].note[current_Note].timing >= 231 - Judge.hit_great) {
			ScoreBoard.combo++;
			GetLocalTime(&current_Time);
			Judge.hit_time = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
				- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
			current_Note++;
			Don.status = 0;
			Judge.type = true;
			if (Map[*Chosen].note[current_Note].color == 1)
				PlaySound("..\\Music\\Setting\\Hit\\hitInner.wav", NULL, SND_ASYNC);
			else
				PlaySound("..\\Music\\Setting\\Hit\\hitOuter.wav", NULL, SND_ASYNC);
			Don.status = 0;
			ScoreBoard.hit300++;
			Update_Accuarcy();
			ScoreBoard.current_Changed++;
		}
		else if (Map[*Chosen].note[current_Note].timing <= 190) {
			current_Note++;
			Don.status = -1;
			ScoreBoard.hitMiss++;
			Update_Accuarcy();
			Update_MaxCombo();
			ComboSetZero();
		}
	}
}

void Update_Current_Timing() {

	if (Map[*Chosen].note[current_Note].start >
		Map[*Chosen].timing[current_Timing].value
		&& current_Timing < Map[*Chosen].timing_Number) {
		Update_Don_Frequency();
		current_Timing++;
	}
}

void Update_Don_Frequency() {

	Don.frequency = 60 / (Map[*Chosen].timing[current_Timing].bpm
		* Map[*Chosen].timing[current_Timing].factor) * 1000;
}

void Update_Accuarcy() {

	ScoreBoard.accuracy = ((ScoreBoard.hit300 * ScoreBoard.max_Weight) +
		(ScoreBoard.hit100 * ScoreBoard.half_Weight) +
		(ScoreBoard.hitMiss * ScoreBoard.miss_Weight)) / current_Note;

	if (ScoreBoard.accuracy_Break)
		return;
	if (ScoreBoard.accuracy == 100)
		ScoreBoard.accuracy_Length = 5;
	else {
		ScoreBoard.accuracy_Length = 4;
		ScoreBoard.accuracy_Break = true;
	}
}

void Update_MaxCombo() {

	if (isEnd)
		int K = 0;
	if (ScoreBoard.combo > ScoreBoard.max_Combo) {
		ScoreBoard.max_Combo = ScoreBoard.combo;
		ScoreBoard.max_Combo_Length = ScoreBoard.combo_Length;
	}
}

void ComboSetZero() {

	ScoreBoard.combo = 0;
	ScoreBoard.combo_Length = 1;
	ScoreBoard.next_Change = 10;
	ScoreBoard.current_Changed = 0;
}

void Update_Don() {

	if (passed_Mileseconds - Don.next_Draw >= Don.frequency) {
		Don.next_Action += 2;
		Don.next_Draw = GetActualTime();
	}
}

void GetComboLength() {

	_Score_Number* temp = (_Score_Number*)malloc(sizeof(_Score_Number) * ScoreBoard.combo_Length);
	temp = ScoreBoard.number;
	ScoreBoard.combo_Length++;
	ScoreBoard.next_Change = pow(10, ScoreBoard.combo_Length);
	ScoreBoard.number = (_Score_Number*)malloc(sizeof(_Score_Number) * ScoreBoard.combo_Length);
	free(temp);
}


void Draw_Slider() {

	switch (Don.status) {
	case -1:
		for (int i = 0; i < Slider_Number; i++)
			putimage(Slider[i].x, 0, &Slider[i].fail);
		break;
	case 0:
		for (int i = 0; i < Slider_Number; i++)
			putimage(Slider[i].x, 0, &Slider[i].hit);
		break;
	}
}

void Draw_Bar() {

	putimage(0, Bar.y, &Bar.drum);
	putimage(Bar.bar_X, Bar.y, &Bar.bar);
	putimage(Judge.x, Judge.y, &Judge.point);
}

void Draw_Combo() {

	int temp;
	int interval = 150 / (ScoreBoard.combo_Length + 1);
	for (int i = 0; i < ScoreBoard.combo_Length; i++) {
		temp = ScoreBoard.combo_Length - i - 1;
		ScoreBoard.number[i].value = (int)(ScoreBoard.combo / pow(10, temp)) % 10;
		ScoreBoard.number[i].x = 10 + (i + 1) * interval - 20;
		putimage(ScoreBoard.number[i].x, 290, &number_Mask[ScoreBoard.number[i].value], NOTSRCERASE);
		putimage(ScoreBoard.number[i].x, 290, &number[ScoreBoard.number[i].value], SRCINVERT);
	}
}

void Draw_Accuarcy() {

	int temp_X, temp_Value, temp;
	int tempAccuracy = ScoreBoard.accuracy * 100;
	for (int i = 0; i < ScoreBoard.accuracy_Length; i++) {
		if (i >= ScoreBoard.accuracy_Length - 2)
			temp_X = 966 - (ScoreBoard.accuracy_Length - i) * 40;
		else
			temp_X = 966 - 25 - (ScoreBoard.accuracy_Length - i) * 40;

		temp = ScoreBoard.accuracy_Length - i - 1;
		temp_Value = (int)(tempAccuracy / pow(10, temp)) % 10;
		putimage(temp_X, 8, &number_Mask[temp_Value], NOTSRCERASE);
		putimage(temp_X, 8, &number[temp_Value], SRCINVERT);
	}
	putimage(861, 8, &number_Mask[Dot], NOTSRCERASE);
	putimage(861, 8, &number[Dot], SRCINVERT);
	putimage(966, 8, &number_Mask[Percent], NOTSRCERASE);
	putimage(966, 8, &number[Percent], SRCINVERT);
}

void Draw_Note() {

	for (int i = current_Note; i < Map[*Chosen].note_Number; i++) {
		if (Map[*Chosen].note[i].act) {
			putimage(Map[*Chosen].note[i].timing, note_y, &normal, NOTSRCERASE);
			if (Map[*Chosen].note[i].color == 1)
				putimage(Map[*Chosen].note[i].timing, note_y, &red, SRCINVERT);
			else
				putimage(Map[*Chosen].note[i].timing, note_y, &blue, SRCINVERT);
		}
	}
}

void Draw_Don() {

	switch (Don.status) {
	case -1:
		if (Don.next_Action >= 4)
			Don.next_Action = 0;
		putimage(Don.x, Don.y, &Don.fail[Don.next_Action], NOTSRCERASE);
		putimage(Don.x, Don.y, &Don.fail[Don.next_Action + 1], SRCINVERT);
		break;
	case 0:
		if (Don.next_Action >= 12)
			Don.next_Action = 0;
		putimage(Don.x, Don.y, &Don.idle[Don.next_Action], NOTSRCERASE);
		putimage(Don.x, Don.y, &Don.idle[Don.next_Action + 1], SRCINVERT);
		break;
	}
}

void DrawJudge(int type) {

	if (type == 1) {
		putimage(Judge.x - 58, Judge.y - 58, &Judge.hit300_Mask, NOTSRCERASE);
		putimage(Judge.x - 58, Judge.y - 58, &Judge.hit300, SRCINVERT);
	}

	else {
		putimage(Judge.x - 58, Judge.y - 58, &Judge.hit100_Mask, NOTSRCERASE);
		putimage(Judge.x - 58, Judge.y - 58, &Judge.hit100, SRCINVERT);
	}
}

void DrawHit(int hit) {

	switch (hit) {
	case 1:
		putimage(Bar.innerLeft_X, Bar.innerLeft_Y, &Bar.innerLeft_mask, NOTSRCERASE);
		putimage(Bar.innerLeft_X, Bar.innerLeft_Y, &Bar.innerLeft, SRCINVERT);
		break;
	case 2:
		putimage(Bar.innerRight_X, Bar.innerRight_Y, &Bar.innerRight_mask, NOTSRCERASE);
		putimage(Bar.innerRight_X, Bar.innerRight_Y, &Bar.innerRight, SRCINVERT);
		break;
	case 3:
		putimage(Bar.outerLeft_X, Bar.outerLeft_Y, &Bar.outerLeft_mask, NOTSRCERASE);
		putimage(Bar.outerLeft_X, Bar.outerLeft_Y, &Bar.outerLeft, SRCINVERT);
		break;
	case 4:
		putimage(Bar.outerRight_X, Bar.outerRight_Y, &Bar.outerRight_mask, NOTSRCERASE);
		putimage(Bar.outerRight_X, Bar.outerRight_Y, &Bar.outerRight, SRCINVERT);
		break;
	}
}


void End() {

	if (current_Note >= Map[*Chosen].note_Number) {
		if (!isEnd)
			isEnd = true;
		if (count_Down <= 0)
			count_Down = 1000;
		Don.status = 0;
		count_Down--;
		if (count_Down <= 0) {
			Get_Hit_Length();
			/*if (ScoreBoard.max_Combo < ScoreBoard.combo) {
				ScoreBoard.max_Combo_Length = ScoreBoard.combo_Length;
				ScoreBoard.max_Combo = ScoreBoard.combo;
			}
			if (ScoreBoard.hit300 == Map[*Chosen].note_Number) {
				ScoreBoard.accuracy = 100;
				ScoreBoard.hit100 = 0;
				ScoreBoard.hitMiss = 0;
			}*/
			Get_Rank();
			for (int i = 0; i < Map[*Chosen].note_Number; i++) {
				Map[*Chosen].note[i].timing = Map[*Chosen].note[i].start;
				Map[*Chosen].note[i].act = false;
			}
			Don.next_Draw = 0;
			Don.frequency = 0;
			current_Note = 0;
			current_Timing = 0;
			first = true;
			Status = 3;
		}

	}
}

void Get_Hit_Length() {

	if (ScoreBoard.hit300 < 10) {
		ScoreBoard.hit300_Length = 1;
	}
	else if (ScoreBoard.hit300 < 100) {
		ScoreBoard.hit300_Length = 2;
	}
	else if (ScoreBoard.hit300 < 1000) {
		ScoreBoard.hit300_Length = 3;
	}
	else if (ScoreBoard.hit300 < 10000) {
		ScoreBoard.hit300_Length = 4;
	}
	else if (ScoreBoard.hit300 < 100000) {
		ScoreBoard.hit300_Length = 5;
	}

	if (ScoreBoard.hit100 < 10) {
		ScoreBoard.hit100_Length = 1;
	}
	else if (ScoreBoard.hit100 < 100) {
		ScoreBoard.hit100_Length = 2;
	}
	else if (ScoreBoard.hit100 < 1000) {
		ScoreBoard.hit100_Length = 3;
	}
	else if (ScoreBoard.hit100 < 10000) {
		ScoreBoard.hit100_Length = 4;
	}
	else if (ScoreBoard.hit100 < 100000) {
		ScoreBoard.hit100_Length = 5;
	}

	if (ScoreBoard.hitMiss < 10) {
		ScoreBoard.hitMiss_Length = 1;
	}
	else if (ScoreBoard.hitMiss < 100) {
		ScoreBoard.hitMiss_Length = 2;
	}
	else if (ScoreBoard.hitMiss < 1000) {
		ScoreBoard.hitMiss_Length = 3;
	}
	else if (ScoreBoard.hitMiss < 10000) {
		ScoreBoard.hitMiss_Length = 4;
	}
	else if (ScoreBoard.hitMiss < 100000) {
		ScoreBoard.hitMiss_Length = 5;
	}
}

void Get_Rank() {

	if (ScoreBoard.accuracy > 99.99 &&
		ScoreBoard.max_Combo == Map[*Chosen].note_Number)
		ScoreBoard.Rank = 0;
	else if (ScoreBoard.accuracy >= 93.00 &&
		ScoreBoard.max_Combo == Map[*Chosen].note_Number)
		ScoreBoard.Rank = 1;
	else if (ScoreBoard.accuracy >= 93.00)
		ScoreBoard.Rank = 2;
	else if (ScoreBoard.accuracy >= 85)
		ScoreBoard.Rank = 3;
	else if (ScoreBoard.accuracy >= 80)
		ScoreBoard.Rank = 4;
	else
		ScoreBoard.Rank = 5;
}




