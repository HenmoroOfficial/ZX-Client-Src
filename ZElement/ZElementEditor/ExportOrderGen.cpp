#include "stdafx.h"
#include "ExportOrderGen.h"
#include <A3DVector.h>
#include <A3DTypes.h>
#include <AAssist.h>

CExportOrderGen::CExportOrderGen(int nRows, int nCols, const A3DVECTOR3& vSunDir)
{
	Init(nRows, nCols, vSunDir);
}

void CExportOrderGen::Init(int nRows, int nCols, const A3DVECTOR3& vSunDir)
{
	// 初始化系数
	iSize.Set(nCols, nRows);
	int iDX = vSunDir.x >= 0 ? 1 : -1;
	int iDY = vSunDir.z >= 0 ? -1 : 1;
	// 第一个处理的块
	iCurChuck.Set(iDX >= 0 ? 1 : (nCols - 2), iDY >= 0  ? 1 : (nRows - 2));
	// 4个邻域的偏移
	iOffset[0].Set(0, 0); 
	iOffset[1].Set(-iDX, 0); 
	iOffset[2].Set(0, -iDY); 
	iOffset[3].Set(-iDX, -iDY);
	iStep.Set(iDX, iDY);
	// 将4块排成左上右上左下右下的顺序
	if (iOffset[0].y > iOffset[2].y)
		a_Swap(iOffset[0], iOffset[2]);
	if (iOffset[0].y > iOffset[3].y)
		a_Swap(iOffset[0], iOffset[3]);
	if (iOffset[1].y > iOffset[2].y)
		a_Swap(iOffset[1], iOffset[2]);
	if (iOffset[1].y > iOffset[3].y)
		a_Swap(iOffset[1], iOffset[3]);
	if (iOffset[0].x > iOffset[1].x)
		a_Swap(iOffset[0], iOffset[1]);
	if (iOffset[2].x > iOffset[3].x)
		a_Swap(iOffset[2], iOffset[3]);
	iNeighbours[0] = iCurChuck + iOffset[0];
	iNeighbours[1] = iCurChuck + iOffset[1];
	iNeighbours[2] = iCurChuck + iOffset[2];
	iNeighbours[3] = iCurChuck + iOffset[3];

	iRow = 0;
	iCol = 0;
	GenList();
}

void CExportOrderGen::GenList()
{
	// 判断除本身外还要计算哪块
	bGenLit[0] = bGenLit[1] = bGenLit[2] = bGenLit[3] = false;
	bGenLit[-iOffset[0].x - iOffset[0].y * 2] = true;
	if (iRow == 0 && iCol == 0)
	{
		bGenLit[(-iOffset[0].x - iStep.x) + (-iOffset[0].y - iStep.y) * 2] = true;
	}
	if (iRow == 0)
	{
		bGenLit[(-iOffset[0].x) + (-iOffset[0].y - iStep.y) * 2] = true;
	}
	if (iCol == 0)
	{
		bGenLit[(-iOffset[0].x - iStep.x) + (-iOffset[0].y) * 2] = true;
	}
}

void CExportOrderGen::Next()
{
	// 移动到下一块
	iCurChuck.x += iStep.x;
	iCol++;
	// 到行尾了，移到下一行
	if (iCurChuck.x + iOffset[0].x < 0
		|| iCurChuck.x + iOffset[1].x >=  iSize.x)
	{
		iCurChuck.x -= iStep.x * (iSize.x - 1);
		iCurChuck.y += iStep.y;
		iRow++;
		iCol = 0;
	}
	iNeighbours[0] = iCurChuck + iOffset[0];
	iNeighbours[1] = iCurChuck + iOffset[1];
	iNeighbours[2] = iCurChuck + iOffset[2];
	iNeighbours[3] = iCurChuck + iOffset[3];
	GenList();
}