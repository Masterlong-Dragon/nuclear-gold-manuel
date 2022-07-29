#pragma once
#include <string>
#include <codecvt>
#include <Windows.h>
//ͨ�ð潫wstringת��Ϊstring
__inline std::string W_To_A(const std::wstring& wstr, unsigned int codepage = CP_ACP)
{
    int nwstrlen = WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (nwstrlen > 0)
    {
        std::string str(nwstrlen, '\0');
        WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, (LPSTR)str.c_str(), nwstrlen, NULL, NULL);
        return std::move(str);
    }
    return ("");
}
//ͨ�ð潫stringת��Ϊwstring
__inline std::wstring A_To_W(const std::string& str, unsigned int codepage = CP_ACP)
{
    int nstrlen = MultiByteToWideChar(codepage, 0, str.c_str(), -1, NULL, 0);
    if (nstrlen > 0)
    {
        std::wstring wstr(nstrlen, L'\0');
        MultiByteToWideChar(codepage, 0, str.c_str(), -1, (LPWSTR)wstr.c_str(), nstrlen);
        return std::move(wstr);
    }
    return (L"");
}


__inline static
#if !defined(UNICODE) && !defined(_UNICODE)
std::string
#else
std::wstring
#endif
A_To_T(const std::string& str)
{
#if !defined(UNICODE) && !defined(_UNICODE)
    return str;
#else
    return A_To_W(str);
#endif
}
__inline static
#if !defined(UNICODE) && !defined(_UNICODE)
std::string
#else
std::wstring
#endif
W_To_T(const std::wstring& wstr)
{
#if !defined(UNICODE) && !defined(_UNICODE)
    return W_To_A(wstr);
#else
    return wstr;
#endif
}
__inline static std::string T_To_A(
    const
#if !defined(UNICODE) && !defined(_UNICODE)
    std::string
#else
    std::wstring
#endif
    & tsT)
{
#if !defined(UNICODE) && !defined(_UNICODE)
    return tsT;
#else
    return W_To_A(tsT);
#endif
}
__inline static std::wstring T_To_W(
    const
#if !defined(UNICODE) && !defined(_UNICODE)
    std::string
#else
    std::wstring
#endif
    & tsT)
{
#if !defined(UNICODE) && !defined(_UNICODE)
    return A_To_W(tsT);
#else
    return tsT;
#endif
}
//��From����ת��ΪTo����
__inline static std::string CodePage_FromTo(const std::string& str,
    unsigned int from_codepage, unsigned int to_codepage)
{
    return W_To_A(A_To_W(str, from_codepage), to_codepage);
}
//��UTF8ת��ΪANSI
__inline static std::string UTF8ToANSI(const std::string& str)
{
    return CodePage_FromTo(str, CP_UTF8, CP_ACP);
}
//��ANSIת��ΪUTF8
__inline static std::string ANSIToUTF8(const std::string& str)
{
    return CodePage_FromTo(str, CP_ACP, CP_UTF8);
}