// Excel.cpp: implementation of the CExcel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementlocalize.h"
#include "Excel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcel::CExcel()
{
	m_udtBEG_FILE_MARKER.opcode = 9;
	m_udtBEG_FILE_MARKER.length = 4;
	m_udtBEG_FILE_MARKER.version = 2;
	m_udtBEG_FILE_MARKER.ftype = 10;
	
	m_udtEND_FILE_MARKER.opcode = 10; 
	m_udtEND_FILE_MARKER.length = 0;
    m_shtHorizPageBreakRows = 0;
	m_shtNumHorizPageBreaks = 0;
	m_pXSLFile = 0;
}

CExcel::~CExcel()
{

}

void CExcel::CreateFile(const char* szFileName)
{
	if(m_pXSLFile) fclose(m_pXSLFile);

	m_pXSLFile = fopen(szFileName,"wb");
	if(m_pXSLFile==NULL) return;
	
	int w = fwrite(&m_udtBEG_FILE_MARKER,sizeof(BEG_FILE_RECORD),1,m_pXSLFile);
	WriteDefaultFormats();
	
	if(m_shtHorizPageBreakRows) delete[] m_shtHorizPageBreakRows;
	m_shtHorizPageBreakRows = 0;
	m_shtNumHorizPageBreaks = 0;
}

void CExcel::WriteDefaultFormats()
{
	FORMAT_COUNT_RECORD cFORMAT_COUNT_RECORD;
	FORMAT_RECORD cFORMAT_RECORD;
	int lIndex;
	int l;
	const char* aFormat[] = 
	{
		"General",
		"0",
		"0.00",
		"#,##0",
		"#,##0.00",
		"#,##0\\ \"$\";\\-#,##0\\ \"$\"",
		"#,##0\\ \"$\";[Red]\\-#,##0\\ \"$\"",
		"#,##0.00\\ \"$\";\\-#,##0.00\\ \"$\"",
		"#,##0.00\\ \"$\";[Red]\\-#,##0.00\\ \"$\"",
		"0%",
		"0.00%",
		"0.00E+00",
		"dd/mm/yy",
		"dd/\\ mmm\\ yy",
		"dd/\\ mmm",
		"mmm\\ yy",
		"h:mm\\ AM/PM",
		"h:mm:ss\\ AM/PM",
		"hh:mm",
		"hh:mm:ss",
		"dd/mm/yy\\ hh:mm",
		"##0.0E+0",
		"mm:ss",
		"@"
	};
        
		
	cFORMAT_COUNT_RECORD.opcode = 0x1f;
	cFORMAT_COUNT_RECORD.length = 0x02;
	cFORMAT_COUNT_RECORD.Count = 23;
	fwrite(&cFORMAT_COUNT_RECORD,sizeof(FORMAT_COUNT_RECORD),1,m_pXSLFile);
	
	int a;
	for(lIndex = 0; lIndex<= 23; lIndex++)
	{
		l = strlen(aFormat[lIndex]);
		cFORMAT_RECORD.opcode = 0x1e;
		cFORMAT_RECORD.length = (short)(l+1);
		cFORMAT_RECORD.FormatLength = (byte)(l);
		fwrite(&cFORMAT_RECORD,sizeof(FORMAT_RECORD),1,m_pXSLFile);

		// Then the actual format
		// 从1开始还是从0开始？！
		char buf[256];
		strcpy(buf,aFormat[lIndex]);
		for(a=0;a<l;a++)
			fwrite(&buf[a],1,1,m_pXSLFile);
	}
}

void CExcel::CloseFile()
{
	short x;
	
	if(NULL != m_pXSLFile)
	{
		//'write the horizontal page breaks if necessary
		int lLoop1,lLoop2,lTemp;
		
		if(m_shtNumHorizPageBreaks > 0)
		{
			//'the Horizontal Page Break array must be in sorted order.
			//'Use a simple Bubble sort because the size of this array would
			//'be pretty small most of the time. A QuickSort would probably
			//'be overkill.
			for(lLoop1=m_shtNumHorizPageBreaks;lLoop1>=0;lLoop1--)
			{
				for(lLoop2=1;lLoop2<=lLoop1;lLoop2++)
				{
					if(m_shtHorizPageBreakRows[lLoop2 - 1] > m_shtHorizPageBreakRows[lLoop2])
					{
						lTemp = m_shtHorizPageBreakRows[lLoop2 - 1];
						m_shtHorizPageBreakRows[lLoop2 - 1] = m_shtHorizPageBreakRows[lLoop2];
						m_shtHorizPageBreakRows[lLoop2] = (short)lTemp;
					}
				}
			}
			//'write the Horizontal Page Break Record
			m_udtHORIZ_PAGE_BREAK.opcode = 27;
			m_udtHORIZ_PAGE_BREAK.length = (short)(2 + (m_shtNumHorizPageBreaks * 2));
			m_udtHORIZ_PAGE_BREAK.NumPageBreaks = (short)m_shtNumHorizPageBreaks;
			
			fwrite(&m_udtHORIZ_PAGE_BREAK,sizeof(HPAGE_BREAK_RECORD),1,m_pXSLFile);
			
			//'now write the actual page break values
			//'the MKI$ function is standard in other versions of BASIC but
			//'VisualBasic does not have it. A KnowledgeBase article explains
			//'how to recreate it (albeit using 16-bit API, I switched it
			//'to 32-bit).
			for(x = 1;x<=m_shtNumHorizPageBreaks;x++)
			{
				char szBuf[2];
				MKI(m_shtHorizPageBreakRows[x],szBuf);
				fwrite(szBuf,2,1,m_pXSLFile);
			}
		}
		fwrite(&m_udtEND_FILE_MARKER,sizeof(END_FILE_RECORD),1,m_pXSLFile);
		fclose(m_pXSLFile);
	}
	
}

void CExcel::MKI(unsigned short x, char*szBuf)
{      
     RtlMoveMemory(szBuf,&x, 2);
}

void CExcel::WriteValue(ValueTypes ValueType , CellFont CellFontUsed, CellAlignment Alignment, CellHiddenLocked HiddenLocked , int lrow, int lcol, const char* Value,int CellFormat)
{
	int l;
	short col,row;
	tText TEXT_RECORD;
	if(lrow>32767 || lrow<0)row = 0;
	else row = (short)(lrow-1);
	if(lcol > 32767 || lcol<0)col = 0;else col = (short)(lcol - 1);
	l = strlen(Value);
	
	TEXT_RECORD.opcode = 4;
	TEXT_RECORD.length = 10;
	//'Length of the text portion of the record
	TEXT_RECORD.TextLength = (byte)l;
	//                          'Total length of the record
	TEXT_RECORD.length = (byte)(8 + l);
	TEXT_RECORD.row = row;
	TEXT_RECORD.col = col;
	TEXT_RECORD.rgbAttr1 = (byte)(HiddenLocked);
	TEXT_RECORD.rgbAttr2 = (byte)(CellFontUsed + CellFormat);
	TEXT_RECORD.rgbAttr3 = (byte)(Alignment);
	//                        'Put record header
	fwrite(&TEXT_RECORD,sizeof(TEXT_RECORD),1,m_pXSLFile);
	//                'Then the actual string data
	fwrite(Value,l,1,m_pXSLFile);
}


void CExcel::SetMargin(MarginTypes Margin , double MarginValue)
{
	
	//'write the spreadsheet's layout information (in inches)
	MARGIN_RECORD_LAYOUT MarginRecord;
	MarginRecord.opcode = (short)Margin;
	MarginRecord.length = 8;
	MarginRecord.MarginValue = MarginValue;// 'in inches
	fwrite(&MarginRecord,sizeof(MARGIN_RECORD_LAYOUT),1,m_pXSLFile);
	
}

void CExcel::SetColumnWidth(int FirstColumn, int LastColumn, short WidthValue)
{
	
	COLWIDTH_RECORD COLWIDTH;
	
	COLWIDTH.opcode = 36;
	COLWIDTH.length = 4;
	COLWIDTH.col1 = (byte)(FirstColumn - 1);
	COLWIDTH.col2 = (byte)(LastColumn - 1);
	COLWIDTH.ColumnWidth = (short)(WidthValue * 256);// 'values are specified as 1/256 of a character
	fwrite(&COLWIDTH,sizeof(COLWIDTH_RECORD),1,m_pXSLFile);
	
}

void CExcel::SetFont(const char* FontName, short FontHeight, FontFormatting FontFormat)
{
	int l;
	//     'you can set up to 4 fonts in the spreadsheet file. When writing a value such
	//    'as a Text or Number you can specify one of the 4 fonts (numbered 0 to 3)
	FONT_RECORD FONTNAME_RECORD;
	l = strlen(FontName);// 'LenB(StrConv(FontName, vbFromUnicode)) 'Len(FontName)
	FONTNAME_RECORD.opcode = 49;
	FONTNAME_RECORD.length = (short)(5 + l);
	FONTNAME_RECORD.FontHeight = (short)(FontHeight * 20);
	FONTNAME_RECORD.FontAttributes1 = (byte)FontFormat;// 'bold/underline etc
	FONTNAME_RECORD.FontAttributes2 = (byte)0;// 'reserved-always zero!!
	FONTNAME_RECORD.FontNameLength = (byte)l;//'CByte(Len(FontName))
	fwrite(&FONTNAME_RECORD,sizeof(FONTNAME_RECORD),1,m_pXSLFile);
	//                        'Then the actual font name data
	fwrite(FontName,l,1,m_pXSLFile);
	
}


