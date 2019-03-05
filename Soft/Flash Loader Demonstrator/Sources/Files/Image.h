/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : Image.h
* Author             : MCD Application Team
* Version            : v2.8.0
* Date               : 01-September-2015
* Description        : Defines the CImage class interface
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE
* "MCD-ST Liberty SW License Agreement V2.pdf"
*******************************************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

class CImage : public CObject 
{
private:
//UNICODE	char		m_LastError[1000];
	CString     m_LastError;
	BYTE		m_bAlternate;
	CPtrArray	*m_pElements;
	BOOL		m_ImageState;
	BOOL		m_bNamed;
//UNICODE	char		m_Name[255];
	CString m_Name;

	BOOL		LoadS19(CString pFilePath);
	BOOL		LoadHEX(CString pFilePath);
	BOOL		LoadBIN(CString pFilePath);

	BOOL		SaveS19(CString pFilePath);
	BOOL		SaveHEX(CString pFilePath);

	void		LDisplayError(CString Str) 
	                                { //UNICODE lstrcpy(m_LastError, Str);
										m_LastError = Str;
								    }
	BOOL        ExistsElementsAtAddress(DWORD Address);
	
	void		CompactElements();
public:
	CImage(CImage *pSource);
	CImage(BYTE bAlternate, BOOL bNamed, PSTR Name);
	CImage(PMAPPING pMapping, BOOL bNamed, PSTR Name);
	CImage(BYTE bAlternate, CString pFilePath, BOOL bNamed, PSTR Name);
	virtual ~CImage();

	BOOL	DumpToFile(CString pFilePath);
	BYTE	GetAlternate() { return m_bAlternate; }
	BOOL	GetImageState() { return m_ImageState; }
	BOOL	GetName(CString Name) { if (m_bNamed) 
		                             //lstrcpy(Name, m_Name);
									 Name = m_Name;
	                                 return m_bNamed; 
	                              }
	void    SetName(CString Name) { //lstrcpy(m_Name, Name);
		                            Name = m_Name;
	                                m_bNamed = TRUE;
	                              }
	BOOL	GetBuffer(DWORD dwAddress, DWORD dwSize, PBYTE pBuffer);

	DWORD	GetNbElements() { return m_pElements->GetSize(); }
	BOOL	SetImageElement(DWORD dwRank, BOOL bInsert, IMAGEELEMENT Element);
	BOOL	GetImageElement(DWORD dwRank, PIMAGEELEMENT pElement);
	BOOL	FilterImageForOperation(PMAPPING pMapping, DWORD Operation, BOOL bTruncateLeadFF);
	BOOL	DestroyImageElement(DWORD dwRank);
};

#endif

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE******/
