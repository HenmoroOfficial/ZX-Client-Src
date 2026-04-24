/********************************************************************
	created:	4/16/2009
	author:		liudong
	
	purpose:   传入一系列数据，根据最近n个数据返回下一个预期值，主要用于平滑不规则数据。
	Copyright (C) All Rights Reserved
*********************************************************************/
#pragma once

template<class T>
class CECDataExpectation
{
public:
	CECDataExpectation()
	{
		m_pBuff			= NULL;
		Release();
	}
	CECDataExpectation(const unsigned int n)
	{
		m_pBuff = NULL;
		Init(n);
	}
	~CECDataExpectation()
	{
		Release();
	}

	void Init(const unsigned int n)
	{
		Release();
		if (n>0)
		{
			m_nBuff = n+1;
			m_pBuff = new T[m_nBuff];
		}
	}

	void Release()
	{
		if (m_pBuff)
		{
			delete[] m_pBuff;
			m_pBuff = NULL;
		}
		m_nBuff = 0;
		Clear();
	}

	void Clear()
	{
		m_iBuffStart	= 0;
		m_iBuffEnd		= 0;
	}

	void AddData(const T data)
	{
		if (m_pBuff)
		{
			m_pBuff[m_iBuffEnd] = data;
			m_iBuffEnd = (m_iBuffEnd+1)%m_nBuff;
			if (m_iBuffEnd==m_iBuffStart)
			{
				m_iBuffStart = (m_iBuffStart+1)%m_nBuff;
			}
		}
	}

	T GetExpectation()
	{
		if (!m_pBuff)
		{
			return 0;
		}
		T expectation = 0;
		unsigned int i=m_iBuffStart;
		while (i!=m_iBuffEnd)
		{
			expectation += m_pBuff[i];
			i = (i+1)%m_nBuff;
		}
		unsigned int count = (m_iBuffEnd-m_iBuffStart+m_nBuff)%m_nBuff;
		expectation/=count;
		return expectation;
	}

protected:
	T*				m_pBuff; 
	unsigned int	m_nBuff;
	unsigned int	m_iBuffStart;//data is in range [m_iBuffStart,m_iBuffEnd).
	unsigned int	m_iBuffEnd;
};

typedef CECDataExpectation<unsigned int>	CECDataExpUInt;