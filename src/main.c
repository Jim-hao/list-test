#include "demo.h"

 static void DEMO_help()
 {
     printf(" for example:             \n"
            "   ./test.out -d ./data/  \n");
 }

static void DEMO_paramCheck(Int32 argc, char *argv[], WKFL_DEMO **ppObj)
{
    if (argc < 3)
    {
        DEMO_help();
        //return -1;
    }

    Char  ch = 0;
    Char  *dirpath = OSA_NULL;
    *ppObj = (WKFL_DEMO *)calloc(1, sizeof(WKFL_DEMO));
    (*ppObj)->dirpath = (Char *)calloc(1, 100);

#if 0
    do
    {
        ch = getopt(argc, argv, "hp:d:o:");
        switch(ch)
        {
            case 'h':
                DEMO_help();
            case 'p':
                break;
            case 'o':
                break;
            case 'd':
                dirpath = optarg;
                memcpy((*ppObj)->dirpath, optarg, strlen(optarg));
                break;
            default:
                break;
        }
    } while(ch > 0);
#endif

    memcpy((*ppObj)->dirpath, "./data/", 8);
}

Int32 DEMO_destroy(WKFL_DEMO *pObj)
{
    free(pObj->dirInfo);
    free(pObj->pfileInfo->pDataResult);
    free(pObj->pfileInfo);
    free(pObj);
    free(pObj->dirpath);

    return OSA_SOK;
}

Int32 DEMO_start(WKFL_DEMO *pObj)
{
    /* 1.calloc dir mem  2.preinit dir */
    DIR_preInit(&pObj->dirInfo, pObj->dirpath);

    /* 1. calloc file mem */
    FILE_memoryInit(pObj->dirInfo, &pObj->pfileInfo);

    /* 1. get all data-file result */
    FILE_getResult(pObj->dirInfo, pObj->pfileInfo, 1);    

    /* 1. creat threads */
    Thread_taskCreate(pObj);
}

Int32 main(Int32 argc, char *argv[])
{
    WKFL_DEMO *pObj = OSA_NULL;

    DEMO_paramCheck(argc, argv, &pObj);

    DEMO_start(pObj);

    DEMO_destroy(pObj);

    OSA_INFO("main exit\n");
    return  0;
}