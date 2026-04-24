#include "AnmTranslator.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmBezierPoint.h"
#include "AnmBezierSegment.h"
#include "AnmBezier.h"
#include "AnmBezierManager.h"
#include "AnmMove.h"
#include "AnmRotate.h"
#include "AnmScale.h"
#include "AnmFOV.h"
#include "AnmTransparent.h"
#include "AnmMotion.h"
#include "AnmJump.h"
#include "AnmBezierMove.h"
#include "AnmDisplayHide.h"
#include "AnmDialogue.h"
#include "AnmSound.h"
#include "AnmHook.h"
#include "AnmSpeedControl.h"
#include "AnmCameraBind.h"
#include "AnmCameraFilter.h"
#include "AnmCameraOscillatory.h"
#include "AnmHideWorldAction.h"
#include "AnmLightning.h"
//#include "AnmColorMap.h"
//#include "AnmFog.h"
//#include "AnmLock.h"
//#include "AnmActionBlur.h"
//#include "AnmPauseGameSound.h"
//#include "AnmPlaySound.h"
#include "AnmActionChannel.h"
#include "AnmActionGroup.h"
#include "AnmActionManager.h"
#include <AF.h>
#include <stdio.h>

CAnmTranslator::CAnmTranslator( CAnimator* pAnimator )
	: m_pAnimator( pAnimator )
{
}

CAnmTranslator::~CAnmTranslator()
{
}

bool CAnmTranslator::SaveFile(const wchar_t *szFile)
{
	int i;
	AFile file;
	DWORD iWriteLength;
	ACString stringName;
	if( ! file.Open( WC2AS(szFile), AFILE_BINARY | AFILE_CREATENEW ) )
		return false;
	file.ResetPointer();

	// Write file head
	AnmFileHead fileHead;
	fileHead.m_iMagic					= ANM_FILE_MAGIC;
	fileHead.m_iVersion					= ANM_FILE_VERSION;
	fileHead.m_dwTotalTime				= m_pAnimator->GetTotalTime();
	fileHead.m_iWorldInstance			= m_pAnimator->GetWorldInstance();
	fileHead.m_bUseHostPlayer			= m_pAnimator->IsUseHostPlayer();
	fileHead.m_bShowHostPlayerWeapon	= m_pAnimator->IsShowHostPlayerWeapon();
	fileHead.m_bSpecifyTime				= m_pAnimator->IsSpecifyTime();
	fileHead.m_iHour					= m_pAnimator->GetHour();
	fileHead.m_iMinute					= m_pAnimator->GetMinute();
	fileHead.m_iSecond					= m_pAnimator->GetSecond();
	fileHead.m_bSpecifyWeather			= m_pAnimator->IsSpecifyWeahter();
	fileHead.m_bRain					= m_pAnimator->IsRain();
	fileHead.m_bSnow					= m_pAnimator->IsSnow();
	file.Write(&fileHead, sizeof(AnmFileHead), &iWriteLength);

	// Write Objects to file
	CAnmObjectManager *pObjectManager = m_pAnimator->GetObjectManager();
	std::vector<unsigned int> objectList;
	pObjectManager->GetObjectIDList(objectList);
	fileHead.m_iObjectAmount = pObjectManager->GetObjectAmount();
	fileHead.m_iObjectOffset = file.GetPos();
	file.Seek(fileHead.m_iObjectOffset, AFILE_SEEK_SET);

	AnmObject object;
	CAnmObject *pObject = 0;
	for (i=0; i<fileHead.m_iObjectAmount; ++i)
	{
		pObject = pObjectManager->GetObject(objectList[i]);
		object.m_uiObjectID = pObject->GetObjectID();
		object.m_uiObjectType = pObject->GetObjectType();
		object.m_fOffsetX = pObject->GetOffset().x;
		object.m_fOffsetY = pObject->GetOffset().y;
		object.m_fOffsetZ = pObject->GetOffset().z;
		stringName = AS2AC(pObject->GetLoadFile());
		wcscpy(object.m_sModelName, AC2WC(&stringName[0]));
		stringName = pObject->GetDisplayName();
		wcscpy(object.m_sDisplayName, AC2WC(&stringName[0]));
		file.Write(&object, sizeof(AnmObject), &iWriteLength);
	}

	// Write Beziers to file
	CAnmBezierManager *pBezierManager = m_pAnimator->GetBezierManager();
	fileHead.m_iBezierAmount = pBezierManager->GetBezierAmount();
	fileHead.m_iBezierOffset = file.GetPos();
	file.Seek(fileHead.m_iBezierOffset, AFILE_SEEK_SET);
	std::vector<unsigned int> bezierList;
	pBezierManager->GetBezierIDList(bezierList);

	AnmBezier bezier;
	CAnmBezier *pBezier = 0;
	for (i=0; i<fileHead.m_iBezierAmount; ++i)
	{
		pBezier = pBezierManager->GetBezier(bezierList[i]);
		bezier.m_uiBezierID = pBezier->GetID();
		bezier.m_iPointAmount = pBezier->GetPointAmount();
		stringName = pBezier->GetDisplayName();
		wcscpy(bezier.m_sDisplayName, AC2WC(&stringName[0]));
		file.Write(&bezier, sizeof(AnmBezier), &iWriteLength);

		// Write BezierPoints and BezierSegments to file
		std::vector<unsigned int> bezierPointList;
		std::vector<unsigned int> bezierSegmentList;
		pBezier->GetPointIDList(bezierPointList);
		pBezier->GetSegmentIDList(bezierSegmentList);
		AnmBezierPoint bezierPoint;
		CAnmBezierPoint *pBezierPoint = 0;
		AnmBezierSegment bezierSegment;
		CAnmBezierSegment *pBezierSegment = 0;
		for (int j=0; j<bezier.m_iPointAmount; ++j)
		{
			if (j > 0)
			{
				pBezierSegment = pBezier->GetSegment(bezierSegmentList[j-1]);
				bezierSegment.m_fAnchorAX = pBezierSegment->GetAnchorA().x;
				bezierSegment.m_fAnchorAY = pBezierSegment->GetAnchorA().y;
				bezierSegment.m_fAnchorAZ = pBezierSegment->GetAnchorA().z;
				bezierSegment.m_fAnchorBX = pBezierSegment->GetAnchorB().x;
				bezierSegment.m_fAnchorBY = pBezierSegment->GetAnchorB().y;
				bezierSegment.m_fAnchorBZ = pBezierSegment->GetAnchorB().z;
				file.Write(&bezierSegment, sizeof(AnmBezierSegment), &iWriteLength);
			}

			pBezierPoint = pBezier->GetPoint(bezierPointList[j]);
			bezierPoint.m_fPosX = pBezierPoint->GetPos().x;
			bezierPoint.m_fPosY = pBezierPoint->GetPos().y;
			bezierPoint.m_fPosZ = pBezierPoint->GetPos().z;
			bezierPoint.m_fOrientationW = pBezierPoint->GetOrientation().w;
			bezierPoint.m_fOrientationX = pBezierPoint->GetOrientation().x;
			bezierPoint.m_fOrientationY = pBezierPoint->GetOrientation().y;
			bezierPoint.m_fOrientationZ = pBezierPoint->GetOrientation().z;
			bezierPoint.m_iTotalFrames = pBezierPoint->GetTotalFrames();
			bezierPoint.m_fFOV = pBezierPoint->GetFOV();
			file.Write(&bezierPoint, sizeof(AnmBezierPoint), &iWriteLength);
		}
	}

	// Write Action Groups to file
	CAnmActionManager *pActionManager = m_pAnimator->GetActionManager();
	fileHead.m_iGroupAmount = pActionManager->GetGroupAmount();
	fileHead.m_iActionOffset = file.GetPos();
	file.Seek(fileHead.m_iActionOffset, AFILE_SEEK_SET);

	std::vector<unsigned int> groupList;
	pActionManager->GetGroupIDList(groupList);
	for(i = 0; i < fileHead.m_iGroupAmount; ++ i )
		SaveActionGroupToFile( file, pActionManager->GetGroup( groupList[i] ) );
	
	file.ResetPointer();
	file.Write(&fileHead, sizeof(AnmFileHead), &iWriteLength);

	file.Close();
	return true;
}

bool CAnmTranslator::LoadFile( const wchar_t* szFile, const A3DVECTOR3& c_vCameraPos )
{
	int i;
    AFileImage file;
    DWORD iReadLength;
    if( ! file.Open( WC2AS(szFile), AFILE_BINARY | AFILE_OPENEXIST ) )
		return false;
    file.ResetPointer();

    // Read file head
    AnmFileHead fileHead;
    file.Read( & fileHead, sizeof( fileHead ), & iReadLength );
    if( fileHead.m_iMagic != ANM_FILE_MAGIC || fileHead.m_iVersion > ANM_FILE_VERSION )
    {
        file.Close();
        return false;
    }
    // Set total time
    m_pAnimator->SetTotalTime(fileHead.m_dwTotalTime);

    // Load World
    m_pAnimator->LoadWorld( fileHead.m_iWorldInstance, c_vCameraPos );

    m_pAnimator->SetUseHostPlayer(fileHead.m_bUseHostPlayer);
    m_pAnimator->SetShowHostPlayerWeapon(fileHead.m_bShowHostPlayerWeapon);

    m_pAnimator->SetSpecifyTime(fileHead.m_bSpecifyTime);
    m_pAnimator->SetWorldTime(fileHead.m_iHour, fileHead.m_iMinute, fileHead.m_iSecond);

    m_pAnimator->SetSpecifyWeather(fileHead.m_bSpecifyWeather);
    m_pAnimator->SetWeatherRain(fileHead.m_bRain);
    m_pAnimator->SetWeatherSnow(fileHead.m_bSnow);

    // Read Objects from file and create Objects for ObjectManager
    CAnmObjectManager *pObjectManager = m_pAnimator->GetObjectManager();
    file.Seek(fileHead.m_iObjectOffset, AFILE_SEEK_SET);

    AnmObject object;
    CAnmObject *pObject = 0;
    for (i=0; i<fileHead.m_iObjectAmount; ++i)
    {
        file.Read(&object, sizeof(AnmObject), &iReadLength);
        pObjectManager->CreateObject(object.m_uiObjectID,
            CAnmObject::ObjectType(object.m_uiObjectType));
        pObject = pObjectManager->GetObject(object.m_uiObjectID);
        pObject->Load(WC2AS(object.m_sModelName));
        pObject->SetDisplayName(WC2AC(object.m_sDisplayName));
        pObject->SetOffset( A3DVECTOR3( object.m_fOffsetX, object.m_fOffsetY, object.m_fOffsetZ ) );
    }
    // Read Beziers from file and create Bezier for BezierManager
    CAnmBezierManager *pBezierManager = m_pAnimator->GetBezierManager();
    file.Seek(fileHead.m_iBezierOffset, AFILE_SEEK_SET);

    AnmBezier bezier;
    CAnmBezier *pBezier = 0;
    for (i=0; i<fileHead.m_iBezierAmount; ++i)
    {
		int j;
        file.Read(&bezier, sizeof(AnmBezier), &iReadLength);
        pBezierManager->CreateBezier(bezier.m_uiBezierID);
        pBezier = pBezierManager->GetBezier(bezier.m_uiBezierID);
        pBezier->SetDisplayName(WC2AC(bezier.m_sDisplayName));

        // Read BezierPoints and BezierSegments from file
        AnmBezierPoint bezierPoint;
        CAnmBezierPoint *pBezierPoint = 0;
        AnmBezierSegment bezierSegment;
        CAnmBezierSegment *pBezierSegment = 0;
        for (j=0; j<bezier.m_iPointAmount; ++j)
        {
            pBezier->AddPoint(A3DVECTOR3(0.0f, 0.0f, 0.0f));
        }
        std::vector<unsigned int> bezierPointList;
        std::vector<unsigned int> bezierSegmentList;
        pBezier->GetPointIDList(bezierPointList);
        pBezier->GetSegmentIDList(bezierSegmentList);

        for (j=0; j<bezier.m_iPointAmount; ++j)
        {
            if (j > 0)
            {
                file.Read(&bezierSegment, sizeof(AnmBezierSegment), &iReadLength);
                pBezierSegment = pBezier->GetSegment(bezierSegmentList[j-1]);
                pBezierSegment->SetAnchorA(A3DVECTOR3(bezierSegment.m_fAnchorAX,
                                                      bezierSegment.m_fAnchorAY,
                                                      bezierSegment.m_fAnchorAZ));
                pBezierSegment->SetAnchorB(A3DVECTOR3(bezierSegment.m_fAnchorBX,
                                                      bezierSegment.m_fAnchorBY,
                                                      bezierSegment.m_fAnchorBZ));
            }

            file.Read(&bezierPoint, sizeof(AnmBezierPoint), &iReadLength);
            pBezierPoint = pBezier->GetPoint(bezierPointList[j]);
            pBezierPoint->SetPos(A3DVECTOR3(bezierPoint.m_fPosX,
                                            bezierPoint.m_fPosY,
                                            bezierPoint.m_fPosZ));
            pBezierPoint->SetOrientation(A3DQUATERNION(bezierPoint.m_fOrientationW,
                                                       bezierPoint.m_fOrientationX,
                                                       bezierPoint.m_fOrientationY,
                                                       bezierPoint.m_fOrientationZ));
            pBezierPoint->SetTotalFrames(bezierPoint.m_iTotalFrames);
            pBezierPoint->SetFOV(bezierPoint.m_fFOV);
        }
        pBezier->ReCalculateLength();
    }

    file.Seek(fileHead.m_iActionOffset, AFILE_SEEK_SET);

    // Read Action Groups from file and create Action Groups for ActionManager
    for(i = 0; i < fileHead.m_iGroupAmount; ++ i )
        LoadActionGroupFromFile( file, fileHead.m_iVersion );

    file.Close();
    return true;
}

bool CAnmTranslator::ExportActionGroup( const wchar_t* szFile, CAnmActionGroup* pActionGroup )
{
	if( ! pActionGroup )
		return true;

	AFile file;
	if( ! file.Open( WC2AS(szFile), AFILE_BINARY | AFILE_CREATENEW ) )
		return false;
	file.ResetPointer();
	SaveActionGroupToFile( file, pActionGroup );
	file.Close();
	return true;
}

bool CAnmTranslator::ImportActionGroup(const wchar_t *szFile)
{
    AFileImage file;
    if( ! file.Open( WC2AS(szFile), AFILE_BINARY | AFILE_OPENEXIST ) )
		return false;
    file.ResetPointer();
    LoadActionGroupFromFile( file );
    file.Close();
    return true;
}

bool CAnmTranslator::SaveConfigFile( const char* szFile )
{
	AFile file;
	if( ! file.Open( szFile, AFILE_BINARY | AFILE_CREATENEW ) )
		return false;
	file.ResetPointer();

	DWORD iWriteLength;
	int iVersion = ANM_CONFIG_FILE_VERSION;
	file.Write( & iVersion, sizeof( iVersion ), & iWriteLength );

	const AnimationMap& c_AnmMap = m_pAnimator->GetAnimationMap();
	int iNum = c_AnmMap.size();
	file.Write( & iNum, sizeof( iNum ), & iWriteLength );

	for( AnimationMap::const_iterator c_iterAnm = c_AnmMap.begin(); 
		c_iterAnm != c_AnmMap.end(); ++ c_iterAnm )
	{
		AnmConfig* pAnimation = c_iterAnm->second;
		file.Write( pAnimation, sizeof( * pAnimation ), & iWriteLength );
	}
	file.Close();
	return true;
}

bool CAnmTranslator::LoadConfigFile( const char* szFile )
{
	AFileImage file;
	if( ! file.Open( szFile, AFILE_BINARY | AFILE_OPENEXIST ) )
		return false;
	file.ResetPointer();

	DWORD iReadLength;
	int iVersion;
	file.Read( & iVersion, sizeof( iVersion ), & iReadLength );
	if( iVersion > ANM_CONFIG_FILE_VERSION )
	{
		file.Close();
		return false;
	}
	int iNum;
	file.Read( & iNum, sizeof( iNum ), & iReadLength );

	AnimationMap& anmMap = m_pAnimator->GetAnimationMap();
	for( AnimationMap::iterator iterAnm = anmMap.begin(); 
		iterAnm != anmMap.end(); ++ iterAnm )
		delete iterAnm->second;
	anmMap.clear();

	if( iVersion < ANM_CONFIG_FILE_VERSION )
	{
		AnmConfig_old animation;
		for( int i = 0; i < iNum; ++ i )
		{
			file.Read( & animation, sizeof( animation ), & iReadLength );
			ASSERT( iReadLength == sizeof( animation ) );
			ASSERT( anmMap.find( animation.m_iID ) == anmMap.end() );

			AnmConfig* pAnimation = new AnmConfig;

			pAnimation->m_iID				= animation.m_iID;
			memcpy( pAnimation->m_szPath, animation.m_szPath, sizeof( animation.m_szPath ) );
			pAnimation->m_bShowElsePlayer	= animation.m_bShowElsePlayer;
			pAnimation->m_bShowNPC			= animation.m_bShowNPC;
			pAnimation->m_bShowDynModel		= false;
			pAnimation->m_bStopAutoMove		= animation.m_bStopAutoMove;
			pAnimation->m_fDestPosX			= animation.m_fDestPosX;
			pAnimation->m_fDestPosZ			= animation.m_fDestPosZ;
			pAnimation->m_bCanEscStop		= animation.m_bCanEscStop;
			pAnimation->m_bMuteGameMusic	= animation.m_bMuteGameMusic;

			anmMap[ pAnimation->m_iID ]	= pAnimation;
		}
	}
	else
	{
		for( int i = 0; i < iNum; ++ i )
		{
			AnmConfig* pAnimation = new AnmConfig;
			file.Read( pAnimation, sizeof( * pAnimation ), & iReadLength );
			ASSERT( iReadLength == sizeof( * pAnimation ) );
			ASSERT( anmMap.find( pAnimation->m_iID ) == anmMap.end() );
			anmMap[ pAnimation->m_iID ] = pAnimation;
		}
	}
	file.Close();
	return true;
}

bool CAnmTranslator::SaveActionGroupToFile( AFile& file, CAnmActionGroup* pActionGroup )
{
	if (pActionGroup)
	{
		AnmActionGroup actionGroup;
		DWORD iWriteLength;
		ACString stringName;
		actionGroup.m_uiGroupID = pActionGroup->GetGroupID();
		actionGroup.m_iChannelAmount = pActionGroup->GetChannelAmount();
		stringName = pActionGroup->GetGroupName();
		wcscpy(actionGroup.m_sGroupName, AC2WC(&stringName[0]));
		file.Write(&actionGroup, sizeof(AnmActionGroup), &iWriteLength);

		// Write Action Channels to file
		std::vector<unsigned int> channelList;
		pActionGroup->GetChannelIDList(channelList);
		for (int i=0; i<actionGroup.m_iChannelAmount; ++i)
		{
			AnmActionChannel actionChannel;
			CAnmActionChannel *pActionChannel = pActionGroup->GetChannel(channelList[i]);
			actionChannel.m_uiChannelID = pActionChannel->GetChannelID();
			actionChannel.m_uiObjectID = pActionChannel->GetObjectID();
			actionChannel.m_uiChannelType = pActionChannel->GetChannelType();
			actionChannel.m_iActionAmount = pActionChannel->GetActionAmount();
			stringName = pActionChannel->GetChannelName();
			wcscpy(actionChannel.m_sChannelName, AC2WC(&stringName[0]));
			file.Write(&actionChannel, sizeof(AnmActionChannel), &iWriteLength);

			// Write Actions to file
			std::vector<unsigned int> actionList;
			pActionChannel->GetActionIDList(actionList);
			for (int j=0; j<actionChannel.m_iActionAmount; ++j)
			{
				AnmAction action;
				CAnmAction *pAction = pActionChannel->GetAction(actionList[j]);
				action.m_uiActionID = pAction->GetActionID();
				action.m_uiObjectID = pAction->GetObjectID();
				action.m_uiActionType = pAction->GetActionType();
				action.m_dwStartTime = pAction->GetStartTime();
				file.Write(&action, sizeof(AnmAction), &iWriteLength);

				// Write Move action
				switch (action.m_uiActionType)
				{
				case CAnmAction::TYPE_MOVE:
					{
						AnmMove move;
						CAnmMove *pMove = dynamic_cast<CAnmMove*>(pAction);
						move.m_fStartPosX = pMove->GetStartPos().x;
						move.m_fStartPosY = pMove->GetStartPos().y;
						move.m_fStartPosZ = pMove->GetStartPos().z;
						move.m_fEndPosX = pMove->GetEndPos().x;
						move.m_fEndPosY = pMove->GetEndPos().y;
						move.m_fEndPosZ = pMove->GetEndPos().z;
						move.m_fOrientationW = pMove->GetOrientation().w;
						move.m_fOrientationX = pMove->GetOrientation().x;
						move.m_fOrientationY = pMove->GetOrientation().y;
						move.m_fOrientationZ = pMove->GetOrientation().z;
						move.m_iRotateAxisType = pMove->GetRotateAxis();
						move.m_fRotateAngle = pMove->GetAngle();
						move.m_fAccelerationPercent = pMove->GetAccelerationPercent();
						move.m_iTotalFrames = pMove->GetTotalFrames();
						move.m_bCloseTerrain = pMove->GetCloseTerrain();
						file.Write(&move, sizeof(move), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_ROTATE:
					{
						AnmRotate rotate;
						CAnmRotate *pRotate = dynamic_cast<CAnmRotate*>(pAction);
						rotate.m_fStartRotateW = pRotate->GetStartRotate().w;
						rotate.m_fStartRotateX = pRotate->GetStartRotate().x;
						rotate.m_fStartRotateY = pRotate->GetStartRotate().y;
						rotate.m_fStartRotateZ = pRotate->GetStartRotate().z;
						rotate.m_fEndRotateW = pRotate->GetEndRotate().w;
						rotate.m_fEndRotateX = pRotate->GetEndRotate().x;
						rotate.m_fEndRotateY = pRotate->GetEndRotate().y;
						rotate.m_fEndRotateZ = pRotate->GetEndRotate().z;
						rotate.m_iTotalFrames = pRotate->GetTotalFrames();
						file.Write(&rotate, sizeof(rotate), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_SCALE:
					{
						AnmScale scale;
						CAnmScale *pScale = dynamic_cast<CAnmScale*>(pAction);
						scale.m_fStartScale = pScale->GetStartScale();
						scale.m_fEndScale = pScale->GetEndScale();
						scale.m_iTotalFrames = pScale->GetTotalFrames();
						file.Write(&scale, sizeof(scale), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_FOV:
					{
						AnmFOV FOV;
						CAnmFOV *pFOV = dynamic_cast<CAnmFOV*>(pAction);
						FOV.m_fStartFOV = pFOV->GetStartFOV();
						FOV.m_fEndFOV = pFOV->GetEndFOV();
						FOV.m_iTotalFrames = pFOV->GetTotalFrames();
						file.Write(&FOV, sizeof(FOV), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_JUMP:
					{
						AnmJump jump;
						CAnmJump *pJump = dynamic_cast<CAnmJump*>(pAction);
						jump.m_fStartPosX = pJump->GetStartPos().x;
						jump.m_fStartPosY = pJump->GetStartPos().y;
						jump.m_fStartPosZ = pJump->GetStartPos().z;
						jump.m_fEndPosX = pJump->GetEndPos().x;
						jump.m_fEndPosY = pJump->GetEndPos().y;
						jump.m_fEndPosZ = pJump->GetEndPos().z;
						jump.m_iTotalFrames = pJump->GetTotalFrames();
						file.Write(&jump, sizeof(jump), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_MOTION:
					{
						AnmMotion motion;
						CAnmMotion *pMotion = dynamic_cast<CAnmMotion*>(pAction);
						motion.m_uiMotionType = pMotion->GetMotionType();
						motion.m_iStartMotionFrame = pMotion->GetStartMotionFrame();
						motion.m_iBlendFrames = pMotion->GetBlendFrames();
						motion.m_fActionSpeed = pMotion->GetActionSpeed();
						motion.m_iTotalFrames = pMotion->GetTotalFrames();
						stringName = pMotion->GetActionName();
						wcscpy(motion.m_sActionName, AC2WC(&stringName[0]));
						motion.m_bShowGFX = pMotion->IsShowGFX();
						file.Write(&motion, sizeof(motion), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_BEZIER_MOVE:
					{
						AnmBezierMove bezierMove;
						CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(pAction);
						bezierMove.m_uiBezierID = pBezierMove->GetBezierID();
						bezierMove.m_bCloseTerrain = pBezierMove->GetCloseTerrain();
						file.Write(&bezierMove, sizeof(bezierMove), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_DISPLAY_HIDE:
					{
						AnmDisplayHide displayHide;
						CAnmDisplayHide *pDisplayHide = dynamic_cast<CAnmDisplayHide*>(pAction);
						displayHide.m_uiVisableType = pDisplayHide->GetVisableType();
						file.Write(&displayHide, sizeof(displayHide), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_DIALOGUE:
					{
						AnmDialogue dialogue;
						CAnmDialogue *pDialogue = dynamic_cast<CAnmDialogue*>(pAction);
						dialogue.m_iTotalFrames = pDialogue->GetTotalFrames();
						stringName = pDialogue->GetDialogue();
						wcscpy(dialogue.m_sDialogue, AC2WC(&stringName[0]));
						file.Write(&dialogue, sizeof(dialogue), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_SOUND:
					{
						AnmSound sound;
						CAnmSound *pSound = dynamic_cast<CAnmSound*>(pAction);
						sound.m_iTotalFrames = pSound->GetTotalFrames();
						sound.m_iStartSeconds = pSound->GetStartSeconds();
						sound.m_uiVolume = pSound->GetVolume();
						stringName = pSound->GetSoundFileName();
						wcscpy(sound.m_sSoundFileName, AC2WC(&stringName[0]));
						file.Write(&sound, sizeof(sound), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_HOOK:
					{
						AnmHook hook;
						CAnmHook *pHook = dynamic_cast<CAnmHook*>(pAction);
						hook.m_iTotalFrames = pHook->GetTotalFrames();

						stringName = pHook->GetHangerName();
						wcscpy(hook.m_sHangerName, AC2WC(&stringName[0]));

						stringName = pHook->GetHookName();
						wcscpy(hook.m_sHookName, AC2WC(&stringName[0]));

						hook.m_uiHookObjectID = pHook->GetHookObjectID();

						stringName = pHook->GetElseHookName();
						wcscpy(hook.m_sElseHookName, AC2WC(&stringName[0]));

						file.Write(&hook, sizeof(hook), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_SPEED_CONTROL:
					{
						AnmSpeedControl speed;
						CAnmSpeedControl* pSpeed = dynamic_cast< CAnmSpeedControl* >( pAction );
						speed.m_iTotalFrames	= pSpeed->GetTotalFrames();
						speed.m_fSpeed			= pSpeed->GetSpeed();
						file.Write( & speed, sizeof( speed ), & iWriteLength );
					}
					break;

				case CAnmAction::TYPE_CAMERA_BIND:
					{
					   AnmCameraBind bind;
					   CAnmCameraBind* pBind = dynamic_cast<CAnmCameraBind*>(pAction);
					   bind.m_uiFollowObj = pBind->GetFllowObj();
					   bind.m_iTotalFrames = pBind->GetTotalFrames();
					   bind.m_fOffsetX = pBind->GetDistance().x;
					   bind.m_fOffsetY = pBind->GetDistance().y;
					   bind.m_fOffsetZ = pBind->GetDistance().z;
					   file.Write(&bind,sizeof(bind),&iWriteLength);
					}
					break;

				case CAnmAction::TYPE_CAMERA_FILTER:
					{
						AnmCameraFilter filter;
						CAnmCameraFilter *pFilter = dynamic_cast<CAnmCameraFilter*>(pAction);
						filter.m_uiColor = pFilter->GetColor();
						filter.m_uiFilterType = pFilter->GetFilterType();
						filter.m_fTransparent = pFilter->GetTransparent();
						filter.m_iTotalFrames = pFilter->GetTotalFrames();
						file.Write(&filter, sizeof(filter), &iWriteLength);

					}
					break;

				case CAnmAction::TYPE_CAMERA_OSCILLATORY:
					{
						AnmCameraOscillatory oscillatory;
						CAnmCameraOscillatory *pOscillatory = dynamic_cast<CAnmCameraOscillatory*>(pAction);
						oscillatory.m_uiOscillatoryType = pOscillatory->GetOscillatoryType();
						oscillatory.m_iOscillatoryAxis = pOscillatory->GetOscillatoryAxis();
						oscillatory.m_iAttenuation = pOscillatory->GetAttenuation();
						oscillatory.m_fDisplacement = pOscillatory->GetDisplacement();
						oscillatory.m_fFrequency = pOscillatory->GetFrequency();
						oscillatory.m_iTotalFrames = pOscillatory->GetTotalFrames();
						file.Write(&oscillatory, sizeof(oscillatory), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_TRANSPARENT:
					{
						AnmTransparent transparent;
						CAnmTransparent *pTransparent = dynamic_cast<CAnmTransparent*>(pAction);
						transparent.m_fStartTransparent = pTransparent->GetStartTransparent();
						transparent.m_fEndTransparent = pTransparent->GetEndTransparent();
						transparent.m_iTotalFrames = pTransparent->GetTotalFrames();
						file.Write(&transparent, sizeof(transparent), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_HIDE_WORLD_ACTION:
					{
						AnmHideWorldAction hideWorldAction;
						CAnmHideWorldAction *pHideWorldAction = dynamic_cast<CAnmHideWorldAction*>(pAction);
						hideWorldAction.m_uiHideWorldType = pHideWorldAction->GetHideWorldType();
						hideWorldAction.m_uiColor = pHideWorldAction->GetColor();
						hideWorldAction.m_iTotalFrames = pHideWorldAction->GetTotalFrames();
						file.Write(&hideWorldAction, sizeof(hideWorldAction), &iWriteLength);
					}
					break;

				case CAnmAction::TYPE_LIGHTNING:
					{
						AnmLightning lightningAction;
						CAnmLightning* pLightningAction = dynamic_cast< CAnmLightning* >( pAction );
						lightningAction.m_iTimes		= pLightningAction->m_iTimes;
						lightningAction.m_iInterval		= pLightningAction->m_iInterval;
						lightningAction.m_iRand			= pLightningAction->m_iRand;
						lightningAction.m_iOnceTime		= pLightningAction->m_iOnceTime;
						lightningAction.m_iOnceInterval	= pLightningAction->m_iOnceInterval;
						lightningAction.m_fStartEnergy	= pLightningAction->m_fStartEnergy;
						lightningAction.m_fEndEnergy	= pLightningAction->m_fEndEnergy;
						lightningAction.m_fLum			= pLightningAction->m_fLum;
						lightningAction.m_iTotalFrames	= pLightningAction->m_iTotalFrames;
						file.Write( & lightningAction, sizeof( lightningAction ), & iWriteLength );
					}
					break;
	
				default:
					ASSERT(0);
					break;
				}
			}
		}
	}
	return true;
}

bool CAnmTranslator::LoadActionGroupFromFile( AFileImage& file, int iVersion )
{
    CAnmActionManager *pActionManager = m_pAnimator->GetActionManager();
    AnmActionGroup actionGroup;
    DWORD iReadLength;
    file.Read(&actionGroup, sizeof(AnmActionGroup), &iReadLength);
    actionGroup.m_uiGroupID = pActionManager->CreateGroup();
    CAnmActionGroup *pActionGroup = pActionManager->GetGroup(actionGroup.m_uiGroupID);
    pActionGroup->SetGroupName(WC2AC(actionGroup.m_sGroupName));

    // Read Action Channels from file and create Action Channels for ActionGroup
    for (int i=0; i<actionGroup.m_iChannelAmount; ++i)
    {
        AnmActionChannel actionChannel;
        file.Read(&actionChannel, sizeof(AnmActionChannel), &iReadLength);
        actionChannel.m_uiChannelID = pActionGroup->CreateChannel(CAnmActionGroup::ActionChannelType(actionChannel.m_uiChannelType));
        CAnmActionChannel *pActionChannel = pActionGroup->GetChannel(actionChannel.m_uiChannelID);
        pActionChannel->BindObjectID(actionChannel.m_uiObjectID);
        pActionChannel->SetChannelName(WC2AC(actionChannel.m_sChannelName));

        // Read Actions from file and create Actions for ActionChannel
        for (int j=0; j<actionChannel.m_iActionAmount; ++j)
        {
            AnmAction action;
            file.Read(&action, sizeof(AnmAction), &iReadLength);
            action.m_uiActionID = pActionChannel->CreateAction(CAnmAction::ActionType(action.m_uiActionType));
            CAnmAction *pAction = pActionChannel->GetAction(action.m_uiActionID);
            pAction->SetStartTime(action.m_dwStartTime);
            pAction->BindObjectID(action.m_uiObjectID);

            switch (action.m_uiActionType)
            {
			case CAnmAction::TYPE_MOVE:
                {
                    AnmMove move;
                    CAnmMove *pMove = dynamic_cast<CAnmMove*>(pAction);
                    file.Read(&move, sizeof(move), &iReadLength);
                    pMove->SetStartPos(A3DVECTOR3(move.m_fStartPosX,
                                                  move.m_fStartPosY,
                                                  move.m_fStartPosZ));
                    pMove->SetEndPos(A3DVECTOR3(move.m_fEndPosX,
                                                move.m_fEndPosY,
                                                move.m_fEndPosZ));
                    pMove->SetOrientation(A3DQUATERNION(move.m_fOrientationW,
                                                        move.m_fOrientationX,
                                                        move.m_fOrientationY,
                                                        move.m_fOrientationZ));
                    pMove->SetRotateAxis(move.m_iRotateAxisType);
                    pMove->SetAngle(move.m_fRotateAngle);
                    pMove->SetAccelerationPercent(move.m_fAccelerationPercent);
                    pMove->SetTotalFrames(move.m_iTotalFrames);
                    pMove->SetCloseTerrain(move.m_bCloseTerrain);
                }
				break;

			case CAnmRotate::TYPE_ROTATE:
                {
                    AnmRotate rotate;
                    CAnmRotate *pRotate = dynamic_cast<CAnmRotate*>(pAction);
                    file.Read(&rotate, sizeof(rotate), &iReadLength);
                    pRotate->SetStartRotate(A3DQUATERNION(rotate.m_fStartRotateW,
                                                          rotate.m_fStartRotateX,
                                                          rotate.m_fStartRotateY,
                                                          rotate.m_fStartRotateZ));
                    pRotate->SetEndRotate(A3DQUATERNION(rotate.m_fEndRotateW,
                                                        rotate.m_fEndRotateX,
                                                        rotate.m_fEndRotateY,
                                                        rotate.m_fEndRotateZ));
                    pRotate->SetTotalFrames(rotate.m_iTotalFrames);
                }
				break;

			case CAnmAction::TYPE_SCALE:
                {
                    AnmScale scale;
                    CAnmScale *pScale = dynamic_cast<CAnmScale*>(pAction);
                    file.Read(&scale, sizeof(scale), &iReadLength);
                    pScale->SetStartScale(scale.m_fStartScale);
                    pScale->SetEndScale(scale.m_fEndScale);
                    pScale->SetTotalFrames(scale.m_iTotalFrames);
                }
				break;

			case CAnmAction::TYPE_FOV:
                {
                    AnmFOV FOV;
                    CAnmFOV *pFOV = dynamic_cast<CAnmFOV*>(pAction);
                    file.Read(&FOV, sizeof(FOV), &iReadLength);
                    pFOV->SetStartFOV(FOV.m_fStartFOV);
                    pFOV->SetEndFOV(FOV.m_fEndFOV);
                    pFOV->SetTotalFrames(FOV.m_iTotalFrames);
                }
				break;

			case CAnmAction::TYPE_JUMP:
                {
                    AnmJump jump;
                    CAnmJump *pJump = dynamic_cast<CAnmJump*>(pAction);
                    file.Read(&jump, sizeof(jump), &iReadLength);
                    pJump->SetStartPos(A3DVECTOR3(jump.m_fStartPosX,
                                                  jump.m_fStartPosY,
                                                  jump.m_fStartPosZ));
                    pJump->SetEndPos(A3DVECTOR3(jump.m_fEndPosX,
                                                jump.m_fEndPosY,
                                                jump.m_fEndPosZ));
                    pJump->SetTotalFrames(jump.m_iTotalFrames);
                }
				break;

			case CAnmAction::TYPE_MOTION:
                {
                    AnmMotion motion;
                    CAnmMotion *pMotion = dynamic_cast<CAnmMotion*>(pAction);
                    file.Read(&motion, sizeof(motion), &iReadLength);
                    pMotion->SetMotionType(CAnmMotion::MotionType(motion.m_uiMotionType));
                    pMotion->SetStartMotionFrame(motion.m_iStartMotionFrame);
                    pMotion->SetBlendFrames(motion.m_iBlendFrames);
                    pMotion->SetTotalFrames(motion.m_iTotalFrames);
                    pMotion->SetActionName(WC2AC(motion.m_sActionName));
                    pMotion->SetActionSpeed(motion.m_fActionSpeed);
                    pMotion->SetShowGFX(motion.m_bShowGFX);
                }
				break;

			case CAnmAction::TYPE_BEZIER_MOVE:
                {
                    AnmBezierMove bezierMove;
                    CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(pAction);
                    file.Read(&bezierMove, sizeof(bezierMove), &iReadLength);
                    pBezierMove->SetBezierID(bezierMove.m_uiBezierID);
                    pBezierMove->SetCloseTerrain(bezierMove.m_bCloseTerrain);
                }
				break;

			case CAnmAction::TYPE_DISPLAY_HIDE:
                {
                    AnmDisplayHide displayHide;
                    CAnmDisplayHide *pDisplayHide = dynamic_cast<CAnmDisplayHide*>(pAction);
                    file.Read(&displayHide, sizeof(displayHide), &iReadLength);
                    pDisplayHide->SetVisableType(CAnmDisplayHide::VisableType(displayHide.m_uiVisableType));
                }
				break;

			case CAnmAction::TYPE_DIALOGUE:
                {
                    AnmDialogue dialogue;
                    CAnmDialogue *pDialogue = dynamic_cast<CAnmDialogue*>(pAction);
                    file.Read(&dialogue, sizeof(dialogue), &iReadLength);
                    pDialogue->SetTotalFrames(dialogue.m_iTotalFrames);
                    pDialogue->SetDialogue(WC2AC(dialogue.m_sDialogue));
                }
				break;

			case CAnmAction::TYPE_SOUND:
                {
                    AnmSound sound;
                    CAnmSound *pSound = dynamic_cast<CAnmSound*>(pAction);
                    file.Read(&sound, sizeof(sound), &iReadLength);
                    pSound->SetTotalFrames(sound.m_iTotalFrames);
                    pSound->SetStartSeconds(sound.m_iStartSeconds);
                    pSound->SetVolume(sound.m_uiVolume);
                    pSound->SetSoundFileName(WC2AC(sound.m_sSoundFileName));
                }
				break;

			case CAnmAction::TYPE_HOOK:
                {
                    AnmHook hook;
                    CAnmHook *pHook = dynamic_cast<CAnmHook*>(pAction);
                    file.Read(&hook, sizeof(hook), &iReadLength);
                    pHook->SetTotalFrames(hook.m_iTotalFrames);
                    pHook->SetHangerName(WC2AC(hook.m_sHangerName));
                    pHook->SetHookName(WC2AC(hook.m_sHookName));
                    pHook->SetHookObjectID(hook.m_uiHookObjectID);
                    pHook->SetElseHookName(WC2AC(hook.m_sElseHookName));
                }
				break;

			case CAnmAction::TYPE_SPEED_CONTROL:
                {
                    AnmSpeedControl speed;
                    CAnmSpeedControl *pSpeed = dynamic_cast<CAnmSpeedControl*>(pAction);
                    file.Read(&speed, sizeof(speed), &iReadLength);
                    pSpeed->SetTotalFrames(speed.m_iTotalFrames);
                    pSpeed->SetSpeed(speed.m_fSpeed);
                }
				break;
			case CAnmAction::TYPE_CAMERA_BIND:
				{
				   AnmCameraBind bind;
				   CAnmCameraBind* pBind = dynamic_cast<CAnmCameraBind*>(pAction);
				   file.Read(&bind,sizeof(bind),&iReadLength);
				   pBind->SetFllowObj(bind.m_uiFollowObj);
				   pBind->SetTotalFrames(bind.m_iTotalFrames);
				   pBind->SetDistance(A3DVECTOR3(bind.m_fOffsetX,bind.m_fOffsetY,bind.m_fOffsetZ));
				}
				break;
			case CAnmAction::TYPE_CAMERA_FILTER:
                {
                    AnmCameraFilter filter;
                    CAnmCameraFilter *pFilter = dynamic_cast<CAnmCameraFilter*>(pAction);
                    file.Read(&filter, sizeof(filter), &iReadLength);
                    pFilter->SetColor(filter.m_uiColor);
                    pFilter->SetFilterType(filter.m_uiFilterType);
                    pFilter->SetTransparent(filter.m_fTransparent);
                    pFilter->SetTotalFrames(filter.m_iTotalFrames);

                }
				break;

			case CAnmAction::TYPE_CAMERA_OSCILLATORY:
                {
                    AnmCameraOscillatory oscillatory;
                    CAnmCameraOscillatory *pOscillatory = dynamic_cast<CAnmCameraOscillatory*>(pAction);
                    file.Read(&oscillatory, sizeof(oscillatory), &iReadLength);
                    pOscillatory->SetOscillatoryType(oscillatory.m_uiOscillatoryType);
                    pOscillatory->SetOscillatoryAxis(oscillatory.m_iOscillatoryAxis);
                    pOscillatory->SetDisplacement(oscillatory.m_fDisplacement);
                    pOscillatory->SetFrequency(oscillatory.m_fFrequency);
                    pOscillatory->SetAttenuation(oscillatory.m_iAttenuation);
                    pOscillatory->SetTotalFrames(oscillatory.m_iTotalFrames);
                }
				break;

			case CAnmAction::TYPE_TRANSPARENT:
                {
                    AnmTransparent transparent;
                    CAnmTransparent *pTransparent = dynamic_cast<CAnmTransparent*>(pAction);
                    file.Read(&transparent, sizeof(transparent), &iReadLength);
                    pTransparent->SetStartTransparent(transparent.m_fStartTransparent);
                    pTransparent->SetEndTransparent(transparent.m_fEndTransparent);
                    pTransparent->SetTotalFrames(transparent.m_iTotalFrames);
                }
				break;

			case CAnmAction::TYPE_HIDE_WORLD_ACTION:
                {
                    AnmHideWorldAction hideWorldAction;
                    CAnmHideWorldAction *pHideWorldAction = dynamic_cast<CAnmHideWorldAction*>(pAction);
                    file.Read(&hideWorldAction, sizeof(hideWorldAction), &iReadLength);
                    pHideWorldAction->SetHideWorldType(hideWorldAction.m_uiHideWorldType);
                    pHideWorldAction->SetColor(hideWorldAction.m_uiColor);
                    pHideWorldAction->SetTotalFrames(hideWorldAction.m_iTotalFrames);
                }
				break;

			case CAnmAction::TYPE_LIGHTNING:
				{
					AnmLightning lightningAction;
					CAnmLightning* pLightningAction = dynamic_cast< CAnmLightning* >( pAction );
					file.Read( & lightningAction, sizeof( lightningAction ), & iReadLength );
					pLightningAction->m_iTimes			= lightningAction.m_iTimes;
					pLightningAction->m_iInterval		= lightningAction.m_iInterval;
					pLightningAction->m_iRand			= lightningAction.m_iRand;
					pLightningAction->m_iOnceTime		= lightningAction.m_iOnceTime;
					pLightningAction->m_iOnceInterval	= lightningAction.m_iOnceInterval;
					pLightningAction->m_fStartEnergy	= lightningAction.m_fStartEnergy;
					pLightningAction->m_fEndEnergy		= lightningAction.m_fEndEnergy;
					pLightningAction->m_fLum			= lightningAction.m_fLum;
					pLightningAction->m_iTotalFrames	= lightningAction.m_iTotalFrames;
				}
				break;

			default:
				ASSERT(0);
				break;
            }
        }
    }
    return true;
}