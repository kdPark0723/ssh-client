#pragma once

#include "stdafx.h"

struct SshInfo
{
    CString ip;
    CString name;
    CString key;
};

inline CArchive& operator<<(CArchive& ar, const SshInfo info) {
	ar << info.ip << info.key << info.name;

	return ar;
}

inline CArchive& operator>>(CArchive& ar, SshInfo info) {
	ar >> info.ip >> info.key >> info.name;

	return ar;
}