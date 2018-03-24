#ifndef         _ULTILITIES_H
#define         _ULTILITIES_H
#include "PlatFormType.h"
typedef enum  _enumTypeNumeral
{
    eBINARY=1,
    eOCTAL,
    eDECIMAL,
    eHEXADECIMAL
}enumTypeNumeral;

uint32 gf_CreatRandomNum(void);
uint32 gf_CreatRandomNumTimer(void);
void UnsignIntegerToString(uint8 bInteger, char* pString);
void str_replace(char *orig, char* result, char *rep, char *with);
void ToHexString(uint16 iData, char* HexString, enumbool eNible);
enumbool StringToInt(enumTypeNumeral eNumeral, uint16* pData, char* pString);
void IntToString(sint16 iData, char* pStr);
void LowerCase(char* pNum);
void UpperCase(char* pNum);
void GetStringByBreakChar(char* pIn,
                          char* pOut,
                          uint8 bStrSize,
                          char* BreakChar);
uint8 CalculateCheckSum(char* pCalculate, uint8 bSize);
uint8 CRC8Bits(uint8* pData, uint16 Len);
#endif /*_ULTILITIES_H*/