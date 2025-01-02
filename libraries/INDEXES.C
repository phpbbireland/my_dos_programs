#include <io.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <indexes.h>

extern Rep(char *, int, int);

long FileIndex[30];  //store 30 locations//
unsigned int  FileIndexCount = 0; //this is the number of locations in file//

int CreateFileIndex(char *datafile, int datahandle)//
{
	int handle;
	int i = 1;//skip first location//
	long loc = 0L;
	char ch;
	char indexfile[81] = "";
	strcpy(indexfile,datafile);
	sprintf(indexfile + strlen(datafile) -3, "NDX");
	remove(indexfile);

	handle = open(indexfile, O_RDWR | O_CREAT | O_BINARY,S_IWRITE );//create new
	if(handle == -1)
		return(-1);
	else
	{
		Rep("About to Create Index File..... ",0x4f,10);
		lseek(handle, 0L, SEEK_SET);
		lseek(datahandle,0L,SEEK_SET);

		FileIndex[i-1] = 0L;// First Index always Start Of File//

		do
		{
			read(datahandle,&ch,1);
			if(ch == '“')                          //endofpage//
			{
				FileIndex[i++] = tell(datahandle);   //save position//
				FileIndexCount++;
			}
		}while(!eof(datahandle));

		write(handle, &FileIndexCount, sizeof(int));
		loc += sizeof(int);
		lseek(handle,loc,SEEK_SET);
		write(handle, &FileIndex, sizeof(FileIndex));
		close(handle);
	}
	Rep("",0x1f,0);
	return(handle);
}

int ReadFileIndex(char *filename)
{
	int handle;
	char fname[81] = "";
	strcpy(fname,filename);

	sprintf(fname + strlen(filename) -3, "NDX");

	handle = open(fname, O_RDWR | O_BINARY, S_IREAD | S_IWRITE);

	if(handle == -1)
		return(handle);
	else
	{
		lseek(handle, 0L, SEEK_SET);
		read(handle, &FileIndexCount, sizeof(int));
		read(handle, &FileIndex, sizeof(FileIndex));
		close(handle);
	}
	return(handle);
}
