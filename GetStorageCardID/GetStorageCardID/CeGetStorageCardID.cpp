#include "stdafx.h"
#include "CeGetStorageCardID.h"

unsigned char *GetStroageID(LPCTSTR szDisk)
{
	//HANDLE hDisk = CreateFile(_T("DSK1:"), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 
	HANDLE hDisk = CreateFile(szDisk, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 
	if (!hDisk) return FALSE;

	PSTORAGE_IDENTIFICATION pStoreInfo = (PSTORAGE_IDENTIFICATION) new BYTE[3000];

	DWORD dwBytesRet;

	if (!DeviceIoControl(hDisk, IOCTL_DISK_GET_STORAGEID, NULL, 0, pStoreInfo, 3000, &dwBytesRet, NULL))
	{
		DWORD err = GetLastError(); 
		delete [] pStoreInfo; 
		CloseHandle(hDisk); 
		return FALSE; 
	}

	BYTE *ManuID = (((BYTE *)pStoreInfo) + pStoreInfo->dwManufactureIDOffset); 
	BYTE *SerialNo = (((BYTE *)pStoreInfo) + pStoreInfo->dwSerialNumOffset);

	return ManuID;
}
