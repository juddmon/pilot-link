/*
 * install-user.c:  Palm Username installer
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pi-source.h"
#include "pi-dlp.h"
#include "pi-header.h"

#include "userland.h"

const char
	*user		= NULL,
	*userid		= NULL;


const struct poptOption options[] = {
	USERLAND_RESERVED_OPTIONS
	{ "user",    'u', POPT_ARG_STRING, &user, 0, "Set the username, use quotes for spaces (see example)", "<username>"},
	{ "id",      'i', POPT_ARG_STRING, &userid, 0, "A 5-digit numeric UserID, required for PalmOS", "<userid>"},
	POPT_TABLEEND
};

poptContext po;

int main(int argc, char *argv[])
{
	int 	sd 		= -1,
		po_err		= -1;

	struct 	PilotUser 	User;

        po = poptGetContext("install-user", argc, (const char **) argv, options, 0);
	poptSetOtherOptionHelp(po, " [-p port] [-u user] [-i id]\n\n"
		"   Assigns your Palm device a Username and unique UserID\n\n"
		"   Example:\n"
		"      -p /dev/pilot -u \"John Q. Public\" -i 12345\n");

        while ((po_err = poptGetNextOpt(po)) >= 0) {
		/* Everything is handled by popt magic */
	}

	if (po_err < -1)
	    plu_badoption(po,po_err);

	sd = plu_connect();
	if (sd < 0)
		goto error;

	if (dlp_ReadUserInfo(sd, &User) < 0)
		goto error_close;

	if (!user && !userid) {
		printf("   Palm user: %s\n", User.username);
		printf("   UserID:    %lu\n\n", User.userID);
		pi_close(sd);
		return 0;
	}

	if (userid)
		User.userID = abs(atoi(userid));

	if (user)
		strncpy(User.username, user, sizeof(User.username) - 1);

	User.lastSyncDate = time(NULL);

	if (dlp_WriteUserInfo(sd, &User) < 0)
		goto error_close;

	if (user)
		printf("   Installed User Name: %s\n", User.username);
	if (userid)
		printf("   Installed User ID: %lu\n", User.userID);
	printf("\n");

	if (dlp_AddSyncLogEntry(sd, "install-user exited normally.\n"
				    "Thank you for using pilot-link.\n") < 0)
		goto error_close;

	if (dlp_EndOfSync(sd, 0) < 0)
		goto error_close;

	if (pi_close(sd) < 0)
		goto error;

	return 0;

error_close:
	pi_close(sd);

error:
	return -1;
}

/* vi: set ts=8 sw=4 sts=4 noexpandtab: cin */
