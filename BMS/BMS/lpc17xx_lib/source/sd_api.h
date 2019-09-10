/**
 * @file		sd_api.h
 * @brief		This file contains the macros for buffer size and file mode and error codes
 *              It also includes file operation related API declarations. 
 *
 * @copyright	NGX Technologies Ltd
 */

#ifndef _SD_API_H_
#define _SD_API_H_

/* Includes */
#include "lpc_types.h"
#include "types.h"
#include "ls.h"

/* Macros */
/** Flag to enable display of list of files in a directory on LCD */
/* #define DISP_LIST */

/* Buffer size used for file operations */
#define MAX_BUFFER 512 /* Note :Need not be power of 2 */

/** Max number of files to be displayed to user */
#define MAX_DISP_FILE_NUM    7

 /* File opening modes */
#define RDNLY				'r'
#define WRITE				'w'
#define APPEND				'a'

/*  Error Codes */
#define SD_NO_ERR 		 	0
#define FILE_NO_ERR		 	0

#define SDCARD_NOT_FOUND 	-1
#define FILE_ERROR			-2
#define FAT_NOT_SUPPORTED 	-3

/* Function declarations */

/** Copy the content of src file to dst file */
int copy_file(char * src_file_name,char * dst_file_name);
	
/** Initialise SD card						*/
esint32 sdCardInit(void);

/** Lists the filenames in the specified directory */
void sdListDir(eint8 * dirName);

/* Creates the specified directory.Note: provide the full path. */
esint8 sdMakeDir(eint8 * dirName);

/*Removes the file from the file system.Note : specify the full path*/
esint8 sdRemoveFile(euint8 * FileName);

/** Open a file in given mode				*/									
esint8 sdOpenFile(eint8 *filename, eint8 mode);

/** Close file								*/				
int sdCloseFile(eint8 mode);

/** Read a file and store in buffer			*/										
euint32 sdReadFile(euint32 size,euint8 *buf);

/** Write the content of buffer to file     */			
euint32 sdWriteFile(euint32 size,euint8 *buf);
			
/** Un-mounting efs							*/
void sdFsUnmount(void);	
								
/** Copies contents of source file to destination file */
int sdCopyFile(char * src_file_name,char * dst_file_name);

#endif /* _SD_API_H_ */
