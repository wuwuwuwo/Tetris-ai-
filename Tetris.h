#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>

#define GRIDSIZE 35
#define STAGE_WIDTH 10	//是从0开始计算的
#define STAGE_HEIGHT 20
#define STAGE_height 10
#define P1_STAGE_CORNER_X 156
#define P1_STAGE_CORNER_Y 174
#define P2_STAGE_CORNER_X 844
#define P2_STAGE_CORNER_Y 174
#define P1_NEXT_CORNER_X 587
#define P1_NEXT_CORNER_Y 125
#define P2_NEXT_CORNER_X 702
#define P2_NEXT_CORNER_Y 125
#define HOLD_CORNER_X 660
#define HOLD_CORNER_Y 275


typedef struct PDresult {
	//PD算法结构体
	float allScore;
	int x;
	int r;
	bool isDie = false;
}PDresult;

typedef enum PLAYROLE
{
	roleNONE,//空
	rolePLAYER1,//玩家1
	rolePLAYER2,//玩家2
};

typedef enum gridShape
{
	//形状数值，I形状是第0个形状
	shapeI,
	shapeS,
	shapeZ,
	shapeT,
	shapeL,
	shapeJ,
	shapeO,
};

using namespace sf;

class Tetris
{
public:
	Tetris();		//发生函数
	~Tetris();
	int Figures[7][4] =
	{
		5,3,1,7,//I
		5,4,2,7,//S
		5,4,3,6,//Z
		5,3,4,7,//T
		5,3,2,7,//L
		5,7,3,6,//J
		5,2,3,4 //O
	};

	//用于hold区的变量
	static int holdcolorNum, holdShapeNum;
	static Vector2i holdSquare[4];

	Vector2i mCornPoint, nextSquareCornPoint, holdSquareCornPoint;//游戏区域位置
	int colorNum, nextcolorNum, tempcolorNum;		//表征方块颜色的值
	int currentShapeNum, nextShapeNum, tempShapeNum;//用于取到方块的值
	int role, gridsize, score;						//角色,块大小,分数

	int b7array[7] = { 0 }, b7INT;					//用于BAG7算法
	int imgBGno, imgSkinNo;							//用于多媒体的input
	Texture* tTiles;
	Texture tBackground, tButtons, tNum, tTimer, tCounter, tGameOver;//创建纹理对象
	Sprite  sBackground, sTiles, sButtons, sNum, sTimer, sCounter, sGameOver;//创建精灵对象  
	int Field[STAGE_HEIGHT][STAGE_WIDTH] = { 0 };						   //对field进行初始化

	int tField[STAGE_HEIGHT][STAGE_WIDTH] = { 0 };	 //对field进行初始化

	Vector2i currentSquare[4], nextSquare[4], tempSquare[4], shadowSquare[4], origionSquare[4], torigionSquare[4];
	PDresult PD[40];

	int dx;										//对于X方向的偏移量
	int count;	//遍历的次数
	int CountD = 0;
	bool rotate, hold, hardDrop, newFunFlag;

	bool gameOver;
	bool isGameBegin_Tetris;					//为了解决没开始游戏的时候有一个黑色的方块的投影的问题。
	//float timer, delay;
	
	PDresult tmy;
	PDresult tPD, aPD, bPD;

	void Initial(Texture* tex);
	void Input(sf::Event event);
	void Logic();

	void Draw(sf::RenderWindow* window);
	bool hitTest();


	void yMove();

	void holdFunc();

	void xMove();
	void rotateFunc();
	void checkline();
	void slowLoading();
	void hardDropFunc();
	void shadow();

	void isWin();

	//有关AI的函数
	bool AIhitTest();
	void AIholdFunc();
	void AILogic();
	void AIyMove();
	void AIcheckline();
	void AIRotateFunc();
	void Move();
	int BAG7();

	void PierreDellacherie();
	int LandingHeight();
	int ErodedPieceCellsMetric();
	int BoardRowTransitions();
	int BoardColTransitions();
	int BoardBuriedHoles();
	int BoardWells();

};

