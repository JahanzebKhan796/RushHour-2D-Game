//============================================================================
// Name        : .cpp
// Author      : FAST CS Department
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Rush Hour...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
#include <fstream>
using namespace std;


string mytime; //stores the game time as string
int stime = 0; //stores the game times in sceonds
int ftime = 0; //stores the game time in 1/10 of a second
int minutes, seconds; //stores the game time in minutes and seconds
int tchoice = 0; //taxi choice(yellow or red)
int steps = 0; //taxi steps
int score=0; //stores player's score
int dropcount = 0; //stores number of passengers dropped
string stscore; //stores score as string 

//use for obstacle and passengers collisions
int leftcoll=0, rightcoll=0, upcoll=0, downcoll=0;
int leftpsngr=0, rightpsngr=0, uppsngr=0, downpsngr=0;

string myname=""; //The name that the player enters

//stores status of whether a passenger has been picked or not
bool picked = false;




// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}


int map[20][20] = {0}; //map grid initialized with all 0 values
int menu = 0; //menu initialized with 0, used to switch type of screen in game


/*This function draws the initital
  road sqaures. Everything else is drawn over these roads*/
void drawRoads() {
 	int ry = 950;
	int rx;	
	do{
	for(int i=0; i<20; i++){
	rx = i*50;
	DrawSquare(rx, ry, 50, colors[MISTY_ROSE]);
	DrawLine(rx+25 , ry+40 ,  rx+25 , ry+10 , 5 , colors[GRAY]);
	}	
	 ry-=50;
	}while(ry>=0); 
	
  glutPostRedisplay();	
}

int ocord[2][50] = {-1};
int ocount = 3;


/*This function gets random cordinates
  to assign to each obstacle*/
void rand_obstacles(){
 int x, y;
 srand(time(0));
 for(int i=0; i<=3; i++){
    do{
       x = rand()%20;
       y = rand()%20;
       }while(map[x][y]!=0 or (x==0 and y==19));
     ocord[0][i]=x;
     ocord[1][i]=y;
     map[x][y] = 2;
     
    }
  }

/*This function gets random cordinates
  to assign to each new obstacle
  These new obstacles are created when 2 more passenegers have been dropped*/
void rand_EObs(){
 int x, y;
 srand(time(0));
 
    do{
       x = rand()%20;
       y = rand()%20;
       }while(map[x][y]!=0 or (x==0 and y==19));
     ocord[0][ocount]=x;
     ocord[1][ocount]=y;
     map[x][y] = 2;
     
    
  }


int bcord[2][70];

/*This function assigns random cordinates for each building*/
void rand_buildings(){
 int x, y;
 srand(time(0));
 for(int i=0; i<=69; i++){
    do{
       x = rand()%20;
       y = rand()%20;
       }while(map[x][y]==1 or (x==0 and y==19));
     bcord[0][i]=x;
     bcord[1][i]=y;
     map[x][y] = 1;
     
    }
  }
  
int pcord[2][3];

/*This function assigns random cordinates for each passenger*/
void rand_passengers(){
   int x, y;
   for(int i=0; i<=2; i++){
    do{
       x = rand()%20;
       y = rand()%20;
       }while(map[x][y]!=0 or (x==0 and y==19));
     
     pcord[0][i]=x;
     pcord[1][i]=y;
     map[x][y] = 4;
     }
}     
 
/*This function is used to draw a stick figure passenger 
  on every passenger cordinate*/  
void drawPassengers(){
  for(int i=0; i<=2; i++){
        DrawCircle(pcord[0][i]*50+25,pcord[1][i]*50+40, 10,colors[BLACK]);
	DrawLine(pcord[0][i]*50+25,pcord[1][i]*50,pcord[0][i]*50+25,pcord[1][i]*50+45,5,colors[BLACK]);
	DrawLine(pcord[0][i]*50+35,pcord[1][i]*50+15,pcord[0][i]*50+25,pcord[1][i]*50+35,5,colors[BLACK]);
	DrawLine(pcord[0][i]*50+15,pcord[1][i]*50+15,pcord[0][i]*50+25,pcord[1][i]*50+35,5,colors[BLACK]);
  
  
  }
  glutPostRedisplay();
 } 
  
/*This function draws a black box on every
  cordinate that is assigned to a building*/  
void drawBuildings() {
 
  for(int i=0; i<=69; i++){
     DrawSquare(bcord[0][i]*50, bcord[1][i]*50, 50, colors[BLACK]);
    }
     
     glutPostRedisplay();
}       


int xI = 400, yI = 400;



int xC ,yC, Xmax, Xmin;
int xC2 ,yC2, Ymax, Ymin;

/*This function assigns 2 random cordinates to 2 cars
  This function makes sure that 1 car doesn't have any obstacle to its
  left and right and the 2nd car doesn't have any obstacle above or below it*/
void car_randMovement(){
 	int x,y;
 	srand(time(0));
 	do{
 	 x = rand()%20;
 	 y = rand()%20;
 	 }while(x==0 or x==19 or map[x][y]!=0 or map[x+1][y]!=0 or map[x-1][y]!=0);
 	 xC = x*50;
 	 yC = y*50;
 	 Xmax = xC+50;  
	 Xmin = xC-50;
	 map[x][y]=3;
	 map[x+1][y]=3;
	 map[x-1][y]=3;

	do{
 	 x = rand()%20;
 	 y = rand()%20;
 	 }while(y==0 or y==19 or map[x][y]!=0 or map[x][y+1]!=0 or map[x][y-1]!=0);
 	 xC2 = x*50;
 	 yC2 = y*50;
 	 Ymax = yC2+50;  
	 Ymin = yC2-50;
	 map[x][y]=3;
	 map[x+1][y]=3;
	 map[x-1][y]=3;
 

}



bool flag = true;
bool flag2 = true;

/*This function changes positions of the cars to move 1
  car up and down 3 blocks and the 2nd car left and right 3 blocks*/
void moveCar() {
	
	if (xC > Xmin && flag) {
		xC -= 10;
		
		if(xC == Xmin)
			
			flag = false;

	}
	else if (xC < Xmax && !flag) {
		
		xC += 10;
		if (xC == Xmax)
			flag = true;
	}
	
	if (yC2 > Ymin && flag2) {
		yC2 -= 10;
		
		if(yC2 == Ymin)
			
			flag2 = false;

	}
	else if (yC2 < Ymax && !flag2) {
		
		yC2 += 10;
		if (yC2 == Ymax)
			flag2 = true;
	}
}


int xT=0, yT=950;

/*This function is used to draw the player's taxi
  It's used to draw both, the red and the yellow taxis*/
void drawTaxi() {
	
	if(tchoice==1){
	DrawRectangle(xT+5, yT+20, 40, 20, colors[ORANGE]); //orange looks more yellow in this function
	DrawRectangle(xT+15,yT+40,20,10,colors[ORANGE]);}
	else if(tchoice==2){
	DrawRectangle(xT+5, yT+20, 40, 20, colors[RED]);
	DrawRectangle(xT+15,yT+40,20,10,colors[RED]);}
	if(picked==true)
	 DrawString(xT+20,yT+18,"P",colors[GREEN]);
	DrawCircle(xT+10, yT+15, 10, colors[BLACK]);
	DrawCircle(xT+40, yT+15, 10, colors[BLACK]);
	glutPostRedisplay();
	}

/*This function draws obstacles on the given cordinates
  in the shape of trees*/	
void drawObstacles(){
  for(int i=0; i<=3; i++){
  	DrawCircle(ocord[0][i]*50+25,ocord[1][i]*50+30,20, colors[GREEN]);
  	DrawRectangle(ocord[0][i]*50+20,ocord[1][i]*50,10,30, colors[BROWN]);
  } 
  glutPostRedisplay();
}


/*This function draws the cars that move
   left and right or up and down*/
void drawCars(){
        DrawRectangle(xC+5, yC+20, 40, 20, colors[BLUE]);
	DrawRectangle(xC+15,yC+40,20,10,colors[BLUE]);
	DrawCircle(xC+10, yC+15, 10, colors[BLACK]);
	DrawCircle(xC+40, yC+15, 10, colors[BLACK]);
	
	DrawRectangle(xC2+5, yC2+20, 40, 20, colors[BROWN]);
	DrawRectangle(xC2+15,yC2+40,20,10,colors[BROWN]);
	DrawCircle(xC2+10, yC2+15, 10, colors[BLACK]);
	DrawCircle(xC2+40, yC2+15, 10, colors[BLACK]);

			

glutPostRedisplay();
}

/*This function draws the extra obstacles
  that are created each time 2 more passengers are dropped*/ 
void drawEObs(){
	for(int i=4; i<=ocount; i++){
  	DrawCircle(ocord[0][i]*50+25,ocord[1][i]*50+30,20, colors[GREEN]);
  	DrawRectangle(ocord[0][i]*50+20,ocord[1][i]*50,10,30, colors[BROWN]);
  }
  glutPostRedisplay(); 
} 

/*This function is used when the
  player decides to choose a random taxi.
  This function choses yellow or red taxi
  for the player.*/
void taxi_Randomizer(){
 srand(time(0));
 tchoice = (rand()%2)+1;
 if(tchoice==1)
  steps = 10;
 else
   steps = 5;
}

int dX, dY; //Drop-off Destination cordinates

/*This function generates a random destination
  to drop off the passenger*/
void rand_destination(){
	int x, y;
	do{
       x = rand()%20;
       y = rand()%20;
       }while(map[x][y]!=0);
      dX = x*50;
      dY = y*50;
      }

/*This function draws the random destination*/
void drawDestination(){
	DrawCircle( dX + 25,dY + 25, 25, colors[GREEN]);
	DrawString( dX + 15, dY + 20, "D" , colors[MISTY_ROSE]);

glutPostRedisplay();
}

/*This function checks the players final score once the game is over
  If the final score deserves a spot in the highscores list
  then it replaces the player's score in the high score file.
  If the file already has 10 names then the last one is removed*/
void update_highscores(){
	int size=0;
	string x;
	string y;
	
	string ntemp;
	int stemp;	
	
	
	
	
	
	ifstream fin("highscores.txt");
	
		 
	 getline(fin, x, '%');
	 getline(fin, y, '\n');
	 while(stoi(y)!=-1){
	  size++;
	  getline(fin, x, '%');
	  getline(fin, y, '\n');
	  }
	
	fin.close();
	size++;
		string *name = new string[size]; 
		int *scores = new int[size];
	cout << "Size = " << size << endl;
	cout << "Names:" << endl;
	 ifstream in("highscores.txt");
	for(int i = 0; i<size-1; i++){
	
	  getline(in, x, '%');
	  getline(in, y, '\n');
	  name[i] = x;
	  scores[i] = stoi(y);
	  
	  cout << name[i] << " " << scores[i] << endl;
	  
	  }
	 in.close();
	 
	 /*cout << "Enter name: ";
	 getline(cin, myname);
	 cout << "Enter score: ";
	 cin >> myscore;*/
	 
	 name[size-1] = myname;
	 scores[size-1] = score;
	 
	 for(int i=0; i<=size-2;i++){
	  for(int j=0; j<=size-2;j++){
	    if(scores[j]<scores[j+1]){
	      stemp = scores[j];
	      ntemp = name[j];
	      scores[j] = scores[j+1];
	      name[j] = name[j+1];
	      scores[j+1] = stemp;
	      name[j+1] = ntemp;
	      }
	 }}

	 cout << endl << endl;
	 cout << "Sorted:" << endl;
	 
	 if(size>10){
	 
	  size=10;
	  } 
	 
	 ofstream fout("highscores.txt");
	 
	 for(int i = 0; i<size; i++){
	
	  
	  fout << name[i] << "%" << scores[i] <<'\n';
	  cout << name[i] << " " << scores[i] << endl;
	  
	  }
	 fout << "%-1\n";
	 fout.close();
	 
	 
}

/*This function draws the leader board 
  after reading it from the highscores file.
  It is called when the user wishes to view the leaderboard*/
void drawLeaderboard(){
	
	int size=0;
	string x;
	string y;
	
	DrawString( 200, 700, "High Scores", colors[RED]);
	
	ifstream fin("highscores.txt");
	
		 
	 getline(fin, x, '%');
	 getline(fin, y, '\n');
	 while(stoi(y)!=-1){
	  size++;
	  getline(fin, x, '%');
	  getline(fin, y, '\n');
	  }
	
	fin.close();
	size++;
		string *name = new string[size]; 
		int *scores = new int[size];
	
	
	ifstream in("highscores.txt");
	for(int i = 0; i<size-1; i++){
	
	  getline(in, x, '%');
	  getline(in, y, '\n');
	  name[i] = x;
	  scores[i] = stoi(y);
	  
	  
	  
	  }
	 in.close();
	
	int dispY = 600;
	
	for(int i = 0; i<size-1; i++){
	 DrawString( 200, dispY, name[i], colors[RED]);
	 DrawString( 400, dispY, Num2Str(scores[i]), colors[RED]);
	 dispY -= 50;
	 }	
	DrawString( 200, dispY-50, "Press Enter To Return..", colors[RED]);





}

/*Menu modes
  0: main menu screen
  1: car choosing screen
  2: game screen
  3: name enter screen
  4: game over screen
  5: leader board screen*/

/*
 * Main Canvas drawing function.
 * */
void GameDisplay()/**/{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(1/*Red Component*/, 1,	//148.0/255/*Green Component*/,
			1/*Blue Component*/, 1 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors
	//Red Square
	/*DrawSquare(400, 20, 40, colors[RED]);
	
	//Green Square
	DrawSquare( 250 , 250 ,20,colors[GREEN]); 
	
	//Display Score
	DrawString( 50, 800, "Score=0", colors[MISTY_ROSE]);
	
	// Trianlge Vertices v1(300,50) , v2(500,50) , v3(400,250)
	DrawTriangle( 300, 450 , 340, 450 , 320 , 490, colors[MISTY_ROSE] ); 
	


	//DrawLine(int x1, int y1, int x2, int y2, int lwidth, float *color)
	DrawLine( 550 , 50 ,  550 , 480 , 10 , colors[MISTY_ROSE] );	
	
	DrawCircle(50,670,10,colors[RED]);
	DrawCircle(70,670,10,colors[RED]);
	DrawCircle(90,670,10,colors[RED]);
	DrawRoundRect(500,200,50,100,colors[DARK_SEA_GREEN],70);
	DrawRoundRect(100,200,100,50,colors[DARK_OLIVE_GREEN],20);	
	DrawRoundRect(100,100,50,100,colors[DARK_OLIVE_GREEN],30);
	DrawRoundRect(200,100,100,50,colors[LIME_GREEN],40);
	DrawRoundRect(350,100,100,50,colors[LIME_GREEN],20);
	
	drawCar();*/
	DrawRectangle(1000,0,400,1000, colors[RED]);
	if(menu==0){
	 
	 DrawString( 400, 600, "PRESS 1: START GAME", colors[RED]);
	 DrawString( 400, 550, "PRESS 2: LEADER BOARD", colors[RED]);
	}
	
	if(menu==1){
	 
	 DrawString( 400, 600, "PRESS 1: Yellow Taxi", colors[RED]);
	 DrawString( 400, 550, "PRESS 2: Red Taxi", colors[RED]);
	 DrawString( 400, 500, "PRESS 3: Random", colors[RED]);
	}
	
	if(menu==2){
	drawRoads();		
	drawBuildings();
	drawObstacles();
	drawEObs();
	drawCars();
	if(picked==false)
	drawPassengers();
	else
	drawDestination();
	
	
	drawTaxi();
	
	
	DrawString( 1200, 900, "TIME", colors[MISTY_ROSE]);
	DrawString( 1200, 850, mytime, colors[MISTY_ROSE]);
	stscore = Num2Str(score);
	DrawString( 1200, 750, "Score", colors[MISTY_ROSE]);
	DrawString( 1220, 700, stscore , colors[MISTY_ROSE]);
	
	}
	
	if(menu==3){
	 DrawString( 400, 700, "Enter Your Name", colors[RED]);
	 DrawString( 400, 600, myname, colors[RED]);
	} 
	 	
	if(menu==4){
	 DrawString( 400, 600, "TIME'S UP", colors[RED]);
	 DrawString( 400, 500, "Your Score", colors[RED]);
	 DrawString( 450, 450, stscore, colors[RED]);
	 if(score>=100)
	  DrawString( 400, 400, "Congrats You Won", colors[RED]);
	 else
	  DrawString( 400, 400, "Sorry You Lost", colors[RED]);
         DrawString( 450, 350, "Press Enter to continue...", colors[RED]);	
	
	}
	
	
	if(menu==5){
	 drawLeaderboard();}		
	glutSwapBuffers(); // do not modify this line..

}



/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	
	
	
	if (key
			== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		
		if(xT!=0){
		
		if(xT%50!=0)
		 xT -= steps;
		else if(yT%50==0 and map[(xT-50)/50][yT/50]!=1)
		 xT -= steps; 
		else if(yT%50!=0 and map[(xT-50)/50][yT/50]!=1 and map[(xT-50)/50][1+(yT/50)]!=1)
		 xT -= steps;
		 
		 
		 if(tchoice==1){
		 if(map[xT/50][yT/50]==2){
		  
		   leftcoll++;
		  if(leftcoll==1){
		  score-=4;
		  upcoll=2;
		  rightcoll=2;
		  downcoll=2;}
		  else if(leftcoll==5  or map[xT/50][yT/50]==0)
		   leftcoll=0;
		 
		 }
		}
		
		else{
		if(map[xT/50][yT/50]==2){
		 leftcoll++;
		  if(leftcoll==1){
		  score-=3;
		  upcoll=2;
		  rightcoll=2;
		  downcoll=2;}
		  else if(leftcoll==10  or map[xT/50][yT/50]==0)
		   leftcoll=0;
		 
		 }
		 }
		 
		 if(map[xT/50][yT/50]==4 and picked==false){
		  
		   leftpsngr++;
		  if(leftpsngr==1){
		  score-=5;
		  uppsngr=2;
		  rightpsngr=2;
		  downpsngr=2;}
		  else if((leftpsngr==5  or map[xT/50][yT/50]==0) and tchoice==1)
		   leftpsngr=0;
		  else if((leftpsngr==10  or map[xT/50][yT/50]==0) and tchoice==2) 
		   leftpsngr=0;
		 }
		 
		 
		}
	} else if (key
			== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		
		if(xT!=950){
		
		//Building collisions
		if(xT%50!=0)
		xT += steps;
		else if(yT%50==0 and map[(xT+50)/50][yT/50]!=1)
		xT += steps;
		else if(yT%50!=0 and map[(xT+50)/50][yT/50]!=1 and map[(xT+50)/50][1+(yT/50)]!=1)
		xT += steps;
		
		
		
		if(tchoice==1){
		if(map[xT/50][yT/50]==2){
		  
		  rightcoll++;
		  if(rightcoll==1){
		  
		  score-=4;
		  leftcoll=2;
		  upcoll=2;
		  downcoll=2;}
		  else if(rightcoll==5 or map[xT/50][yT/50]==0)
		   rightcoll=0;
		 
		 } 
		
		 }
		 
		 else{
		 
		if(map[xT/50][yT/50]==2){
		  
		  rightcoll++;
		  if(rightcoll==1){
		  
		  score-=3;
		  leftcoll=2;
		  upcoll=2;
		  downcoll=2;}
		  else if(rightcoll==10 or map[xT/50][yT/50]==0)
		   rightcoll=0;
		 
		 } 
		 
		} 
		  if(map[xT/50][yT/50]==4 and picked==false){
		  
		   rightpsngr++;
		  if(rightpsngr==1){
		  score-=5;
		  uppsngr=2;
		  leftpsngr=2;
		  downpsngr=2;}
		  else if((rightpsngr==5  or map[xT/50][yT/50]==0) and tchoice==1)
		   rightpsngr=0;
		  else if((rightpsngr==10  or map[xT/50][yT/50]==0) and tchoice==2) 
		   rightpsngr=0;
		 }
		}
	} else if (key
			== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
		
		if(yT!=950){
		if(yT%50!=0)
		yT += steps;
		else if(xT%50==0 and map[xT/50][(yT+50)/50]!=1)
		yT += steps;
		else if(xT%50!=0 and map[xT/50][(yT+50)/50]!=1 and map[1+(xT/50)][(yT+50)/50]!=1)
		yT += steps;
		
		}
		if(tchoice==1){
		if(map[xT/50][yT/50]==2){
		  
		  upcoll++;
		  if(upcoll==1){
		  score-=4;
		  leftcoll=2;
		  rightcoll=2;
		  downcoll=2;
		  }
		  else if(upcoll==5 or map[xT/50][yT/50]==0)
		   upcoll=0;
		 
		 } 
		}
		
		else{
		 if(map[xT/50][yT/50]==2){
		  
		  upcoll++;
		  if(upcoll==1){
		  score-=3;
		  leftcoll=2;
		  rightcoll=2;
		  downcoll=2;
		  }
		  else if(upcoll==10 or map[xT/50][yT/50]==0)
		   upcoll=0;
		 
		 } 
		}
		if(map[xT/50][yT/50]==4 and picked==false){
		  
		   uppsngr++;
		  if(uppsngr==1){
		  score-=5;
		  rightpsngr=2;
		  leftpsngr=2;
		  downpsngr=2;}
		  else if((uppsngr==5  or map[xT/50][yT/50]==0) and tchoice==1)
		   uppsngr=0;
		  else if((uppsngr==10  or map[xT/50][yT/50]==0) and tchoice==2) 
		   uppsngr=0;
		 }
		
	
	}

	else if (key
			== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
		
		if(yT!=0){
		if(yT%50!=0)
		yT -= steps;
		else if(xT%50==0 and map[xT/50][(yT-50)/50]!=1)
		yT -= steps;
		else if(xT%50!=0 and map[xT/50][(yT-50)/50]!=1 and map[1+(xT/50)][(yT-50)/50]!=1)
		yT -= steps;
		
		if(tchoice==1){
		if(map[xT/50][yT/50]==2){
		  
		  downcoll++;
		  if(downcoll==1){
		  score-=4;
		  leftcoll=2;
		  rightcoll=2;
		  upcoll=2;}
		  else if(downcoll==5 or map[xT/50][yT/50]==0)
		   downcoll=0;
		 
		 } 
		}
		
		else{
		if(map[xT/50][yT/50]==2){
		  
		  downcoll++;
		  if(downcoll==1){
		  score-=3;
		  leftcoll=2;
		  rightcoll=2;
		  upcoll=2;}
		  else if(downcoll==5 or map[xT/50][yT/50]==0)
		   downcoll=0;
		 
		 }
		 }
		 if(map[xT/50][yT/50]==4 and picked==false){
		  
		   downpsngr++;
		  if(downpsngr==1){
		  score-=5;
		  rightpsngr=2;
		  leftpsngr=2;
		  uppsngr=2;}
		  else if((downpsngr==5  or map[xT/50][yT/50]==0) and tchoice==1)
		   downpsngr=0;
		  else if((downpsngr==10  or map[xT/50][yT/50]==0) and tchoice==2) 
		   downpsngr=0;
		 }
		 
		}
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
	
		//delete oxcord;
		//delete oycord;	
		exit(1); 
		
		// exit the program when escape key is pressed.
	}

	if ((key == 'b' || key == 'B') and menu==2) //Key for placing the bomb
			{
		//do something if b is pressed
		//cout << "b pressed" << endl;
		
		/*for(int i=0; i<size; i++){
		 map[oxcord[i]][oycord[i]]=0;
		 }
		size++;
		rand_obstacles();	
	for(int i=0; i<=19; i++){
	 for(int j=0; j<=19; j++){
	  cout << map[j][i] << " ";}
	  cout << endl;*/
	  cout << endl << "tchoice: " << tchoice << endl;
	  ocount++;
	  rand_EObs();
	  }
	
	
	if( key == '1' and menu==0)
	 menu = 3;
	else if( key == '2' and menu==0)
	 menu = 5;
	else if(key == '1' and menu==1){
	 menu = 2;
	 tchoice = 1;
	 steps = 10;}
	 else if(key == '2' and menu==1){
	 menu = 2;
	 tchoice = 2;
	 steps = 5;}
	 else if(key == '3' and menu==1){
	 menu = 2;
	 taxi_Randomizer();}
	 else if(key==13 and menu==3)
	  menu = 1;
	 else if(key==13 and menu==4){
	  menu = 0;
	  update_highscores();
	  myname="";
	  score=0;
	  ftime=0;
	  stime=0;
	  dropcount=0;
	  ocount = 3;
	  }
	  else if(key==13 and menu==5)
	   menu=0;
	 
	 
	 
	 if(menu==3 and ((key>=65 and key<=90) or (key>=97 and key<=122) or key==' ')) {
	  myname += key;
	  }
	  /*else if(menu==3 and key==8){
	   myname[myname.length()-1]=NULL; 
	   
	   }*/
	   
	   
	   
	   
	   
	 //Passenger pickup
	 if( key == ' ' and menu==2){
          if(picked==false and xT%50==0 and yT%50==0 and (map[(xT/50)+1][yT/50]==4 or  map[(xT/50)-1][yT/50]==4 or map[(xT/50)][(yT/50)+1]==4 or map[(xT/50)][(yT/50)-1]==4)){  	 
	  	picked = true;
	  	for(int i =0; i<=2; i++){
	  	 map[pcord[0][i]][pcord[1][i]] = 0;}
	  	rand_passengers();}
	  //passenger drop
	  if(picked==true and xT==dX and yT==dY){
	  	picked = false;
		rand_destination();
		score += 10;
		dropcount++;
		if(dropcount>0 and dropcount%2==0){
		ocount++;
	        rand_EObs();}
	  }  	 	 
	
	
	}
	glutPostRedisplay();
}



/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here
	moveCar();
	if(menu==2){
	ftime++;
	if(ftime%10==0)
	 stime++;
	minutes = stime/60;
	seconds = stime-(minutes*60);
	if(seconds<10)
	 mytime = "0" + Num2Str(minutes) + ":" + "0" + Num2Str(seconds);
	else 
	 mytime = "0" + Num2Str(minutes) + ":" + Num2Str(seconds) ;
	}
	
	if(stime==180){ //3 minutes up
	  menu = 4;
	  
	  
	  }
	  
	   
	
	
	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{
			cout<<"Right Button Pressed"<<endl;

	}
	glutPostRedisplay();
}
/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	int width = 1400, height = 1000; // i have set my window size to be 800 x 600

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Rush Hour by Naveed Ahmad"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
	rand_buildings();
	rand_obstacles();
	car_randMovement();
	rand_passengers();
	rand_destination();
	
	
	
	for(int i=0; i<=19; i++){
	 for(int j=0; j<=19; j++){
	  cout << map[j][i] << " ";}
	  cout << endl;
	  }
	
	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse
	
	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	
	glutMainLoop();
	
	return 1;
}
#endif /* RushHour_CPP_ */
