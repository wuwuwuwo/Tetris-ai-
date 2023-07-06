#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>
#include "Tetris.h"
#define B_START_CORNER_X 621
#define B_START_CORNER_Y 763
#define B_HOLD_CORNER_X 621
#define B_HOLD_CORNER_Y 822
#define B_LEFT_CORNER_X 70    
#define B_LEFT_CORNER_Y 460    
#define B_RIGHT_CORNER_X 1295    
#define B_RIGHT_CORNER_Y 460 
#define  P1_SCORE_CORNER_X 366
#define  P1_SCORE_CORNER_Y 110
#define  P2_SCORE_CORNER_X 1195
#define  P2_SCORE_CORNER_Y 110
#define  INFO_CORNER_X 622
#define  INFO_CORNER_Y 422

using namespace sf;
using namespace std;

typedef enum ButtonState {
	Continue_Dark,//继续暗
	Continue_Light,//继续亮
	Hold_Dark,//暂停暗
	Hold_Light,//暂停亮
	Close_Dark,//结束暗
	Close_Light,//结束亮
	Start_Dark,//开始暗
	Start_Light,//开始亮
};

class Game
{
public:
	Game();
	~Game();

	sf::RenderWindow window;
	Tetris player1, player2;
	bool gameOver, gameQuit;
	//bool P2AISwitch;//控制P2是否为AI
	Clock clock;
	int Window_width, Window_height, stageWidth, stageHeight;
	bool isGameBegin, isGameBegin_over;///游戏是否开始
	bool isGameHold;//记录游戏开始和暂停的状态
	int isGameOverState;//游戏结束的状态
	Vector2i mCornPoint;//游戏区域位置
	int  gridSize;//块大小
	int imgBGno, imgSkinNo;
	int imgSetNo;//记录游戏的素材编号
	Texture tTiles;
	Sprite sTiles;
	Texture tBackground, tButtons, tSwitcher, tFrame, tCover, tScore, tGameOver;//创建纹理对象
	Sprite sBackground, sButtons, sSwitcher, sFrame, sCover, sScore, sGameOver;//创建精灵对象  
	IntRect ButtonRectStart, ButtonRectHold, ButtonRectLeft, ButtonRectRight;
	int ButtonState_Start, ButtonState_Hold;//记录按钮的状态
	SoundBuffer sbWin;
	Sound soundWin;
	Music bgMusic;

	Clock gameClock;

	Font font;
	Text text;
	void gameInitial();
	void LoadMediaData();
	void gameInput();
	void gameLogic();
	void gameDraw();
	void gameRun();
	void DrawButton();
	void TextOutPut();
	void DrawResults();
};

