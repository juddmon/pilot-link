/*
 * address.c:  Translate Pilot address book data formats
 *
 * Copyright (c) 1996, Kenneth Albanowski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pi-source.h"
#include "pi-socket.h"
#include "pi-dlp.h"
#include "pi-address.h"

void
free_Address(struct Address *a)
{
   int i;

   for (i = 0; i < 19; i++)
      if (a->entry[i])
	 free(a->entry[i]);
}

#define hi(x) (((x) >> 4) & 0x0f)
#define lo(x) ((x) & 0x0f)
#define pair(x,y) (((x) << 4) | (y))

int
unpack_Address(struct Address *a, unsigned char *buffer, int len)
{
   unsigned long contents;
   unsigned long v;
   unsigned char *start = buffer;

   if (len < 9)
      return 0;

   /*get_byte(buffer); gapfil */
   a->showPhone = hi(get_byte(buffer + 1));
   a->phoneLabel[4] = lo(get_byte(buffer + 1));
   a->phoneLabel[3] = hi(get_byte(buffer + 2));
   a->phoneLabel[2] = lo(get_byte(buffer + 2));
   a->phoneLabel[1] = hi(get_byte(buffer + 3));
   a->phoneLabel[0] = lo(get_byte(buffer + 3));

   contents = get_long(buffer + 4);

   /*get_byte(buffer+8) offset */

   buffer += 9;
   len -= 9;

   /*if(flag & 0x1) { 
      a->lastname = strdup(buffer);
      buffer += strlen(buffer) + 1;
      } else {
      a->lastname = 0;
      } */

   for (v = 0; v < 19; v++) {
      if (contents & (1 << v)) {
	 if (len < 1)
	    return 0;
	 a->entry[v] = strdup((char *) buffer);
	 buffer += strlen((char *) buffer) + 1;
	 len -= strlen(a->entry[v]) + 1;
      }
      else {
	 a->entry[v] = 0;
      }
   }

   return (buffer - start);
}

int
pack_Address(struct Address *a, unsigned char *record, int len)
{
   unsigned char *start = record;
   unsigned char *buffer;
   unsigned long contents;
   unsigned long v;
   unsigned long phoneflag;
   unsigned char offset;
   int l;

   int destlen = 9;

   for (v = 0; v < 19; v++)
      if (a->entry[v])
	 destlen += strlen(a->entry[v]) + 1;

   if (!record)
      return destlen;
   if (len < destlen)
      return 0;

   buffer = record + 9;

   phoneflag = 0;
   contents = 0;
   offset = 0;

   for (v = 0; v < 19; v++) {
      if (a->entry[v] && strlen(a->entry[v])) {
	 if (v == entryCompany)
	    offset = (unsigned char) (buffer - record) - 8;
	 contents |= (1 << v);
	 l = strlen(a->entry[v]) + 1;
	 memcpy(buffer, a->entry[v], l);
	 buffer += l;
      }
   }

   phoneflag = ((unsigned long) a->phoneLabel[0]) << 0;
   phoneflag |= ((unsigned long) a->phoneLabel[1]) << 4;
   phoneflag |= ((unsigned long) a->phoneLabel[2]) << 8;
   phoneflag |= ((unsigned long) a->phoneLabel[3]) << 12;
   phoneflag |= ((unsigned long) a->phoneLabel[4]) << 16;
   phoneflag |= ((unsigned long) a->showPhone) << 20;

   set_long(record, phoneflag);
   set_long(record + 4, contents);
   set_byte(record + 8, offset);

   return (buffer - start);
}

int
unpack_AddressAppInfo(struct AddressAppInfo *ai, unsigned char *record,
		      int len)
{
   int i;
   unsigned char *start = record;
   unsigned long r;
   int destlen = 4 + 16 * 22 + 2 + 2;

   i = unpack_CategoryAppInfo(&ai->category, record, len);
   if (!record)
      return i + destlen;
   if (!i)
      return i;
   record += i;
   len -= i;

   if (len < destlen)
      return 0;

   r = get_long(record);
   for (i = 0; i < 22; i++)
      ai->labelRenamed[i] = !!(r & (1 << i));

   record += 4;
   memcpy(ai->labels, record, 16 * 22);
   record += 16 * 22;
   ai->country = get_short(record);
   record += 2;
   ai->sortByCompany = get_byte(record);
   record += 2;

   for (i = 3; i < 8; i++)
      strcpy(ai->phoneLabels[i - 3], ai->labels[i]);
   for (i = 19; i < 22; i++)
      strcpy(ai->phoneLabels[i - 19 + 5], ai->labels[i]);

   return (record - start);
}

int
pack_AddressAppInfo(struct AddressAppInfo *ai, unsigned char *record, int len)
{
   int i;
   unsigned char *pos = record;
   unsigned long r;
   int destlen = 4 + 16 * 22 + 2 + 2;

   i = pack_CategoryAppInfo(&ai->category, record, len);
   if (!record)
      return destlen + i;
   if (!i)
      return i;

   pos += i;
   len -= i;

   for (i = 3; i < 8; i++)
      strcpy(ai->phoneLabels[i - 3], ai->labels[i]);
   for (i = 19; i < 22; i++)
      strcpy(ai->phoneLabels[i - 19 + 5], ai->labels[i]);

   memset(pos, 0, destlen);

   r = 0;
   for (i = 0; i < 22; i++)
      if (ai->labelRenamed[i])
	 r |= (1 << i);
   set_long(pos, r);
   pos += 4;

   memcpy(pos, ai->labels, 16 * 22);
   pos += 16 * 22;
   set_short(pos, ai->country);
   pos += 2;
   set_byte(pos, ai->sortByCompany);
   pos += 2;

   for (i = 3; i < 8; i++)
      strcpy(ai->phoneLabels[i - 3], ai->labels[i]);
   for (i = 19; i < 22; i++)
      strcpy(ai->phoneLabels[i - 19 + 5], ai->labels[i]);

   return (pos - record);
}