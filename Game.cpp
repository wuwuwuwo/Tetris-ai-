#include "Game.h"
#include<fstream>
Game::Game()
{
	Window_width = 1350;
	Window_height = 1000;
	imgBGno = 1;
	imgSkinNo = 1;
	imgSetNo = 1;
	gameOver = false;
	
	window.create(sf::VideoMode(Window_width, Window_height), L"双人俄罗斯方块");
	ButtonState_Start = Start_Dark;
	ButtonState_Hold = Hold_Dark;//放到构造函数
	/*P2AISwitch = true;*/
}

Game::~Game()
{
}

void Game::gameInitial()
{
	window.setFramerateLimit(10);//设置游戏帧频
	LoadMediaData();//加载游戏素材
	srand(time(NULL));
	gameOver = false;
	isGameBegin = false;
	isGameHold = false;
	isGameBegin_over = false;
	

	player1.role = rolePLAYER1;//定义Tetris对象为player1
	player2.role = rolePLAYER2;//定义Tetris对象为player2

	player1.Initial(&tTiles);//将方块的素材传给Tetris对象，由Tetris对象绘制方块
	player2.Initial(&tTiles);//将方块的素材传给Tetris对象，由Tetris对象绘制方块

}

void Game::LoadMediaData()
{
	imgBGno = imgSetNo;
	imgSkinNo = imgSetNo;
	std::stringstream ss;
	ss << "data/images/bg" << imgBGno << ".jpg";
	if (!tBackground.loadFromFile(ss.str()))//加载纹理图片
		std::cout << "BK image 没有找到" << std::endl;

	ss.str("");//清空字符串

	ss << "data/images/tiles" << imgSkinNo << ".jpg";
	if (!tTiles.loadFromFile(ss.str()))//加载纹理图片
		std::cout << "tiles image 没有找到" << std::endl;

	if (!tFrame.loadFromFile("data/images/frame.png"))//加载纹理图片
		std::cout << "tFrame image 没有找到" << std::endl;

	if (!tCover.loadFromFile("data/images/cover.png"))//加载纹理图片
		std::cout << "tCover image 没有找到" << std::endl;

	if (!tGameOver.loadFromFile("data/images/end.png"))//加载纹理图片
		std::cout << "tGameOver image 没有找到" << std::endl;


	sBackground.setTexture(tBackground);//设置精灵对象的纹理
	//sTiles.setTexture(tTiles);//由tetris对象绘制方块，两个玩家各自绘制自己的方块
	sFrame.setTexture(tFrame);
	sCover.setTexture(tCover);//sCover 对应游 戏舞台上的边框遮盖对象
	sGameOver.setTexture(tGameOver);

	if (!tButtons.loadFromFile("data/images/button.png"))
		cout << "button.png 没有找到" << std::endl;
	if (!tSwitcher.loadFromFile("data/images/bgSwitch.png"))
		cout << "bgSwap.png 没有找到" << std::endl;
	sButtons.setTexture(tButtons);
	sSwitcher.setTexture(tSwitcher);
	sSwitcher.setOrigin(sSwitcher.getLocalBounds().width / 2.0, sSwitcher.getLocalBounds().height / 2.0);//对相应的精灵对象 sSwitcher 进行旋转中心点的设置
	if (!font.loadFromFile("data/Fonts/方正粗黑宋简体.ttf"))
		cout << "字体没有找到" << std::endl;
	text.setFont(font);
}

void Game::gameInput()
{
	sf::Event event;
	window.setKeyRepeatEnabled(false);//按键按下只响应一次
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
			gameQuit = true;
		}
		if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Escape)
		{
			window.close();
			gameQuit = true;
		}
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			if (ButtonRectStart.contains(event.mouseButton.x, event.mouseButton.y))
				if (isGameBegin == false)
				{
					player1.isGameBegin_Tetris = true;////为了解决没开始游戏的时候有一个黑色的方块的投影的问题。
					player2.isGameBegin_Tetris = true;////为了解决没开始游戏的时候有一个黑色的方块的投影的问题。
					isGameBegin = true;
					isGameBegin_over = true;
					ButtonState_Start = Close_Light;
					//gameInitial();//游戏开始  
				}
				else
				{
					isGameBegin = false;
					ButtonState_Start = Start_Light;
				}

			if (ButtonRectHold.contains(event.mouseButton.x, event.mouseButton.y))
				if (isGameHold == false)
				{
					isGameHold = true;
					ButtonState_Hold = Continue_Light;
				}
				else
				{
					isGameHold = false;
					ButtonState_Hold = Hold_Light;
				}
			if (ButtonRectLeft.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSetNo--;
				if (imgSetNo < 1)//小于 1 的时候从皮肤 4 开始再循环  
					imgSetNo = 4;//重新轮换皮肤图  
				LoadMediaData();
			}
			if (ButtonRectRight.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSetNo++;
				if (imgSetNo > 4)//大于皮肤图的总数时候  
					imgSetNo = 1;//重新轮换皮肤图  
				LoadMediaData();
			}
		}

		if (event.type == sf::Event::MouseMoved)
		{
			if (ButtonRectStart.contains(event.mouseMove.x, event.mouseMove.y))
				if (isGameBegin == false)
					ButtonState_Start = Start_Light;
				else
					ButtonState_Start = Close_Light;
			else
				if (isGameBegin == false)
					ButtonState_Start = Start_Dark;
				else
					ButtonState_Start = Close_Dark;

			if (ButtonRectHold.contains(event.mouseMove.x, event.mouseMove.y))
				if (isGameHold == false)
					ButtonState_Hold = Hold_Light;
				else
					ButtonState_Hold = Continue_Light;
			else
				if (isGameHold == false)
					ButtonState_Hold = Hold_Dark;
				else
					ButtonState_Hold = Continue_Dark;
		}
		player1.Input(event);
		player2.Input(event);
	}
}

void Game::gameLogic()
{
	if (isGameHold == true)
		return;
	//double time = clock.getElapsedTime().asSeconds();
	//clock.restart();
	//player1.timer += time;
	//player2.timer += time;
	//player1.Logic();
	/*if(!P2AISwitch)
		player2.Logic();
	else*/
	/*player1.gameOver = true;*/
	/*if(player1.gameOver==false) player1.AILogic();*/

	if (player2.gameOver == false) player2.AILogic();
}

void Game::gameDraw()
{
	window.clear();//清屏
	//绘制背景
	sBackground.setPosition(0, 0);
	window.draw(sBackground);
	window.draw(sFrame);
	player1.Draw(&window);
	player2.Draw(&window);

	//对左右玩家的游戏舞台进 行加“盖子”的操作，用于解决部分位于舞台边缘的方块跑出原游戏舞台边框的问题
	sCover.setPosition(P1_STAGE_CORNER_X, P1_STAGE_CORNER_Y);
	window.draw(sCover);
	sCover.setPosition(P2_STAGE_CORNER_X, P2_STAGE_CORNER_Y);
	window.draw(sCover);
	DrawButton();
	TextOutPut();
	DrawResults();

	window.display();//把显示缓冲区的内容，显示在屏幕上。SFML 采 用的是双缓冲机制
}

void Game::DrawResults() {
	int ButtonWidth, ButtonHeight;
	ButtonWidth = 250;
	ButtonHeight = sGameOver.getLocalBounds().height;
	if (player1.gameOver || player2.gameOver) {
		sGameOver.setTextureRect(IntRect(player1.gameOver * ButtonWidth, 0, ButtonWidth, ButtonHeight));
		sGameOver.setPosition(P1_STAGE_CORNER_X + GRIDSIZE * 1.5, 0);
		window.draw(sGameOver);
		sGameOver.setTextureRect(IntRect(player2.gameOver * ButtonWidth, 0, ButtonWidth, ButtonHeight));
		sGameOver.setPosition(P2_STAGE_CORNER_X + GRIDSIZE * 1.5, 0);
		window.draw(sGameOver);
	}
	//if (player2.gameOver) {
	//	ofstream OutFile; //利用构造函数创建txt文本，并且打开该文本
	//	OutFile.open("Test.txt", ios::in | ios::ate);
	//	OutFile << player2.score << endl;  //把字符串内容"This is a Test!"，写入Test.txt文件
	//	OutFile.close();
	//	isGameBegin = false;
	//}
}
void Game::TextOutPut() {
	int initialX, initialY;
	int CharacterSize = 48;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 0, 0, 255));//红色字体  
	text.setStyle(Text::Bold); // |Text::Underlined   
	text.setPosition(P1_SCORE_CORNER_X, P1_SCORE_CORNER_Y);
	std::stringstream ss;
	//player1.score = 9999;  
	ss << player1.score;
	text.setString(ss.str()); window.draw(text);
	text.setPosition(P2_SCORE_CORNER_X - CharacterSize * 3, P2_SCORE_CORNER_Y);
	//player2.score = 6666;  
	ss.str("");
	ss << player2.score;
	text.setString(ss.str()); window.draw(text);
}

void Game::DrawButton()
{
	int ButtonWidth, ButtonHeight;
	ButtonWidth = 110;
	ButtonHeight = sButtons.getLocalBounds().height;
	//ButtonRectStart
	sButtons.setTextureRect(IntRect(ButtonState_Start * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域 
	sButtons.setPosition(B_START_CORNER_X, B_START_CORNER_Y);
	ButtonRectStart.left = B_START_CORNER_X;
	ButtonRectStart.top = B_START_CORNER_Y;
	ButtonRectStart.width = ButtonWidth;
	ButtonRectStart.height = ButtonHeight;
	window.draw(sButtons);

	//ButtonRectHold
	sButtons.setTextureRect(IntRect(ButtonState_Hold * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(B_HOLD_CORNER_X, B_HOLD_CORNER_Y);
	ButtonRectHold.left = B_HOLD_CORNER_X;
	ButtonRectHold.top = B_HOLD_CORNER_Y;
	ButtonRectHold.width = ButtonWidth;
	ButtonRectHold.height = ButtonHeight;
	window.draw(sButtons);

	//背景素材切换
	ButtonWidth = sSwitcher.getLocalBounds().width;
	ButtonHeight = sSwitcher.getLocalBounds().height;

	//ButtonRectLeft    
	sSwitcher.setPosition(B_LEFT_CORNER_X, B_LEFT_CORNER_Y);
	ButtonRectLeft.left = B_LEFT_CORNER_X - ButtonWidth / 2;
	ButtonRectLeft.top = B_LEFT_CORNER_Y - ButtonHeight / 2;
	ButtonRectLeft.width = ButtonWidth;
	ButtonRectLeft.height = ButtonHeight;
	window.draw(sSwitcher);

	//ButtonRectRight
	sSwitcher.setPosition(B_RIGHT_CORNER_X, B_RIGHT_CORNER_Y);
	ButtonRectRight.left = B_RIGHT_CORNER_X - ButtonWidth / 2;
	ButtonRectRight.top = B_RIGHT_CORNER_Y - ButtonHeight / 2;
	ButtonRectRight.width = ButtonWidth;
	ButtonRectRight.height = ButtonHeight;
	sSwitcher.rotate(180);//只有一个箭头素材，当然要靠自己代码生成另一个^_^  
	window.draw(sSwitcher);
	sSwitcher.rotate(180);//还原角度  

}

void Game::gameRun()
{
	do {
		gameInitial();
		while (window.isOpen() && gameOver == false)
		{
			gameInput();
			if (isGameBegin == true)
				gameLogic();
			else {
				/*if (isGameBegin_over == true) {
					gameInitial();
				}*/
				//if (isGameBegin == false)
				//{
				//	player1.isGameBegin_Tetris = true;////为了解决没开始游戏的时候有一个黑色的方块的投影的问题。
				//	player2.isGameBegin_Tetris = true;////为了解决没开始游戏的时候有一个黑色的方块的投影的问题。
				//	isGameBegin = true;
				//	ButtonState_Start = Close_Light;
				//	//gameInitial();//游戏开始  
				//}
			}
			gameDraw();
			
		}
	} while (!gameQuit);
}