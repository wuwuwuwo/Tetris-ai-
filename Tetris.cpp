#include "Tetris.h"
int Tetris::holdcolorNum = 0, Tetris::holdShapeNum = 0;
Vector2i Tetris::holdSquare[4] = { {0,0},{0,0},{0,0},{0,0} };

Tetris::Tetris()
{
	dx = 0;//X方向偏移量
	rotate = false;//是否旋转
	colorNum = 1;//色块的颜色
	//timer = 0;//计时间隔
	//delay = 0;//方块下落速度
	isGameBegin_Tetris = false;
	CountD = 0;
}

Tetris::~Tetris()
{

}

void Tetris::Initial(Texture* tex)
{
	gameOver = false;
	score = 0;
	newFunFlag = false;//为了消行定义的标志
	tTiles = tex;
	dx = 0;//x方向偏移量
	rotate = false;//是否旋转
	colorNum = 1;//色块的颜色
	//timer = 0;
	//delay = 0.3;//下落速度
	b7INT = 0;

	hold = false;//是否有 hold 块图形 
	hardDrop = false;//是否硬降
	holdSquareCornPoint = { HOLD_CORNER_X,HOLD_CORNER_Y };
	holdShapeNum = -1;//游戏初始时候，hold 区的图形设置一个异常值  
	sTiles.setTexture(*tTiles);
	/*for (int i = -2; i < STAGE_HEIGHT; i++) {
		for (int j = 0; j < STAGE_WIDTH; j++) {
			Field[i][j] = 0;
		}
	}*/
	if (role == rolePLAYER1)
	{
		mCornPoint = { P1_STAGE_CORNER_X,P1_STAGE_CORNER_Y };
		nextSquareCornPoint = { P1_NEXT_CORNER_X,P1_NEXT_CORNER_Y };
	}
	if (role == rolePLAYER2)
	{
		mCornPoint = { P2_STAGE_CORNER_X,P2_STAGE_CORNER_Y };
		nextSquareCornPoint = { P2_NEXT_CORNER_X,P2_NEXT_CORNER_Y };
	}
	sTiles.setTexture(*tTiles);

	//初始化方块，让第一个落下的不是一个正方形
	colorNum = 1 + rand() % 7;
	currentShapeNum = rand() % 7;
	
	//更新下个方块图形
	nextcolorNum = 1 + rand() % 7;
	nextShapeNum = rand() % 7;
	nextShapeNum = BAG7();

	for (int i = 0; i < 4; i++)
	{
		currentSquare[i].x = Figures[currentShapeNum][i] % 2 + STAGE_WIDTH / 2;
		currentSquare[i].y = Figures[currentShapeNum][i] / 2;
		nextSquare[i].x = Figures[nextShapeNum][i] % 2;
		nextSquare[i].y = Figures[nextShapeNum][i] / 2;
	}
	for (int i = 0; i < 4; i++) {
		torigionSquare[i] = currentSquare[i];
	}
	PierreDellacherie();

	Move();
	/*for (int j = 0; j < tPD.r; j++) {
		rotateFunc();
	}
	for (int i = 0; i < 4; i++) {
		currentSquare[i].x += tPD.x;
	}*/
}
void Tetris::xMove()
{
	//// <‐ 水平 Move ‐> /// 
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].x += dx;
	}

	if (!hitTest())//如果撞上了 
		for (int i = 0; i < 4; i++)
			currentSquare[i] = tempSquare[i];//到左右的边界，不能移出边界 
}
void Tetris::Move()
{
	
	bool flag = false;
	/*if (tPD.r == 0 && tPD.x == 0)  hardDropFunc();*/
	if (tPD.r > 0) {
		rotateFunc();
		tPD.r--;
	}
	for (int i = 0; i < 4; i++) {
		if (currentSquare[i].y > -2) flag = true;
		else {
			flag = false;
			break;
		}
	}
	if (tPD.x != 0 && tPD.r == 0&& flag == true)
	{
		if (tPD.x > 0) {
			for (int i = 0; i < 4; i++) {
				currentSquare[i].x += 1;
			}
			tPD.x--;
		}
		if (tPD.x < 0) {
			for (int i = 0; i < 4; i++) {
				currentSquare[i].x -= 1;
			}
			tPD.x++;
		}
	}
	/*if(tPD.r == 0&& tPD.x==0)  hardDropFunc();*/
}
void Tetris::rotateFunc()
{
	//// <rotate> /// 
	int originalHeight = currentSquare[0].y;
	for (int j = 0; j < 4; j++)//踢墙中心偏移遍历
	{
		Vector2i p = currentSquare[0];//设置旋转中心点
		for (int i = 0; i < 4; i++)
		{//顺时针旋转90度
			int x = currentSquare[i].y - p.y;//原 Y 方向距离中心点的差值，作为新的差 值，传递给 X 方向
			int y = currentSquare[i].x - p.x;//原 X 方向距离中心点的差值，作为新的差 值，传递给 Y 方向


			currentSquare[i].x = p.x - x;//新坐标 X=中心点坐标‐新的 X 方向差值
			currentSquare[i].y = p.y + y;//新坐标 Y=中心点坐标+新的 Y 方向差值 
		}
		if (hitTest())//如果没撞上
		{
			int detaY = 0;
			detaY = currentSquare[0].y - originalHeight;//新老重心的高度差
			if (detaY != 0)
				for (int i = 0; i < 4; i++)
					currentSquare[i].y -= detaY;//对高度差进行修正
			if (!hitTest())
				for (int i = 0; i < 4; i++)
					currentSquare[i] = tempSquare[i];
			else
				break;
		}
		else
		{
			if (!hitTest())//如果撞上
				for (int i = 0; i < 4; i++)
					currentSquare[i] = tempSquare[i];
		}
	}
}
void Tetris::AIRotateFunc()
{
	Vector2i p = torigionSquare[0];//设置旋转中心点
	for (int i = 0; i < 4; i++)
	{//顺时针旋转90度
		int x = torigionSquare[i].y - p.y;//原 Y 方向距离中心点的差值，作为新的差 值，传递给 X 方向
		int y = torigionSquare[i].x - p.x;//原 X 方向距离中心点的差值，作为新的差 值，传递给 Y 方向
		torigionSquare[i].x = p.x - x;//新坐标 X=中心点坐标‐新的 X 方向差值
		torigionSquare[i].y = p.y + y;//新坐标 Y=中心点坐标+新的 Y 方向差值 
	}
}
void Tetris::AIcheckline()
{
	{
		//// <check lines 消行> /// 
		CountD++;
		int yCount = 0;
		for (int i = STAGE_height - 1; i >= 0; i--)
		{
			int count = 0;
			for (int j = 0; j < STAGE_WIDTH; j++)
			{
				if (CountD <= 2)
				{
					Field[i][j] = 0;
				}
				else
					gameOver = false;
			}
			yCount++;
		}
		//for (int i = 0; i < yCount; i++)
		//{
		//	for (int j = 0; j < STAGE_WIDTH; j++)
		//	{
		//		Field[i][j] = 0;
		//	}
		//}
		switch (yCount)
		{
		case 1:
			score += 1;
			break;
		case 2:
			score += 2;
			break;
		case 3:
			score += 3;
			break;
		case 4:
			score += 4;
			break;
		}
	}
}

void Tetris::checkline()
{
	//// <check lines 消行> /// 
	int k = STAGE_HEIGHT - 1;
	int yCount = 0;
	for (int i = STAGE_HEIGHT - 1; i >= 0; i--)
	{
		int count = 0;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (Field[i][j])
				count++;
			Field[k][j] = Field[i][j];
		}
		if (count < STAGE_WIDTH)
			k--;
		else
			yCount++;
	}
	for (int i = 0; i < yCount; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			Field[i][j] = 0;
		}
	}
	switch (yCount)
	{
	case 1:
		score += 1;
		break;
	case 2:
		score += 2;
		break;
	case 3:
		score += 3;
		break;
	case 4:
		score += 4;
		break;
	}
}
void Tetris::Logic()
{
	//// <‐ 水平 Move ‐> ///    
	xMove();

	//////Rotate//////    
	if (rotate)
	{
		rotateFunc();
		rotate = false;
	}

	shadow();
	if (hardDrop)
	{
		hardDropFunc();
		hardDrop = false;
	}

	slowLoading();
	//// <tick 下落> /// 

	yMove();
	//timer = 0;


///////check lines//////////
	checkline();

	//hold方块图形
	if (hold)
	{
		holdFunc();
		hold = false;
	}

	isWin();
}

void Tetris::AILogic()
{
	
	AIyMove();
	/*hardDropFunc();*/
	
	checkline();
	

	if (newFunFlag) {
		/*for (int i = 0; i < 4; i++) {
			torigionSquare[i] = currentSquare[i];
		}
		PierreDellacherie();*/
		AIholdFunc();
		/*
		for (int j = 0; j < tPD.r; j++) {
			rotateFunc();
		}
		for (int i = 0; i < 4; i++) {
			currentSquare[i].x += tPD.x;
		}*/
		
		newFunFlag = false;
	}
	/*if (currentSquare[0].y>8) hardDropFunc();*/
	for (int i = 0; i < 1; i++) {
		if (tPD.r == 0 && tPD.x == 0) break;
		Move();
		
	}
	  /* Move();
	   Move();*/
	  /* if (tPD.r == 0 && tPD.x == 0)  hardDropFunc();*/
	//hold方块图形
	/*if (hold)
	{
		holdFunc();
		hold = false;
	}*/
	shadow();
	/*isWin();*/
}

void Tetris::Input(sf::Event event)
{
	if (role == rolePLAYER2)//玩家2的按键响应
	{
		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::Up)
				//if(currentShapeNum!=shapeO)
				rotate = true;
			if (event.key.code == Keyboard::Left)
				dx = -1;
			else if (event.key.code == Keyboard::Right)
				dx = 1;
			if (event.key.code == Keyboard::Down)
				;//delay = 0.05;
		}

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::RControl)
				hold = true;
			if (event.key.code == Keyboard::Space)
				hardDrop = true;



			if (event.key.code == Keyboard::Left || event.key.code == Keyboard::Right)
				dx = 0;
			if (event.key.code == Keyboard::Down)
				;//delay = 0.3;//时响应间隔变大，方块下降速度变慢
		}
	}

	if (role == rolePLAYER1)//玩家1的按键响应
	{
		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::W)
				//if(currentShapeNum!=shapeO)
				rotate = true;
			if (event.key.code == Keyboard::A)
				dx = -1;
			else if (event.key.code == Keyboard::D)
				dx = 1;
			if (event.key.code == Keyboard::S)
				;//delay = 0.05;
		}

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::LControl)
				hold = true;
			if (event.key.code == Keyboard::Q)
				hardDrop = true;



			if (event.key.code == Keyboard::A || event.key.code == Keyboard::D)
				dx = 0;
			if (event.key.code == Keyboard::S)
				;//delay = 0.3;//时响应间隔变大，方块下降速度变慢
		}
	}
}

void Tetris::Draw(sf::RenderWindow* window)
{
	//绘制NEXT区的方块
	for (int i = 0; i < 4; i++)
	{
		sTiles.setTextureRect(IntRect(nextcolorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
		sTiles.setPosition(nextSquare[i].x * GRIDSIZE, nextSquare[i].y * GRIDSIZE);
		sTiles.move(nextSquareCornPoint.x, nextSquareCornPoint.y);//offset
		window->draw(sTiles);
	}


	//绘制hold区的方块
	if (holdShapeNum > -1)//hold区图形正常时候进行绘制
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(holdcolorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(holdSquare[i].x * GRIDSIZE, holdSquare[i].y * GRIDSIZE);
			sTiles.move(holdSquareCornPoint.x, holdSquareCornPoint.y);
			window->draw(sTiles);
		}

	//绘制shadow的方块
	if (isGameBegin_Tetris) {
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(shadowSquare[i].x * GRIDSIZE, shadowSquare[i].y * GRIDSIZE);
			sTiles.setColor(Color(50, 50, 50, 255));//设置阴影的颜色
			sTiles.move(mCornPoint.x, mCornPoint.y);
			window->draw(sTiles);
			sTiles.setColor(Color(255, 255, 255, 255));//颜色改回原色
		}

		//绘制固定的方块
		for (int i = 0; i < STAGE_HEIGHT; i++)
			for (int j = 0; j < STAGE_WIDTH; j++)
			{
				if (Field[i][j] == 0)
					continue;//在无方块的地方，（空地）就略过
				sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
				sTiles.move(mCornPoint.x, mCornPoint.y);//offset设置偏移量
				window->draw(sTiles);
			}

		//绘制活动的方块
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(currentSquare[i].x * GRIDSIZE, currentSquare[i].y * GRIDSIZE);
			sTiles.move(mCornPoint.x, mCornPoint.y);//offset设置偏移量
			window->draw(sTiles);
		}
	}
}

bool Tetris::hitTest()
{
	for (int i = 0; i < 4; i++)
		if (/*currentSquare[i].x < 0 || currentSquare[i].x >= STAGE_WIDTH ||*/ currentSquare[i].y >= STAGE_HEIGHT)
			return false;
		else if (Field[currentSquare[i].y][currentSquare[i].x] > 0)
			return false;
	return true;
}
bool Tetris::AIhitTest()
{
	for (int i = 0; i < 4; i++)
		if (torigionSquare[i].x < 0 || torigionSquare[i].x >= STAGE_WIDTH || torigionSquare[i].y >= STAGE_HEIGHT)
			return false;
		else if (Field[torigionSquare[i].y][torigionSquare[i].x] > 0)
			return false;
	return true;
}
void Tetris::yMove()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].y += 1;

	}

	if (!hitTest())//如果撞上了
	{
		for (int i = 0; i < 4; i++)
			Field[tempSquare[i].y][tempSquare[i].x] = colorNum;

		//delay = 0.3;
		//取下个方块图形
		colorNum = nextcolorNum;

		currentShapeNum = nextShapeNum;

		//更新下个方块图形
		nextcolorNum = 1 + rand() % 7;
		nextShapeNum = BAG7();

		for (int i = 0; i < 4; i++)
		{
			currentSquare[i] = nextSquare[i];
			currentSquare[i].x = nextSquare[i].x + STAGE_WIDTH / 2;
			nextSquare[i].x = Figures[nextShapeNum][i] % 2;
			nextSquare[i].y = Figures[nextShapeNum][i] / 2;
		}

	}
}

void Tetris::AIyMove()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].y += 1;
	}

	if (!hitTest())//如果撞上了
	{
		for (int i = 0; i < 4; i++)
			Field[tempSquare[i].y][tempSquare[i].x] = colorNum;

		//delay = 0.3;
		//取下个方块图形
		colorNum = nextcolorNum;

		currentShapeNum = nextShapeNum;

		//更新下个方块图形
		nextcolorNum = 1 + rand() % 7;
		nextShapeNum = BAG7();

		for (int i = 0; i < 4; i++)
		{
			currentSquare[i].y = nextSquare[i].y - 2;
			currentSquare[i].x = nextSquare[i].x + STAGE_WIDTH / 2;
			nextSquare[i].x = Figures[nextShapeNum][i] % 2;
			nextSquare[i].y = Figures[nextShapeNum][i] / 2;
		}
		newFunFlag = true;
		count = 0;
	}
}
int Tetris::BAG7()
{
	int num;

	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	num = rand() % 7;
	/*for (int i = 0; i < b7INT; i++)
	{
		if (b7array[i] == num)
		{
			i = -1;
			num = rand() % 7;
		}
	}

	b7array[b7INT] = num;

	b7INT++;
	if (b7INT == 7)
	{
		b7INT = 0;
		for (int i = 0; i < 7; i++)
			b7array[i] = 0;

	}*/
	return num;
}

void Tetris::holdFunc()
{
	Vector2i backUpSquare[4];
	tempcolorNum = holdcolorNum;
	tempShapeNum = holdShapeNum;

	holdcolorNum = colorNum;
	holdShapeNum = currentShapeNum;
	for (int i = 0; i < 4; i++)
	{
		holdSquare[i].x = Figures[holdShapeNum][i] % 2;
		holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		tempSquare[i].x = Figures[tempShapeNum][i] % 2;
		tempSquare[i].y = Figures[tempShapeNum][i] / 2;
		backUpSquare[i] = currentSquare[i];
	}

	if (tempShapeNum == -1)//hold 区图形的异常值表示 hold 区为空的状态，所以要从 Next 区 取值
	{
		//如果原 hold 区为空，则当前图形从 Next 取   
		//取下个方块图形
		colorNum = nextcolorNum;
		currentShapeNum = nextShapeNum;

		//更新下个方块图形
		nextcolorNum = 1 + rand() % 7;
		nextShapeNum = BAG7();

		for (int i = 0; i < 4; i++)
		{
			currentSquare[i] = nextSquare[i];//当前块更新
			currentSquare[i].x = currentSquare[i].x + STAGE_WIDTH / 2;
			nextSquare[i].x = Figures[nextShapeNum][i] % 2;
			nextSquare[i].y = Figures[nextShapeNum][i] / 2;
		}
	}

	else //当前图形取原来hold图形的值，发生交换
	{
		colorNum = tempcolorNum;
		currentShapeNum = tempShapeNum;
		//从hold区置换过来的方块图形，在舞台上坐标的计算
		int minCurrentX = currentSquare[0].x,
			minCurrentY = currentSquare[0].y,
			minTempX = tempSquare[0].x,
			minTempY = tempSquare[0].y;
		int dx, dy;
		for (int i = 1; i < 4; i++)//?/??????????????????????????????
		{
			if (currentSquare[i].x < minCurrentX)
				minCurrentX = currentSquare[i].x;

			if (currentSquare[i].y < minCurrentY)
				minCurrentY = currentSquare[i].y;

			if (tempSquare[i].x < minTempX)
				minTempX = tempSquare[i].x;

			if (tempSquare[i].y < minTempY)
				minTempY = tempSquare[i].y;
		}

		dx = minCurrentX - minTempX;
		dy = minCurrentY - minTempY;
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i].x = tempSquare[i].x + dx;
			currentSquare[i].y = tempSquare[i].y + dy;
			holdSquare[i].x = Figures[holdShapeNum][i] % 2;
			holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		}
	}

	if (!hitTest())
	{
		holdcolorNum = tempcolorNum;
		holdShapeNum = tempShapeNum;
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i] = backUpSquare[i];
		}
	}
}
void Tetris::hardDropFunc()
{
	for (int j = 0; j < STAGE_HEIGHT; j++)//一直下落
	{
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i].y += 1;
		}
		if (!hitTest())//如果撞上了
		{
			for (int i = 0; i < 4; i++)//到底了，返回上次位置
			{
				//currentSquare[i].y -= 1;
				currentSquare[i] = shadowSquare[i];
			}
			break;
		}
	}
}

void Tetris::slowLoading()//底部缓冲
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].y += 1;
	}
	if (!hitTest())
	{
		//delay = 0.45;
	}
	for (int i = 0; i < 4; i++)
		currentSquare[i] = tempSquare[i];
}

void Tetris::shadow()
{
	for (int i = 0; i < 4; i++)
	{
		shadowSquare[i] = currentSquare[i];
	}
	for (int j = 0; j < STAGE_HEIGHT; j++)//一直下落
	{
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i].y += 1;
		}

		if (!hitTest())
		{
			for (int i = 0; i < 4; i++)//到底了，返回上次位置
			{
				currentSquare[i].y -= 1;
			}
			break;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i] = shadowSquare[i];
		shadowSquare[i] = tempSquare[i];
	}
}

void Tetris::isWin()
{
	//原本应该用碰撞检测来做结束判断，但此处，两方块位置有一不为零，游戏即结束 
	for (int i = 0; i < STAGE_WIDTH; i++) {
		if (Field[0][i])
		{
			gameOver = true;
			//AIcheckline();
		}

	}
}