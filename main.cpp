#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<cstdlib> 
#include<ctime>
#include<string>
#include<fstream>
using namespace std;

struct Snake{
	int s[100][2]; //100节最大  x y坐标 
	int len;
};
Snake snake;

PIMAGE h_up = newimage();
PIMAGE h_down = newimage();
PIMAGE h_left = newimage();
PIMAGE h_right = newimage();
PIMAGE wall = newimage();
PIMAGE tail = newimage();
PIMAGE heart = newimage();
PIMAGE over = newimage();
PIMAGE title_1 = newimage();
PIMAGE title_2 = newimage();

MUSIC game_over_sound;
MUSIC eat_sound;
MUSIC mu; 

int fx=3;
int fxs[4][2] = {{0,20}, {0,-20}, {-20,0}, {20,0}}; //up,down,left,right

int food[2]={320,240};    //food_location

int best[4];  //best_score

int cnt;  //延迟时间 控制速度 

int pt=0; //score
int mode=1;
int speed=2;

bool map[32][32];

void print_map() {
	
	setfont(40,20,"consolas");
	setcolor(WHITE);
	string bs_s = "best score: "+to_string(best[mode]);
	outtextxy(680, 220, bs_s.c_str());

	setfont(80,40,"consolas");
	setcolor(CYAN);
	string mp_s = "map "+to_string(mode);
	outtextxy(710, 140, mp_s.c_str());
}

print_score() {
	setfont(40,20,"consolas");
	setcolor(WHITE);
	string pt_s = "Score: "+to_string(pt);
	outtextxy(720, 540, pt_s.c_str());
}

print_speed() {
	setfont(40,20,"consolas");
	setcolor(YELLOW);
	string sp_s = "Speed: ";
	switch(speed) {
		case 1:
			sp_s += "SLOW";
			break;
		case 2:
			sp_s += "MEDIUM";
			break;
		case 3:
			sp_s += "FAST";
			break;
		case 4:
			sp_s += "FAST+";
			break;
	}
	
	outtextxy(680, 480, sp_s.c_str());
}

print_menu() {
	setfont(40,20,"consolas");
	setcolor(WHITE);
	outtextxy(680, 60, "press '0'-'3'");
	outtextxy(670, 100, "to select map");
	
	outtextxy(700, 540, "press SPACE");
	
	outtextxy(680, 290, "select speed:");
	
	setfont(30,15,"consolas");
	
	outtextxy(680, 580, "when you are ready");
	
	outtextxy(700, 340, "press S  -slow");
	outtextxy(700, 370, "press M  -medium");
	outtextxy(700, 400, "press F  -fast");
	outtextxy(700, 430, "press P  -fast+");
	
}


release_all() {
	for(int i=0; i<32; i++) {
		for(int j=0; j<32; j++) map[i][j]=0;
	}	
}

release_wall_1() {
	for(int i=0; i<32; i++) {
		map[0][i] = 0;
	}
	for(int j=1; j<31; j++) {
		map[j][0]=0; map[j][31]=0;
	}
	for(int i=0; i<32; i++) {
		map[31][i] = 0;
	}
}

release_wall_2() {
	for(int j=8; j<=23; j++) {
		map[j][11]=0;
		map[j][20]=0;
	}
	for(int j=12; j<20; j++) {
		map[8][j] = 0;
	}
}

release_wall_3() {
	for(int j=8; j<=23; j++) {
		map[j][11]=0;
		map[j][20]=0;
	}
	
	for(int j=0; j<32; j++) {
		map[0][j]=0; map[31][j]=0;
	}
	for(int i=1; i<=7; i++) {
		map[i][0]=0; map[i][31]=0;
		map[31-i][0]=0; map[31-i][31]=0;	
	}
}


set_wall_0() {
	cleardevice();
	if(mode == 1) release_wall_1();
	if(mode == 2) release_wall_2();
	if(mode == 3) release_wall_3();
	setcolor(WHITE);
	line(640, 0, 640, 640);
	mode=0;
}


set_wall_1() {
	cleardevice();
	if(mode == 2) release_wall_2();
	if(mode == 3) release_wall_3();
	for(int i=0; i<32; i++) {
		map[0][i] = 1;
		putimage_withalpha(NULL, wall, 0, i*20);
	}
	for(int j=1; j<31; j++) {
		map[j][0]=1; map[j][31]=1;
		putimage_withalpha(NULL, wall, j*20, 0);
		putimage_withalpha(NULL, wall, j*20, 620);
	}
	for(int i=0; i<32; i++) {
		map[31][i] = 1;
		putimage_withalpha(NULL, wall, 620, i*20);
	}
	setcolor(WHITE);
	line(640, 0, 640, 640);
	mode=1;
}

set_wall_2() {
	cleardevice();
	if(mode == 1) release_wall_1();
	if(mode == 3) release_wall_3();
	for(int j=8; j<=23; j++) {
		map[j][11] = 1; map[j][20]=1;
		putimage_withalpha(NULL, wall, j*20, 11*20);
		putimage_withalpha(NULL, wall, j*20, 20*20);
	}
	
	for(int j=12; j<20; j++) {
		map[8][j] = 1;
		putimage_withalpha(NULL, wall, 8*20, j*20);
	}
	
	setcolor(WHITE);
	line(640, 0, 640, 640);
	mode=2;
}

set_wall_3() {
	cleardevice();
	if(mode == 1) release_wall_1();
	if(mode == 2) release_wall_2();
	for(int j=8; j<=23; j++) {
		map[j][11]=1; map[j][20]=1;
		putimage_withalpha(NULL, wall, j*20, 11*20);
		putimage_withalpha(NULL, wall, j*20, 20*20);
	}
	
	for(int j=0; j<32; j++) {
		map[0][j]=1; map[31][j]=1;
		putimage_withalpha(NULL, wall, 0, j*20);
		putimage_withalpha(NULL, wall, 31*20, j*20);
	}
	for(int i=1; i<=7; i++) {
		map[i][0]=1; map[i][31]=1;
		map[31-i][0]=1; map[31-i][31]=1;
		putimage_withalpha(NULL, wall, i*20, 0);
		putimage_withalpha(NULL, wall, i*20, 31*20);
		putimage_withalpha(NULL, wall, (31-i)*20, 0);
		putimage_withalpha(NULL, wall, (31-i)*20, 31*20);		
	}
	
	setcolor(WHITE);
	line(640, 0, 640, 640);
	mode=3;
}


start_menu() {
	putimage_withalpha(NULL, title_1, 55, 0);
	putimage_withalpha(NULL, title_2, -30, 60);
	
	getch();
	cleardevice(); 
	
	char c;
	set_wall_1();
	print_speed();
	print_menu();
	print_map();
	
	while(c != ' ') {
		c = getch();
		cleardevice();
		if(c == 's') speed=1;
		else if(c == 'm') speed=2;
		else if(c == 'f') speed=3;
		else if(c == 'p') speed=4;
		else if(c == '0') set_wall_0();
		else if(c == '1') set_wall_1();
		else if(c == '2') set_wall_2();
		else if(c == '3') set_wall_3();
		
		if(mode==0) set_wall_0();
		if(mode==1) set_wall_1();
		if(mode==2) set_wall_2();
		if(mode==3) set_wall_3();
		
		print_speed();
		print_menu();
		print_map();
	}
	
	
}


keyevent() {
	key_msg kmsg;
	while(kbhit()) {
		kmsg = getkey();
		int key = kmsg.key;
		if(key=='w' && fx!=1) fx=0;
		if(key=='s' && fx!=0) fx=1;
		if(key=='a' && fx!=3) fx=2;
		if(key=='d' && fx!=2) fx=3;
	}
}

void ReadBest(int* a) {
	ifstream bfile("temp/best.txt");
	for(int i=0; i<4; i++) bfile >> a[i];
	//for(int i=0; i<4; i++) cout << a[i] << " ";
	//cout << endl;
	bfile.close();
}

void WriteBest(int* a) {
	ofstream bfile("temp/best.txt");
	for(int i=0; i<4; i++) bfile << a[i] << endl;
	bfile.close();
}

void chkBest() {
	if(pt > best[mode]) {
		best[mode] = pt;
		
		setfont(24,12,"consolas");
		setcolor(MAGENTA);
		string bs_s = "best score uploaded: "+to_string(pt)+"!";
		outtextxy(670, 430, bs_s.c_str());
	}
	WriteBest(best);
}



int main() {
	initgraph(960, 640);
	Snake snake;
	int x, y;
	
start:	
	cleardevice();
	release_all();
	ReadBest(best);
	pt=0; fx=3;

	snake.len = 2;
	snake.s[0][0] = snake.s[0][1] = 320;
	snake.s[1][0] = 320; snake.s[1][1] = 300;
	
	getimage(heart, "temp/china.png");
	getimage(wall, "temp/wall.png");
	
	getimage(h_up, "temp/snakeHeadUp.png");
	getimage(h_down, "temp/snakeHeadDown.png");
	getimage(h_left, "temp/snakeHeadLeft.png");
	getimage(h_right, "temp/snakeHeadRight.png");
	getimage(tail, "temp/snakeTail.png");
	
	getimage(over, "temp/game_over.png");
	getimage(title_1, "temp/snake.gif");
	getimage(title_2, "temp/start.png");
	
	game_over_sound.OpenFile("temp/game_over.wav");
	eat_sound.OpenFile("temp/eat.wav");
	mu.OpenFile("temp/music.wav");
	
	mu.Play();

	setfillcolor(WHITE);
	setcolor(WHITE); 
	
	start_menu();
	
	//getch();
	
	for(; is_run(); delay_fps(60)) {
		//cleardevice();
		
		if(mode == 0) set_wall_0();
		else if(mode == 1) set_wall_1();
		else if(mode == 2) set_wall_2();
		else if(mode == 3) set_wall_3();
		
		print_score(); 
		print_speed();
		print_map();
		
		putimage_withalpha(NULL, heart, food[0], food[1]);  //print_food
		
		x = snake.s[snake.len-1][0];
		y = snake.s[snake.len-1][1];
		map[x/20][y/20] = 0;   //释放tail空间 

		for(int i=snake.len-1; i>=1; i--) {   //tail_move 
			snake.s[i][0] = snake.s[i-1][0];
			snake.s[i][1] = snake.s[i-1][1];
		}
		
		x = snake.s[0][0] = (snake.s[0][0]+fxs[fx][0]+640)%640;  //headx_move 
		y = snake.s[0][1] = (snake.s[0][1]-fxs[fx][1]+640)%640;  //heady_move

		
		for(int i=1; i<snake.len; i++) {    //print_tail 
			x = snake.s[i][0];
			y = snake.s[i][1];
			putimage_withalpha(NULL, tail, x, y);
		}
		
		x=snake.s[0][0]; y=snake.s[0][1];  //print_head
		if(fx == 0) putimage_withalpha(NULL, h_up, x, y);
		else if(fx == 1) putimage_withalpha(NULL, h_down, x, y);
		else if(fx == 2) putimage_withalpha(NULL, h_left, x, y);
		else if(fx == 3) putimage_withalpha(NULL, h_right, x, y);
		
		
		if(map[x/20][y/20]) {   //die
			
			mu.Stop();
			game_over_sound.Play(0);
			
			putimage_withalpha(NULL, over, -40, 0);
			//string pt_s = "Score: "+to_string(pt);
			
			chkBest();
			WriteBest(best);
			
			setfont(40,20,"consolas");
			setcolor(WHITE);
			outtextxy(680, 380, "RETRY?  (Y/N)");
			
			
			while(1) {
				char c=getch();
				if(c == 'y') goto start;
				else if(c == 'n') {
					closegraph();
					return 0;
				}
			}
		} 
		map[x/20][y/20] = 1;  //标记head 
		
		if(food[0]==snake.s[0][0] && food[1]==snake.s[0][1]) {  //eat
			eat_sound.Play(0);
			
			pt++;
			
			snake.len++;
			snake.s[snake.len-1][0] = snake.s[snake.len-2][0];
			snake.s[snake.len-1][1] = snake.s[snake.len-2][1];
			srand(static_cast<unsigned int>(time(nullptr)));
			
			do {
				food[0] = (rand()%31+1)*20;
				food[1] = (rand()%31+1)*20;	
				
				//printf("food:%d %d\n\n", food[0], food[1]);
			} while(map[food[0]/20][food[1]/20]);
			
		}
		
		if(speed == 1) cnt=5;
		else if(speed == 2) cnt=3;  
		else if(speed == 3) cnt=2;
		else if(speed == 4) cnt=1;
//		cnt=5; //speed_set
		while(cnt--) {
			Sleep(50);
			keyevent();
		}
		
		
		DWORD status = mu.GetPlayStatus();
        if (status == MUSIC_MODE_STOP) {   //replay_music
            // 如果播放完毕，重新播放
            mu.Play(0);
        }
		 
	}
	
	getch();
	return 0;
}
