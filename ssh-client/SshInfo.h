#pragma once

#include "stdafx.h"

struct SshInfo
{
    CString ip;
    CString name;
};

inline CArchive& operator<<(CArchive& ar, const SshInfo info) {
	ar << info.ip << info.name;

	return ar;
}

inline CArchive& operator>>(CArchive& ar, SshInfo info) {
	ar >> info.ip >> info.name;

	return ar;
}