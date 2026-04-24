#pragma once
#include <APoint.h>

class A3DVECTOR3;

class CExportOrderGen
{
private:
    void Init(int nRows, int nCols, const A3DVECTOR3& vSunDir);
	void GenList(); 

public:
    APointI iCurChuck;
    APointI iOffset[4];
    APointI iNeighbours[4];
    bool    bGenLit[4];
    APointI iStep;
    APointI iSize;
	int iRow;
	int iCol;
	
    CExportOrderGen(int nRows, int nCols, const A3DVECTOR3& vSunDir);
    void Next();
};