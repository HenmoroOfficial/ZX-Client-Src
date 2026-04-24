#ifndef _EC_AUTOMOVE_H_
#define _EC_AUTOMOVE_H_

#include "A3DTypes.h"
#include "vector.h"
#include "EC_Counter.h"

class CECModel;

#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#define CECViewport ECViewport
#endif

namespace AutoMove
{
	class CTransferMap;
}

//#define  EC_AUTOMOVE_MULTITHREAD

class CECAutoMove
{
public:

	enum
	{
		enumStateNone,
		enumStateSearching,
		enumStateSuccess,
		enumStateFail,
	};

	CECAutoMove(); 
	~CECAutoMove(); 

protected:



	volatile int m_iState;
	A3DVECTOR3 m_vStart;
	abase::vector<A3DVECTOR3> m_a3DPath;
	int		 m_nCurStep;
	int     m_CurId;           //current map id
	int     m_DestId;          //dest map id
	A3DVECTOR3   m_DestPos;    //dest pos
	bool		m_NeedTransfer;
	AutoMove::CTransferMap * m_TransMap;
	
	bool      m_bAdjustEndLayer;  //adjust end layer or set default 0
public:

	int GetState() const { return m_iState; }
	int GetWayPointCount() const { return m_a3DPath.size(); }
	const A3DVECTOR3& GetPosByIndex(int nIndex) const { return m_a3DPath[nIndex]; }
	bool SearchPath(const A3DVECTOR3& vStart, const A3DVECTOR3& vEnd, int endId, bool bAdjustEndLayer = true);
	/**
	 * \brief 
	 * \param[in] cancel,  true if the path is broken by some reasons.
	 * \param[out]
	 * \return
	 * \note
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 10/4/2007
	 * \see 
	 */
	void ClearPath(bool cancel);

	void SetStartPos(const A3DVECTOR3& v) { m_vStart = v; }
	void Tick(DWORD dwDeltaTime);
#ifdef BACK_VERSION
	void Render(CECViewport* pViewport);
#else
	void Render(ECViewport* pViewport);
#endif

	void LoadTransMap(const char * szFilePath);
	void CompleteSearch(const A3DVECTOR3& curPos);

	int  MoveOn(const int lastIndex);

private:
	bool _LocalSearch(const A3DVECTOR3& start);
};

extern CECAutoMove g_ECAutoMove;

#endif
