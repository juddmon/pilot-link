/* pilot-addresses.c:  Pilot address transfer utility
 *
 * This is free software, licensed under the GNU Public License V2.
 * See the file COPYING for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "pi-source.h"
#include "pi-socket.h"
#include "pi-dlp.h"
#include "pi-address.h"

/* Yet more hair: reorganize fields to match visible appearence */

int realentry[19] =   { 0, 1, 13, 2, 
                        3, 4, 5, 6, 7, 
                        8, 9, 10, 11, 12,
                        14, 15, 16, 17,
                        18 };

int read_field(char * dest, FILE * in) {
  int c;
  
  do { /* Absorb whitespace */
     c = getc(in);
  } while ((c != EOF) && ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r')));
  
  if (c == '"') {
    c = getc(in);
    
    while (c != EOF) {
      if (c == '"') {
        c = getc(in);
        if (c != '"')
          break;
      }
      *dest++ = c;
      c = getc(in);
    }
  } else {
    while (c != EOF) {
      if (c == ',') {
          break;
      }
      *dest++ = c;
      c = getc(in);
    }
  }
  *dest++ = '\0';
  
  while ( (c != EOF)  && ( (c==' ')||(c=='\t') ) ) /* Absorb whitespace */
    c = getc(in);
    
  if (c == ',')
    return 1;
  else if (c == ';')
    return 2;
  else if (c == EOF)
    return -1; /* No more */
  else
    return 0;
}

int write_field(FILE * out, char * source, int more) {

  putc('"', out);
  while (*source) {
    putc(*source, out);
    if (*source == '"')
      putc(*source, out);
    source++;
  }
  putc('"',out);
  
  if (more==1)
    putc(',',out);
  else if (more==2)
    putc(';',out);
  else if (more==0)
    putc('\n',out);
  
  return 0;
}

int match_category(char * buf, struct AddressAppInfo * aai)
{
  int i;
  for (i=0;i<16;i++)
    if (strcasecmp(buf, aai->CategoryName[i])==0)
      return i;
  return atoi(buf); /* 0 is default */
}

int match_phone(char * buf, struct AddressAppInfo * aai)
{
  int i;
  for (i=0;i<8;i++)
    if (strcasecmp(buf, aai->phonelabels[i])==0)
      return i;
  return atoi(buf); /* 0 is default */
}

int defaultcategory = 0;

int read_file(FILE * in, int sd, int db, struct AddressAppInfo * aai)
{
  struct Address a;
  int i,l;
  char buf[0xffff];
  int category,attribute;

  do {
    i = read_field(buf,in);

    memset(&a,0,sizeof(a));
    a.whichphone = 0;
    
    if (i==2) {
      category = match_category(buf, aai);
      i = read_field(buf, in);
      if (i==2) {
        a.whichphone = match_phone(buf, aai);
        i = read_field(buf, in);
      }
    } else
      category = defaultcategory;
    
    if (i<0)
      break;

    attribute = 0;
    
    for (l=0; (i>=0) && (l<19) ;l++) {
      int l2 = realentry[l];

      if ( (l2>=3) && (l2<=7) ) {
        if (i != 2)
          a.phonelabel[l2-3] = l2-3;
        else {
          a.phonelabel[l2-3] = match_phone(buf, aai);
          i = read_field(buf, in);
        }
      }
      
      a.entry[l2] = strdup(buf);
      
      if (i==0)
        break;
        
      i = read_field(buf, in);
    }
    
    attribute = (atoi(buf) ? dlpRecAttrSecret : 0);
    
    while (i > 0) { /* Too many fields in record */
      i = read_field(buf, in);
    }

#ifdef DEBUG    
    printf("Category %s (%d)\n", aai->CategoryName[category], category);
    for (l=0;l<19;l++) {
      if ((l>=3) && (l<=7))
        printf(" %s (%d): %s\n", aai->phonelabels[a.phonelabel[l-3]], a.phonelabel[l-3], a.entry[l]);
      else
        printf(" %s: %s\n", aai->labels[l], a.entry[l]);
    }
#endif

    pack_Address(&a, buf, &l);
#ifdef DEBUG
    dumpdata(buf,l);
#endif    
    dlp_WriteRecord(sd, db, attribute, 0, category, buf, l, 0);
    
  } while(i >= 0);
  
  return 0;
}

int augment = 0;

int write_file(FILE * out, int sd, int db, struct AddressAppInfo * aai)
{
  struct Address a;
  int i,j,l;
  char buf[0xffff];
  int category,attribute;
  
  for(i=0;
      (j=dlp_ReadRecordByIndex(sd, db, i, buf, 0, &l, &attribute, &category))>=0;
      i++)
      {
    if (attribute & dlpRecAttrDeleted)
      continue;
    unpack_Address(&a, buf, l);

/* Simplified system */
#if 0    
    write_field(out, "Category", 1);
    write_field(out, aai->CategoryName[category],-1);
    
    for (j=0;j<19;j++) {
      if (a.entry[j]) {
        putc(',',out);
        putc('\n',out);
        if ( (j>=4) && (j<=8) )
          write_field(out, aai->phonelabels[a.phonelabel[j-4]], 1);
        else
          write_field(out, aai->labels[j], 1);
        write_field(out, a.entry[j], -1);
      }
    }
    putc('\n',out);
#endif
    
/* Complex system */
#if 1
    if (augment && (category || a.whichphone)) {
      write_field(out,aai->CategoryName[category],2);
      if (a.whichphone) {
        write_field(out,aai->phonelabels[a.whichphone],2);
      }
    }

    for (j=0;j<19;j++) {
      if (augment && (j >= 4) && (j<=8))
        if (a.phonelabel[j-4] != j-4)
          write_field(out, aai->phonelabels[a.phonelabel[j-4]], 2);
      if (a.entry[realentry[j]])
        write_field(out, a.entry[realentry[j]], 1);
      else
        write_field(out, "", 1);
    }
    
    sprintf(buf, "%d", (attribute & dlpRecAttrSecret) ? 1 : 0);
    write_field(out, buf, 0);
    
#endif
  }
  
  return 0;
}

char * progname;

void Help(void)
{
  fprintf(stderr,"usage:%s %s [-c category] [-a] -r|-w file\n",progname,TTYPrompt);
  exit(2);
}

int main(int argc, char *argv[])
{
  struct pi_sockaddr addr;
  int db;
  int sd;
  int l;
  struct PilotUser U;
  int ret;
  char buf[0xffff];
  struct AddressAppInfo aai;
  char * defaultcategoryname = 0;
  int mode = 0;
  int c;
#ifdef sun
  extern char* optarg;
  extern int optind;
#endif

  progname = argv[0];

  if (argc < 4)
    Help();

  optind = 2;
  while (((c = getopt(argc, argv, "c:arw")) != -1) && (mode == 0)) {
    switch (c) {
    case 'a':
      augment = 1;
    case 'c':
      defaultcategoryname = optarg;
      break;
    case 'r':
      mode = 1;
      break;
    case 'w':
      mode = 2;
      break;
    case 'h': case '?':
      Help();
    }
  }
  
  if (mode == 0)
    Help();
  
  if (!(sd = pi_socket(PI_AF_SLP, PI_SOCK_STREAM, PI_PF_PADP))) {
    perror("pi_socket");
    exit(1);
  }
    
  addr.sa_family = PI_AF_SLP;
  addr.port = 3;
  strcpy(addr.device,argv[1]);
  
  ret = pi_bind(sd, &addr, sizeof(addr));
  if(ret == -1) {
    perror("pi_bind");
    exit(1);
  }

  ret = pi_listen(sd,1);
  if(ret == -1) {
    perror("pi_listen");
    exit(1);
  }

  sd = pi_accept(sd, 0, 0);
  if(sd == -1) {
    perror("pi_accept");
    exit(1);
  }

  /* Ask the pilot who it is. */
  dlp_ReadUserInfo(sd,&U);
  
  /* Tell user (via Pilot) that we are starting things up */
  dlp_OpenConduit(sd);
  
  /* Open the Memo Pad's database, store access handle in db */
  if(dlp_OpenDB(sd, 0, 0x80|0x40, "AddressDB", &db) < 0) {
    puts("Unable to open AddressDB");
    dlp_AddSyncLogEntry(sd, "Unable to open AddressDB.\n");
    exit(1);
  }
  
  l = dlp_ReadAppBlock(sd, db, 0, buf, 0xffff);
  unpack_AddressAppInfo(&aai, buf, l);

  if (defaultcategoryname)
    defaultcategory = match_category(defaultcategoryname,&aai);
  else
    defaultcategory = 0; /* Unfiled */

  if (mode == 2) { /* Write */
    FILE * f = fopen(argv[optind],"w");
    write_file(f, sd, db, &aai);
    fclose(f);
  } else if (mode == 1) {
    FILE * f;
    while(optind < argc) {
      f = fopen(argv[optind],"r");
      read_file(f, sd, db, &aai);
      fclose(f);
      optind++;
    }
  }
  
  /* Close the database */
  dlp_CloseDB(sd, db);

  /* Tell the user who it is, with a different PC id. */
  U.lastSyncPC = 0xDEADBEEF;
  U.succSyncDate = time(NULL);
  U.lastSyncDate = U.succSyncDate;
  dlp_WriteUserInfo(sd,&U);

  dlp_AddSyncLogEntry(sd, "Wrote addresses to Pilot.\n");
  
  /* All of the following code is now unnecessary, but harmless */
  
  dlp_EndOfSync(sd,0);
  pi_close(sd);
  exit(0);
}
