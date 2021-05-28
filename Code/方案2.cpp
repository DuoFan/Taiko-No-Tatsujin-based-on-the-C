#include<stdio.h>
#include<conio.h>
#include <time.h>
#include<math.h>
#include"Menu.h"
#include"Map.h"

#define Slider_Number 4 //总共四个滑条
#define Dot 10
#define Percent 11

//滑条的结构定义
typedef struct _Slider {

	int width = 410;
	int speed = 2;//移动速度，每次循环移动两个像素点
	int x;
	IMAGE hit;//当击中时的素材
	IMAGE fail;//当漏击时的素材
}_Slider;

//长条的结构定义
typedef struct _Bar {

	int y = 214;
	int bar_X = 181;//长条的x轴位置
	//内部和外部的左右侧四个键各自的x，y轴位置
	int innerLeft_X = 31;
	int innerLeft_Y = y + 47;
	int innerRight_X = 31 + 54;
	int innerRight_Y = y + 47;
	int outerLeft_X = 16;
	int outerLeft_Y = y + 32;
	int outerRight_X = 15 + 70;
	int outerRight_Y = y + 32;
	//对应的素材
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

//判定的结构定义
typedef struct _Judge {
	
	//判定点的显示位置
	int x = 181 + 50;
	int y = 214 + 50;
	//不同判定等级的各个区间
	int hit_great = 33;
	int hit_normal = 50;
	int hit_time;//击中时刻点
	bool type;//判定类型
	IMAGE point;
	IMAGE hit300;
	IMAGE hit300_Mask;
	IMAGE hit100;
	IMAGE hit100_Mask;
}_Judge;

//小人的结构定义
typedef struct _Don {
	
	//显示位置
	int x = 14;
	int y = 87;
	int next_Action;//下一次的动作
	int next_Draw;//下一次绘制时间点
	float frequency;//行动频率
	int status;//行动状态
	IMAGE idle[12];//空闲时的素材
	IMAGE fail[4];//漏击时的素材
}_Don;

_Slider Slider[Slider_Number];
_Bar Bar;
_Judge Judge;
_Don Don;
_ScoreBoard ScoreBoard;

//得到当前的时间和游戏开始时间，是游戏运行的关键变量
SYSTEMTIME current_Time;
SYSTEMTIME Start_Time;

//必要素材的变量定义和引入
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

int Current = 0;//当前的谱面编号
int current_Note;//当前的音符编号
int current_Timing;//当前的时间点编号
int last_Slider;//记录最后的滑条的x轴位置
int passed_Mileseconds;//当前经过的时间长度，单位是毫秒
int station;//时间站点
int last_Input = 0;//最后一次输入的时间点
int note_y = Judge.y + 13;//音符的y轴位置
int drawHit;//根据位置记录输入类型
int currentHit;//根据颜色记录输入类型
int count_Down;//倒计时
float note_Speed;//音符移动的标准速度，对应100bpm

bool first = true;//初始化标志
bool canInput[4];//限制输入，防止过度响应
bool isPlay;//游玩标志
bool isEnd;//结束标志

extern void PlayMusicOnce(char*, int);
extern int GetLength(char*);
extern char* CutAddress(char*, int);

int GetActualTime();//更新时间

void Playing();//游玩
void Auto_Playing();//自动验奏
void Show();//显示
void UpdateWithoutInput();//无需输入的数据更新
void Auto_Update();//自动演奏时的数据更新
void UpdateWithInput();//接收输入时的数据更新

void StartUp();//程序启动后的加载
//加载对应素材
void Load_BackGround();
void Load_Bar();
void Load_Judge();
void Load_Don();
void Load_Number();
void Load_Rank();
void Load_Cursor();
void Load_Auto();
void Load_End();

void First();//初始化函数
//初始化各个变量的数据
void Initialize_Slider();
void Initialize_Note();
void Search_Timing(int i);//搜寻合适的时间点
void Initialize_Judge();
void Initialize_Key();
void Initialize_Don();
void Initialize_ScoreBoard();
void PlayMusic();//播放对应的音乐

void Count_Down();//倒计时
void ReleaseKey();//释放按键
void Slider_Move();//滑条的左移
void Act_Note();//使能音符
void Note_Move();//音符的左移
void Put_Judge();//给出判定
void Auto_Put_Judge();//自动演奏时的给出判定
void Update_Current_Timing();//更新当前的时间点
void Update_Don_Frequency();//更新小人的动作频率
void Update_Accuarcy();//更新准确度
void Update_MaxCombo();//更新最大连击
void ComboSetZero();//连击置零
void Update_Don();//更新小人的数据
void GetComboLength();//得到连击数的长度

//在相应的位置绘制相应的数据和素材
void Draw_Slider();
void Draw_Bar();
void Draw_Combo();
void Draw_Accuarcy();
void Draw_Note();
void Draw_Don();
void DrawJudge(int);
void DrawHit(int);

void End();//结束
void Get_Hit_Length();//得到每级判定数的长度
void Get_Rank();//得到对应的等级


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
	//当前经过的时间等于当前时间的毫秒数减去开始时间的毫秒数
	passed_Mileseconds = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
		- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
	return passed_Mileseconds;
}


void StartUp() {

	ReadMap(Current);//从0号谱面开始读取谱面数据
	mciSendString("open ..\\Music\\Setting\\BackGround\\ÓÄÐþ¥ÎÂÒ.mp3 alias backmusic", NULL, 0, NULL);//音乐名乱码了，正确显示为:幽玄ノ乱
	mciSendString("play backmusic repeat", NULL, 0, NULL);//播放背景音乐

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
	if (_kbhit())//只有接收到按键时才进行输入更新
		UpdateWithInput();
	UpdateWithoutInput();
	Show();
}

void Auto_Playing() {
	
	//自动演奏不需要输入
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
	
	//当经过的时间减去上一次判定时间点的值小等于100毫秒时，绘制判定，也即是显示100毫秒的判定
	if (passed_Mileseconds - Judge.hit_time <= 100
		&& Judge.hit_time != 0)
		DrawJudge(Judge.type);
	
	//当经过的时间减去上一次输入时间点的值小等于100毫秒时，绘制判定，也即是显示100毫秒的输入
	if (passed_Mileseconds - last_Input <= 100)
		DrawHit(drawHit);

	FlushBatchDraw();
	//Sleep(1);
}

void UpdateWithoutInput() {

	if (!isPlay)//初进入游玩时进行倒计时，给一些准备时间
		Count_Down();
	ReleaseKey();
	Slider_Move();

	if (isPlay) {//倒计时结束后开始更新数据
		GetActualTime();
		Act_Note();
		Note_Move();
		Put_Judge();
		Update_MaxCombo();
		Update_Current_Timing();
		Update_Don();
	}
	
	//如果当前连击数等于下一次连击数值长度变化对应的值
	//则更新连击数长度
	if (ScoreBoard.combo == ScoreBoard.next_Change)
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

	//如果当前连击数等于下一次连击数值长度变化对应的值
	//则更新连击数长度
	if (ScoreBoard.combo == ScoreBoard.next_Change)
		GetComboLength();
}

void UpdateWithInput() {

	//四个按键分别是W,E,7,8，按下后根据位置和颜色得到值，播放对应的音效，得到输入的时间点，并限制该键的输入，防止过度响应
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
	count_Down = 500;//500次循环的倒计时
	isEnd = false;
	first = false;
}

//初始化四个滑条的x轴位置，并记录第四个滑条的x轴位置为最后的滑条位置
void Initialize_Slider() {

	for (int i = 0; i < Slider_Number; i++) {
		loadimage(&Slider[i].hit, "..\\Picture\\Slider\\taiko-slider.png");
		loadimage(&Slider[i].fail, "..\\Picture\\Slider\\taiko-slider-fail.png");
		Slider[i].x = 0 + i * Slider[i].width;
		if (i == Slider_Number - 1)
			last_Slider = Slider[i].x;
	}
}

//初始化当前谱面的所有音符的各个数值，是关键函数
void Initialize_Note() {

	int i;
	isPlay = false;//设置游玩标志位为否，防止过早开始游戏
	note_Speed = 1;//100bpm时的标准速度，每次更新移动一个像素点
	current_Note = 0;//将当前的音符编号设为0
	current_Timing = 0;//将当前的时间点编号设为0
	float tempFactor;//临时的因数变量
	//遍历当前所选谱面的所有音符
	for (i = 0; i < Map[*Chosen].note_Number; i++) {
		Map[*Chosen].note[i].start = Map[*Chosen].note[i].timing;//将音符所处的时间点赋给各自的开始时间，以便后续使用
		/*if (Map[*Chosen].note[i].start <= 1024)
			Map[*Chosen].note[i].act = true;
		else*/
			Map[*Chosen].note[i].act = false;//将所有音符行动置否
		Search_Timing(i);//从第i号时间点开始寻找合适的时间点
		//若当前时间点已经是最后的时间点，以及当前音符的开始时间等于该时间点或在其之后，则做对应的处理
		if (current_Timing >= Map[*Chosen].timing_Number - 1
			&& Map[*Chosen].note[i].start >= Map[*Chosen].timing[current_Timing].value) {
			tempFactor = ((Map[*Chosen].timing[current_Timing].bpm *
				Map[*Chosen].timing[current_Timing].factor) / 200);//当前时间点的bpm乘以乘算因数，并除以200，得到临时的乘算因数
			//之所以是200是因为每2ms更新一次音符位置，标准速度1所对应的bpm是100，所以这里要除以200
			//如果是每x秒更新一次音符位置，则要除以x*100
			Map[*Chosen].note[i].speed = note_Speed * tempFactor;//将标准速度乘以临时因数，即是每个音符自己的速度
				Map[*Chosen].note[i].offset = 1024 / Map[*Chosen].note[i].speed;//设置每个音符的延迟，也即是提早显示的时间
			//因为音符需要在屏幕上显示一段时间后才能到达判定点被击中，所以需要提早显示
			//1024是屏幕的宽度，之所以除以该音符的速度是为了求得该音符在屏幕中应该显示的时长
		}
	}
	current_Timing = 0;//重新设置当前时间点为0，供更新小人使用
	station = 0;//设置时间站点为0
}

//用于让当前的音符能正确的找到对应的时间点，其余功能与音符初始化内的大致相同
void Search_Timing(int i) {

	if (current_Timing < Map[*Chosen].timing_Number - 1
		&& Map[*Chosen].note[i].start >= Map[*Chosen].timing[current_Timing].value
		&& Map[*Chosen].note[i].start < Map[*Chosen].timing[current_Timing + 1].value) {
		float tempFactor;
		tempFactor = ((Map[*Chosen].timing[current_Timing].bpm *
			Map[*Chosen].timing[current_Timing].factor) / 200);
		Map[*Chosen].note[i].speed = note_Speed * tempFactor;
		if(isAuto)
			Map[*Chosen].note[i].offset = 1024 / Map[*Chosen].note[i].speed;
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

	Judge.hit_time = 0;//设置当前判定时间为0
}

void Initialize_Key() {

	for (int i = 0; i < 4; i++)
		canInput[i] = true;//初始化4个按键为可输入
}

void Initialize_Don() {

	Don.next_Action = 0;//设置小人的下一个动作是第0号动作
	Don.next_Draw = 0;//设置下一次绘制时间是0
	Don.status = 0;//设置小人状态为空闲
}

void Initialize_ScoreBoard() {

	ScoreBoard.combo = 0;
	ScoreBoard.max_Combo = 0;
	ScoreBoard.next_Change = 10;//设置下一次连击长度改变所对应的值为10
	ScoreBoard.combo_Length = 1;//设置连击长度为1
	ScoreBoard.max_Combo_Length = 1;
	ScoreBoard.accuracy = 0;
	ScoreBoard.accuracy_Length = 4;
	ScoreBoard.number = (_Score_Number*)malloc(sizeof(ScoreBoard.number) * ScoreBoard.combo_Length);//根据连击长度分配number的大小，以便显示连击数使用
	ScoreBoard.hit300 = 0;
	ScoreBoard.hit300_Length = 1;
	ScoreBoard.hit100 = 0;
	ScoreBoard.hit100_Length = 1;
	ScoreBoard.hitMiss = 0;
	ScoreBoard.hitMiss_Length = 1;
	ScoreBoard.accuracy_Break = false;//设置准确度破坏为假
}

void PlayMusic() {

	musicAddress_Length = GetLength(Map[*Chosen].music_Address) - 1;
	PlayMusicOnce(Map[*Chosen].music_Address, musicAddress_Length);
	GetLocalTime(&Start_Time);
}


void Count_Down() {

	count_Down--;
	//当倒计时小等于0时播放对应音乐，并真正开始游戏
	if (count_Down <= 0 && !isPlay) {
		PlayMusic();
		isPlay = true;
		
	}
}

//在防止过度响应后，如果四个按键中的某个被松开，则可再次输入
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

//左移各个滑条的x轴位置，如果有某个滑条移出屏幕，则让其回到最后一个滑条的位置上
void Slider_Move() {

	for (int i = 0; i < Slider_Number; i++) {
		Slider[i].x -= Slider->speed;
		if (Slider[i].x + Slider->width < 0)
			Slider[i].x = last_Slider;
	}
}

//从当前的音符开始，到最后一个音符，如果当前经过的时间大等于某个音符的开始时间减去偏移量，则将其使能，并置于屏幕最右端，准备显示
//如果该音符已被使能则跳过，如果有某个音符不符合条件则退出循环，因为在其之后的音符肯定也不符合条件
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

//若当前经过的时间大等于站点，则站点等于当前的时间加2ms
//从当前的音符开始，到最后一个音符，若该音符已被使能，则减去两倍的自身速度，否则退出循环
//若该音符的位置小等于181，则当前的音符编号加一
//之所以是181，是因为181是除了最后一个音符外，其余所有音符能被允许显示的最左位置
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

//在必要时给出判定，最后一个音符和其余音符的处理不同
void Put_Judge() {
	
	//若当前音符是最后一个音符时，如果其位置小等于某个判定等级的时间
	//以及大等于该判定等级时间的负数，同时该音符的类型与输入类型一致
	//则令当前连击数加一，得到本次判定的时间点和判定类型，用于绘制判定图案
	//小人的状态置为空闲，对应击中的判定等级数加一，并更新准确度
	if (current_Note == Map[*Chosen].note_Number && !isEnd) {
		if (Map[*Chosen].note[current_Note].timing <= Judge.hit_great &&
			Map[*Chosen].note[current_Note].timing >= -Judge.hit_great 
			&& currentHit == Map[*Chosen].note[current_Note].color) {
			ScoreBoard.combo++;
			GetLocalTime(&current_Time);
			Judge.hit_time = ((current_Time.wMinute * 60 + current_Time.wSecond) * 1000 + current_Time.wMilliseconds)
				- ((Start_Time.wMinute * 60 + Start_Time.wSecond) * 1000 + Start_Time.wMilliseconds);
			Judge.type = true;
			Don.status = 0;
			ScoreBoard.hit300++;
			Update_Accuarcy();
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
		}
		//如果当前的音符位置小于-190，则表示没有击中，小人的状态置为漏击
		//漏击数加一。更新准确度，最大连击数，将当前连击数置零
		else if (Map[*Chosen].note[current_Note].timing <= -190) {
			Don.status = -1;
			ScoreBoard.hitMiss++;
			Update_Accuarcy();
			Update_MaxCombo();
			ComboSetZero();
		}
	}
	//若当前音符不是最后一个音符时，如果其位置小等于231加上某个判定等级的时间
	//以及大等于231减去该判定等级时间，同时该音符的类型与输入类型一致
	//则令当前连击数加一，得到本次判定的时间点和判定类型，用于绘制判定图案
	//小人的状态置为空闲，对应击中的判定等级数加一，并更新准确度
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
		}
		//如果当前的音符位置小于190，则表示没有击中，小人的状态置为漏击
		//漏击数加一。更新准确度，最大连击数，将当前连击数置零
		else if (Map[*Chosen].note[current_Note].timing <= 190) {
				current_Note++;
			Don.status = -1;
			ScoreBoard.hitMiss++;
			Update_Accuarcy();
			Update_MaxCombo();
			ComboSetZero();
		}
	}
	//重置输入类型
	currentHit = 0;
}

//用于自动演奏时给出判定，因为是自动演奏，所以不需要判断输入类型，另外自动演奏只有最高级判定和漏击
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

//用于更新当前时间点，而后更新小人的动作频率
void Update_Current_Timing() {
	
	//若当前音符的开始时间大于当前时间点的值，则执行更新，并把当前时间点编号加一
	if (Map[*Chosen].note[current_Note].start >
		Map[*Chosen].timing[current_Timing].value
		&& current_Timing < Map[*Chosen].timing_Number) {
		Update_Don_Frequency();
		current_Timing++;
	}
}

void Update_Don_Frequency() {
	
	//当前小人的动作频率是60除以当前时间点的真实bpm并乘以1000
	//之所以要乘以1000，是为了将其单位转成毫秒
	Don.frequency = 60 / (Map[*Chosen].timing[current_Timing].bpm
		* Map[*Chosen].timing[current_Timing].factor) * 1000;
}

//更新准确度
void Update_Accuarcy() {
	
	//准确度计算公式为:当前各个判断等级的数量乘以各自的权重后相加，最后除以当前的音符编号，也即当前经过的音符数
	ScoreBoard.accuracy = ((ScoreBoard.hit300 * ScoreBoard.max_Weight) +
		(ScoreBoard.hit100 * ScoreBoard.half_Weight) +
		(ScoreBoard.hitMiss * ScoreBoard.miss_Weight)) / current_Note;
	
	//如果准确度等于100，则准确度值的长度为5，否则准确度破坏标志置真，准确度值长度为4
	//如果准确度被破坏，则退出，不做后续判断
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
	
	//如果当前的连击数大于最大连击数，则最大连击数等于当前连击数
	//最大连击数值的长度等于当前连击数值的长度
	if (ScoreBoard.combo > ScoreBoard.max_Combo) {
		ScoreBoard.max_Combo = ScoreBoard.combo;
		ScoreBoard.max_Combo_Length = ScoreBoard.combo_Length;
	}
}

void ComboSetZero() {

	//将当前连击数置0，长度置1，下一次变化对应值置10
	ScoreBoard.combo = 0;
	ScoreBoard.combo_Length = 1;
	ScoreBoard.next_Change = 10;
}

void Update_Don() {

	//如果当前经过的时间减去下一次小人绘制时间大于等于小人的动作频率
	//则更新小人动作，并更新下一次绘制时间
	//之所以加2，是因为把掩码图和普通图片放在一个数组里，并且相邻着
	if (passed_Mileseconds - Don.next_Draw >= Don.frequency) {
		Don.next_Action += 2;
		Don.next_Draw = GetActualTime();
	}
}

//用于更新当前连击数的长度，因为实际游玩时连击数的位数可能变动较多
//所以专门写了个处理函数
void GetComboLength() {

	//动态分配临时变量的空间等于当前连击数的空间
	//让临时变量指向当前记分板的number成员
	//number成员是用来正确显示连击数各个位的数字
	//让当前的连击数值的长度加一
	//更新下一次变化对应的值
	//重新为number分配更大一位的空间，并释放之前的空间
	_Score_Number* temp = (_Score_Number*)malloc(sizeof(_Score_Number) * ScoreBoard.combo_Length);
	temp = ScoreBoard.number;
	ScoreBoard.combo_Length++;
	ScoreBoard.next_Change = pow(10, ScoreBoard.combo_Length);
	ScoreBoard.number = (_Score_Number*)malloc(sizeof(_Score_Number) * ScoreBoard.combo_Length);
	free(temp);
}


void Draw_Slider() {
	
	//根据小人的状态来绘制滑条，若空闲则绘制彩色，若漏击则绘制黑色
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

	//在对应的位置绘制当前连击数的每一位数字
	//临时遍量temp用于为每一位数字找到正确的指数
	//interval是用于计算每个数字间的间隔
	//之所以是150，是因为留给绘制连击的空间只有150像素宽
	int temp;
	int interval = 150 / (ScoreBoard.combo_Length + 1);
	for (int i = 0; i < ScoreBoard.combo_Length; i++) {
		temp = ScoreBoard.combo_Length - i - 1;
		//计算每一位数字的数值以及对应的x轴位置，并绘制
		ScoreBoard.number[i].value = (int)(ScoreBoard.combo / pow(10, temp)) % 10;
		ScoreBoard.number[i].x = 10 + (i + 1) * interval - 20;
		putimage(ScoreBoard.number[i].x, 290, &number_Mask[ScoreBoard.number[i].value], NOTSRCERASE);
		putimage(ScoreBoard.number[i].x, 290, &number[ScoreBoard.number[i].value], SRCINVERT);
	}
}

//绘制准确度与绘制连击数大致相同
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
	//从当前的音符到最后一个音符，若某个音符被使能，则在屏幕上将其绘制
	//根据颜色的不同绘制不同的素材
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
	
	//根据小人的状态绘制小人的动作
	//空闲动作有六种，漏击动作有两种，每种各有掩码和图片
	//在绘制前如果所要绘制的下一个动作超出数组范围，则将其置0
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

//根据判断类型绘制判定
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

//根据输入类型绘制对应的按键
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


//结束函数
void End() {
	
	//若当前的音符编号以及大等于音符数，说明谱面结束
	//将结束标识符置真，倒计时1000次循环，小人的状态置为空闲
	if (current_Note >= Map[*Chosen].note_Number) {
		if (!isEnd)
			isEnd = true;
		if (count_Down <= 0)
			count_Down = 1000;
		Don.status = 0;
		count_Down--;
		//当倒计时结束后则作收尾处理
		//得到每个判定等级的数量，并根据准确度得到评价等级
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
			//将所有音符的当前时间点设置为对应的开始时间点，并置否
			for (int i = 0; i < Map[*Chosen].note_Number; i++) {
				Map[*Chosen].note[i].timing = Map[*Chosen].note[i].start;
				Map[*Chosen].note[i].act = false;
			}
			//小人的下一次绘制时间置0，频率置0
			//当前音符编号置零，当前时间点编号置零
			//开始标识置真，状态转为3(结算菜单)
			Don.next_Draw = 0;
			Don.frequency = 0;
			current_Note = 0;
			current_Timing = 0;
			first = true;
			Status = 3;
		}

	}
}

//这里采用比较简单的处理，理论上是够用了
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

//根据准确度和是否断连给出对应的评价等级
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




