#ifndef _ELEMENTPCKDIR_H_
#define _ELEMENTPCKDIR_H_

const char *	g_szPckDir[] = {
	"configs",
	"facedata",
	"gfx",
	"grasses",
	"help\\script",
	"interfaces",
	"building",
	"models",
	"sfx",
	"shaders",
	"surfaces",
	"textures",
	"trees",
	"litmodels",
	"loddata",
};

const char *	g_szNoPckDir[][2] = {
	"cursors", "", // r means recursive for param 2
	"data", "",
	"font", "",
	"help\\html", "r",
	"maps", "r",
	"userdata\\userpic", "",
	"userdata\\character", "r",
	"userdata\\server", "",
	"music", "r",
	"info", "",
	"bbsfiles", "r",
	".", "s",		// s means file must be in the sep files list
	"reportbugs", "s",
	"kdfimage", "",
	"video", "",
	"gt", "",
	"arc", "",
	"directx", "",
	"microsoft.vc90.crt", "",
};

const char *	g_szSepFiles[] =
{
	"dbserver.conf",
	"elementclient.exe",
	"dbghelp.dll",
	"elementskill.dll",
	"ftdriver.dll",
	"ifc22.dll",
	"immwrapper.dll",
	"speedtreert.dll",
	"unicows.dll",
	"zlibwapi.dll",
	"zreportbugs.exe",
	"mfc42.dll",
	"modelview.exe",
	"kdfapi2.dll",
	"ArcSDK.dll",
	"fmodex.dll",
	"d3dx9_43.dll"
};

#endif
