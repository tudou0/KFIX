#pragma once
#define KXVER  3
#include "k.h"

#define MAXERR		1024

#ifdef KFIXSEARCH_EXPORTS
#define KFIXSEARCH_API __declspec(dllexport)
#else
#define KFIXSEARCH_API __declspec(dllimport)
#endif

typedef struct k_ret
{
	enum {INT, LONG, DOUBLE, STRING, C_STRING, DT};
	short type;	
	union
	{
		struct
		{
			S s;
			I len;
		} _str;
		I _int;
		J _long;
		F _double;
		struct
		{
			tm _tm;
			I _ms;
		} _dt;
	};
} K_RET;

class KFIXSEARCH_API kc
{
public:
	kc();
	~kc();

	bool k_connect(char* host, I port, char* userpasswd);
	bool k_query(S x);
	bool k_fetch(I col, long row, K_RET &ret);
	void k_closecur();
	long k_getrows(){ return m_rows; };
	long k_getcols(){ return m_cols; };
	bool k_querycolname(S table);
	S k_fetchcolname(I col);
	void k_close();

	char *k_geterr(){return m_err;	}

private:
	bool handleOk(I handle);
	bool isRemoteErr(K x);
	void GetDT(F dt, tm &_tm, I &_ms);

	I m_handle;

	K m_k, m_col, m_val;
	long m_rows, m_cols;
	char m_err[MAXERR];
};
