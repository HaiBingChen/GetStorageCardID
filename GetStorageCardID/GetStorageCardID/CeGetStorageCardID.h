#include "winioctl.h"
#include "storemgr.h"

#define ENGLISH           0
#define SIM_CHINESE       1
#define LANGUAGE          SIM_CHINESE

#define IOCTL_DISK_GET_STORAGEID             CTL_CODE(FILE_DEVICE_DISK,0x709,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define SDCARD_ID_FILE                       _T("\\GPS CARD\\SDCARD_ID.txt")
#define SDCARD_ID_ALL                        _T("\\USB Disk\\SDCARD_ID_ALL.txt")
#define SDCARD_PATH                          _T("\\GPS CARD")
#define USB_PATH                             _T("\\USB Disk")
#define SDCARD_MAP_PATH	                     _T("\\GPS CARD\\Navigation")
#define SDCARD_MAP_LICENSE                   _T("\\GPS CARD\\Navigation\\license")
#define USB_MAP_LICENSE                      _T("\\USB Disk\\license")

typedef struct _STORAGE_IDENTIFICATION
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwManufactureIDOffset;
	DWORD dwSerialNumOffset;
} STORAGE_IDENTIFICATION, *PSTORAGE_IDENTIFICATION;

unsigned char * GetStroageID(LPCTSTR szDisk);