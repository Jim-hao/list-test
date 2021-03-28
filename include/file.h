#ifndef __FILE_H__
#define __FILE_H__
#include "osa.h"

#define  MAX_FILENAME_LEN     (100)
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

#endif