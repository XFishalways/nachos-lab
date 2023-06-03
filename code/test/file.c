#include "syscall.h"

int main() {
    char* file1 = "xfish_test1.txt";
    char* file2 = "xfish_test2.txt";
    char* buffer = "xfish writes";

    int fileId1;
    int fileId2;
    Create(file1);
    Create(file2);
    fileId1 = Open(file1);
    Write(buffer, 12, fileId1);
    Close(fileId1);
    Remove(file2);
}