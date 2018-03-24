#define _ULTILITIES_C

/* Includes ------------------------------------------------------------------*/
#include "Utilities.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* Use timer to create random */
#include "Timer_Function.h"
/*******************************************************************************
* Function Name  : RfAL_CreatRandomNum
* Description    : creat a random number
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
uint32 gf_CreatRandomNum(void)
{
    static uint32 OldNum;
    static uint8 i;
    uint32 RandomNum;
    uint32 Temp1, Temp2;
    static uint32 Addr;
    Addr = Addr + (((OldNum)>>i)&0xfff); /*Ram memory from 0x20000000 to 0x20000FFF*/
    
	i++;
    i = i%8;
    do
    {
        if ((Addr>0x00bfff)|(Addr<0x008000))
        {
            Addr = 0x008000 + (((OldNum)>>i)&0xfff);
        }
        RandomNum +=  *(uint32*)Addr;
        Temp1 = RandomNum>>16;
        Temp2 = RandomNum&0xFFFF;
        RandomNum ^= (Temp1+Temp2)<<16 + (Temp1^Temp2);
        Addr = Addr + (((OldNum)>>i)&0xff); 
        i++;
   		i = i%8;
    }while ((RandomNum==0)||(RandomNum==0xFFFFFFFF)||(RandomNum==OldNum));
    OldNum = RandomNum;

    return RandomNum;
}
/*******************************************************************************
* Function Name  : RfAL_CreatRandomNumTimer
* Description    : creat a random number base on timer
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
uint32 gf_CreatRandomNumTimer(void)
{
	static uint32 OldNum;
    uint32 RandomNum;
	/* Test with random Time */
	RandomNum = OldNum;
	while(RandomNum == OldNum)
	{
		RandomNum = vGetCurrentCounterTimeBaseUs();
	}
	OldNum = RandomNum;
    return RandomNum;
}
/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: UnsignIntegerToString
*Description:   change unsigned integer to string
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
void UnsignIntegerToString(uint8 bInteger, char* pString)
{
    pString[1] = (bInteger>=10)?(bInteger%10 +'0'):(0);
    pString[0] = (bInteger>=10)?(bInteger/10 +'0'):(bInteger+'0');
    if (bInteger>10)
        pString[2] = 0;
}

/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: str_replace
*Description:   
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
void str_replace(char *orig, char* result, char *rep, char *with)
{
    char *ins;    // the next insert point
    char *tmp;    // varies
    uint16 len_rep;  // length of rep
    uint16 len_with; // length of with
    uint16 len_front; // distance between rep and end of last rep
    uint16 count;    // number of replacements
    
    if (!orig)
        result =  NULL;
    if (!rep || !(len_rep = strlen(rep)))
        result =  NULL;
    if (!(ins = (char* )strstr(orig, rep))) 
        result =  NULL;
    if (!with)
        with = "";
    len_with = strlen(with);
    
    if (result!=NULL)
    {
        for (count = 0; tmp = (char* )strstr(ins, rep); ++count) {
            ins = tmp + len_rep;
        }
        
        // first time through the loop, all the variable are set correctly
        // from here on,
        //    tmp points to the end of the result string
        //    ins points to the next occurrence of rep in orig
        //    orig points to the remainder of orig after "end of rep"
        //        tmp = result = (char* )malloc(strlen(orig) + (len_with - len_rep) * count + 1);
        tmp = result;
        if (!result)
            result =  NULL;
        
        while (count--) {
            ins = (char* )strstr(orig, rep);
            len_front = ins - orig;
            tmp = (char* )strncpy(tmp, orig, len_front) + len_front;
            tmp = (char* )strcpy(tmp, with) + len_with;
            orig += len_front + len_rep; // move to next "end of rep"
        }
        strcpy(tmp, orig);
    }
}

/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: ToHexString
*Description:   
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
void ToHexString(uint16 iData, char* HexString, enumbool eNible)
{
    uint8 bData;
    uint8 i=0,j;
    
    if (iData<=0xFF)
        if ((eNible)&(iData<=0x0F))
            j = 0;
        else 
            j = 1;
    else
        if ((eNible)&((iData>>8)<=0x0F))
            j = 2;
        else 
            j = 3;
        
        do
        {
            bData = (iData>>((j-i)*4))&0x0F;
            if (bData<=9)
                HexString[i]=bData+'0';
            else if (bData==10)
                HexString[i]='A';
            else if (bData==11)
                HexString[i]='B';
            else if (bData==12)
                HexString[i]='C';  
            else if (bData==13)
                HexString[i]='D';
            else if (bData==14)
                HexString[i]='E';
            else if (bData==15)
                HexString[i]='F';
        }while (i++<j);
}

/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: HexStringToInt
*Description:
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
enumbool StringToInt(enumTypeNumeral eNumeral, uint16* pData, char* pString)
{
    uint16 iData=0;
    uint8 bData;
    uint8 i=0;
    enumbool Ret = eTRUE;
    
    switch (eNumeral)
    {
    case eHEXADECIMAL:
        do
        {
            iData<<=4;
            if ((pString[i]<='9')&(pString[i]>='0'))
                bData = pString[i]-'0';
            else if ((pString[i]=='A')|(pString[i]=='a'))
                bData = 10;
            else if ((pString[i]=='B')|(pString[i]=='b'))
                bData = 11;
            else if ((pString[i]=='C')|(pString[i]=='c'))
                bData = 12;
            else if ((pString[i]=='D')|(pString[i]=='d'))
                bData = 13;
            else if ((pString[i]=='E')|(pString[i]=='e'))
                bData = 14;
            else if ((pString[i]=='F')|(pString[i]=='f'))
                bData = 15;
            else
            {
                Ret = eFALSE;
                break;
            }
            iData|=bData;
        }while (pString[++i]!=0);
        *pData = iData;
        break;
    case  eDECIMAL:
        *pData = 0;
        do
        {
            *pData *=10;
            if ((pString[i]<='9')&&(pString[i]>='0'))
                *pData += pString[i] - '0';
            else
            {
                Ret = eFALSE;
                break;
            }
        }while(pString[++i]!=0);
    }
    return Ret;
}

/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: IntToString
*Description:   change 16bit number to string
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
void IntToString(sint16 iData, char* pStr)
{
    uint16 Temp;
    uint8 i=0,j=0;
    char str[5];
    
    Temp = abs(iData);
    while (eTRUE)
    {
        str[i]='0'+ Temp%10;
        if (Temp/10)
        {
            str[i++]='0'+ Temp%10;
            Temp = Temp/10;
        }
        else
            break;
    }
    if (iData<0)
    {
        pStr[j++]='-';
        i++;
    }
    do
    {
        pStr[j]=str[i-j];
    }while (j++<i);
    pStr[j]=0;/*Add Null character*/
}

/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: LowerCase
*Description:   change string to lower case
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
void LowerCase(char* pNum)
{
    uint16 i = 0;
    char Temp;
    
    do
    {
        Temp = pNum[i];
        if ((Temp>='A')&&(Temp<='Z'))
            pNum[i] = Temp + 32;
    }while (pNum[++i]!=0);
}

/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: UpperCase
*Description:   change string to upper case
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
void UpperCase(char* pNum)
{
    uint16 i = 0;
    char Temp;
    
    do
    {
        Temp = pNum[i];
        if ((Temp>='a')&&(Temp<='z'))
            pNum[i] = Temp - 32;
    }while (pNum[++i]!=0);
}

/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: GetStringByBreakChar
*Description:   get small string in pIn,  
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
void GetStringByBreakChar(char* pIn,
                          char* pOut,
                          uint8 bStrSize,
                          char* BreakChar)
{
    uint32 iAddr;
    uint8  bLen;
    
    iAddr = (uint32)strstr(pIn,BreakChar);
    if (iAddr)
    {
        memcpy(pOut,pIn,(iAddr-(uint32)pIn));
    }
    else
    {
        bLen = strlen(pIn);
        if (bLen>bStrSize)
            memcpy(pOut,pIn,bStrSize);
        else
            strcpy(pOut,pIn);
    }
}
/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: CalculateCheckSum
*Description:   calculate checksum byte  
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
uint8 CalculateCheckSum(char* pCalculate, uint8 bSize)
{
	uint8 bIndex, bCheckSum;
	/* reset checksum */
	bCheckSum = 0;
	for(bIndex=0; bIndex<bSize; bIndex++)
	{
		bCheckSum+=pCalculate[bIndex];
	}
	return bCheckSum;
}
/*-----------------------------------------------------------------------------
*Engineer: 
*Historical: 
*Function name: CRC8Bits
*Description:   calculate crc
*Input: None
*Output: None
-----------------------------------------------------------------------------*/
uint8 CRC8Bits(uint8* pData, uint16 Len)
{
    uint16 i=0;
    uint8 bCRC=0;
    
    do{
        bCRC ^= pData[i];
    }while (++i<Len);
    return bCRC;
}
