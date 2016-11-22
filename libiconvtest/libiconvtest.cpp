// libiconvtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
using namespace std;

#include "iconv.h"



#if defined(WIN32) && defined(_WIN32) && !defined(_WIN64) //x86
#ifdef _DEBUG
#if defined(_MT) && defined(_DLL) // MDd
#pragma comment(lib,"x86\\debug-MDd\\libiconv.lib")
#else // MTd
#pragma comment(lib,"x86\\debug-MTd\\libiconv.lib")
#endif // !_MT
#else
#if defined(_MT) && defined(_DLL) // MDd
#pragma comment(lib,"x86\\release-MD\\libiconv.lib")
#else // MTd
#pragma comment(lib,"x86\\release-MT\\libiconv.lib")
#endif // !_MT
#endif // DEBUG
#else //x64
#ifdef _DEBUG
#if defined(_MT) && defined(_DLL) // MDd
#pragma comment(lib,"x64\\debug-MDd\\libiconv.lib")
#else // MTd
#pragma comment(lib,"x64\\debug-MTd\\libiconv.lib")
#endif // !_MT
#else
#if defined(_MT) && defined(_DLL) // MDd
#pragma comment(lib,"x64\\release-MD\\libiconv.lib")
#else // MTd
#pragma comment(lib,"x64\\release-MT\\libiconv.lib")
#endif // !_MT
#endif // DEBUG
#endif

int code_convert(char *from_charset, char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	const char **pin = &inbuf;
	char **pout = &outbuf;
	
	iconv_t cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, (char **)pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}


/*UTF - 8 to GBK  */
int u2g(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	return code_convert("UTF-8", "GBK", inbuf, inlen, outbuf, outlen);
}


/*GBK to UTF - 8 */
int g2u(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	return code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, outlen);
}


void readFile(const char* pchFileName, string &strFileContent)
{
	char* buffer = NULL;
	int nLen = 0;
	FILE *fp = fopen(pchFileName, "rb");
	if (fp == NULL)
	{
		cout << pchFileName << " open failed!" << endl;
		exit(1);
	}


	fseek(fp, 0L, SEEK_END);      //文件指针移到文件尾  
	long fsize = ftell(fp);     //得到当前指针位置，既文件的长度
	rewind(fp);                 //文件指针恢复到文件头位置
								//动态申请空间，为保存字符串结尾标志'\0'，多申请一个字符的空间。
	buffer = (char*)malloc(sizeof(char)*fsize + 1);
	if (buffer == NULL)
	{
		cout << "memory error!" << endl;
		fclose(fp); //不再使用文件，关闭
		exit(1);
	}


	nLen = fread(buffer, sizeof(char), fsize, fp);
	buffer[nLen] = '\0';
	//添加字符串结尾标志
	string strBuffer(buffer);
	strFileContent = strBuffer;
	free(buffer);
	buffer = NULL;
	//释放后也要把指针设成NULL，避免野指针。
	fclose(fp);
}


int main()
{
	string strFileContent;
	readFile("3.txt", strFileContent);
	cout << strFileContent << endl;


	size_t iInBytes, oOutBytes;
	iInBytes = strFileContent.length();
	oOutBytes = 3 * iInBytes + 1;
	char *pchDestStr = new char[oOutBytes];
	memset(pchDestStr, 0, oOutBytes);
	g2u(strFileContent.c_str(), iInBytes, pchDestStr, oOutBytes);
	cout << pchDestStr << endl;
	
	system("pause");
	return 0;
}