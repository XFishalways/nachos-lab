#include "syscall.h"

int main() {
    char* file1 = "file1.txt";
    char* file2 = "file2.txt";

    char buf[128];

    int fileId1;
    int fileId2;
    fileId1 = Open(file1);
    Create(file2);
    Read(buf, 128, fileId1);
    fileId2 = Open(file2);
    Write(buf, 128, fileId2);
    Close(file1);
    Close(file2);
}