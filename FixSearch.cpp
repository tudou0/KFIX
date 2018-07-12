#include "stdafx.h"
#include "FixSearch.h"


CFixSearch::CFixSearch()
{
}


CFixSearch::~CFixSearch()
{
}

bool CFixSearch::Init(char* host, int port, char* userpasswd)
{
	if (k_connect(host, port, userpasswd))
		return true;
	else
	{
		throw k_geterr();
		return false;
	}
}

bool CFixSearch::Fetch(int col, long row, K_RET &ret)
{
	if (k_fetch(col, row, ret))
		return true;
	else
	{
		throw k_geterr();
		return false;
	}
}

void CFixSearch::CloseQuery()
{
	k_closecur();
}

void CFixSearch::CloseConn()
{
	k_close();
}

long CFixSearch::Query(char *q)
{
	if (k_query(q))
	{
		return k_getrows();
	}
	else
	{
		throw k_geterr();
		return 0;
	}
}

long CFixSearch::GetCols()
{ 
	return k_getcols(); 
}

bool CFixSearch::QueryColName(char *pchTable)
{
	if (k_querycolname(pchTable))
	{
		return true;
	}
	else
	{
		throw k_geterr();
		return false;
	}
}

char *CFixSearch::GetColName(int col)
{
	char *p;
	if (p=k_fetchcolname(col))
	{
		return p;
	}
	else
	{
		throw k_geterr();
		return NULL;
	}
}

void CFixSearch::GetCROrd(int dir, char *session, char *ordid, char *OrdId_Str, int& Cnt)
{
	char q[1024];
	if(0==dir)
		sprintf_s(q, 1024, "select sOrigClOrdID from fixmsgs where (tMsgDate= 2018.04.16T00:00:00.000)&(sMsgType=`8)&(sOrdStatus=`5)&(cClOrdID like \"%s\")", ordid);
	else
		sprintf_s(q, 1024, "select cClOrdID from fixmsgs where (tMsgDate= 2018.04.16T00:00:00.000)&(sMsgType=`8)&(sOrdStatus=`5)&(sOrigClOrdID=`$(\"%s\"))", ordid);

	if(Query(q) > 0)
	{
		K_RET ret;
		if (Fetch(0, 0, ret))
		{
			char chNewOrdId[64];

			Cnt++;
			strncpy_s(chNewOrdId, ret._str.len+1, ret._str.s, ret._str.len);
			chNewOrdId[ret._str.len] = 0;
			sprintf_s(OrdId_Str, ret._str.len + 4, ";\"%s\"", chNewOrdId);
			CloseQuery();

			GetCROrd(dir, session, chNewOrdId, OrdId_Str + strlen(OrdId_Str), Cnt);
			return;
		}
	}

	CloseQuery();
	return;
}