#include <iostream>
using namespace std;
#include <winbgim.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define 	MINW 		20
#define 	MINH 		20
#define 	MAXW 		420
#define 	MAXH 		220
#define 	MINX 		20
#define 	MINY 		20
#define 	MAXX 		420
#define 	MAXY 		220
#define 	BACKGROUND	15

struct Point {
	int x,y,x0,y0;
};
struct HighScore {
	int score;
	char name[30];
};

void initGame ();
bool checkPoint ();
void drawPoint (int x,int y,int r);
void moveSnake ();
void drawSnake ();
void drawFood ();
void drawGame ();
void classic();
void modern();
void mainLoop (void (*gloop)());
void run ();
void changeDirecton (int x);
void showHighScore();
void getHighScore();
void checkHighScore(int score);
void initScore();
bool isEmpty();
void showText(int x,int y,char *str);
void showTextBackground(int x,int y,char *str,int color);


int level;
bool endGame;
int snakeLength;
Point snake[100];
Point direction;
Point food;
const int DIRECTION	= 10;
HighScore  highscore[5];
char* score_str = new char[20];// Convert score to string
void initScore(){
		if (isEmpty()){
			for (int i = 0;i < 5;i++){
			strcpy(highscore[i].name,"PLAYER");
			highscore[i].score = 0;
			}
			getHighScore();
		}else{
			char arr[20];
			int count = 0;
			FILE *f;
			f = fopen("highscore.txt", "r");
			for (int i = 0;i < 5;){
				while (!feof(f)){
					count++;
					fscanf(f, "%s", arr);
				   if (count%2 == 1){
				   		strcpy(highscore[i].name,arr);
						//printf ("%s\t",ch);
					}
					else{
						highscore[i].score = atoi(arr);
						i++;
						}
				   }
				}
			}
}

void showTextBackground(int x,int y,char *str,int color){
	int bk = getbkcolor();
	setbkcolor(color);
	outtextxy(x,y,str);
	delay(200);
	setbkcolor(bk);
}

void showText(int x,int y,char *str){
	int c = getcolor();
	int color = rand() % 16 + 1;
	setcolor(color);
	outtextxy(x+3,y,str);
	setcolor(c);
	delay(50);
}

bool isEmpty(){
	FILE *fp;
	long size;
	fp = fopen("highscore.txt","r");
	if(fp){
		fseek (fp, 0, SEEK_END);
        size = ftell(fp);
		fclose(fp);
	}
	return(size == 0);
}
bool checkPoint (){
	for (int i = 0;i < snakeLength;i++){
		if (food.x == snake[i].x && food.y == snake[i].y)
		return false;
	}
	return true;
}
void initGame(){
	setbkcolor (15);
	cleardevice ();
	setwindowtitle ("SNAKE-Playing....");
	endGame = false;
	setfillstyle (1,5);
	bar (MINX,MINY,MAXX,MINY+5);
	bar (MAXX,MINY,MAXX-5,MAXY);
	bar (MAXX,MAXY,MINX,MAXY-5);
	bar (MINX,MINY,MINX+5,MAXY);
    setcolor (0);
    int bk = getbkcolor();
    setfillstyle (SOLID_FILL,0);
	int a[] = {15,350,30,350,30,345,50,355,30,365,30,360,15,360,15,350};
    fillpoly (8,a);
    settextstyle (4,0,3); outtextxy (70,340,"Right");
    int b[] = {15,410,30,400,30,405,45,405,45,415,30,415,30,420,15,410};
    fillpoly (8,b);outtextxy (70,400,"Left");
	int c[] = {235,370,235,355,230,355,240,340,250,355,245,355,245,370,235,370};
	fillpoly (8,c);outtextxy (265,340,"Up");
	int d[] = {235,400,235,415,230,415,240,430,250,415,245,415,245,400,235,400};
	fillpoly (8,d);outtextxy (265,400,"Down");
	bar (50,450,170,480);setcolor (15);settextstyle(1,0,1);setbkcolor(0);outtextxy(60,455,"SPACE");setbkcolor(bk);
	setcolor(0);settextstyle (4,0,3);outtextxy(200,450,"Pause/Resum");
	bar (50,500,170,530);setcolor (15);settextstyle(1,0,1);setbkcolor(0);outtextxy(80,505,"ESC");setbkcolor(bk);
	setcolor(0);settextstyle (4,0,3);outtextxy(200,500,"End Game");
	snake[0].x = 60;snake[0].y = 50;
	snake[1].x = 50;snake[1].y = 50;
	snake[2].x = 40;snake[2].y = 50;
	snakeLength = 3;
	setfillstyle (1,BACKGROUND);
	bar (25,25,415,215);
	direction.x = 10;direction.y = 0; // Khoi tao di theo huong trai qua phai
	setcolor (2);
	srand ( time(NULL));
	//Init food
    do{
        food.x = (rand() % (39) + 3)*10;
    	food.y = (rand() % (19) + 3)*10;
	}while (checkPoint() == false);
}

void drawPoint (int x,int y,int r){
	circle (x,y,r);
	floodfill (x,y,getcolor());
}


// Snake is moving
void modern(){
	for (int i = 0;i < snakeLength;i++){
		if (i == 0){
			snake[0].x0 = snake[0].x;snake[0].y0 = snake[0].y;
			snake[0].x += direction.x;
			snake[0].y += direction.y;
		}
		else {
			snake[i].x0 = snake[i].x;snake[i].y0 = snake[i].y;
			snake[i].x = snake[i-1].x0;snake[i].y = snake[i-1].y0;
		}
		if (snake[0].x < MINX+5 || snake[0].y < MINY+5 || snake[0].x > MAXX-5 || snake[0].y > MAXY - 5 )
			endGame = true;
		if (i != 0 && snake[0].x == snake[i].x && snake[0].y == snake[i].y)	 endGame = true;
	}
	if (snake[0].x == food.x && snake[0].y == food.y){
		snake[snakeLength].x = snake[snakeLength-1].x0;snake[snakeLength].y = snake[snakeLength-1].y0;
		snakeLength++;
		PlaySound(TEXT("eatFood.wav"), NULL, SND_ASYNC);
		//Random again food if sanke ate food
		srand ( time(NULL));
		do{
        	food.x = (rand() % (39) + 3)*10;
    		food.y = (rand() % (19) + 3)*10;
		}while (checkPoint() == false);
	}
}

void classic(){
    for (int i = 0; i < snakeLength; i++) {
        if (i == 0) {
            snake[0].x0 = snake[0].x;snake[0].y0 = snake[0].y;
			snake[0].x += direction.x;
			snake[0].y += direction.y;
        }else{
            snake[i].x0 = snake[i].x;snake[i].y0 = snake[i].y;
			snake[i].x = snake[i-1].x0;snake[i].y = snake[i-1].y0;
        }
         
        if (snake[i].x >= MAXX) snake[i].x = MINX + 10;
        if (snake[i].x <= MINX) snake[i].x = MAXX - 10;
        if (snake[i].y >= MAXY) snake[i].y = MINY + 10;
        if (snake[i].y <= MINY) snake[i].y = MAXY - 10;
         
        if (i != 0 && (snake[0].x == snake[i].x && snake[0].y == snake[i].y)) endGame = true;
    }
    if (snake[0].x == food.x && snake[0].y == food.y){
		snake[snakeLength].x = snake[snakeLength-1].x0;snake[snakeLength].y = snake[snakeLength-1].y0;
		snakeLength++;
		PlaySound(TEXT("eatFood.wav"), NULL, SND_ASYNC);
		//Random again food if sanke ate food
		srand ( time(NULL));
        do{
        	food.x = (rand() % (39) + 3)*10;
    		food.y = (rand() % (19) + 3)*10;
		}while (checkPoint() == false);
	}
}

void drawSnake (){
	setfillstyle (1,10);
	int count = 0;
	for (int i = 0;count < snakeLength;i++){
			drawPoint(snake[i].x, snake[i].y,5);
			count++;
		}
	//delete point when snake move
	int x = getcolor();
	setfillstyle (1,BACKGROUND);
	drawPoint(snake[snakeLength-1].x0,snake[snakeLength-1].y0,5);
	setcolor (BACKGROUND);
	circle (snake[snakeLength-1].x0,snake[snakeLength-1].y0,5);
	setcolor(x);
	setfillstyle (1,5);
}

void drawFood (){
	int x = getcolor();
	int c = rand() % 14 + 1;
		setcolor(c);setfillstyle (SOLID_FILL, c);
		drawPoint(food.x, food.y,5);
	setcolor(x);setfillstyle (1,BACKGROUND);
}

//Ve toan bo giao dien game
void drawGame (){
	drawSnake();
	drawFood();
	int x = getcolor ();
	setcolor (2);
	sprintf(score_str,"%d",snakeLength*level*10-level*30);    //show score
    settextstyle(4,0,3);
    outtextxy(450,20,"SCORE:");
    setcolor(12);
    outtextxy(600,20,score_str);
	setcolor (x);
}


void changeDirecton (int x){
	switch(x){
                case 72: 
                    if (direction.y != DIRECTION) {
                    	PlaySound(TEXT("beep.wav"), NULL, SND_ASYNC);
            			direction.y = -DIRECTION; direction.x = 0;
        			}
                    break;
                case 80: 
                	if (direction.y != -DIRECTION) {
                		PlaySound(TEXT("beep.wav"), NULL, SND_ASYNC);
            			direction.y = DIRECTION; direction.x = 0;
        			}
                   break;
                case 77:
                    if (direction.x != -DIRECTION) {
                    	PlaySound(TEXT("beep.wav"), NULL, SND_ASYNC);
            			direction.x = DIRECTION; direction.y = 0;
        			}
                    break;
                case 75:
                    if (direction.x != DIRECTION) {
                    	PlaySound(TEXT("beep.wav"), NULL, SND_ASYNC);
            			direction.x = -DIRECTION; direction.y = 0;
        			}
                    break;
                case 27:
                	endGame = true;
					break;
            }
}

void mainLoop (void (*xxx)()){
	int x = getcolor();
	if (!kbhit()){
		xxx();
	}
	else{
		char ch;
        ch = getch();
		if (ch == -32) ch = getch();
		if ( ch == 32){
			setcolor(0);outtextxy (100,100,"Game Pause");setcolor(x);
			ch = getch();
			while (ch != 32){
				ch = getch();
				delay(0);
			}
			setcolor (BACKGROUND);
			outtextxy (100,100,"Game Pause");
			setcolor (x);
			delay(1000);
		}
        changeDirecton(ch);
        mainLoop (xxx);
    }
}

void showHighScore(){
	FILE *f;
	f = fopen("highscore.txt", "r");
	char ch[20];
	settextstyle (1,0,5);
	setcolor (15);
	outtextxy (150,50,"HIGH SCORE");
	settextstyle (1,0,4);
	int y = 150,count = 0;
	while (!feof(f)){
		if (count == 10) break;
		count++;
		fscanf(f, "%s", ch);
   		if (count%2 == 1){
   			setcolor (10);
   			outtextxy (180,y,ch);
   			y+=50;
		}else{
			setcolor (12);
			outtextxy (500,y-50,ch);	   	
		}
	}
	fclose(f);
}
void getHighScore (){
	FILE *f;
	f = fopen("highscore.txt", "w");
	for (int i = 0;i < 5;i++){
		fputs(highscore[i].name,f);
		fputs(" ",f);
		fprintf(f,"%d",highscore[i].score);
		fputs("\n",f);
	}
	fclose(f);
}
void checkHighScore (int _score){
	char _name[20]={""};
	for (int i = 0;i < 5;i++){
		if (_score > highscore[i].score){
			//to do sth
			settextstyle(1,0,3);
				for (int j = 0;j < 50;j++){
					if (j%2 == 0){
						setcolor (14);
						if (i == 0)
						outtextxy(460,100,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}else{
						setcolor (9);
						if (i == 0)
						outtextxy(460,100,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}
				}
				settextstyle(1,0,2);
				setcolor (4);outtextxy(430,150,"Player:");
				delay(100);
				char ch1;
				int x = 0;
				char str[2];
				str[1] = 0;
				while (ch1 != 13 && x < 10){
					do{
						ch1 = getch();
					}while (ch1 < 65 && ch1 >90 || ch1 < 97 && ch1 > 132);
					x++;
					str[0] = ch1;
					strcat(_name,str);
					outtextxy(540,150,_name);
				}
			for (int j = 4;j > i;j--){
				strcpy(highscore[j].name,highscore[j-1].name);
				highscore[j].score = highscore[j-1].score;
			}
			strcpy(highscore[i].name,_name);
			highscore[i].score = _score;
			break;
		}
	}
	getHighScore();
}
void run (){
	initwindow (800,600);
	initScore();
	int x = 0;
	char *s = new char[5];
	while (x <= 100){
		cleardevice();
		setcolor (10);settextstyle(4,0,5);outtextxy (60,200,"Game Loading...");
		sprintf (s,"%d",x);
		setcolor (4);
		outtextxy (550,200,strcat(s,"%"));
		if (x == 100)
			delay (500);
		delay (20);
		x++;
	}
	
	while (true){
	x1:
	int bk = getbkcolor();
	cleardevice();
	setbkcolor(0);
	cleardevice();
	clearmouseclick(WM_LBUTTONDOWN);
	PlaySound(TEXT("faded.wav"), NULL , SND_ASYNC);
	while (!ismouseclick(WM_LBUTTONDOWN) || mousex() < 200 || mousey() > 465 || mousey() < 190 || mousex() > 580 ){
		settextstyle(1,0,6);
		setcolor (10);
		outtextxy(120,70,"GAME MENU");
		settextstyle(10,0,5);
		showText (220,200,"NEW GAME");
		showText (220,270,"HIGH SCORE");
		showText (220,340,"QUIT GAME");
		showText (220,410,"ABOUT");
	}
	clearmouseclick(WM_LBUTTONDOWN);
	if (mousey() <= 255){
		showTextBackground(220,200,"NEW GAME",15);
		x2:
		setbkcolor(0);
		cleardevice();
		int nameFunction;
		clearmouseclick(WM_LBUTTONDOWN);
		while (!ismouseclick(WM_LBUTTONDOWN) || mousex() < 290 || mousex() > 520 || mousey() < 240 || mousey() > 400){
			settextstyle(1,0,6);setcolor(10);
			outtextxy (150,150,"GAME TYPE");
			setcolor(15);settextstyle(1,0,4);
			showTextBackground(250,250,"CLASSIC",0);
			showTextBackground (250,350,"MODERN",0);
		}
		if (ismouseclick(WM_LBUTTONDOWN) && mousex() >= 290 && mousex() <= 520 && mousey() >= 240 && mousey() <= 400){
			if(mousey() <= 300) nameFunction = 1;
			else nameFunction = 2;
		}
		setbkcolor(0);
		cleardevice();
		setcolor(4);settextstyle(4,0,3);
		outtextxy(10,310,"YOUR CHOICE :");
		settextstyle (4,0,6);
		setcolor (10);
		outtextxy (150,200,"LEVEL 1 - 5");
		setcolor(4);settextstyle(4,0,3);
		outtextxy(10,310,"YOUR CHOICE :");
		int xx = 330;
		char str1[2];
		clearmouseclick(WM_LBUTTONDOWN);
		while (!ismouseclick(WM_LBUTTONDOWN) || mousex() < 300 || mousex() > 790 || mousey() < 280 || mousey() > 350) 
		for(int i = 1;i <= 5;i++){
			sprintf(str1,"%d",i);
			settextstyle(4,0,4);
			showTextBackground(xx,300,str1,0);
			xx+= 100;
		}
		if (ismouseclick(WM_LBUTTONDOWN) && mousex() >= 300 && mousex() <= 790 && mousey() >= 280 && mousey() <= 350){
			if(mousex() <= 390) level = 1;
			else if(mousex() <= 490) level = 2;
			else if(mousex() <= 590) level = 3;
			else if(mousex() <= 690) level = 4;
			else level = 5;
		}
		PlaySound(TEXT("start.wav"), NULL , SND_ASYNC);
		initGame();
		while (!endGame){
			//Change delay folow level
		delay (250 - (level-1)*40);
		(nameFunction == 1) ? mainLoop(classic):mainLoop(modern);
		drawGame();
	}
		PlaySound(TEXT("endGame.wav"), NULL , SND_ASYNC);
		settextstyle (1,0,4);
		setcolor (0);
		outtextxy (100,110,"GAME OVER!");
		setcolor (8);
		settextstyle (1,0,2);
		outtextxy (100,160,"Your Score : ");
		setcolor(10);
		outtextxy (300,160,score_str);
		//Here is check high score
		checkHighScore(snakeLength*level*10-level*30);
		setcolor(10);settextstyle(4,0,5);
		outtextxy (100,200,"Play Again ?");
		settextstyle(4,0,3);
		setcolor(1);
		clearmouseclick(WM_LBUTTONDOWN);
		while (!ismouseclick(WM_LBUTTONDOWN)){
			showTextBackground(150,270,"YES",12);showTextBackground(300,270,"NO",7);
		}
		if(ismouseclick(WM_LBUTTONDOWN)){
			if (mousex() >= 140 && mousex() <= 230 && mousey() >= 260 && mousey() <= 310){
				showTextBackground(150,270,"YES",7);
				delay(100);
				goto x2;
			}
			else if(mousex() >= 290 && mousex() <= 360 && mousey() >= 260 && mousey() <= 310){
				showTextBackground(300,270,"NO",12);
				delay(100);
				goto x1;
			}
        	}
    	}else if (mousey() <= 325){
			showTextBackground(220,270,"HIGH SCORE",15);
			cleardevice();
			showHighScore();
			while (!ismouseclick(WM_LBUTTONDOWN)){
				delay(0);
			}
			goto x1;
		}else if(mousey() <= 395){
			showTextBackground (220,340,"QUIT GAME",15);
			exit(0);
		}else{
			
		cleardevice();
		settextstyle (1,0,4);
		setcolor (15);
		int x = getcolor();
		while(!ismouseclick(WM_LBUTTONDOWN)){
			settextstyle(10,0,6);setcolor(15);
			outtextxy(260,20,"ABOUT");
			settextstyle(1,0,4);setcolor(9);
			outtextxy(20,100,"TRUONG DAI HOC DIEN LUC");
			settextstyle(7,0,5);
			showText(290,150,"DE TAI");
			settextstyle(4,0,2);
			showText(170,200,"TRO CHOI \"RAN SAN MOI\"");
			settextstyle(7,0,3);
			showText(200,270,"SINH VIEN THUC HIEN");
			settextstyle(4,0,2);
			showText(150,320,"NGUYEN VAN HIEU- 1481310027");
			settextstyle(3,0,3);
			showText(250,390,"GIANG VIEN HUONG DAN");
			settextstyle(4,0,3);
			showText(180,440,"THAY NGO NGOC THANH");
		}
		setcolor(x);
		settextstyle (1,0,2);
		setcolor (4);
		goto x1;
		}
	}
}
int main (){
	run();
	//Giai phong bo nho con tro;
	delete score_str;
	getch();
	closegraph ();
    return 0;
}
