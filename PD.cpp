#include "Tetris.h"

void Tetris::AIholdFunc()
{
	Vector2i tempSquareA[4], tempSquareB[4];
	//保存
	for (int i = 0; i < 4; i++) {
		torigionSquare[i] = currentSquare[i];
	}
	PierreDellacherie();
	aPD = tPD;
	if (holdShapeNum < 0)
	{
		for (int i = 0; i < 4; i++) {
			torigionSquare[i] = nextSquare[i];
			torigionSquare[i].x = torigionSquare[i].x + STAGE_WIDTH / 2;
		}
	}
	else {

		for (int i = 0; i < 4; i++) {
			torigionSquare[i].x = Figures[holdShapeNum][i] % 2 + STAGE_WIDTH / 2;
			torigionSquare[i].y = Figures[holdShapeNum][i] / 2;
		}
	}
	PierreDellacherie();
	bPD = tPD;
	if (aPD.allScore < bPD.allScore-600)
	{
		tPD = bPD;
		hold = true;
		if (hold)// hold方块图形
		{
			holdFunc();
			hold = false;
		}
	}
	else {
		tPD = aPD;
	}
}
void Tetris::PierreDellacherie() {
	for (int i = 0; i < STAGE_HEIGHT; i++) {
		for (int j = 0; j < STAGE_WIDTH; j++) {
			tField[i][j] = Field[i][j];
		}
	}
	float all = -10000000;
	int dxCount = 0, roCount = 0;
	count = 0;
	for (int j = 0; j < 40; j++) {
		PD[j].allScore = all; 
		PD[j].r = roCount; 
		PD[j].x = dxCount;
	}
	//保存
	for (int i = 0; i < 4; i++) {
		origionSquare[i] = torigionSquare[i];
	}

	//穷举
	for (int j = 0; j < 4; j++) {//旋转

		dxCount = 0;
		for (int k = 0; k < j; k++) {
			AIRotateFunc();
		}
		roCount = j;
		//一直左移回归左上角
		for (int j = 0; j < STAGE_WIDTH; j++) {
			for (int i = 0; i < 4; i++) 
			{
				torigionSquare[i].x -= 1;
			}
			dxCount--;//平移量
			//碰撞检测
			if (!AIhitTest()) 
			{
				for (int i = 0; i < 4; i++) {//到边了，返回上一次位置
					torigionSquare[i].x += 1;
				}
				dxCount++;
				break;
			}
		}

		for (int t = 0; t < 4; t++) {
			tempSquare[t] = torigionSquare[t];
		}
		for (int i = 0; i < STAGE_WIDTH; i++) {//平移
	
			float allValue;

			/*allValue = (-45) * LandingHeight() * 1.0
				+ (34) * ErodedPieceCellsMetric() * 1.0
				+ (-33) * BoardRowTransitions() * 1.0
				+(-93) * BoardColTransitions() * 1.0
				+ (-78) * BoardBuriedHoles() * 1.0
				+ (-34) * BoardWells() * 1.0;*/
			allValue = (-45) * LandingHeight() * 1.0
				+ (34) * ErodedPieceCellsMetric() * 1.0
				+ (-33) * BoardRowTransitions() * 1.0
				+ (-93) * BoardColTransitions() * 1.0
				+ (-78) * BoardBuriedHoles() * 1.0
				+ (-34) * BoardWells() * 1.0;
			
			PD[count].allScore = allValue;
			PD[count].x = dxCount;
			PD[count].r = roCount;//记录评估值、平移、旋转
			count++;

			//记录完毕，回归左上角
			for (int t = 0; t < 4; t++) {
				torigionSquare[t] = tempSquare[t];
			}
			for (int t = 0; t < 4; t++) {//计算完成后右移i格
				torigionSquare[t].x += i + 1;
			}
			dxCount++;//平移量
			if (!hitTest()) {
				break;
			}
		}
		//下一次
		for (int i = 0; i < 4; i++) {
			torigionSquare[i] = origionSquare[i];
		}
	}
	PDresult tempPD;
	for (int z = count; z > 0; z--) {
		for (int t = 0; t < z - 1; t++) {
			if (PD[t].allScore > PD[t + 1].allScore) {
				tempPD = PD[t];
				PD[t] = PD[t + 1];
				PD[t + 1] = tempPD;
			}
		}
	}
	for (int t = count - 1; t > 0; t--) {
		if (PD[t].isDie == false) {
			tPD = PD[t];
			break;
		}
	}
}

//Landing height 
int Tetris::LandingHeight() {
	
	for (int j = 0; j < STAGE_HEIGHT; j++) {//一直下落
		for (int i = 0; i < 4; i++) {
			torigionSquare[i].y += 1;
		}
		if (!AIhitTest()) {
			for (int i = 0; i < 4; i++) {//到底了，返回上一次位置
				torigionSquare[i].y -= 1;
			}
			break;
		}
	}
	return 20- torigionSquare[0].y-1;
}
//Eroded piece cells
int Tetris::ErodedPieceCellsMetric() {
	int row = 0, squ = 0;
	int sk = STAGE_HEIGHT - 1;
	for (int i = 0; i < 4; i++)
		Field[torigionSquare[i].y][torigionSquare[i].x] = colorNum;
	for (int i = STAGE_HEIGHT - 1; i > 0; i--)
	{

		int count = 0;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (Field[i][j]) {
				count++;
			}
			Field[sk][j] = Field[i][j];
		}
		if (count < STAGE_WIDTH)
			sk--;
		if (count == STAGE_WIDTH) {
			row++;
			for (int k = 0; k < 4; k++) {
				if (torigionSquare[k].y == i)squ++;
			}
		}

	}
	
	return row* squ;
}
//Row transitions
int Tetris::BoardRowTransitions() {
	int rowTrans = 0;

	for (int i = STAGE_HEIGHT - 1; i >= 0; i--)
	{
		for (int j = 0; j < STAGE_WIDTH - 1; j++)
		{
			if (Field[i][j] == 0) {
				if (Field[i][j + 1] > 0)rowTrans++;
			}
			if (Field[i][j] > 0) {
				if (Field[i][j + 1] == 0)rowTrans++;
			}
		}
		if (Field[i][0] == 0) {
			rowTrans++;
		}
		if (Field[i][STAGE_WIDTH - 1] == 0) {
			rowTrans++;
		}
	}

	return rowTrans;
}
//Colum transitions
int Tetris::BoardColTransitions() {
	int colTrans = 0;
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		for (int i = STAGE_HEIGHT - 1; i > 0 + 1; i--)
		{
			if (Field[i][j] == 0)
			{
				if (Field[i - 1][j] > 0)colTrans++;
			}
			if (Field[i][j] > 0)
			{
				if (Field[i - 1][j] == 0)colTrans++;
			}
		}
		if (Field[STAGE_HEIGHT - 1][j] == 0) {
			colTrans++;
		}
	}

	return colTrans;
}
//Number of Holes
int Tetris::BoardBuriedHoles() {
	int holes = 0;
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		int colholes = -1;
		for (int i = 0; i < STAGE_HEIGHT; i++)
		{
			if (colholes < 0 && Field[i][j] > 0) {
				colholes = 0;
			}
			if (colholes >= 0 && Field[i][j] == 0) {
				colholes++;
			}
		}
		if (colholes > 0) {
			holes = holes + colholes;
		}
	}

	return holes;
}
//Well Sum
int Tetris::BoardWells() {
	int sum_n[] = { 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120, 136, 153, 171, 190, 210 };
	int bWell = 0;
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		int wells = 0;
		for (int i = STAGE_HEIGHT - 1; i > 0 + 1; i--)
		{
			if (Field[i][j] == 0) {
				if ((j - 1 < 0 || Field[i][j - 1] > 0) && (j + 1 >= STAGE_WIDTH || Field[i][j + 1] > 0))
				{//两边是墙或者实心方格
					wells++;
				}
				else
				{
					bWell += sum_n[wells];
					wells = 0;
				}
			}
			else
			{
				bWell += sum_n[wells];
				wells = 0;
			}
		}
	}
	for (int i = 0; i < STAGE_WIDTH; i++) {
		PD[count].isDie = false;
		if (Field[0][i]) {
			PD[count].isDie = true;
			break;
		}
	}
	for (int i = 0; i < 4; i++)
		Field[torigionSquare[i].y][torigionSquare[i].x] = 0;
	for (int i = 0; i < STAGE_HEIGHT; i++) {
		for (int j = 0; j < STAGE_WIDTH; j++) {
			Field[i][j] = tField[i][j];
		}
	}
	return bWell;
}