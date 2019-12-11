#include "stdafx.h"
#include "util.h"

std::string convertCstringToString(const CString &cstring) {
    CT2CA pszConvertedAnsiString{ cstring };

    return std::string{ pszConvertedAnsiString };
}