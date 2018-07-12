#pragma once
#include "kc.h"

class KFIXSEARCH_API CFixSearch :
	public kc
{
public:
	CFixSearch();
	~CFixSearch();

	bool Init(char* host, int port, char* userpasswd);
	long Query(char *q);
	bool Fetch(int col, long row, K_RET &ret);
	long GetCols();
	bool QueryColName(char *pchTable);
	char *GetColName(int col);
	void CloseQuery();
	void CloseConn();

	void GetCROrd(int dir, char *session, char *ordid, char *OrdId_Str, int& Cnt);
};

