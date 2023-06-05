// filehdr.cc 
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the 
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the 
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect 
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector, 
//
//      Unlike in a real system, we do not keep track of file permissions, 
//	ownership, last modification date, etc., in the file header. 
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "filehdr.h"
#include "debug.h"
#include "synchdisk.h"
#include "main.h"

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------

bool
FileHeader::Allocate(PersistentBitmap *freeMap, int fileSize)
{ 
    if(numSectors < NumDirect) {
        if(freeMap->NumClear() < numSectors) {
            return FALSE;
        }
        for (int i = 0; i < numSectors; i++) {
            dataSectors[i] = freeMap->FindAndSet();
            ASSERT(dataSectors[i] >= 0);
        }
    } else {
        if (freeMap->NumClear() < numSectors + 1){
            return false;
        }
        for (int i = 0; i < numSectors; i++) {
            dataSectors[i] = freeMap->FindAndSet();
            ASSERT(dataSectors[i] >= 0);
        }
        int *indirect = new int[SectorSize / sizeof(int)];
        for (int i = 0; i < numSectors - NumDirect + 1; i++) {
            indirect[i] = freeMap->FindAndSet();
            ASSERT(indirect[i] >= 0);
        }
        kernel->synchDisk->WriteSector(dataSectors[NumDirect-1], (char *)this);
    }

    return TRUE;
}

//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

void 
FileHeader::Deallocate(PersistentBitmap *freeMap)
{
    for (int i = 0; i < numSectors; i++) {
	ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
	freeMap->Clear((int) dataSectors[i]);
    }

    if(numSectors < NumDirect) {
        for (int i = 0; i < numSectors; i++) {
            ASSERT(freeMap->Test((int)dataSectors[i]));
            freeMap->Clear((int)dataSectors[i]);
        }
    } else {
        char *indirect = new char[SectorSize];
        kernel->synchDisk->ReadSector(dataSectors[NumDirect-1], (char*)this);
        for (int i = 0; i < numSectors; i++) {
            ASSERT(freeMap->Test((int)dataSectors[i]));
            freeMap->Clear((int)dataSectors[i]);
        }
        for (int i = 0; i < numSectors - NumDirect + 1; i++) {
            ASSERT(freeMap->Test((int)indirect[i*4]));
            freeMap->Clear((int)indirect[i*4]);
        }
        delete [] indirect;
    }
}

//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk. 
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector)
{
 
    kernel->synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk. 
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector)
{
    
    kernel->synchDisk->WriteSector(sector, (char *)this); 
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

int
FileHeader::ByteToSector(int offset)
{
    if(offset < (NumDirect-1) * SectorSize) {
        return dataSectors[offset / SectorSize];
    } else {
        int sector = (offset - (NumDirect - 1) * SectorSize) / SectorSize;
        char *indirect = new char[SectorSize];
        kernel->synchDisk->ReadSector(dataSectors[NumDirect-1], (char*)this);
        int result = indirect[sector * 4];
        delete [] indirect;
        return result;
    }
}

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength()
{
    return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void
FileHeader::Print()
{
    char *indirect = new char[SectorSize];
    if(numSectors < NumDirect) {
        for (int i = 0; i < numSectors; i++) {
            printf("block %d", i);
            printf("%d ", dataSectors[i]);
        }
    } else {
        printf("indirect sector: %d\n", dataSectors[NumDirect-1]);
        for(int i = 0; i < numSectors - NumDirect + 1; i++) {
            printf("%d ", indirect[i*4]);
        }
    }
    int k;
    if(numSectors < NumDirect) {
        for (int i = 0; i < numSectors; i++) {
            kernel->synchDisk->ReadSector(dataSectors[i], indirect);
            printf("block %d", i);
            printf("%d ", dataSectors[i]);
            for(int j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
                if('\040' <= indirect[j] && indirect[j] <= '\176') {
                    printf("%c", indirect[j]);
                } else {
                    printf("\\&x", (unsigned char)indirect[j]);
                }
                printf("\n");
            }
        }
    } else {
        for(int i = k = 0; i < NumDirect - 1; i++) {
            kernel->synchDisk->ReadSector(dataSectors[i], indirect);
            for(int j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
                if('\040' <= indirect[j] && indirect[j] <= '\176') {
                    printf("%c", indirect[j]);
                } else {
                    printf("\\&x", (unsigned char)indirect[j]);
                }
                printf("\n");
        }
    }

    printf("FileHeader contents.  File size: %d.  File blocks:\n", numBytes);
    
    printf("type: %s\n", type);
    printf("create time: %s\n", createTime);
    printf("change time: %s", lastChangeTime);
    printf("last visit time: %s\n", lastVisitTime);
    
    // for (i = 0; i < numSectors; i++)
	// printf("%d ", dataSectors[i]);
    // printf("\nFile contents:\n");
    // for (i = k = 0; i < numSectors; i++) {
	// kernel->synchDisk->ReadSector(dataSectors[i], data);
    //     for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
	//     if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
	// 	printf("%c", data[j]);
    //         else
	// 	printf("\\%x", (unsigned char)data[j]);
	// }
    //     printf("\n"); 
    // }
    // delete [] data;
    }
}

void
FileHeader::setCreateTime()
{
    time_t timefile;
    time(&timefile);
    strncpy(createTime,asctime(gmtime(&timefile)),25);
    createTime[24]='\0';
    printf("create file at:%s\n",createTime);
}

void
FileHeader::setChangeTime()
{
    time_t timefile;
    time(&timefile);
    strncpy(lastChangeTime,asctime(gmtime(&timefile)),25);
    lastChangeTime[24]='\0';
    printf("change file at %s\n",lastChangeTime);
}

void
FileHeader::setVisitTime(int sector)
{
    time_t timefile;
    time(&timefile);
    strncpy(lastVisitTime,asctime(gmtime(&timefile)),25);
    lastVisitTime[24]='\0';
    printf("file %d last visit at %s\n", sector, lastVisitTime);
}