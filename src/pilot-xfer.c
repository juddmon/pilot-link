/* ex: set tabstop=8 expandtab: */
/* 
 * pilot-xfer.c:  Palm Database transfer utility
 *
 * (c) 1996, 1998, Kenneth Albanowski.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h>
#include <utime.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "pi-source.h"
#include "pi-socket.h"
#include "pi-file.h"
#include "pi-dlp.h"
#include "pi-version.h"
#include "pi-header.h"
#include "pi-tconfig.h"

/* unsigned char typedef byte; */
typedef unsigned char byte;

typedef struct {
  byte data[4];
  char attr;
  byte id[3];
} recInfo_t;

typedef struct {
  char name[32];
  byte attr[2];
  byte version[2];
  byte cdate[4];
  byte mdate[4];
  byte backupdate[4];
  byte modno[4];
  byte appinfo[4];
  byte sortinfo[4];
  char dbType[4];
  char dbCreator[4];
  byte seed[4];
  byte nextRecList[4];
  char nRec[2];
} pdb_t;

/* Declare prototypes */
static void display_help(char *progname);
void print_splash(char *progname);
int pilot_connect(char *port);
void packInt(byte* dest, unsigned long l, int size);

struct option options[] = {
	{"port",        required_argument, NULL, 'p'},
	{"help",        no_argument,       NULL, 'h'},
	{"version",     no_argument,       NULL, 'v'},
	{"backup",      required_argument, NULL, 'b'},
	{"update",      required_argument, NULL, 'u'},
	{"sync",        required_argument, NULL, 's'},
	{"time",        no_argument,       NULL, 't'},
	{"novsf",       no_argument,       NULL, 'S'},
	{"restore",     required_argument, NULL, 'r'},
	{"install",     required_argument, NULL, 'i'},
	{"merge",       required_argument, NULL, 'm'},
	{"fetch",       required_argument, NULL, 'f'},
	{"delete",      required_argument, NULL, 'd'},
	{"exclude",     required_argument, NULL, 'e'},
	{"Purge",       no_argument,       NULL, 'P'},
	{"list",        no_argument,       NULL, 'l'},
	{"Listall",     no_argument,       NULL, 'L'},
	{"archive",     required_argument, NULL, 'a'},
	{"exec",	required_argument, NULL, 'x'},
	{"Flash",       no_argument,       NULL, 'F'},
	{"Osflash",     no_argument,       NULL, 'O'},
	{"Illegal",     no_argument,       NULL, 'I'},
	{NULL,          0,                 NULL, 0}
};

static const char *optstring = "-p:hvb:u:s:tSr:i:m:f:d:e:PlLa:x:FOI";

int	novsf	= 0;

#define pi_mktag(c1,c2,c3,c4) (((c1)<<24)|((c2)<<16)|((c3)<<8)|(c4))

int 	sd 	= 0;
char *	port 	= NULL;

#define MAXEXCLUDE 100
char 	*exclude[MAXEXCLUDE];
int 	numexclude = 0;



/***********************************************************************
 *
 * Function:    MakeExcludeList
 *
 * Summar:      Excludes a list of dbnames from the operation called
 *
 * Parameters:  None
 *
 * Return:      Nothing
 *
 ***********************************************************************/
static void MakeExcludeList(char *efile)
{
	char 	temp[1024];
	FILE 	*f = fopen(efile, "r");

	if (!f) {
		printf("   Unable to open exclude list file '%s'.\n", efile);
		exit(1);
	}

	while ((fgets(temp, sizeof(temp), f)) != NULL) {
		if (temp[strlen(temp) - 1] == '\n')
			temp[strlen(temp) - 1] = '\0';
		printf("Now excluding: %s\n", temp);
		exclude[numexclude++] = strdup(temp);
		if (numexclude == MAXEXCLUDE) {
			printf("Maximum number of exclusions reached [%d]\n", MAXEXCLUDE);
			break;
		}
	}
}

/***********************************************************************
 *
 * Function:    protect_name
 *
 * Summary:     Protects filenames and paths which include 'illegal' 
 *              characters, such as '/' and '=' in them. 
 *
 * Parameters:  None
 *
 * Return:      Nothing
 *
 ***********************************************************************/
static void protect_name(char *d, char *s)
{

	while (*s) {
		switch (*s) {
		case '/':
			*(d++) = '=';
			*(d++) = '2';
			*(d++) = 'F';
			break;
		case '=':
			*(d++) = '=';
			*(d++) = '3';
			*(d++) = 'D';
			break;
		case '\x0A':
			*(d++) = '=';
			*(d++) = '0';
			*(d++) = 'A';
			break;
		case '\x0D':
			*(d++) = '=';
			*(d++) = '0';
			*(d++) = 'D';
			break;
		/* If you feel the need: 
		case ' ': 
			*(d++) = '='; 
			*(d++) = '2'; 
			*(d++) = '0'; 
			break; 
		*/
		default:
			*(d++) = *s;
		}
		++s;
	}
	*d = '\0';
}

/***********************************************************************
 *
 * Function:    Connect
 *
 * Summary:     Establish the connection with the device
 *
 * Parameters:  None
 *
 * Return:      Nothing
 *
 ***********************************************************************/
static void Connect(void)
 {
	if (sd != 0)
		return;
  
	sd = pilot_connect(port);
	if (sd < 0)
		exit(1);
}

/***********************************************************************
 *
 * Function:    VoidSyncFlags
 *
 * Summary:     Get the last PCID and lastSyncTime from the Palm
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/ 
static void VoidSyncFlags(void)
{
	struct 	PilotUser User;

	Connect();

	if (dlp_ReadUserInfo(sd, &User) >= 0) {
		User.lastSyncPC = 0x00000000;	
		/* Hopefully unique constant, to tell any Desktop software
		   that databases have been altered, and that a slow sync is
		   necessary 
		 */
		User.lastSyncDate = User.successfulSyncDate = time(0);
		dlp_WriteUserInfo(sd, &User);
	}
}

/***********************************************************************
 *
 * Function:    RemoveFromList
 *
 * Summary:     Remove the excluded files from the op list
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void RemoveFromList(char *name, char **list, int max)
{
	int 	i;

	for (i = 0; i < max; i++) {
		if (list[i] != NULL && strcmp(name, list[i]) == 0) {
			free(list[i]);
			list[i] = NULL;
		}
	}
}

/***********************************************************************
 *
 * Function:    creator_is_PalmOS
 *
 * Summary:     Skip Palm files which match the internal Palm CreatorID
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static int creator_is_PalmOS(long creator)
{
	union {
		long 	L;
		char 	C[4];
	} buf;
	int 	n;
	static long special_cases[] = {
		pi_mktag('p', 'p', 'p', '_'),
		pi_mktag('u', '8', 'E', 'Z')
	};


	/* Test for special cases -- PalmOS CRIDs outside of lowercase alpha
	   range */
	for (n = 0; n < sizeof(special_cases) / sizeof(long); n++)

		if (creator == special_cases[n])
			return 1;

	/* Test for all lowercase alpha -- range reserved by PalmOS */
	buf.L = creator;

	for (n = 0; n < 4; n++)
		if (buf.C[n] < 'a' || buf.C[n] > 'z')
			return 0;

	return 1;
}

/***********************************************************************
 *
 * Function:    Backup
 *
 * Summary:     Build a filelist and back up the Palm to destination
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Backup(char *dirname, int only_changed, int remove_deleted,
	    int rom, int unsaved, char *archive_dir)
{
	int 	i,
		ofile_len,
		ofile_total;
	
	static int totalsize;

	struct 	dirent *dirent;
	struct 	stat sbuf;

	char 	**orig_files = 0;
	DIR 	*dir;

 	if (access(dirname, F_OK) == -1) {
		mkdir(dirname, 0700);
	} else if (access(dirname, R_OK|W_OK|X_OK) != 0) {
		fprintf(stderr, "\n");
		perror("   ERROR");
		fprintf(stderr, "   Unable to write to %s\n", dirname);
		fprintf(stderr, "   Please check ownership and permissions.\n\n");
		exit(1);
	} 

	Connect();

	if (archive_dir)
		mkdir(archive_dir, 0700);

	/* Read original list of files in the backup dir */
	ofile_total 	= 0;
	ofile_len 	= 0;

	if (only_changed) {
		dir = opendir(dirname);
		while ((dirent = readdir(dir))) {
			char name[256];

			if (dirent->d_name[0] == '.')
				continue;

			if (!orig_files) {
				ofile_len += 256;
				orig_files =
				    malloc(sizeof(char *) * ofile_len);
			} else if (ofile_total >= ofile_len) {
				ofile_len += 256;
				orig_files =
				    realloc(orig_files,
					    sizeof(char *) * ofile_len);
			}

			sprintf(name, "%s/%s", dirname, dirent->d_name);
			orig_files[ofile_total++] = strdup(name);
		}
		closedir(dir);
	}

	i = 0;
	for (;;) {
		struct 	DBInfo info;
		struct 	pi_file *f;
		struct 	utimbuf times;

		/* This is supposed to be fixed on NeXT's Openstep 4.2.
		   Anyone care to test this?
		struct utimbuf {
			long actime;
			long modtime;
		}; 
		*/

		int 	skip = 0,
			x;
		
		char 	name[256];
		struct 	stat statb;

		if (dlp_ReadDBList(sd, 0, (rom ? 0x40 : 0x80), i, &info) <
		    0)
			break;
		i = info.index + 1;

		if (dlp_OpenConduit(sd) < 0) {
			printf("\n");
			printf("Exiting on cancel, all data was not backed " 
			       "up.\nStopped before backing up '%s'.\n\n",
				info.name);
			fflush(stdout);
			exit(1);
		}

		strncpy(name, dirname, sizeof(name));
		strcat(name, "/");
		protect_name(name + strlen(name), info.name);

		if (info.flags & dlpDBFlagResource)
			strcat(name, ".prc");
		else if (!(info.flags & dlpDBFlagClipping))
			strcat(name, ".pdb");

		for (x = 0; x < numexclude; x++) {
			if (strcmp(exclude[x], info.name) == 0) {
				fprintf(stdout, "== Excluding '%s'...\n", name);
				RemoveFromList(name, orig_files, ofile_total);
				skip = 1;
			}
		}

		if (skip == 1)
			continue;

		if (rom == 1 && creator_is_PalmOS(info.creator)) {
			printf("== OS file, skipping '%s'.\n", info.name);
			continue;
		} else if (rom == 2 && !creator_is_PalmOS(info.creator)) {
			printf("== Non-OS file, skipping '%s'\n", info.name);
			continue;
		}

		if (!unsaved
		    && strcmp(info.name, "Unsaved Preferences") == 0) {
			printf("== Skipping '%s'\n", info.name);
			continue;
		}

		if (only_changed) {
			if (stat(name, &statb) == 0) {
				if (info.modifyDate == statb.st_mtime) {
					printf("No change, skipping '%s'.\n", info.name);
					RemoveFromList(name, orig_files, ofile_total);
					continue;
				}
			}
		}

		if (only_changed) {
			fprintf(stdout, "Synchronizing %-35s\n", name); 
		} else {
			/* printf("Backing up %-35s", name); */
			fflush(stdout);
		}

		/* Ensure that DB-open and DB-ReadOnly flags are not kept */
		info.flags &= ~(dlpDBFlagOpen | dlpDBFlagReadOnly);

		f = pi_file_create(name, &info);
		if (f == 0) {
			printf("Failed, unable to create file\n");
			break;
		}

		if (pi_file_retrieve(f, sd, 0) < 0) {
			printf("Failed, unable to back up database\n");
		} else if (stat(name, &sbuf) == 0) {
			/* 
			printf("\n\t(%7ld bytes, %3d kb total)\n\n", 
				sbuf.st_size, totalsize/1024);
			*/
			totalsize += sbuf.st_size;
		} else {
			printf("\n");
		}

		pi_file_close(f);

		/* Note: This is no guarantee that the times on the host
		   system actually match the GMT times on the Palm. We only
		   check to see whether they are the same or different, and
		   do not treat them as real times. */

		times.actime 	= info.createDate;
		times.modtime 	= info.modifyDate;
		utime(name, &times);

		RemoveFromList(name, orig_files, ofile_total);
	}
		
	if (orig_files) {
		int 	dirname_len = strlen(dirname);
		char 	newname[256];

		if (remove_deleted && dlp_OpenConduit(sd) < 0) {
			/* If the connection has gone down here, there is
			   probably a communication error. */
			printf("Exiting on error, stopped before removing files.\n");
			exit(1);
		}

		for (i = 0; i < ofile_total; i++)
			if (orig_files[i] != NULL) {
				if (remove_deleted) {

					if (archive_dir) {
						printf("Archiving '%s'.\n", orig_files[i]);
						sprintf(newname, "%s/%s", archive_dir, &orig_files[i] [dirname_len + 1]);
						if (rename (orig_files[i], newname) != 0) {
							printf("rename(%s, %s) ", orig_files [i], newname);
							perror("failed");
						}
					} else {
						printf("Removing '%s'.\n", orig_files[i]); 
						unlink(orig_files[i]);
					}
				}
				free(orig_files[i]);
			}
		if (orig_files)
			free(orig_files);
	}

	printf("%s backup complete.\n",
	       (rom == 2 ? "\nOS" : (rom == 1 ? "\nFlash" : "\nRAM")));

	dlp_AddSyncLogEntry(sd, "Backup successful.\nThank you for using pilot-link.\n");
}

/***********************************************************************
 *
 * Function:    Fetch
 *
 * Summary:     Grab a file from the Palm, write to disk
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Fetch(char *dbname)
{
	struct 	DBInfo info;
	char 	name[256];
	struct 	pi_file *f;
	char    *tmpPtr = dbname;

	/* FIXME: Need to handle users who pass no extension on the 
	          target filename, such as: '-f AddressDB' instead of 
		  using '-f AddressDB.pdb' */

	if (access(dbname, F_OK) == 0 && access(dbname, R_OK|W_OK) != 0) {
		fprintf(stderr, "\n");
		fprintf(stderr, "   Unable to write to %s, check "
			"ownership and permissions.\n\n", dbname);
		exit(1);
	}

	/* removes .pxx from the end of supplied dbname for the case where
	   user wants ThisDB and specifies it as ThisDB.prc, etc */
	while (*tmpPtr != '\0' && tmpPtr <= dbname + 265) tmpPtr++;
	if (*(tmpPtr - 4) == '.') {
		if (*(tmpPtr - 8) != '.' || /* handle special case .pdf.prc */ 
			(*(tmpPtr - 7) != 'p' && *(tmpPtr - 7) != 'P')) {
			if (*(tmpPtr - 3) == 'p' || *(tmpPtr - 3) == 'P') { 
				*(tmpPtr - 4) = '\0';
			}
		}
	}

	Connect();

	if (dlp_FindDBInfo(sd, 0, 0, dbname, 0, 0, &info) < 0) {
		printf("Unable to locate database '%s', fetch skipped.\n",
		       dbname);
		return;
	}

	protect_name(name, dbname);

	/* Judd - Graffiti hack 
	   Graffiti ShortCuts with a space on the end or not is really
	   supposed to be the same file, so we will treat it as such to
	   avoid confusion, remove the space.
	 */
	if (strcmp(name, "Graffiti ShortCuts ") == 0) {
		strncpy(name, "Graffiti ShortCuts", sizeof(name));
	}

	if (info.flags & dlpDBFlagResource)
		strcat(name, ".prc");
	else if (!(info.flags & dlpDBFlagClipping))
		strcat(name, ".pdb");


	printf("Fetching '%s'... ", name);
	fflush(stdout);

	info.flags &= 0x2fd;

	f = pi_file_create(name, &info);
	if (f == 0) {
		printf("Failed, unable to create file\n");
		return;
	}

	if (pi_file_retrieve(f, sd, 0) < 0)
		printf("Failed, unable to fetch database\n");
	else
		printf("OK\n");
	pi_file_close(f);

	printf("Fetch complete.\n");
}

/***********************************************************************
 *
 * Function:    Delete
 *
 * Summary:     Delete a database from the Palm
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Delete(char *dbname)
{
	struct 	DBInfo info;

	Connect();

	dlp_FindDBInfo(sd, 0, 0, dbname, 0, 0, &info);

	printf("Deleting '%s'... ", dbname);
	if (dlp_DeleteDB(sd, 0, dbname) >= 0) {
		if (info.type == pi_mktag('b', 'o', 'o', 't')) {
			printf(" (rebooting afterwards) ");
		}
		printf("OK\n");
	} else {
		printf("Failed, unable to delete database\n");
	}
	fflush(stdout);

	printf("Delete complete.\n");
}

struct db {
	int 	flags,
		maxblock;

	char 	name[256];

	unsigned long creator, type;
};

static int compare(struct db *d1, struct db *d2)
{
	/* types of 'appl' sort later then other types */
	if (d1->creator == d2->creator)
		if (d1->type != d2->type) {
			if (d1->type == pi_mktag('a', 'p', 'p', 'l'))
				return 1;
			if (d2->type == pi_mktag('a', 'p', 'p', 'l'))
				return -1;
		}
	return d1->maxblock < d2->maxblock;
}

/***********************************************************************
 *
 * Function:    Restore
 *
 * Summary:     Send files to the Palm from disk, restoring Palm
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Restore(char *dirname)
{
	int 	dbcount 	= 0,
		i,
		j,
		max,
		size,
		save_errno 	= errno;
	DIR 	*dir;
	struct 	dirent *dirent;
	struct 	DBInfo info;
	struct 	db **db 	= NULL;
	struct 	pi_file *f;
	struct  stat sbuf;

        struct  CardInfo Card;

        Card.card = -1;
        Card.more = 1;
		
	if ((dir = opendir(dirname)) == NULL) {
		fprintf(stderr, "\n");
		perror("   ERROR");
		fprintf(stderr, "   opendir() failed. Cannot open directory %s.\n", dirname);
		fprintf(stderr, "   Does the directory exist?\n\n");
		errno = save_errno;
		exit(1);
	}

	/* Find out how many directory entries exist, so that we can
	   allocate the buffer.  We avoid scandir() for maximum portability.

	   The count is a bit conservative, as it includes . and .. entries.
	 */
	while (readdir(dir))
		dbcount++;

	db = (struct db **) calloc(dbcount, sizeof(struct db *));

	if (!db) {
		printf("Unable to allocate memory for directory entry table\n");
		exit(1);
	}

	dbcount = 0;
	rewinddir(dir);

	while ((dirent = readdir(dir)) != NULL) {

		if (dirent->d_name[0] == '.')
			continue;

		db[dbcount] = (struct db *) malloc(sizeof(struct db));

		sprintf(db[dbcount]->name, "%s/%s", dirname,
			dirent->d_name);

		f = pi_file_open(db[dbcount]->name);
		if (f == 0) {
			printf("Unable to open '%s'!\n",
			       db[dbcount]->name);
			break;
		}

		pi_file_get_info(f, &info);

		db[dbcount]->creator 	= info.creator;
		db[dbcount]->type 	= info.type;
		db[dbcount]->flags 	= info.flags;
		db[dbcount]->maxblock 	= 0;

		pi_file_get_entries(f, &max);

		for (i = 0; i < max; i++) {
			if (info.flags & dlpDBFlagResource)
				pi_file_read_resource(f, i, 0, &size, 0,
						      0);
			else
				pi_file_read_record(f, i, 0, &size, 0, 0,
						    0);

			if (size > db[dbcount]->maxblock)
				db[dbcount]->maxblock = size;
		}

		pi_file_close(f);
		dbcount++;
	}

	closedir(dir);

	for (i = 0; i < dbcount; i++)
		for (j = i + 1; j < dbcount; j++)
			if (compare(db[i], db[j]) > 0) {
				struct db *temp = db[i];

				db[i] = db[j];
				db[j] = temp;
			}

	Connect();

	for (i = 0; i < dbcount; i++) {

		f = pi_file_open(db[i]->name);
		if (f == 0) {
			printf("Unable to open '%s'!\n", db[i]->name);
			break;
		}
		printf("Restoring %s... ", db[i]->name);
		fflush(stdout);

	        stat(db[i]->name, &sbuf);
 
	        while (Card.more) {  
        	        if (dlp_ReadStorageInfo(sd, Card.card + 1, &Card) < 0)
	                        break;
	        }

	        if (sbuf.st_size > Card.ramFree) {
        	        fprintf(stderr, "\n\n");
                	fprintf(stderr, "   Insufficient space to install this file on your Palm.\n");
	                fprintf(stderr, "   We needed %ld and only had %lu available..\n\n",
        	                sbuf.st_size, Card.ramFree);
	                return;
        	}

		if (pi_file_install(f, sd, 0) < 0)
			printf("failed.\n");
		else
			printf("OK\n");
		pi_file_close(f);
	}

	if (!novsf)
		VoidSyncFlags();

	for (i = 0; i < dbcount; i++) {
		free(db[i]);
	}
	free(db);

	printf("Restore done\n");
}

/***********************************************************************
 *
 * Function:    Install
 *
 * Summary:     Push file(s) to the Palm from filelist
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Install(char *filename)
{
	static int totalsize;

	struct 	pi_file *f;
	struct 	stat sbuf;

        struct  CardInfo Card;

        Card.card = -1;
        Card.more = 1;

	Connect();

	f = pi_file_open(filename);

	if (f == 0) {
		printf("Unable to open '%s'!\n", filename);
		return;
	}

	if (dlp_OpenConduit(sd) < 0) {
                fprintf(stderr, "\nExiting on cancel, some files were not installed\n\n");
		exit(1);
	}

	fprintf(stderr, "Installing %-35s", filename);
	fflush(stdout);

	stat(filename, &sbuf);

        while (Card.more) {
                if (dlp_ReadStorageInfo(sd, Card.card + 1, &Card) < 0)
                        break;
	}

	if (sbuf.st_size > Card.ramFree) {
		fprintf(stderr, "\n\n");
		fprintf(stderr, "   Insufficient space to install this file on your Palm.\n");
		fprintf(stderr, "   We needed %ld and only had %lu available..\n\n", 
			sbuf.st_size, Card.ramFree);
		return;
	}

	if (pi_file_install(f, sd, 0) < 0) {
		fprintf(stderr, "failed.\n");

	} else if (stat(filename, &sbuf) == 0) {
		/*
		printf("\n\t(%7ld bytes, %3d kb total)\n\n",
			sbuf.st_size, totalsize/1024);
		*/
		totalsize += sbuf.st_size;
	}
	
	pi_file_close(f);

	if (!novsf)
		VoidSyncFlags();
}

/***********************************************************************
 *
 * Function:    Merge
 *
 * Summary:     Adds the records in <file> into the corresponding
 *		Palm database
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Merge(char *filename)
{
	struct pi_file *f;

	Connect();

	f = pi_file_open(filename);
	if (f == 0) {
		printf("Unable to open '%s'!\n", filename);
		return;
	}


	printf("Merging %s... ", filename);
	fflush(stdout);
	if (pi_file_merge(f, sd, 0) < 0)
		printf("failed.\n");
	else
		printf("OK\n");
	pi_file_close(f);

	if (!novsf)
		VoidSyncFlags();

	printf("Merge done\n");
}

/***********************************************************************
 *
 * Function:    List
 *
 * Summary:     List the databases found on the Palm device
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void List(int rom)
{
	int 	i,
		dbcount 	= 0; 
	struct 	DBInfo info;

	Connect();

	if (rom)
		printf("Reading list of databases in RAM and ROM...\n");
	else
		printf("Reading list of databases in RAM...\n");

	i = 0;

	for (;;) {
		if (dlp_ReadDBList
		    (sd, 0, (rom ? 0x80 | 0x40 : 0x80), i, &info) < 0)
			break;
		dbcount++;
		i = info.index + 1;
		printf("%s\n", info.name);
	}
	printf("\nList complete. %d files found.\n", dbcount);

}

/***********************************************************************
 *
 * Function:    Purge
 *
 * Summary:     Purge any deleted data that hasn't been cleaned up
 *              by a sync
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Purge(void)
{
	int 	i,
		h;
	struct 	DBInfo info;

	Connect();

	printf("Reading list of databases to purge...\n");

	i = 0;
	for (;;) {
		if (dlp_ReadDBList(sd, 0, 0x80, i, &info) < 0)
			break;
		i = info.index + 1;

		if (info.flags & 1)
			continue;	/* skip resource databases */

		printf("Purging deleted records from '%s'... ", info.name);

		h = 0;
		if ((dlp_OpenDB(sd, 0, 0x40 | 0x80, info.name, &h) >= 0) &&
		    (dlp_CleanUpDatabase(sd, h) >= 0) &&
		    (dlp_ResetSyncFlags(sd, h) >= 0)) {
			printf("OK\n");
		} else {
			printf("Failed\n");
		}

		if (h != 0)
			dlp_CloseDB(sd, h);

	}

	if (!novsf)
		VoidSyncFlags();
	
	printf("Purge complete.\n");
}


void packInt(byte* dest, unsigned long l, int size) {
	int i;
	for( i=size; i-->0; ) {
		dest[i] = l & 0x000000ff;
		l >>= 8;
	}
}

/***********************************************************************
 *
 * Function:    Time
 *
 * Summary:     Sync time
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void Time(void)
{
	time_t  t1,t2;
	char    text[24];
	struct  SysInfo s;

	Connect();

	dlp_ReadSysInfo(sd, &s);

	if ((s.romVersion) == 0x03303000) {
		dlp_AddSyncLogEntry(sd, "Syncing time will cause the device to\nhard-reset on PalmOS version 3.3!\n");
	} else {
		dlp_GetSysDateTime(sd, &t2);
		t1 = time(NULL);
		dlp_SetSysDateTime(sd, t1);
		sprintf(text,"Time sync successful.\n");
		dlp_AddSyncLogEntry(sd, text);
	}
}

/***********************************************************************
 *
 * Function:    display_help
 *
 * Summary:     Print out the --help options and arguments
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void display_help(char *progname)
{
	printf("   Sync, backup, install, delete and more from your Palm device.\n");
	printf("   This is the swiss-army-knife of the entire pilot-link suite.\n\n");
	printf("   Usage: %s [-p port] [ -F|-O -I -q|-c ] command(s)\n", progname);
	printf("   Options:\n");
	printf("     -p, --port <port>       Use device file <port> to communicate with Palm\n");
	printf("     -h, --help              Display help information for %s\n", progname);
	printf("     -v, --version           Display %s version information\n", progname);
	printf("     -b, --backup <dir>      Back up your Palm to <dir>\n");
	printf("     -u, --update <dir>      Update <dir> with newer Palm data\n");
	printf("     -s, --sync <dir>        Same as -u above, but removes local files if\n");
	printf("                             data is removed from your Palm\n");
	printf("     -S, --novsf             Do NOT reset the SyncFlags when sync completes\n");
	printf("     -r, --restore <dir>     Restore backupdir to your Palm\n");
	printf("     -i, --install [db] ..   Install local prc, pdb, pqa files to your Palm\n");
	printf("     -m, --merge [file] ..   Adds the records in <file> into the corresponding\n");
	printf("                             Palm database\n");
	printf("     -f, --fetch [db]        Retrieve [db] from your Palm\n");
	printf("     -d, --delete [db]       Delete (permanently) [db] from your Palm\n");
	printf("     -e, --exclude <file>    Exclude databases listed in <file> from being included\n");
	printf("                             by -b, -s, or -u (See pilot-xfer(1) for more detail)\n");
	printf("     -P, --Purge             Purge any deleted data that hasn't been cleaned up\n");
	printf("                             by a sync\n");
	printf("     -l, --list              List all application and 3rd party Palm data/apps\n");
	printf("     -L, --List              List all data, internal and external on the Palm\n");
	printf("     -a, --archive           Modifies -s to archive deleted files in specified\n");
	printf("                             directory.\n");
	printf("     -x, --exec              Execute a shell command for intermediate processing\n");
	printf("     -t, --time              Sync the time on the Palm with the desktop time\n");
	printf("     -F, --Flash             Modifies -b, -u, and -s, to back up non-OS db's\n");
	printf("                             from Flash ROM\n");
	printf("     -O, --Osflash           Modifies -b, -u, and -s, to back up OS db 's from\n");
	printf("                             Flash ROM\n");
	printf("     -I, --Illegal           Modifies -b, -u, and -s, to back up the 'illegal'\n");
	printf("                             database Unsaved Preferences.prc (normally skipped,\n");
	printf("                             per Palm's recommendation)\n\n");
	printf("   The serial port used to connect to may be specified by the $PILOTPORT\n");
	printf("   environment variable in your shell instead of the command line.  If it is\n");
	printf("   not specified anywhere, it will default to /dev/pilot.\n\n");
	printf("   Additionally, the baud rate to connect with may be specified by the\n");
	printf("   $PILOTRATE environment variable.If not specified, it will default to\n");
	printf("   a safe rate of 9600.\n\n");
	printf("   Please use caution setting $PILOTRATE to higher values, as several types\n");
	printf("   of workstations have problems with higher baud rates.  Always consult the\n");
	printf("   man page(s) for additional usage of these options as well as details on\n");
	printf("   the results of combining other parameters together.\n\n");

	return;
}

int main(int argc, char *argv[])
{
	int 	c,		/* switch */
	 	do_rom 		= 0,
		do_unsaved 	= 0,
		timespent 	= 0,
		last_c          = 0;

        time_t 	start,end;
	
	char 	*archive_dir 	= NULL,
		*progname 	= argv[0];

        start = time(NULL);

	while ((c = getopt_long(argc, argv, optstring, options, NULL)) != -1) {
		switch (c) {

		case 'h':
			display_help(progname);
			return 0;
		case 'v':
			print_splash(progname);
			return 0;
		case 'p':
			port = optarg;
			break;

		/* This means a field is unknown, could be multiple arg */
		case 1:
			if (last_c=='i') {
				Install(optarg);
			}
			/* else { Unknown param, let it go by quietly } */
			break;

		case 'b':
			Backup(optarg, 0, 0, do_rom, do_unsaved, archive_dir);
			break;
		case 'u':
			Backup(optarg, 1, 0, do_rom, do_unsaved, archive_dir);
			break;
		case 's':
			Backup(optarg, 1, 1, do_rom, do_unsaved, archive_dir);
			break;
		case 't':
			Time();
			break;
		case 'r':
			Restore(optarg);
			break;
		case 'i':
			Install(optarg);
			break;
		case 'm':
			Merge(optarg);
			break;
		case 'f':
			Fetch(optarg);
			break;
		case 'd':
			Delete(optarg);
			break;
		case 'e':
			MakeExcludeList(optarg);
			break;
		case 'P':
			Purge();
			break;
		case 'l':
			List(0);
			break;
		case 'L':
			List(1);
			break;
		case 'a':
			archive_dir = optarg;
			break;
		case 'x':
			if (system(optarg)) {
				fprintf(stderr,"system() failed, aborting.\n");
				return -1;
			}
			break;
		case 'F':
			do_rom = !do_rom;
			break;
		case 'O':
			do_rom = 2;
			break;
		case 'I':
			do_unsaved = 1;
			break;
		case 'S':
			novsf = 1;
			break;
		default:
			display_help(progname);
			return 0;
		}

		if (c > 1) {
			last_c=c;
		}
	}

	if (argc < 2) {
		printf("ERROR: Insufficient number of arguments\n\n");
		puts("Hit any key to continue..");
		for( c = ' ' ; c != '\n' && c != EOF ; c = getchar() );
		display_help(progname);
	} else {
		end=time(NULL);
		timespent = (end-start);
		printf("Time elapsed: %d:%02d:%02d\n",timespent/3600, (timespent/60)%60, timespent%60);
	}
	pi_close(sd);
	return 0;

}
