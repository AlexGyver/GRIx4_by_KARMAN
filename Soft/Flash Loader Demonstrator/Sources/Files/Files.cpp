/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : Files.cpp
* Author             : MCD Application Team
* Version            : v2.8.0
* Date               : 01-September-2015
* Description        : Implements the Files DLL interface
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

//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
//#define _CRT_SECURE_NO_DEPRECATE 
//#define _CRT_SECURE_NO_WARNINGS 
//#define _CRT_NONSTDC_NO_DEPRECATE  1
//#define _AFX_SECURE_NO_WARNINGS 1
//#define _ATL_SECURE_NO_WARNINGS  1

#include "stdafx.h"
#include "Files.h"
#include "Ini.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "STFILE.h"

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CFilesApp

BEGIN_MESSAGE_MAP(CFilesApp, CWinApp)
	//{{AFX_MSG_MAP(CFilesApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilesApp construction

CFilesApp::CFilesApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFilesApp object

CFilesApp theApp;


extern "C" DWORD EXPORT FILES_CreateImage(PHANDLE pHandle, BYTE nAlternate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD Ret=FILES_BADPARAMETER;

	CImage *pImage=new CImage(nAlternate, FALSE, NULL);
	if (pImage)
	{
		*pHandle=(HANDLE)pImage;
		Ret=FILES_NOERROR;
	}
	return Ret;
}

extern "C" DWORD EXPORT FILES_CreateImageFromMapping(PHANDLE pHandle, PMAPPING pMapping)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD Ret=FILES_BADPARAMETER;

	CImage *pImage=new CImage(pMapping, FALSE, NULL);
	if (pImage)
	{
		*pHandle=(HANDLE)pImage;
		Ret=FILES_NOERROR;
	}
	return Ret;
}

extern "C" DWORD EXPORT FILES_DuplicateImage(HANDLE hSource, PHANDLE pDest)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	DWORD dwRet=FILES_NOERROR;

	CImage *obImage=new CImage((CImage*)hSource);
	if (obImage->GetImageState())
	{
			*pDest=(HANDLE)obImage;
	}
	else
	{
			free(obImage);
			dwRet=FILES_BADFORMAT;
	}
	return dwRet;
}

extern "C" DWORD EXPORT FILES_GetImageNbElement(HANDLE Handle, PDWORD pNbElements)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

	CImage *pImage=(CImage *)Handle;
	*pNbElements=pImage->GetNbElements();

	return dwRet;
}

extern "C" DWORD EXPORT FILES_SetImageElement(HANDLE Handle, DWORD dwRank, BOOL bInsert, IMAGEELEMENT Element)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

	CImage *pImage=(CImage *)Handle;
	pImage->SetImageElement(dwRank, bInsert, Element);
			
	return dwRet;
}

extern "C" DWORD EXPORT FILES_GetImageElement(HANDLE Handle, DWORD dwRank, PIMAGEELEMENT pElement)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

	CImage *pImage=(CImage *)Handle;		
	pImage->GetImageElement(dwRank, pElement);
		
	return dwRet;
}

extern "C" DWORD EXPORT FILES_FilterImageForOperation(HANDLE Handle, PMAPPING pMapping, DWORD Operation, BOOL bTruncateLeadFFForUpgrade)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

	if (!pMapping)
		return FILES_BADPARAMETER;

    CImage *pImage=(CImage *)Handle;
	pImage->FilterImageForOperation(pMapping, Operation, bTruncateLeadFFForUpgrade);
		
	return dwRet;
}

extern "C" DWORD EXPORT FILES_DestroyImageElement(HANDLE Handle, DWORD dwRank)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

	CImage *pImage=(CImage *)Handle;
	pImage->DestroyImageElement(dwRank);
	
	return dwRet;
}

extern "C" DWORD EXPORT FILES_DestroyImage(PHANDLE pHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

    CImage *pImage=(CImage *)(*pHandle);
			
	free(pImage);
	*pHandle=0;
		
	return dwRet;
}


extern "C" DWORD EXPORT FILES_ImageFromFile(PSTR pPathFile, PHANDLE pImage, BYTE nAlternate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	DWORD Ret=FILES_BADPARAMETER;

	CImage *obImage=new CImage(nAlternate, pPathFile, FALSE, NULL);
	if (obImage)
	{
		if (obImage->GetImageState())
		{
			*pImage=(HANDLE)obImage;
			Ret=FILES_NOERROR;
		}
		else
		{
			free(obImage);
			Ret=FILES_BADFORMAT;
		}
	}
	return Ret;
}

extern "C" DWORD EXPORT FILES_ImageToFile(PSTR pPathFile, HANDLE Image)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

    CImage *pImage=(CImage *)(Image);
			
	if (!pImage->DumpToFile(pPathFile)) 
		dwRet=FILES_BADPARAMETER;
		
	return dwRet;
}

extern "C" DWORD EXPORT FILES_GetImageAlternate(HANDLE Image, PBYTE pAlternate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

    CImage *pImage=(CImage *)(Image);		
	*pAlternate=pImage->GetAlternate();
			
	return dwRet;
}

extern "C" DWORD EXPORT FILES_GetImageName(HANDLE Image, PSTR Name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

	CImage *pImage=(CImage *)(Image);
			
	if (!pImage->GetName(Name))
		lstrcpy(Name, "");
			
	return dwRet;
}

extern "C" DWORD EXPORT FILES_SetImageName(HANDLE Image, PSTR Name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwRet=FILES_NOERROR;

    CImage *pImage=(CImage *)(Image);
	pImage->SetName(Name);
		
	return dwRet;
}

extern "C" DWORD EXPORT FILES_GetMemoryMapping(PSTR pPathFile, PWORD Size, PSTR MapName, PWORD PacketSize, PMAPPING pMapping, PBYTE PagesPerSector)
{
	int NumberOfSections = 0;
	CIni Ini((LPCSTR)pPathFile);
	CStringArray sections;
	Ini.GetSectionNames(&sections);

	NumberOfSections = sections.GetSize();
	*Size = NumberOfSections - 1;

	if (pMapping == NULL) return 0;

	//pMapping = (PMAPPING)malloc(sizeof(MAPPING));
	//pMapping->NbSectors = 0;
	//pMapping->pSectors = (PMAPPINGSECTOR) malloc((NumberOfSections+1) * sizeof(MAPPINGSECTOR));
	

	PMAPPINGSECTOR auxsector = pMapping->pSectors;

	for(int pos=0; pos<NumberOfSections; pos++)
	{
       PMAPPINGSECTOR sector  = (PMAPPINGSECTOR) malloc(sizeof(MAPPINGSECTOR));
	   memset(sector, 0x00, sizeof(MAPPINGSECTOR)); 

	   CString HexStr = CString("");
	   CString section;
	   section.Format(sections[pos]);

	   if(section.Compare("Product") == 0)
	   {
           HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"PacketSize", "100");
		   WORD vPacketSize = (WORD)_tcstoul(HexStr, 0, 16) ;
	       *PacketSize = vPacketSize;

		   HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"MAPNAME", "Sectors");
		   MapName = (LPSTR)(LPCSTR)HexStr;

		   HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"PagesPerSector", "Sectors");
		   *PagesPerSector = (BYTE)(LPCSTR)_tcstoul(HexStr, 0, 10) ;

		   continue;
	   }

	   HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Name", "Page x");
	   sector->Name = (char*) malloc(section.GetLength());
	   memset(sector->Name, 0xFF, section.GetLength());
	   strcpy(sector->Name,(LPSTR)(LPCTSTR)section);

	  
       HexStr = CString("00000000");
       HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Index", "00000000");
	   sector->dwSectorIndex = _tcstoul(HexStr, 0, 16) ;

	   HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Address", "00000000");
	   sector->dwStartAddress = _tcstoul(HexStr, 0, 16) ;

	   HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Size", "00000000");
	   sector->dwSectorSize  = _tcstoul(HexStr, 0, 16) ;

	   
	   HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Type"  ,"111");
	   sector->bSectorType = (BYTE)_tcstoul(HexStr, 0, 2) ;    
	   
       sector->UseForOperation = TRUE; //Ini.GetInt((LPCTSTR)section,(LPCTSTR)"UFO"    ,OPERATION_UPLOAD);
	   sector->UseForErase = TRUE; //Ini.GetInt((LPCTSTR)section,(LPCTSTR)"UFO"    ,OPERATION_UPLOAD);
	   sector->UseForUpload = TRUE; //Ini.GetInt((LPCTSTR)section,(LPCTSTR)"UFO"    ,OPERATION_UPLOAD);
       
	   memcpy(auxsector, sector, sizeof(MAPPINGSECTOR));
	   auxsector++;

	   pMapping->NbSectors++;

	   delete sector;
	}

	return 0;
}
