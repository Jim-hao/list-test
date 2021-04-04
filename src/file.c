#include "osa.h"
#include "list.h"
#include "file.h"

#define  MAX_COLUM_LEN     (10)
#define  MAX_COMMAND_LEN   (100)

/* analyse the file dir  */
Int32  DIR_preInit(DirInfo **ppdirInfo, Char *pDirPath)
{
    struct dirent *ptr = OSA_NULL;

    /* calloc dir memory */
    *ppdirInfo = (DirInfo *)calloc(1, sizeof(DirInfo));
    DirInfo *pdirInfo = *ppdirInfo;
    pdirInfo->dirpath = pDirPath;

    /* open dir */
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
Int32  FILE_memoryInit(DirInfo  *dirInfo, FileInfo  **ppfileInfo)
{
    struct dirent  *ptr = OSA_NULL;
    FileInfo  *ptmpfile = OSA_NULL;
 
    DIR  *pdir = opendir(dirInfo->dirpath);
    if (OSA_isNull(pdir))
    {
        OSA_ERROR("opendir faild!\n");
        return OSA_EFAULT;        
    }

    /* calloc file memory */
    *ppfileInfo = (FileInfo *)calloc(dirInfo->fileNum, sizeof(FileInfo));
    ptmpfile = *ppfileInfo;
    while((ptr = readdir(pdir)) != OSA_NULL)
    {
        if (strlen(ptr->d_name) > 2)
        {
            /* joint the fileName and dirpath together */
            Int32 len = strlen(dirInfo->dirpath) + strlen(ptr->d_name) + 1;
            snprintf(ptmpfile->fileName, len, "%s%s", dirInfo->dirpath, ptr->d_name);                    
            FILE_getlineNum(ptmpfile->fileName, &ptmpfile->lineNum);
            //OSA_INFO("file Name: %s  linenum:%d\n",  ptmpfile->fileName, ptmpfile->lineNum);
            ptmpfile->pDataResult = (Char *)calloc(ptmpfile->lineNum, sizeof(Char) * 4);
            ptmpfile++;
        }
    }

    closedir(pdir);
    pdir = OSA_NULL;
}

Int32  FILE_getlineNum(Char *pfilePath, Int32 *plineNum)
{
    volatile Int32 linenum = 0;

    FILE *pfile = fopen(pfilePath, "r");
    if (OSA_isNull(pfile))
    {
        perror("fopen");
        return OSA_EFAULT;
    }

    Char *pline = OSA_NULL;
    size_t len = 0;
    while(getline(&pline, &len, pfile) != -1)
    {    
        ++linenum;
    }
    
    *plineNum = linenum;

    free(pline);
    fclose(pfile);
    pfile = OSA_NULL;

    return linenum;
}

static void FILE_readcolum(Char *pline, Int32 columnNum, Char *result)
{
    Int32  i, j = 0;
    Char  cmd[MAX_COMMAND_LEN] = {0};
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
        pcur = cmd;     
        sscanf(pline, cmd, pcolum[i]);

        /* analyse result */
        result[0] = pcolum[i][0];
        result[1] = pcolum[i][1];
        result[2] = pcolum[i][3];
        result[3] = pcolum[i][5];
        //OSA_DEBUG("%5s\n",pcolum[i]);
        //OSA_DEBUG("%1c%1c%1c%1c\n", result[0], result[1], result[2], result[3]);
        memset(cmd, 0, sizeof(cmd));                
    }
 
    for (i = 0;  i < columnNum; i++)
    {
        free(pcolum[i]);
        pcolum[i] = OSA_NULL;
    }

    free(pcolum);
    pcolum = OSA_NULL;
}

static Int32  FILE_analyData(FileInfo *pfileInfo, Int32 columnNum)
{
    size_t bufflen = 0;
    Char *pline = OSA_NULL;
    Char *pcur = OSA_NULL;
   
    FILE *pfile = fopen(pfileInfo->fileName, "r+");
    if (OSA_isNull(pfile))
    {
        perror("fopen");
        return -1;
    }

    Int32 linenum = 0;
    while(getline(&pline, &bufflen, pfile) != -1)
    {    
        FILE_readcolum(pline, columnNum, pfileInfo->pDataResult + (linenum*4));
        ++linenum;
    }

    fclose(pfile);
    pfile = OSA_NULL;

    if (OSA_isNotNull(pline))
    {
        free(pline);
    }

    return OSA_SOK;
}

Int32 FILE_getResult(DirInfo *dirInfo, FileInfo *fileInfo, Int32 columNum)
{
    Int32  i = 0;
    FileInfo *pfile = fileInfo;

    for (i = 0; i < dirInfo->fileNum; i++)
    {
        OSA_INFO("analyse file: %s, lineNum:%d\n", pfile->fileName, pfile->lineNum);
        FILE_analyData(pfile, columNum);
        pfile++;
    }
}
