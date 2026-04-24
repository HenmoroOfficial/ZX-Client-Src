// Excel.h: interface for the CExcel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCEL_H__98CDDFC5_592D_4E59_B0F7_2CAF62E65D72__INCLUDED_)
#define AFX_EXCEL_H__98CDDFC5_592D_4E59_B0F7_2CAF62E65D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
Modified： 转VB版本到VC++版本 xinqingfeng
*/

class CExcel  
{
public:
	CExcel();
	virtual ~CExcel();

public:

	enum ValueTypes
	{
		xlsInteger = 0,
		xlsNumber = 1,
		xlsText = 2,
    };
	
    //enum to hold cell alignment
	enum CellAlignment
	{
		xlsGeneralAlign = 0,
		xlsLeftAlign = 1,
		xlsCentreAlign = 2,
		xlsRightAlign = 3,
		xlsFillCell = 4,
		xlsLeftBorder = 8,
		xlsRightBorder = 16,
		xlsTopBorder = 32,
		xlsBottomBorder = 64,
		xlsShaded = 128,
	};

    //enum to handle selecting the font for the cell
    enum CellFont
	{
		//used by rgbAttr2
        //bits 0-5 handle the *picture* formatting, not bold/underline etc...
        //bits 6-7 handle the font number
        xlsFont0 = 0,
        xlsFont1 = 64,
        xlsFont2 = 128,
        xlsFont3 = 192,
    };

    enum CellHiddenLocked
	{//used by rgbAttr1
        //bits 0-5 must be zero
        //bit 6 locked/unlocked
        //bit 7 hidden/not hidden
        xlsNormal = 0,
        xlsLocked = 64,
        xlsHidden = 128,
	};

    //set up variables to hold the spreadsheet's layout
     enum MarginTypes
	 {
		xlsLeftMargin = 38,
        xlsRightMargin = 39,
        xlsTopMargin = 40,
        xlsBottomMargin = 41,
	 };

    enum FontFormatting
	{//'add these enums together. For example: xlsBold + xlsUnderline
        xlsNoFormat = 0,
        xlsBold = 1,
        xlsItalic = 2,
        xlsUnderline = 4,
        xlsStrikeout = 8,
	};

#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对齐

    struct FONT_RECORD
	{
		unsigned short opcode;//'49
        unsigned short length;//5+len(fontname)
        unsigned short FontHeight;
        //bit0 bold, bit1 italic, bit2 underline, bit3 strikeout, bit4-7 reserved
        BYTE FontAttributes1; 
        BYTE FontAttributes2; //'reserved - always 0
        BYTE FontNameLength;
	};

    struct PASSWORD_RECORD
	{
		unsigned short opcode;// As unsigned short '47
        unsigned short length;// As unsigned short 'len(password)
    };

    struct HEADER_FOOTER_RECORD
	{
		unsigned short opcode; //As unsigned short '20 Header, 21 Footer
        unsigned short length; //As unsigned short '1+len(text)
        BYTE  TextLength; //As Byte
    };

    struct PROTECT_SPREADSHEET_RECORD
	{
		unsigned short opcode; //As unsigned short '18
        unsigned short length; //As unsigned short '2
        unsigned short Protect; //As unsigned short
    };

    struct FORMAT_COUNT_RECORD
	{
		unsigned short opcode;// As unsigned short '1f
        unsigned short length;// As unsigned short '2
        unsigned short Count;// As unsigned short
    };

    struct FORMAT_RECORD
	{
		unsigned short opcode; //As unsigned short '1e
        unsigned short length; //As unsigned short '1+len(format)
        BYTE  FormatLength;// As Byte 'len(format)
    };

    struct COLWIDTH_RECORD
	{
		unsigned short opcode;// As unsigned short '36
        unsigned short length;// As unsigned short '4
        BYTE col1;// As Byte 'first column
        BYTE col2;// As Byte 'last column
        unsigned short ColumnWidth;// As unsigned short 'at 1/256th of a character
    };

    //'Beginning Of File record
    struct BEG_FILE_RECORD
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;// As unsigned short
        unsigned short version;// As unsigned short
        unsigned short ftype;// As unsigned short
    };

    //'End Of File record
    struct END_FILE_RECORD
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;// As unsigned short
    };

    //'true/false to print gridlines
    struct PRINT_GRIDLINES_RECORD
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;// As unsigned short
        unsigned short PrintFlag;// As unsigned short
    };

    //Integer record
    struct tInteger
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;// As unsigned short
        unsigned short row;// As unsigned short 'unsigned integer
        unsigned short col;// As unsigned short
        //'rgbAttr1 handles whether cell is hidden and/or locked
        BYTE rgbAttr1;// As Byte
        //'rgbAttr2 handles the Font# and Formatting assigned to this cell
        BYTE rgbAttr2;// As Byte
        //'rgbAttr3 handles the Cell Alignment/borders/shading
        BYTE rgbAttr3;// As Byte
        unsigned short intValue;// As unsigned short 'the actual integer value
    };

    //'Number record
    struct tNumber
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;// As unsigned short
        unsigned short row;// As unsigned short
        unsigned short col;// As unsigned short
        BYTE rgbAttr1;// As Byte
        BYTE rgbAttr2;// As Byte
        BYTE rgbAttr3;// As Byte
        double NumberValue;// As Double '8 Bytes
    };

    //'Label (Text) record
    struct tText
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;//As unsigned short
        unsigned short row;// As unsigned short
        unsigned short col;// As unsigned short
        BYTE rgbAttr1;// As Byte
        BYTE rgbAttr2;// As Byte
        BYTE rgbAttr3;// As Byte
        BYTE TextLength;// As Byte
    };

    struct MARGIN_RECORD_LAYOUT
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;// As unsigned short
        double  MarginValue;// As Double '8 bytes
    };

    struct  HPAGE_BREAK_RECORD
	{
		unsigned short opcode;// As unsigned short
        unsigned short length;// As unsigned short
        unsigned short NumPageBreaks;// As unsigned short
    };

    struct DEF_ROWHEIGHT_RECORD
	{
		int opcode;// As Integer
        int length;// As Integer
        int RowHeight;// As Integer
	};

    struct ROW_HEIGHT_RECORD
	{
		int opcode;// As Integer  '08
        int length;// As Integer  'should always be 16 bytes
        int RowNumber;// As Integer
        int FirstColumn;// As Integer
        int LastColumn;// As Integer
        int RowHeight;// As Integer  'written to file as 1/20ths of a point
        int internal;// As Integer
        BYTE DefaultAttributes;// As Byte  'set to zero for no default attributes
        int FileOffset;// As Integer
        BYTE rgbAttr1;// As Byte
        BYTE rgbAttr2;// As Byte
        BYTE rgbAttr3;// As Byte
    };
#pragma pack(pop)//设定为1字节对齐

    //'the memory copy API is used in the MKI$ function which converts an integer
    //'value to a 2-byte string value to write to the file. (used by the Horizontal
    //'Page Break function).
    //Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef lpvDest As String, ByRef lpvSource As unsigned short, ByVal cbCopy As Integer)
public:
	void CreateFile(const char* szFileName);
	void WriteValue(ValueTypes ValueType , CellFont CellFontUsed, CellAlignment Alignment, CellHiddenLocked HiddenLocked , int lrow, int lcol, const char* Value,int CellFormat);
	void CloseFile();
	void SetFont(const char* FontName, short FontHeight, FontFormatting FontFormat);
	void SetColumnWidth(int FirstColumn, int LastColumn, short WidthValue);
	void SetMargin(MarginTypes Margin , double MarginValue);
private:
	void WriteDefaultFormats();
	void MKI(unsigned short x, char*szBuf);
private:
    unsigned short m_shtFileNumber;// As unsigned short
    BEG_FILE_RECORD m_udtBEG_FILE_MARKER;// As BEG_FILE_RECORD
    END_FILE_RECORD m_udtEND_FILE_MARKER;// As END_FILE_RECORD
    HPAGE_BREAK_RECORD m_udtHORIZ_PAGE_BREAK;// As HPAGE_BREAK_RECORD

    //'create an array that will hold the rows where a horizontal page
    //'break will be inserted just before.
    unsigned short* m_shtHorizPageBreakRows;//As unsigned short
    unsigned short  m_shtNumHorizPageBreaks;// As unsigned short

	FILE *m_pXSLFile;

};

#endif // !defined(AFX_EXCEL_H__98CDDFC5_592D_4E59_B0F7_2CAF62E65D72__INCLUDED_)
