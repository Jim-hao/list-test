#ifndef __FILE_H__
#define __FILE_H__
#include "osa.h"

#define  MAX_FILENAME_LEN     (50)
#define  MAX_DATARESULT_LEN   (3)

typedef struct
{
    Int32   lineNum;
    Char    fileName[MAX_FILENAME_LEN];
    Char    *pDataResult;
}FileInfo;

typedef struct
{
    Char    *dirpath;
    DIR     *pdir;
    Int32   fileNum;
}DirInfo;

extern Int32  DIR_preInit(DirInfo **ppdirInfo, Char *pDirPath);
extern Int32  FILE_memoryInit(DirInfo  *dirInfo, FileInfo  **ppfileInfo);
extern Int32  FILE_getlineNum(Char *pfilePath, Int32 *plineNum);
extern Int32 FILE_getResult(DirInfo *dirInfo, FileInfo *fileInfo, Int32 columNum);

#endif