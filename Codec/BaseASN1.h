#ifndef BASEASN1_H
#define BASEASN1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EasyLog.h"

// #define EASYDER_NoErr 0
// 自定义基础数据类型
typedef int              EASY_INT;
typedef unsigned char    EASY_UINT8;
typedef unsigned short   EASY_UINT16;
typedef unsigned long    EASY_UINT32;
typedef signed long      EASY_SINT32;
typedef unsigned char    ITASN1_BOOLEAN;

// 数据节点结构体(类)
typedef struct EASY_ANYBUF_
{
    EASY_UINT8 	  *pData;
    EASY_UINT32     dataLen;

    EASY_UINT32     unusedBits;	/* for bit string */
    EASY_UINT32     memoryType;
    EASY_UINT32     dataType;
    struct EASY_ANYBUF_ *next;    /* for sequence and set */
    struct EASY_ANYBUF_ *prev;
}EASY_ANYBUF;

// 自定义复合数据类型
typedef EASY_ANYBUF    ITASN1_INTEGER;
typedef EASY_ANYBUF    ITASN1_OCTETSTRING;
typedef EASY_ANYBUF    ITASN1_BITSTRING;
typedef EASY_ANYBUF    ITASN1_PRINTABLESTRING;
/*begin of bmpstring*/
typedef EASY_ANYBUF	 ITASN1_BMPSTRING;
/*end of bmpstring*/
typedef EASY_ANYBUF    ITASN1_ENUMERATED;
typedef EASY_ANYBUF    ITASN1_IA5STRING;
typedef EASY_ANYBUF    ITASN1_SEQUENCE;
typedef EASY_ANYBUF    ITASN1_SET;

class BaseASN1
{
public:
    // 标记内存类型
    enum MemoryType{StaticMemory=1, MallocMemory=2};
    // 错误描述
    enum DerErrType{
        NoErr=0,
        MemoryErr=200,
        LengthErr,
        LengthNotEqual,
        DataRangeErr,
        InvalidTag
    };
    // 整形数的每一位代表不同的数据类型0-31, 即: tag的值对应的数据类型
    enum DerTag{
        EASY_DER_ID_RESERVED,
        EASY_DER_ID_BOOLEAN,
        EASY_DER_ID_INTEGER,
        EASY_DER_ID_BITSTRING,
        EASY_DER_ID_OCTETSTRING,
        EASY_DER_ID_NULL,
        EASY_DER_ID_OBJECT_IDENTIFIER,
        EASY_DER_ID_OBJECT_DESCRIPTOR,
        EASY_DER_ID_EXTERNAL,
        EASY_DER_ID_REAL,
        EASY_DER_ID_ENUMERATED,
        EASY_DER_ID_EMBEDDED_PDV,
        EASY_DER_ID_STRING_UTF8,
        EASY_DER_ID_13,
        EASY_DER_ID_14,
        EASY_DER_ID_15,
        EASY_DER_ID_SEQUENCE,
        EASY_DER_ID_SET,
        EASY_DER_ID_STRING_NUMERIC,
        EASY_DER_ID_STRING_PRINTABLE,
        EASY_DER_ID_STRING_T61,
        EASY_DER_ID_STRING_VIDEOTEX,
        EASY_DER_ID_STRING_IA5,
        EASY_DER_ID_TIME_UTC,
        EASY_DER_ID_TIME_GENERALIZED,
        EASY_DER_ID_STRING_GRAPHIC,
        EASY_DER_ID_STRING_ISO646,
        EASY_DER_ID_STRING_GENERAL,
        EASY_DER_ID_STRING_UNIVERSAL,
        EASY_DER_ID_29,
        EASY_DER_ID_STRING_BMP
    };

    // 构造函数
    BaseASN1();

    EASY_INT DER_ItAsn1_WriteInteger(EASY_UINT32 integer, ITASN1_INTEGER **ppDerInteger);

    EASY_INT DER_ItAsn1_ReadInteger(ITASN1_INTEGER *pDerInteger, EASY_UINT32 *pInteger);

    EASY_INT DER_ItAsn1_WriteBitString(ITASN1_BITSTRING *pBitString, ITASN1_BITSTRING **ppDerBitString);

    EASY_INT DER_ItAsn1_ReadBitString(ITASN1_BITSTRING *pDerBitString, ITASN1_BITSTRING **ppBitString);

    EASY_INT DER_ItAsn1_WritePrintableString(ITASN1_PRINTABLESTRING *pPrintString, ITASN1_PRINTABLESTRING **ppDerPrintString);

    EASY_INT DER_ItAsn1_ReadPrintableString(ITASN1_PRINTABLESTRING *pDerPrintString, ITASN1_PRINTABLESTRING **ppPrintString);

    EASY_INT DER_ItAsn1_WriteSequence(ITASN1_SEQUENCE *pSequence, EASY_ANYBUF **ppDerSequence);

    EASY_INT DER_ItAsn1_ReadSequence(EASY_ANYBUF *pDerSequence, ITASN1_SEQUENCE **ppSequence);

    EASY_INT DER_ItAsn1_WriteNull(EASY_ANYBUF ** ppDerNull);

    EASY_INT DER_ItAsn1_ReadNull(EASY_ANYBUF * ppDerNull, EASY_UINT8 * pInt);

    // 释放一个序列(链表), pAnyBuf为链表的头结点
    EASY_INT DER_EASY_FreeQueue(EASY_ANYBUF *pAnyBuf);

    // 创建EASY_ANYBUF, 将strOrigin写入创建的EASY_ANYBUF内存中, 通过pOriginBuf将内存地址传出
    EASY_INT DER_EASY_String_To_AnyBuf(EASY_ANYBUF **pOriginBuf, unsigned char * strOrigin, int strOriginLen);

    int WriteNullSequence(EASY_ANYBUF **pOutData);

    // 同 EncodeChar 函数
    int EncodeUnsignedChar(unsigned char *pData, int dataLen, EASY_ANYBUF **outBuf);

    int DecodeUnsignedChar(EASY_ANYBUF *inBuf, unsigned char **Data, int *pDataLen);

    // pData编码为EASY_ANYBUF(有malloc动作), 将新的EASY_ANYBUF节点地址赋值给outBuf
    int EncodeChar(char *pData, int dataLen, EASY_ANYBUF **outBuf);

    // 解析节点inBuf中的字符串数据, 通过第二个参数Data指针传出
    int DecodeChar(EASY_ANYBUF *inBuf, char **Data, int *pDataLen);

private:
    EASY_INT DER_ItAsn1_Low_GetTagInfo(
            EASY_UINT8 **ppDerData,
            EASY_UINT32 **ppTagValue,
            EASY_UINT32 **ppTagSize);
    EASY_UINT32 DER_ItAsn1_Low_Count_LengthOfSize(EASY_UINT32 iLength);
    EASY_INT DER_ItAsn1_GetLengthInfo(
            EASY_ANYBUF *pDerData,
            int *pLengthValue,
            int *pLengthSize);
    EASY_INT DER_ItAsn1_Low_GetLengthInfo(
            EASY_UINT8 **ppDerData,
            EASY_UINT32 **ppLengthValue,
            EASY_UINT32 **ppLengthSize);
    EASY_INT DER_ItAsn1_Low_IntToChar(
            EASY_UINT32 integer,
            EASY_UINT8 **ppData,
            EASY_UINT32 **ppLength);
    EASY_INT DER_ItAsn1_Low_CharToInt(
            EASY_UINT8 *aData,
            EASY_UINT32 lLength,
            EASY_UINT32 **ppInteger);
    EASY_INT DER_ItAsn1_Low_WriteTagAndLength(
            EASY_ANYBUF *pAnyIn,
            EASY_UINT8 cTag,
            EASY_ANYBUF **ppAnyOut,
            EASY_UINT8 **ppUint8Value);
    EASY_INT DER_ItAsn1_Low_ReadTagAndLength(
            EASY_ANYBUF *pAnyIn,
            EASY_UINT8 **ppUint8Data,
            EASY_ANYBUF **ppAnyOut,
            EASY_UINT8 **ppUint8Value);
    EASY_INT DER_ItAsn1_WriteCharString(
            EASY_ANYBUF *pCharString,
            EASY_ANYBUF **ppDerCharString);
    EASY_INT DER_ItAsn1_ReadCharString(
            EASY_ANYBUF *pDerCharString,
            EASY_ANYBUF **ppCharString);
    EASY_INT DER_ItAsn1_WriteBmpString(
            ITASN1_BMPSTRING *pBmpString,
            ITASN1_BMPSTRING **ppDerBmpString);
    EASY_INT DER_ItAsn1_ReadBmpString(
            ITASN1_BMPSTRING *pDerBmpString,
            ITASN1_BMPSTRING **ppBmpString);
    void DER_DI_FreeAnybuf(EASY_ANYBUF  * pAnyBuf);
    // 给一个EASY_ANYBUF类型空节点指针分配存储空间
    int DER_CREATE_LOW_EASY_ANYBUF(EASY_ANYBUF *&point);
    // 内联函数
    inline void DER_EASY_Free(void *memblock)
    {
        if(memblock)
        {
            free(memblock);
            memblock = NULL;
        }
    }
    // 计算数据类型对应的tag
    inline EASY_UINT32 DER_ITASN1_LOW_IDENTIFIER(EASY_UINT8 &cTag)
    {
        return cTag & EASY_DER_SHORT_ID_MASK;
    }
    // 在堆上创建一个指定大小的数组, 使用指针的引用 == 使用指针的指针
    inline int DER_ITASN1_LOW_CREATEUINT8(EASY_UINT8* &point, EASY_UINT32 size)
    {
        point = (EASY_UINT8*)malloc(size);
        if (point==NULL)
        {
            return 7002;
        }
        //memset(point,0,size)
        return 0;
    }
    // 在堆上创建一个 EASY_UINT32 大小的内存
    // 原来的宏参数是指针, 所以此次应该使用指针的引用
    inline int DER_ITASN1_LOW_CREATEUINT32(EASY_UINT32* &point)
    {
        point = (EASY_UINT32*)malloc(sizeof(EASY_UINT32));
        if (point==NULL)
            return 7002;
        memset(point, 0, sizeof(EASY_UINT32));
        return 0;
    }
    // 通过检测iResult的值, 返回错误类型
    inline DerErrType DER_ITASN1_LOW_CHECKERR(EASY_INT iResult, DerErrType iErrNumber)
    {
        if (iResult == iErrNumber)
            return iErrNumber;
        return NoErr;
    }
    // 释放节点内存
    inline void DER_DACERT_LOW_FREE_ANYBUF(EASY_ANYBUF *point)
    {
        DER_EASY_Free(point->pData);
        DER_EASY_Free(point);
    }

private:
    EasyLog m_log;       // log类

	const EASY_UINT8 TRUE = 1;
	const EASY_UINT8 FALSE = 0;
	/* 标记当前数据是原始数据还是编码数据 */
	const EASY_UINT32 EASY_DER_CONSTRUCTED = 0x20;
	const EASY_UINT32 EASY_DER_PRIMITIVE = 0x00;

	/* The encodings for the universal types */
	const EASY_UINT32 EASY_DER_UNIVERSAL = 0x00;
	const EASY_UINT32 EASY_DER_APPLICATION = 0x40;
	const EASY_UINT32 EASY_DER_CONTEXT_SPECIFIC = 0x80;
	const EASY_UINT32 EASY_DER_PRIVATE = 0xC0;

	const EASY_UINT32 EASY_DER_RESERVED = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_RESERVED);
	const EASY_UINT32 EASY_DER_BOOLEAN = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_BOOLEAN);
	const EASY_UINT32 EASY_DER_INTEGER = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_INTEGER);
	const EASY_UINT32 EASY_DER_BITSTRING = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_BITSTRING);
	const EASY_UINT32 EASY_DER_OCTETSTRING = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_OCTETSTRING);
	const EASY_UINT32 EASY_DER_NULL = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_NULL);
	const EASY_UINT32 EASY_DER_OBJECT_IDENTIFIER = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_OBJECT_IDENTIFIER);
	const EASY_UINT32 EASY_DER_OBJECT_DESCRIPTOR = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_OBJECT_DESCRIPTOR);
	const EASY_UINT32 EASY_DER_EXTERNAL = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_EXTERNAL);
	const EASY_UINT32 EASY_DER_REAL = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_REAL);
	const EASY_UINT32 EASY_DER_ENUMERATED = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_ENUMERATED);
	const EASY_UINT32 EASY_DER_EMBEDDED_PDV = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_EMBEDDED_PDV);
	const EASY_UINT32 EASY_DER_STRING_UTF8 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_UTF8);
	const EASY_UINT32 EASY_DER_13 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_13);
	const EASY_UINT32 EASY_DER_14 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_14);
	const EASY_UINT32 EASY_DER_15 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_15);
	const EASY_UINT32 EASY_DER_SEQUENCE = (EASY_DER_UNIVERSAL | EASY_DER_CONSTRUCTED | EASY_DER_ID_SEQUENCE);
	const EASY_UINT32 EASY_DER_SET = (EASY_DER_UNIVERSAL | EASY_DER_CONSTRUCTED | EASY_DER_ID_SET);
	const EASY_UINT32 EASY_DER_STRING_NUMERIC = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_NUMERIC);
	const EASY_UINT32 EASY_DER_STRING_PRINTABLE = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_PRINTABLE);
	const EASY_UINT32 EASY_DER_STRING_T61 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_T61);
	const EASY_UINT32 EASY_DER_STRING_VIDEOTEX = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_VIDEOTEX);
	const EASY_UINT32 EASY_DER_STRING_IA5 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_IA5);
	const EASY_UINT32 EASY_DER_TIME_UTC = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_TIME_UTC);
	const EASY_UINT32 EASY_DER_TIME_GENERALIZED = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_TIME_GENERALIZED);
	const EASY_UINT32 EASY_DER_STRING_GRAPHIC = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_GRAPHIC);
	const EASY_UINT32 EASY_DER_STRING_ISO646 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_ISO646);
	const EASY_UINT32 EASY_DER_STRING_GENERAL = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_GENERAL);
	const EASY_UINT32 EASY_DER_STRING_UNIVERSAL = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_UNIVERSAL);
	const EASY_UINT32 EASY_DER_29 = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_29);
	const EASY_UINT32 EASY_DER_STRING_BMP = (EASY_DER_UNIVERSAL | EASY_DER_PRIMITIVE | EASY_DER_ID_STRING_BMP);

	/* Masks to extract information from a tag number */
	const EASY_UINT32 EASY_DER_CLASS_MASK = 0xC0;
	const EASY_UINT32 EASY_DER_CONSTRUCTED_MASK = 0x20;
	const EASY_UINT32 EASY_DER_SHORT_ID_MASK = 0x1F;
	const EASY_UINT32 EASY_DER_FIRST_NOT_ID_MASK = 0x7F;     //xia
	const EASY_UINT32 EASY_DER_FIRST_YES_ID_MASK = 0x80;     //xia
	const EASY_UINT32 EASY_DER_ALL_YES_ID_MASK = 0xFF;     //xia
															   /* The maximum size for the short tag number encoding, and the magic value
															   which indicates that a long encoding of the number is being used */
	const EASY_UINT32 ITASN1_MAX_SHORT_BER_ID = 30;
	const EASY_UINT32 ITASN1_LONG_BER_ID = 0x1F;

};

#endif // BASEASN1_H
