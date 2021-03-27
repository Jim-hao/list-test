#include "osa.h"
#include "list.h"
#include "file.h"

#define  MAX_COLUM_LEN  (10)
#define  CMD_LEN        (100)

/* analyse the file dir  */
Int32  DIR_preInit(DirInfo *pdirInfo)
{
    struct dirent *ptr = OSA_NULL;
    pdirInfo->pdir = opendir(pdirInfo->dirpath);
    while((ptr = readdir(pdirInfo->pdir)) != OSA_NULL)
    {
        if (strlen(ptr->d_name) > 2)
        {
            pdirInfo->fileNum++;
        }
    }
    closedir(pdirInfo->pdir);
    pdirInfo->pdir = OSA_NULL;
}

/* calloc file memory, get total lines for each file */
Int32  FILE_preInit(DirInfo  *dirInfo, FileInfo  **ppfileInfo)
{
    struct dirent  *ptr = OSA_NULL;
    FileInfo  *ptmpfile = OSA_NULL;
 
    DIR  *pdir = opendir(dirInfo->dirpath);
    if (OSA_isNull(pdir))
    {
        OSA_ERROR("opendir faild!\n");
        return OSA_EFAULT;        
    }

    /* calloc */
    *ppfileInfo = (FileInfo *)calloc(dirInfo->fileNum, sizeof(FileInfo));
    ptmpfile = *ppfileInfo;
    while((ptr = readdir(pdir)) != OSA_NULL)
    {
        if (strlen(ptr->d_name) > 2)
        {
            /* joint the fileName and dirpath together */
            snprintf(ptmpfile->fileName, MAX_FILENAME_LEN, "%s%s", dirInfo->dirpath, ptr->d_name);           
            OSA_INFO("file Name: %s\n",  ptmpfile->fileName);
            FILE_getlineNum(ptmpfile->fileName, &ptmpfile->lineNum);
            ptmpfile++;
        }
    }

    closedir(pdir);
    pdir = OSA_NULL;
}

Int32  FILE_getlineNum(Char *pfilePath, Int32 *plineNum)
{
    Int32 linenum = 0;
    size_t bufflen = 0;
    Char *pline = OSA_NULL;

    FILE *pfile = fopen(pfilePath, "r+");
    if (OSA_isNull(pfile))
    {
        perror("fopen");
        return OSA_EFAULT;
    }

    while(getline(&pline, &bufflen, pfile) != -1)
    {    
        ++linenum;
    }

    *plineNum = linenum;

    fclose(pfile);
    pfile = OSA_NULL;

    return OSA_SOK;
}

static void FILE_readcolum(Char *pline, Int32 columnNum)
{
    Int32  i, j = 0;
    Char  cmd[CMD_LEN] = {0};
    Char  *pcur = cmd;

    Char **pcolum = (Char **)calloc(columnNum, sizeof(Char *));
    for (i = 0;  i < columnNum; i++)
    {
        pcolum[i] = (Char *)calloc(1, MAX_COLUM_LEN);
    }

    for (i = 0; i < columnNum; i++)
    {
        for (j = 1; j <= i && i > 0; j++)
        {
            memcpy(pcur, "%*s", 3);
            pcur += 3;
        }
        memcpy(pcur, "%s", 3);
        pcur = cmd;  /* return to cmd */
        sscanf(pline, cmd, pcolum[i]);
        OSA_DEBUG("%5s", pcolum[i]);
        memset(cmd, 0, sizeof(cmd));                
    }
    putchar(10);

    for (i = 0;  i < columnNum; i++)
    {
        free(pcolum[i]);
        pcolum[i] = OSA_NULL;
    }

    free(pcolum);
    pcolum = OSA_NULL;
}

Int32  FILE_getResult(FileInfo  *pfileInfo, Int32 columnNum)
{
    Int32  i =  0;
    size_t bufflen = 0;
    Char *pline = OSA_NULL;

    FILE *pfile = fopen(pfileInfo->fileName, "r+");
    if (OSA_isNull(pfile))
    {
        perror("fopen");
        return -1;
    }

    while(getline(&pline, &bufflen, pfile) != -1)
    {    
        FILE_readcolum(pline, columnNum);
        //OSA_msleep(500);
    }

    fclose(pfile);
    pfile = OSA_NULL;

    if (OSA_NULL != pline)
    {
        free(pline);
    }
}