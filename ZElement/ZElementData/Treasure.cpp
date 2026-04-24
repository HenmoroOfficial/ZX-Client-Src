#include "StdAfx.h"
#include "Global.h"
#include <time.h>
#include "Treasure.h"

void CTreasureMan::Add(TREASURE_ITEM_VERSION11* pNewItem)
{
	if(pNewItem) listTreasure.push_back(pNewItem);
}

void CTreasureMan::Delete( int idx)
{
	TREASURE_ITEM_VERSION11 *pObj = listTreasure[idx];
	listTreasure.erase( &listTreasure[idx] );
	delete pObj;
}

bool CTreasureMan::Load(const char * szName)
{
	char szPath[MAX_PATH] = {0};
	sprintf(szPath,"TreasureData\\%s",szName);
	FILE *pFile = fopen(szPath,"rb");
	if(pFile==0) return false;
	DWORD dwVersion;
	fread(&dwVersion,sizeof(DWORD),1,pFile);
	if(dwVersion == 1)
	{
		int size = 0;
		fread(&size,sizeof(int),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) return false;
			for( int j = 0; j < 4; ++j)
			{
				pNew->buy_fashion[j].price = 0;
				pNew->buy_fashion[j].time  = 0;
			}
			
			fread(&pNew->id,sizeof(unsigned int),1,pFile);
			fread(&pNew->szName,128,1,pFile);
			fread(&pNew->count,sizeof(unsigned int),1,pFile);
			fread(&pNew->buy_fashion[0].price,sizeof(unsigned int),1,pFile);
			fread(&pNew->others,sizeof(DWORD),1,pFile);
			pNew->main_type = 0;
			pNew->sub_type = 1;
			pNew->bonus = 0;
			pNew->tid = ++m_nCurTID;
			listTreasure.push_back(pNew);
		}
		SUB_TYPE old_sub;
		old_sub.id = 1;
		m_nSubCurID = 1;
		strcpy(old_sub.szName,"旧的商品!");
		listMainType[0].listSubType.push_back(old_sub);
		m_bChanged = true;
	}
	else if(dwVersion == 2)
	{
		int size = 0;
		fread(&size,sizeof(int),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) return false;
			TREASURE_ITEM_VERSION1 temp;
			fread(&temp,sizeof(TREASURE_ITEM_VERSION1),1,pFile);
			pNew->buy_fashion[0].price = temp.buy_fashion[0].price;
			pNew->buy_fashion[0].time = temp.buy_fashion[0].time;
			pNew->count = temp.count;
			pNew->id = temp.id;
			pNew->others = temp.others;
			pNew->bonus = 0;
			strcpy(pNew->szName,temp.szName);
			pNew->main_type = 0;
			pNew->sub_type = 1;
			pNew->tid = ++m_nCurTID;
			listTreasure.push_back(pNew);
		}
		SUB_TYPE old_sub;
		old_sub.id = 1;
		m_nSubCurID = 1;
		strcpy(old_sub.szName,"旧的商品!");
		listMainType[0].listSubType.push_back(old_sub);
		m_bChanged = true;

	}
	else if(dwVersion == 3)
	{
		//新的东西
		int size = 0;
		fread(&size,sizeof(int),1,pFile);
		int i;
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) return false;
			TREASURE_ITEM_VERSION3 temp;
			fread(&temp,sizeof(TREASURE_ITEM_VERSION3),1,pFile);
			
			memcpy(&pNew->buy_fashion[0],&temp.buy_fashion[0],sizeof(unsigned int)*2);
			pNew->count = temp.count;
			pNew->id = temp.id;
			pNew->others = temp.others;
			pNew->bonus = 0;
			strcpy(pNew->szName,temp.szName);
			memset(pNew->file_icon,0,128);
			memset(pNew->desc,0,sizeof(WORD)*512);
			pNew->tid = temp.tid;
			if(pNew->tid > m_nCurTID) m_nCurTID = pNew->tid;
			pNew->main_type = temp.main_type;
			pNew->sub_type = temp.sub_type;

			
			if(pNew->tid > m_nCurTID) m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
		}
		
		//读类型表
		for( i = 0; i < 7; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
	}
	else if(dwVersion == 4)
	{
		//新的东西
		int size = 0;
		fread(&size,sizeof(int),1,pFile);
		int i;
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) return false;
			fread(pNew,sizeof(TREASURE_ITEM_VERSION6),1,pFile);
			if(pNew->tid > m_nCurTID) m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
			pNew->bonus = 0;
		}
		
		//读类型表
		for( i = 0; i < 7; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
	}
	else if(dwVersion > 4 && dwVersion < 6)
	{
		//新的东西
		int size = 0;
		int i;
		fread(&size,sizeof(int),1,pFile);
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) return false;
			fread(pNew,sizeof(TREASURE_ITEM_VERSION6),1,pFile);
			if(pNew->tid > m_nCurTID) m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
			pNew->bonus = 0;
		}
		
		//读类型表
		for( i = 0; i < 7; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
		
		//读NPC数据
		int num;
		fread(&num,sizeof(int),1,pFile);
		for( i = 0; i < num; ++i)
		{
			MAIN_TYPE *pNewMain = new MAIN_TYPE;
			fread(&pNewMain->id,sizeof(unsigned int),1,pFile);
			
			if(pNewMain->id > m_nCurNpcMainID) m_nCurNpcMainID = pNewMain->id; 
			
			fread(pNewMain->szName,64,1,pFile);
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size ; ++j)
			{
				SUB_TYPE sub;
				fread(&sub.id,sizeof(int),1,pFile);
				fread(&sub.szName,64,1,pFile);
				if(sub.id > m_nSubCurID) m_nSubCurID = sub.id;
				pNewMain->listSubType.push_back(sub);
			}
			listNpcMainType.push_back(pNewMain);
		}
	}
	else if(dwVersion > 5 && dwVersion < 7)
	{
		//新的东西
		int size = 0;
		int i;
		fread(&size,sizeof(int),1,pFile);
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) return false;
			fread(pNew,sizeof(TREASURE_ITEM_VERSION6),1,pFile);
			pNew->bonus = 0;
			if(pNew->tid > m_nCurTID) m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
		}
		
		//读类型表
		for( i = 0; i < 8; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
		
		//读NPC数据
		int num;
		fread(&num,sizeof(int),1,pFile);
		for( i = 0; i < num; ++i)
		{
			MAIN_TYPE *pNewMain = new MAIN_TYPE;
			fread(&pNewMain->id,sizeof(unsigned int),1,pFile);
			
			if(pNewMain->id > m_nCurNpcMainID) m_nCurNpcMainID = pNewMain->id; 
			
			fread(pNewMain->szName,64,1,pFile);
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size ; ++j)
			{
				SUB_TYPE sub;
				fread(&sub.id,sizeof(int),1,pFile);
				fread(&sub.szName,64,1,pFile);
				if(sub.id > m_nSubCurID) m_nSubCurID = sub.id;
				pNewMain->listSubType.push_back(sub);
			}
			listNpcMainType.push_back(pNewMain);
		}
	}
	else if(dwVersion < 8)
	{
		//新的东西
		int size = 0;
		int i;
		fread(&size,sizeof(int),1,pFile);
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) return false;
			fread(pNew,sizeof(TREASURE_ITEM_VERSION7),1,pFile);
			pNew->discount = 100;
			if(pNew->tid > m_nCurTID) m_nCurTID = pNew->tid;

			pNew->others = pNew->others & 0x0000ffff | 0x00ff0000;
			listTreasure.push_back(pNew);
		}
		
		//读类型表
		for( i = 0; i < 8; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
		
		//读NPC数据
		int num;
		fread(&num,sizeof(int),1,pFile);
		for( i = 0; i < num; ++i)
		{
			MAIN_TYPE *pNewMain = new MAIN_TYPE;
			fread(&pNewMain->id,sizeof(unsigned int),1,pFile);
			
			if(pNewMain->id > m_nCurNpcMainID) m_nCurNpcMainID = pNewMain->id; 
			
			fread(pNewMain->szName,64,1,pFile);
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size ; ++j)
			{
				SUB_TYPE sub;
				fread(&sub.id,sizeof(int),1,pFile);
				fread(&sub.szName,64,1,pFile);
				if(sub.id > m_nSubCurID) m_nSubCurID = sub.id;
				pNewMain->listSubType.push_back(sub);
			}
			listNpcMainType.push_back(pNewMain);
		}
	}
	else if(dwVersion == 8)
	{
		int size = 0;
		int i;
		fread(&size,sizeof(int),1,pFile);
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) 
				return false;
			fread(pNew,sizeof(TREASURE_ITEM),1,pFile);

			if(pNew->tid > m_nCurTID) 
				m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
		}
		
		//读类型表
		for( i = 0; i < 8; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
		
		//读NPC数据
		int num;
		fread(&num,sizeof(int),1,pFile);
		for( i = 0; i < num; ++i)
		{
			MAIN_TYPE *pNewMain = new MAIN_TYPE;
			fread(&pNewMain->id,sizeof(unsigned int),1,pFile);
			
			if(pNewMain->id > m_nCurNpcMainID) m_nCurNpcMainID = pNewMain->id; 
			
			fread(pNewMain->szName,64,1,pFile);
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size ; ++j)
			{
				SUB_TYPE sub;
				fread(&sub.id,sizeof(int),1,pFile);
				fread(&sub.szName,64,1,pFile);
				if(sub.id > m_nSubCurID) m_nSubCurID = sub.id;
				pNewMain->listSubType.push_back(sub);
			}
			listNpcMainType.push_back(pNewMain);
		}
	}
	else if ( 9 == dwVersion )	//版本9
	{
		int size = 0;
		int i;
		fread(&size,sizeof(int),1,pFile);
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) 
				return false;
			fread(pNew,sizeof(TREASURE_ITEM_VERSION9),1,pFile);
			if(pNew->tid > m_nCurTID) 
				m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
		}
		
		//读类型表
		for( i = 0; i < 8; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
		
		//读NPC数据
		int num;
		fread(&num,sizeof(int),1,pFile);
		for( i = 0; i < num; ++i)
		{
			MAIN_TYPE *pNewMain = new MAIN_TYPE;
			fread(&pNewMain->id,sizeof(unsigned int),1,pFile);
			
			if(pNewMain->id > m_nCurNpcMainID) m_nCurNpcMainID = pNewMain->id; 
			
			fread(pNewMain->szName,64,1,pFile);
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size ; ++j)
			{
				SUB_TYPE sub;
				fread(&sub.id,sizeof(int),1,pFile);
				fread(&sub.szName,64,1,pFile);
				if(sub.id > m_nSubCurID) m_nSubCurID = sub.id;
				pNewMain->listSubType.push_back(sub);
			}
			listNpcMainType.push_back(pNewMain);
		}		
	}
	else if ( 10 == dwVersion )	//版本10
	{
		int size = 0;
		int i;
		fread(&size,sizeof(int),1,pFile);
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) 
				return false;
			fread(pNew,sizeof(TREASURE_ITEM_VERSION10),1,pFile);
			if(pNew->tid > m_nCurTID) 
				m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
		}
		
		//读类型表
		for( i = 0; i < 8; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
		
		//读NPC数据
		int num;
		fread(&num,sizeof(int),1,pFile);
		for( i = 0; i < num; ++i)
		{
			MAIN_TYPE *pNewMain = new MAIN_TYPE;
			fread(&pNewMain->id,sizeof(unsigned int),1,pFile);
			
			if(pNewMain->id > m_nCurNpcMainID) m_nCurNpcMainID = pNewMain->id; 
			
			fread(pNewMain->szName,64,1,pFile);
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size ; ++j)
			{
				SUB_TYPE sub;
				fread(&sub.id,sizeof(int),1,pFile);
				fread(&sub.szName,64,1,pFile);
				if(sub.id > m_nSubCurID) m_nSubCurID = sub.id;
				pNewMain->listSubType.push_back(sub);
			}
			listNpcMainType.push_back(pNewMain);
		}
	}
	else		//版本11
	{
		int size = 0;
		int i;
		fread(&size,sizeof(int),1,pFile);
		for( i = 0; i < size; ++i)
		{
			TREASURE_ITEM_VERSION11 *pNew = new TREASURE_ITEM_VERSION11;
			if(pNew==0) 
				return false;
			fread(pNew,sizeof(TREASURE_ITEM_VERSION11),1,pFile);
			if(pNew->tid > m_nCurTID) 
				m_nCurTID = pNew->tid;
			listTreasure.push_back(pNew);
		}
		
		//读类型表
		for( i = 0; i < 8; ++i)
		{
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size; ++j)
			{
				SUB_TYPE temp;
				fread(&temp.id,sizeof(int),1,pFile);
				if(temp.id > m_nSubCurID) m_nSubCurID = temp.id;
				fread(&temp.szName,64,1,pFile);
				listMainType[i].listSubType.push_back(temp);
			}
		}
		
		//读NPC数据
		int num;
		fread(&num,sizeof(int),1,pFile);
		for( i = 0; i < num; ++i)
		{
			MAIN_TYPE *pNewMain = new MAIN_TYPE;
			fread(&pNewMain->id,sizeof(unsigned int),1,pFile);
			
			if(pNewMain->id > m_nCurNpcMainID) m_nCurNpcMainID = pNewMain->id; 
			
			fread(pNewMain->szName,64,1,pFile);
			fread(&size,sizeof(int),1,pFile);
			for( int j = 0; j < size ; ++j)
			{
				SUB_TYPE sub;
				fread(&sub.id,sizeof(int),1,pFile);
				fread(&sub.szName,64,1,pFile);
				if(sub.id > m_nSubCurID) m_nSubCurID = sub.id;
				pNewMain->listSubType.push_back(sub);
			}
			listNpcMainType.push_back(pNewMain);
		}
	}

	fclose(pFile);
	return true;
}

bool CTreasureMan::Save(const char * szName)
{
	char szPath[MAX_PATH] = {0};
	sprintf(szPath,"TreasureData\\%s",szName);
	FILE *pFile = fopen(szPath,"wb");
	if(pFile==0) 
		return false;
	DWORD dwVersion = TREASURE_VERSION;
	fwrite(&dwVersion,sizeof(DWORD),1,pFile);
	int size = listTreasure.size();
	fwrite(&size,sizeof(int),1,pFile);
	int i;
	for( i = 0; i < size; ++i)
		fwrite(listTreasure[i],sizeof(TREASURE_ITEM_VERSION11),1,pFile);
	
	//写类型表
	for( i = 0; i < 8; ++i)
	{
		size = listMainType[i].listSubType.size();
		fwrite(&size,sizeof(int),1,pFile);
		for( int j = 0; j < size ; ++j)
		{
			fwrite(&listMainType[i].listSubType[j].id,sizeof(int),1,pFile);
			fwrite(&listMainType[i].listSubType[j].szName,64,1,pFile);
		}
	}

	//写NPC类型表
	int num = listNpcMainType.size();
	fwrite(&num,sizeof(int),1,pFile);
	for( i = 0; i < num; ++i)
	{
		fwrite(&listNpcMainType[i]->id,sizeof(unsigned int),1,pFile);
		fwrite(listNpcMainType[i]->szName,64,1,pFile);
		
		size = listNpcMainType[i]->listSubType.size();
		fwrite(&size,sizeof(int),1,pFile);
		for( int j = 0; j < size ; ++j)
		{
			fwrite(&listNpcMainType[i]->listSubType[j].id,sizeof(int),1,pFile);
			fwrite(&listNpcMainType[i]->listSubType[j].szName,64,1,pFile);
		}
	}
	fclose(pFile);
	return true;
}

bool CTreasureMan::Export(const char* szPath, bool bFremdness)
{
	abase::vector<TREASURE_ITEM_VERSION11*> listTemp;
	char szInfo[256];
	int i;
	for( i = 0; i < listTreasure.size(); ++i)
	{
		if(listTreasure[i]->id == 0)
		{
			sprintf(szInfo, "在名为 \"%s\" 的商品中，没有指定关联物品!", listTreasure[i]->szName);
			MessageBoxA(NULL,szInfo,"错误", MB_ICONSTOP);
			return false;
		}

		if(listTreasure[i]->main_type <= 6)
			listTemp.push_back(listTreasure[i]);
	}

	FILE *pFile = fopen(szPath,"wb");
	if(pFile==0) return false;
	DWORD dwTimeStamp = (DWORD)time(NULL);
	fwrite(&dwTimeStamp,sizeof(DWORD),1,pFile);
	
	int size = listTemp.size();
	
	fwrite(&size,sizeof(int),1,pFile);
	for( i = 0; i < size; ++i)
	{
		fwrite(&listTemp[i]->id,sizeof(unsigned int),1,pFile);
		fwrite(&listTemp[i]->count,sizeof(unsigned int),1,pFile);
		//szInfo
		memset(szInfo,0,256);
		strcpy(szInfo,listTemp[i]->file_icon);
		fwrite(szInfo,128,1,pFile);//新加

		bool bNoPrice = true;
		bool bEndPrice = false;
		bool bForeverPrice = false;
		for( int j = 0; j < 1; j++)
		{
			if( listTemp[i]->buy_fashion[j].price > 0 )
				bNoPrice = false;
			if( listTemp[i]->buy_fashion[j].price == 0 )
				bEndPrice = true;
			if( bForeverPrice && listTemp[i]->buy_fashion[j].price > 0 && listTemp[i]->buy_fashion[j].time == 0 )
			{
				sprintf(szInfo, "在名为 %s 的商品中，永久买断的物品价格有两个!", listTemp[i]->szName);
				MessageBoxA(NULL,szInfo,"错误", MB_ICONSTOP);
				return false;
			}
			if( listTemp[i]->buy_fashion[j].price > 0 && listTemp[i]->buy_fashion[j].time == 0 )
				bForeverPrice = true;

			if( bEndPrice == true && listTemp[i]->buy_fashion[j].price > 0 )
			{
				sprintf(szInfo, "在名为 %s 的商品中，价格为0的条目后还有价格非0的东西!", listTemp[i]->szName);
				MessageBoxA(NULL,szInfo,"错误", MB_ICONSTOP);
				return false;
			}
			
			fwrite(&listTemp[i]->buy_fashion[j].price, sizeof(unsigned int),1,pFile);
			fwrite(&listTemp[i]->buy_fashion[j].time, sizeof(unsigned int),1,pFile);
		}

		if( bNoPrice )
		{
			sprintf(szInfo, "在名为 %s 的商品中，没有标明合法的价格!", listTemp[i]->szName);
			MessageBoxA(NULL,szInfo,"错误", MB_ICONSTOP);
			return false;
		}
		if(!bFremdness) fwrite(&listTemp[i]->discount,sizeof(int),1,pFile);
		fwrite(&listTemp[i]->bonus,sizeof(int),1,pFile);
		
		fwrite(&listTemp[i]->others,sizeof(DWORD),1,pFile);
		fwrite(&listTemp[i]->main_type,sizeof(unsigned int),1,pFile);
		unsigned int idx = GetSubIndex( listTemp[i]->main_type, listTemp[i]->sub_type);
		fwrite(&idx,sizeof(unsigned int),1,pFile);
		fwrite(&listTemp[i]->tid,sizeof(unsigned int),1,pFile);
		WORD swTemp[512];
		memset(swTemp,0,sizeof(WORD)*512);
		wcscpy((WCHAR*)swTemp,(WCHAR*)listTemp[i]->desc);
		fwrite(swTemp,sizeof(WORD)*512,1,pFile);//新加

		//导出商品名字
		WORD szBuffer[32];
		memset(szBuffer,0,sizeof(WORD)*32);
		memset(szInfo,0,256);
		strcpy(szInfo,listTemp[i]->szName);
		MultiByteToWideChar(CP_ACP,0,szInfo,32,(WCHAR*)szBuffer,32);
		fwrite(szBuffer,sizeof(WORD)*32,1,pFile);


		//导出赠品相关
		fwrite(&listTemp[i]->bHasPresent,sizeof(bool),1,pFile);
		
		memset(szBuffer,0,sizeof(WORD)*32);
		memset(szInfo,0,256);
		strcpy(szInfo,listTemp[i]->szPresentName);
		MultiByteToWideChar(CP_ACP,0,szInfo,32,(WCHAR*)szBuffer,32);
		fwrite(szBuffer,sizeof(WORD)*32,1,pFile);

		fwrite(&listTemp[i]->Presentid,sizeof(unsigned int),1,pFile);
		fwrite(&listTemp[i]->Presentcount,sizeof(unsigned int),1,pFile);
		fwrite(&listTemp[i]->Presenttime,sizeof(unsigned int),1,pFile);

		memset(szInfo,0,256);
		strcpy(szInfo,listTemp[i]->Present_file_icon);
		fwrite(szInfo,128,1,pFile);

		fwrite(&listTemp[i]->bPresentBind,sizeof(bool),1,pFile);
		
		memset(swTemp,0,sizeof(WORD)*512);
		wcscpy((WCHAR*)swTemp,(WCHAR*)listTemp[i]->Presentdesc);
		fwrite(swTemp,sizeof(WORD)*512,1,pFile);

		//导出新增加的商城物品上架时间数据 Added 2011-11-08.
		fwrite( &listTemp[i]->valid_time.type, sizeof(int), 1, pFile );
		int start_time = listTemp[i]->valid_time.start_time;
		int end_time = listTemp[i]->valid_time.end_time;
		if ( 1 == listTemp[i]->valid_time.type )
		{
			CTime time = CTime::GetCurrentTime();	
#ifdef _ANGELICA22
			tm tm1;
			tm tm2;
			tm1 = *time.GetGmtTm(&tm1);
			tm2 = *time.GetLocalTm(&tm2);
#else
			tm tm1 = *time.GetGmtTm();
			tm tm2 = *time.GetLocalTm();
#endif // #ifdef _ANGELICA22
			int nZone = tm2.tm_hour - tm1.tm_hour;
			start_time += nZone*3600;
			end_time += nZone*3600;
		}
		fwrite( &start_time, sizeof(int), 1, pFile );
		fwrite( &end_time, sizeof(int), 1, pFile );
		fwrite( &listTemp[i]->valid_time.param, sizeof(int), 1, pFile );
		//Added end.
		memset(swTemp,0,sizeof(WORD)*512);
		wcscpy((WCHAR*)swTemp,(WCHAR*)listTemp[i]->SearchKey);
		fwrite(swTemp,sizeof(WORD)*64,1,pFile);
	}
	
	//写类型表
	WORD szBuffer[64];
	char szTempA[64];
	size = 7 + listNpcMainType.size();
	fwrite(&size,sizeof(int),1,pFile);
	for( i = 0; i < 7; ++i)
	{
		fwrite(&listMainType[i].id,sizeof(unsigned int),1,pFile);
		memset(szBuffer,0,sizeof(WORD)*64);
		memset(szTempA,0,64);
		strcpy(szTempA,listMainType[i].szName);
		MultiByteToWideChar(CP_ACP,0,szTempA,64,(WCHAR*)szBuffer,64);
		fwrite(szBuffer,sizeof(WORD)*64,1,pFile);
		
		//子类型
		size = listMainType[i].listSubType.size();
		fwrite(&size,sizeof(int),1,pFile);
		for( int j = 0; j < size; ++j)
		{
			memset(szBuffer,0,sizeof(WORD)*64);
			memset(szTempA,0,64);
			strcpy(szTempA,listMainType[i].listSubType[j].szName);
			MultiByteToWideChar(CP_ACP,0,szTempA,64,(WCHAR*)szBuffer,64);
			fwrite(szBuffer,sizeof(WORD)*64,1,pFile);
		}
		
	}
	
	//写NPC类型表
	for( i = 0; i < listNpcMainType.size(); ++i)
	{
		fwrite(&listNpcMainType[i]->id,sizeof(unsigned int),1,pFile);
		memset(szBuffer,0,sizeof(WORD)*64);
		memset(szTempA,0,64);
		strcpy(szTempA,listNpcMainType[i]->szName);
		MultiByteToWideChar(CP_ACP,0,szTempA,64,(WCHAR*)szBuffer,64);
		fwrite(szBuffer,sizeof(WORD)*64,1,pFile);
		
		//子类型
		size = listNpcMainType[i]->listSubType.size();
		fwrite(&size,sizeof(int),1,pFile);
		for( int j = 0; j < size; ++j)
		{
			memset(szBuffer,0,sizeof(WORD)*64);
			memset(szTempA,0,64);
			strcpy(szTempA,listNpcMainType[i]->listSubType[j].szName);
			MultiByteToWideChar(CP_ACP,0,szTempA,64,(WCHAR*)szBuffer,64);
			fwrite(szBuffer,sizeof(WORD)*64,1,pFile);
		}
	}
	
	fclose(pFile);
	return true;	
}

int CTreasureMan::GetSubIndex( int mid, int id_sub )
{
	if(mid < 8)
	{
		int size = listMainType[mid].listSubType.size();
		for( int i = 0; i < size; ++i)
		{
			if(listMainType[mid].listSubType[i].id==id_sub)
				return i;
		}
	}
	
	for( int i = 0; i < listNpcMainType.size(); ++i)
	{
		if(listNpcMainType[i]->id == mid)
		{
			for( int j = 0; j < listNpcMainType[i]->listSubType.size(); ++j)
				if(listNpcMainType[i]->listSubType[j].id==id_sub) return j;
		}
	}

	ASSERT(FALSE);
	return 0;
}

int CTreasureMan::GetMainIndex( int mid)
{	
	if(mid < 8) return mid;
	
	for( int i = 0; i < listNpcMainType.size(); ++i)
	{
		if(listNpcMainType[i]->id == mid)
		{
			return i + 8;
		}
	}
	ASSERT(FALSE);
	return 0;
}

void CTreasureMan::Release()
{
	int size = listTreasure.size();
	int i;
	for( i = 0; i < size; ++i)
		delete listTreasure[i];	
	listTreasure.clear();

	for(i = 0; i < 8; ++i)
		listMainType[i].listSubType.clear();


	for( i = 0; i < listNpcMainType.size(); ++i)
	{
		listNpcMainType[i]->listSubType.clear();
		delete listNpcMainType[i];
	}
	listNpcMainType.clear();
	
}

void CTreasureMan::DeleteSubType( int id)
{
	int i;
	for(i = 0; i < 8; ++i)
	{
		for( int j = 0; j < listMainType[i].listSubType.size(); ++j)
		{
			if(listMainType[i].listSubType[j].id == id)
			{
				listMainType[i].listSubType.erase(&(listMainType[i].listSubType[j]));
				
				for( int k = 0; k < listTreasure.size(); ++k)
				{
					if(listTreasure[k]->sub_type == id)
					{
						delete listTreasure[k];
						listTreasure.erase(&listTreasure[k]);
						k = -1;
					}
				}
				return;
			}
		}
	}

	for(i = 0; i < listNpcMainType.size(); ++i)
	{
		for( int j = 0; j < listNpcMainType[i]->listSubType.size(); ++j)
		{
			if(listNpcMainType[i]->listSubType[j].id == id)
			{
				listNpcMainType[i]->listSubType.erase(&(listNpcMainType[i]->listSubType[j]));
				
				for( int k = 0; k < listTreasure.size(); ++k)
				{
					if(listTreasure[k]->sub_type == id)
					{
						delete listTreasure[k];
						listTreasure.erase(&listTreasure[k]);
						k = -1;
					}
				}
				return;
			}
		}
	}
}

MAIN_TYPE* CTreasureMan::AddNpcMainType(const char *szNpcMainType)
{
	MAIN_TYPE *pNew = new MAIN_TYPE;
	strcpy(pNew->szName,szNpcMainType);
	pNew->id = GenNpcMainID();
	listNpcMainType.push_back(pNew);
	return pNew;
}

void CTreasureMan::DeleteNpcMainType( int mid)
{
	for( int i = 0; i < listNpcMainType.size(); ++i)
	{
		if(listNpcMainType[i]->id==mid)
		{
			for( int j = 0; j < listNpcMainType[i]->listSubType.size(); ++j)
			{
				int sid = listNpcMainType[i]->listSubType[j].id;
				for( int k = 0; k < listTreasure.size(); ++k)
				{
					if(listTreasure[k]->sub_type == sid)
					{
						delete listTreasure[k];
						listTreasure.erase(&listTreasure[k]);
						k = -1;
					}
				}
			}
			listNpcMainType[i]->listSubType.clear();
			listNpcMainType.erase(&listNpcMainType[i]);
			return;
		}
	}
}
