#include <iostream>
#include <cstring>
#include "VoteController.h"
//#include "debug.h"

using namespace std;

static size_t totalBytesFreed = 0;

size_t getTotalBytesFreed() 
{
    return totalBytesFreed;
}

// Overload the delete operator
void operator delete(void* ptr, size_t size) noexcept
{
    totalBytesFreed += size;
    std::free(ptr); // Call the standard free function to actually free the memory
}

// Overload the delete[] operator
void operator delete[](void* ptr, size_t size) noexcept 
{
    totalBytesFreed += size;
    std::free(ptr);
}

int main(int argc, char* argv[])
{
//  #ifdef _DEBUG
//      _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//  #endif

    const int MaxArgumentSize = 256;
    char* registeredVotersFile = new char[MaxArgumentSize];
    char* bucketEntriesNum = new char[MaxArgumentSize];

    for (int i = 0; i < MaxArgumentSize; i++)
    {
        registeredVotersFile[i] = '\0';
        bucketEntriesNum[i] = '\0';
    }

    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) 
        {
            strncpy(registeredVotersFile, argv[i + 1], MaxArgumentSize - 1);
            i++; // skip next argument which is file name
        }
        else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) 
        {
            strncpy(bucketEntriesNum, argv[i + 1], MaxArgumentSize - 1);
            i++; // skip next argument which is bucket size
        }
        else 
        {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            return EXIT_FAILURE;
        }
    }
    if (!registeredVotersFile[0] || !bucketEntriesNum[0]) 
    {
        std::cerr << "Missing required arguments!" << std::endl;
        return EXIT_FAILURE;
    }

    VoteController* controller = new VoteController(atoi(bucketEntriesNum), registeredVotersFile);
    controller->Init();

    delete[] registeredVotersFile;
    delete[] bucketEntriesNum;
    long bytesFreedBeforeExit = (long)getTotalBytesFreed();
    delete controller;
    long bytesFreedAfterExit = (long)getTotalBytesFreed() - bytesFreedBeforeExit;

    cout << bytesFreedAfterExit << " of Bytes Released" << endl << endl;
    
    return EXIT_SUCCESS;
}
