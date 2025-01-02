#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <string.h>
#include <stdio.h>
#include <io.h>

extern long FileIndex[30];
extern unsigned int NumberOfIndexes;

//int WriteFileIndex(char *filename)//
//int UpdateFileIndex(char *filename)//

// Read A File Index // Sep 1996 //
int ReadFileIndex(char *filename)
{
	int handle;
	int i;
	long loc;
	sprintf(filename + strlen(filename) -3, "NDX");
	handle = open(filename, O_RDWR | O_BINARY, S_IREAD | S_IWRITE);

	if(handle > 0)
	{
		loc = 0;
		lseek(handle, loc, 0);
		// get number of indexes //
		read(handle, &NumberOfIndexes, sizeof(int));
		loc += sizeof(int);

		for(i = 0; i < NumberOfIndexes; i++)
		{
			lseek(handle,loc,0);
			read(handle, &FileIndex[i], sizeof(long));
			loc += sizeof(long);
		}
		close(handle);

	}
	return(handle);
}


// Create A File Index // Sep 1996 //
int CreateFileIndex(char *filename)
{
	int handle;
	int i;
	long loc;
	sprintf(filename + strlen(filename) -3, "NDX");
	handle = open(filename, O_RDWR | O_TRUNC | O_CREAT | O_BINARY, S_IREAD | S_IWRITE);

	if(handle > 0)
	{
		loc = 0;
		lseek(handle, loc, 0);
		write(handle, &NumberOfIndexes, sizeof(int));
		loc += sizeof(int);
		for(i = 0; i < NumberOfIndexes; i++)
		{
			lseek(handle,loc,0);
			write(handle, &FileIndex[i], sizeof(long));
			loc += sizeof(long);
		}
		close(handle);

	}
	return(handle);
}