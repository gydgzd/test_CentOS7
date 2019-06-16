/*
 * gbk_utf8.cpp
 *
 *  Created on: 2018年11月8日
 *      Author: gyd
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <locale.h>

/*
 * DESCRIPTION: 实现由utf8编码到gbk编码的转换 
 * Input: gbkStr,转换后的字符串;  
 * Implement by mbstowcs and wcstombs
 * utf8->unicode->GBK
 * srcStr,待转换的字符串; 
 * maxGbkStrlen, gbkStr的最大长度 
 * Output: gbkStr 
 * Returns: -1,fail;>0,success
 */
int utf82gbk(char *gbkStr, const char *srcStr, int maxGbkStrlen)
{
	if (NULL == srcStr)
	{
		printf("Bad Parameter\n");
		return -1;
	}
	 //首先先将utf8编码转换为unicode编码 
	if (NULL == setlocale(LC_ALL, "zh_CN.utf8")) //设置转换为unicode前的码,当前为utf8编码           
	{
		printf("Bad Parameter\n");
		return -1;
	}
	int unicodeLen = mbstowcs(NULL, srcStr, 0);
	//计算转换后的长度  
	if (unicodeLen <= 0)
	{
		printf("Can not Transfer!!!\n");
		return -1;
	}
	wchar_t *unicodeStr = (wchar_t *) calloc(sizeof(wchar_t), unicodeLen + 1);
	mbstowcs(unicodeStr, srcStr, strlen(srcStr));
	//将utf8转换为unicode   
	//将unicode编码转换为gbk编码     
	if (NULL == setlocale(LC_ALL, "zh_CN.gbk")) //设置unicode转换后的码,当前为gbk           
	{
		printf("Bad Parameter\n");
		return -1;
	}
	int gbkLen = wcstombs(NULL, unicodeStr, 0);
	//计算转换后的长度  
	if (gbkLen <= 0)
	{
		printf("Can not Transfer!!!\n");
		return -1;
	} else if (gbkLen >= maxGbkStrlen) //判断空间是否足够           
	{
		printf("Dst Str memory not enough\n");
		return -1;
	}
	wcstombs(gbkStr, unicodeStr, gbkLen);
	gbkStr[gbkLen] = 0; //添加结束符     
	free(unicodeStr);
	return gbkLen;
}

/* 
 * function: gbk2utf8 
 * description: 实现由gbk编码到utf8编码的转换  
 * Implement by mbstowcs and wcstombs
 * GBK->unicode->utf8
 * input: utfstr,转换后的字符串;  srcstr,待转换的字符串; maxutfstrlen, utfstr的最大长度 
 * output: utfstr 
 * returns: -1,fail;>0,success 
 */
int gbk2utf8(char *destStr,const char *srcstr,size_t maxutfstrlen)
{
	if(NULL == srcstr)
	{
		printf(" bad parameter\n");
		return -1;
	}
	//首先先将gbk编码转换为unicode编码  
	if(NULL == setlocale(LC_ALL,"zh_CN.gbk"))//设置转换为unicode前的码,当前为gbk编码  
	{
		printf("setlocale to zh_CN.gbk error!\n");
		return -1;
	}
	size_t unicodelen=mbstowcs(NULL, srcstr, 0);//计算转换后的长度  
	if(unicodelen<=0)
	{
		printf("can not transfer!!!\n");
		return -1;
	}
	wchar_t *pstr_unicode = NULL;
	pstr_unicode=(wchar_t *)calloc(sizeof(wchar_t),unicodelen+1);
	mbstowcs(pstr_unicode,srcstr,strlen(srcstr));//将gbk转换为unicode  


	//将unicode编码转换为utf8编码  
	if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//设置unicode转换后的码,当前为utf8  
	{
		printf("bad parameter\n");
		if(pstr_unicode != NULL)
			free(pstr_unicode);
		return -1;
	}
	size_t destlen=wcstombs(NULL,pstr_unicode,0);//计算转换后的长度  
	if(destlen<=0)
	{
		printf("can not transfer!!!\n");
		if(pstr_unicode != NULL)
			free(pstr_unicode);
		return -1;
	}
	else if(destlen>=maxutfstrlen)//判断空间是否足够  
	{
		printf("dst str memory not enough\n");
		if(pstr_unicode != NULL)
			free(pstr_unicode);
		return -1;
	}
	wcstombs(destStr, pstr_unicode, destlen);
	destStr[destlen]=0;           //添加结束符  
	if(pstr_unicode != NULL)
		free(pstr_unicode);
	return destlen;
}


