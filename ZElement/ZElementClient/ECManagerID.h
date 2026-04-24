#pragma once

///////////////////////////////////////////////////////////////////////////
//	
//	Manager ID in client
//
//  yaochunhui 2013.1.9	
///////////////////////////////////////////////////////////////////////////

//	Manager index
enum
{
    MAN_PLAYER = 0,		//	Player manager
    MAN_NPC,			//	NPC manager
    MAN_MATTER,			//	Matter manager
    MAN_ORNAMENT,		//	Ornament manager
    MAN_SKILLGFX,		//	Skill Gfx Manager
    MAN_DECAL,			//	Decal manager
    MAN_DEMOPLAYER,		//	Demo player
    MAN_DISTBOARD,      //  Distant board
    NUM_MANAGER,
};

//	Manager mask
enum
{
    MANMASK_PLAYER		= 0x0001,
    MANMASK_NPC			= 0x0002,
    MANMASK_MATTER		= 0x0004,
    MANMASK_ORNAMENT	= 0x0008,
    MANMASK_SKILLGFX	= 0x0010,
    MANMASK_DECAL		= 0x0020,
    MANMASK_DEMOPLAYER	= 0x0040,
    MANMASK_DISTBOARD	= 0x0080,
};
