/*
 * connect.c:  Palm Serial, USB, IR connection routines
 *
 * Copyright (c) 2001, David A. Desrosiers
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <sys/stat.h>

#include "pi-socket.h"
#include "pi-dlp.h"
#include "pi-header.h"

/***********************************************************************
 *
 * Function:    pilot_connect
 *
 * Summary:     Connect to a Palm device.
 *
 * Parameters:  port. Communications port through which the Palm device is
 *              connected.  If this is NULL, pilot_connect() will attempt to
 *              discover the correct port, and fall back to /dev/pilot.
 *
 * Returns:     Socket descriptor of type 'client_sd', if successful.
 *		Returns 1, if the connection can not be established.
 *
 *  If 'port' is NULL, the PILOTPORT environment variable is checked.
 *
 *  If neither of them are set, the port defaults to /dev/pilot.
 *
 *  A socket is created. A message is displayed, reminding the user to press
 *  the HotSync button. pilot_connect() waits for communication to be
 *  established... potentially forever. Once communication is established,
 *  the socket descriptor is returned.
 *
 ***********************************************************************/
int
pilot_connect(const char *port)
{
	int 	sd	= -1, 	/* Socket, formerly parent/client_socket */
		result, 
		count	= 0;

	struct 	pi_sockaddr addr;
	struct 	stat attr;
	struct  SysInfo sys_info;
	char 	*defport = "/dev/pilot";

	if (port == NULL && (port = getenv("PILOTPORT")) == NULL) {
		fprintf(stderr, "   No $PILOTPORT specified and no -p "
			"<port> given.\n"
			"   Defaulting to '%s'\n", defport);
		port = defport;
	}

	fprintf(stderr, "\n");
	if ((sd = pi_socket(PI_AF_PILOT,
			PI_SOCK_STREAM, PI_PF_DLP)) < 0) {
		fprintf(stderr, "\n   Unable to create socket '%s'\n", port);
		return -1;
	}

	begin:
	addr.pi_family = PI_AF_PILOT;
	strncpy(addr.pi_device, port, sizeof(addr.pi_device));
	result = pi_bind(sd, (struct sockaddr *) &addr, sizeof(addr));

	if (result < 0) {
		int 	save_errno = errno;
		char	realport[50];
		
		realpath(port, realport);
		errno = save_errno;

		if (errno == ENOENT) {
			fprintf(stderr,
					" The device %s does not exist..\n",
					port);
			fprintf(stderr,
					" Possible solution:\n\n\tmknod %s c "
					"<major> <minor>\n\n", port);
		} else if (errno == EACCES) {
			fprintf(stderr, "   Please check the "
					"permissions on %s..\n", realport);
			fprintf(stderr,
					" Possible solution:\n\n\tchmod 0666 "
					"%s\n\n", realport);
		} else if (errno == ENODEV) {
			while (count <= 5) {
				if (isatty(fileno(stdout))) {
					fprintf(stderr,
							"\r   Port not connected,"
							" sleeping for 2 seconds, ");
					fprintf(stderr,
							"%d retries..",
							5-count);
				}
				sleep(2);
				count++;
				goto begin;
			}
			fprintf(stderr,
					"\n\n   Device not found on %s, \
					Did you hit HotSync?\n\n", realport);	
		} else if (errno == EISDIR) {
			fprintf(stderr, " The port specified must"
					" contain a device name, and %s was"
					" a directory.\n"
					"   Please change that to reference a"
					" real device, and try"
					" again\n\n", port);
		}
		
		fprintf(stderr, "   Unable to bind to port: %s\n", 
				port);

		fprintf(stderr, "   Please use --help for more "
				"information\n\n");
		return -1;
	}

	if (isatty(fileno(stdout))) {
		printf("\n   Listening to port: %s\n\n"
			"   Please press the HotSync button now... ",
			port);
	}

	if (pi_listen(sd, 1) < 0) {
		fprintf(stderr, "\n   Error listening on %s\n", port);
		pi_close(sd);
		return -1;
	}

	sd = pi_accept(sd, 0, 0);
	if (sd < 0) {
		fprintf(stderr, "\n   Error accepting data on %s\n", port);
		pi_close(sd);
		return -1;
	}

	if (isatty(fileno(stdout))) {
		printf("connected!\n\n");
	}

	if (dlp_ReadSysInfo(sd, &sys_info) < 0) {
		fprintf(stderr, "\n   Error read system info on %s\n", port);
		pi_close(sd);
		return -1;
	}

	dlp_OpenConduit(sd);
	return sd;
}
