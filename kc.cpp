#include "stdafx.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctime>
#include "kc.h"

kc::kc() : m_err(""), m_k(NULL)
{
}

kc::~kc()
{
}

bool kc::handleOk(I handle)
{
	if (handle > 0)
		return true;

	if (handle == 0)
		strcpy_s(m_err, MAXERR, "Authentication error");
	else if (handle == -1)
		strcpy_s(m_err, MAXERR, "Connection error");
	else if (handle == -2)
		strcpy_s(m_err, MAXERR, "Time out error");
	return false;
}

bool kc::isRemoteErr(K x) 
{
	if (!x) 
	{
		strcpy_s(m_err, "Network error: ");
		strerror_s(m_err+15, MAXERR, errno);
		return true;
	}
	else if (-128 == xt) {
		sprintf_s(m_err, MAXERR, "Error message returned: %s", x->s);
		r0(x);
		return true;
	}
	return false;
}

void kc::GetDT(F dt, tm &_tm, I &_ms)
{
	time_t sec;
	double f_sec;

	f_sec = dt * 24 * 3600;
	sec = (time_t)f_sec;
	_ms = (I)(f_sec - sec) * 1000;

	_tm.tm_year = 100;
	_tm.tm_mon = 0;
	_tm.tm_mday = 1;
	_tm.tm_hour = 0;
	_tm.tm_min = 0;
	_tm.tm_sec = 0;
	sec += mktime(&_tm);

	localtime_s(&_tm, &sec);
}

bool kc::k_connect(char* host, I port, char* userpasswd)
{
	m_handle = khpu(host, port, userpasswd);
	return handleOk(m_handle);
}

bool kc::k_query(S x)
{
	if (m_k)
	{
		strcpy_s(m_err, MAXERR, "invalid cursor status");
		return false;
	}

	m_k = k(m_handle, x, (K)0);
	if (isRemoteErr(m_k))
		return false;

	// Extract columns and values elements 
	if (98 == m_k->t)
	{	// table
		m_col = kK(m_k->k)[0];
		m_val = kK(m_k->k)[1];

		m_cols = (I)m_col->n;
		m_rows = (long)kK(m_val)[0]->n;
	}
	else if (99 == m_k->t)
	{	// dictionary
		m_col = kK(m_k)[0];
		m_val = kK(m_k)[1];

		m_cols = (I)m_col->n;
		m_rows = (long)m_val->n;
	}
	else
	{
		m_cols = 0;
		m_rows = 0;
		strcpy_s(m_err, MAXERR, "invalid query object type");
		return false;
	}

	return true;
}

bool kc::k_querycolname(S table)
{
	if (m_k)
	{
		strcpy_s(m_err, MAXERR, "invalid cursor status");
		return false;
	}

	char q[64];
	sprintf_s(q, 63, "cols %s", table);
	m_k = k(m_handle, q, (K)0);
	if (isRemoteErr(m_k))
		return false;

	m_cols = (long)m_k->n;

	return true;
}

S kc::k_fetchcolname(I col)
{
	if (col >= 0 && col < m_cols)
	{
		return kS(m_k)[col];
	}
	else
	{
		strcpy_s(m_err, MAXERR, "invalid fetch col# or row#");
		return NULL;
	}
}

void kc::k_closecur()
{
	if (!m_k)
		return;

	r0(m_k);
	m_k = NULL;
}

bool kc::k_fetch(I col, long row, K_RET &ret)
{
	K cols = kK(m_val)[col];
	if (col < 0 || row < 0 || col >= m_col->n || row >= cols->n)
	{
		strcpy_s(m_err, MAXERR, "invalid fetch col# or row#");
		return false;
	}

	K k0;
	switch (cols->t)
	{
	case 0:
		k0 = kK(cols)[row];
		switch (k0->t)
		{
		case 10:
			ret.type = ret.C_STRING;
			ret._str.s=(char*)kC(k0);
			ret._str.len = (I)k0->n;
			return true;
		}
		break;
	case 6:	// int
		ret.type = ret.INT;
		ret._int= kI(cols)[row];
		return true;
	case 7:	// long
		ret.type = ret.LONG;
		ret._long = kI(cols)[row];
		return true;
	case 9:	// float
		ret.type = ret.DOUBLE;
		ret._double = kF(cols)[row];
		return true;
	case 11:
		ret.type = ret.STRING;
		ret._str.s = kS(cols)[row];
		ret._str.len= (int)strlen(ret._str.s);
		return true;
	case 15:
		ret.type = ret.DT;
		GetDT(kF(cols)[row], ret._dt._tm, ret._dt._ms);
		return true;
	}

	strcpy_s(m_err, MAXERR, "invalid fetch column type");
	return false;
}

void kc::k_close()
{
	kclose(m_handle);
}

