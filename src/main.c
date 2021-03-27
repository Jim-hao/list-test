#include "osa.h"
#include "list.h"
#include "file.h"

typedef struct wkfl_demo
{
    DirInfo   dirInfo;
    FileInfo  *pfileInfo;
}WKFL_DEMO;

 static void DEMO_HELP()
 {
     printf(" for example: \n"
            "   ./test.out -d ./data/  \n");
 }

Int32 main(Int32 argc, char *argv[])
{
    if (argc < 3)
    {
        DEMO_HELP();
        return -1;
    }

    Char  ch = 0;
    WKFL_DEMO  *pObj = (WKFL_DEMO *)calloc(1, sizeof(WKFL_DEMO));
    do
    {
        ch = getopt(argc, argv, "hp:d:o:");
        switch(ch)
        {
            case 'h':
                DEMO_HELP();
            case 'p':
                break;
            case 'o':
                break;
            case 'd':
                pObj->dirInfo.dirpath = optarg;
                break;
            default:
                break;
        }
    } while(ch > 0);

    DIR_preInit(&pObj->dirInfo);

    FILE_preInit(&pObj->dirInfo, &pObj->pfileInfo);

    FILE_getResult(&pObj->pfileInfo[0], 1);

    free(pObj->pfileInfo);
    free(pObj);
    OSA_INFO("main exit\n");
    return  0;
}