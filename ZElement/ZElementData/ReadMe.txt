========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : ElementData
========================================================================


AppWizard has created this ElementData application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your ElementData application.

ElementData.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

ElementData.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CElementDataApp application class.

ElementData.cpp
    This is the main application source file that contains the application
    class CElementDataApp.

ElementData.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

ElementData.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\ElementData.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file ElementData.rc.

res\ElementData.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.



/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CMDIFrameWnd and controls all MDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in ElementData.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

For the child frame window:

ChildFrm.h, ChildFrm.cpp
    These files define and implement the CChildFrame class, which
    supports the child windows in an MDI application.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

ElementDataDoc.h, ElementDataDoc.cpp - the document
    These files contain your CElementDataDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CElementDataDoc::Serialize).

ElementDataView.h, ElementDataView.cpp - the view of the document
    These files contain your CElementDataView class.
    CElementDataView objects are used to view CElementDataDoc objects.

res\ElementDataDoc.ico
    This is an icon file, which is used as the icon for MDI child windows
    for the CElementDataDoc class.  This icon is included by the main
    resource file ElementData.rc.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named ElementData.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

分版本系统：2012-8-14

sourcesafe根目录包含文件edit_version.ini
DataSeparatorVersion=服务器版本号

编辑器版本变量：
Global.cpp
unsigned long g_ulVersionCurrent;	// 保存DataSeparatorVersion=服务器版本号
const unsigned long g_ulVersion;	// 编辑器版本号，等于ExpTypes.h中#define ELEMENTDATA_VERSION

一个编辑器是否可以编辑，是否显示一项，判断依据是
BaseDataIDManBase::CanViewTempl()
BaseDataIDManBase::CanEditVersion()

举例：
DataSeparatorVersion=2
若
#define ELEMENTDATA_VERSION 3
则编辑器为新版本
若
#define ELEMENTDATA_VERSION 2
则编辑器为老版本
若
#define ELEMENTDATA_VERSION 1
则编辑器过期，会提示不可使用

只在新编辑器中可以使用的模板需要在OrgData.cfg中加版本号，老模板可以不加或加老版本号、

如果需要修改老模板，*.org需要修改，则需要将老的*.org复制一份并保存为xx_*.org，然后修改*.org，
这样编辑器会判断编辑器的版本号，老版本使用xx_*.org，新版本使用*.org。
版本2发布以后，服务器版本号将变为3，这时候需要吧old*.org删除，如果服务器找不到old*.org，会使用*.org。

每次修改或新加模板后修改下面相关代码，编辑器会取修改相关的文件
static const CString filesToGet[]

/////////////////////////////////////////////////////////////////////////////
