/*
 * net.c: Protocl for NetSync connections
 *
 * Copyright (c) 1997, Kenneth Albanowski
 * Copyright (c) 1999, Tilo Christ
 * Copyright (c) 1999, John Franks
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
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "pi-debug.h"
#include "pi-source.h"
#include "pi-net.h"

#define PI_NET_TIMEOUT 10*1000

static int net_getsockopt(pi_socket_t *ps, int level, int option_name, 
			  void *option_value, size_t *option_len);
static int net_setsockopt(pi_socket_t *ps, int level, int option_name, 
			  const void *option_value, size_t *option_len);

/***********************************************************************
 *
 * Function:    net_protocol_dup
 *
 * Summary:     clones an existing pi_protocol struct
 *
 * Parameters:  pi_protocol_t*
 *
 * Returns:     pi_protocol_t* or NULL if operation failed
 *
 ***********************************************************************/
static pi_protocol_t
*net_protocol_dup (pi_protocol_t *prot)
{
	pi_protocol_t *new_prot = NULL;
	pi_net_data_t	*data = NULL,
			*new_data = NULL;

	ASSERT(prot != NULL);
	
	new_prot = (pi_protocol_t *)malloc (sizeof (pi_protocol_t));
	if (new_prot != NULL) {
		new_data = (pi_net_data_t *)malloc (sizeof (pi_net_data_t));
		if (new_data == NULL) {
			free(new_prot);
		}
	}

	if ( (new_prot != NULL) && (new_data != NULL) ) {
		new_prot->level 	= prot->level;
		new_prot->dup 		= prot->dup;
		new_prot->free 		= prot->free;
		new_prot->read 		= prot->read;
		new_prot->write 	= prot->write;
		new_prot->getsockopt 	= prot->getsockopt;
		new_prot->setsockopt 	= prot->setsockopt;

		data 			= (pi_net_data_t *)prot->data;
		new_data->type 		= data->type;
		new_data->txid 		= data->txid;
		new_prot->data 		= new_data;
	}

	return new_prot;
}


/***********************************************************************
 *
 * Function:    net_protocol_free
 *
 * Summary:     frees an existing pi_protocol struct
 *
 * Parameters:  pi_protocol*
 *
 * Returns:     void
 *
 ***********************************************************************/
static
void net_protocol_free (pi_protocol_t *prot)
{

	ASSERT (prot != NULL);

	if (prot != NULL) {
		if (prot->data != NULL) {
			free(prot->data);
			prot->data = NULL;
		}
		free(prot);
	}
}


/***********************************************************************
 *
 * Function:    net_protocol
 *
 * Summary:     creates and inits pi_protocol struct instance
 *
 * Parameters:  void
 *
 * Returns:     pi_protocol_t* or NULL if operation failed
 *
 ***********************************************************************/
pi_protocol_t
*net_protocol (void)
{
	pi_protocol_t *prot = NULL;
	pi_net_data_t *data = NULL;

	prot 	= (pi_protocol_t *)malloc (sizeof (pi_protocol_t));	
	if (prot != NULL) {
		data 	= (pi_net_data_t *)malloc (sizeof (pi_net_data_t));
		if (data == NULL) {
			free(prot);
			prot = NULL;
		}
	}

	if ( (prot != NULL) && (data != NULL) ) {

		prot->level 		= PI_LEVEL_NET;
		prot->dup 		= net_protocol_dup;
		prot->free 		= net_protocol_free;
		prot->read 		= net_rx;
		prot->write 		= net_tx;
		prot->getsockopt 	= net_getsockopt;
		prot->setsockopt 	= net_setsockopt;

		data->type 		= PI_NET_TYPE_DATA;
		data->txid 		= 0x00;
		prot->data 		= data;
	}
	
	return prot;
}


/***********************************************************************
 *
 * Function:    net_rx_handshake
 *
 * Summary:     RX handshake
 *
 * Parameters:  pi_socket_t*
 *
 * Returns:     0 for success, -1 otherwise
 *
 ***********************************************************************/
int
net_rx_handshake(pi_socket_t *ps)
{
	unsigned char msg1[50] = 
		"\x12\x01\x00\x00\x00\x00\x00\x00\x00\x20\x00\x00\x00"
		"\x24\xff\xff\xff\xff\x3c\x00\x3c\x00\x00\x00\x00\x00"
		"\x00\x00\x00\x00\xc0\xa8\xa5\x1f\x04\x27\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	unsigned char msg2[46] = 
		"\x13\x01\x00\x00\x00\x00\x00\x00\x00\x20\x00\x00\x00"
		"\x20\xff\xff\xff\xff\x00\x3c\x00\x3c\x00\x00\x00\x00"
		"\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x00";
	pi_buffer_t *buffer;
	
	buffer = pi_buffer_new (200);
	if (buffer == NULL) {
		errno = ENOMEM;
		return -1;
	}

	if (net_rx(ps, buffer, 22, 0) >= 0  &&
		net_tx(ps, msg1, 50, 0) >= 0	&&
		net_rx(ps, buffer, 50, 0) >= 0  &&
		net_tx(ps, msg2, 46, 0) >= 0	&&
		net_rx(ps, buffer, 8, 0) >= 0)
	{
		pi_buffer_free (buffer);
		return 0;
	}

	pi_buffer_free (buffer);
	return -1;
}


/***********************************************************************
 *
 * Function:    net_tx_handshake
 *
 * Summary:     TX handshake
 *
 * Parameters:  pi_socket_t*
 *
 * Returns:     0 for success, -1 otherwise
 *
 ***********************************************************************/
int
net_tx_handshake(pi_socket_t *ps)
{
	unsigned char msg1[22] = 
		"\x90\x01\x00\x00\x00\x00\x00\x00\x00\x20\x00\x00\x00"
		"\x08\x01\x00\x00\x00\x00\x00\x00\x00";
	unsigned char msg2[50] = 
		"\x92\x01\x00\x00\x00\x00\x00\x00\x00\x20\x00\x00\x00"
		"\x24\xff\xff\xff\xff\x00\x3c\x00\x3c\x40\x00\x00\x00"
		"\x01\x00\x00\x00\xc0\xa8\xa5\x1e\x04\x01\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	unsigned char msg3[8]  = 
		"\x93\x00\x00\x00\x00\x00\x00\x00";
	pi_buffer_t *buffer;
	
	buffer = pi_buffer_new (200);
	if (buffer == NULL) {
		errno = ENOMEM;
		return -1;
	}
	
	if (net_tx(ps, msg1, 22, 0) >= 0	&&
		net_rx(ps, buffer, 50, 0) >= 0  &&
		net_tx(ps, msg2, 50, 0) >= 0	&&
		net_rx(ps, buffer, 46, 0) >= 0  &&
		net_tx(ps, msg3, 8, 0) >= 0)
	{
		pi_buffer_free (buffer);
		return 0;
	}

	pi_buffer_free (buffer);
	return -1;
}


/***********************************************************************
 *
 * Function:    net_tx
 *
 * Summary:     Transmit NET Packets
 *
 * Parameters:  pi_socket_t*, char* to buf, buf length, flags
 *
 * Returns:     A negative number on error, 0 otherwise
 *
 ***********************************************************************/
ssize_t
net_tx(pi_socket_t *ps, unsigned char *msg, size_t len, int flags)
{
	int 	bytes;

	pi_protocol_t	*prot,
			*next;

	pi_net_data_t *data;
	unsigned char *buf;

	prot = pi_protocol(ps->sd, PI_LEVEL_NET);
	if (prot == NULL)
		return -1;
	data = (pi_net_data_t *)prot->data;
	next = pi_protocol_next(ps->sd, PI_LEVEL_NET);
	if (next == NULL)
		return -1;

	/* Create the header */
	buf = (unsigned char *) malloc(PI_NET_HEADER_LEN + len);
	buf[PI_NET_OFFSET_TYPE] = data->type;
	if (data->type == PI_NET_TYPE_TCKL)
		buf[PI_NET_OFFSET_TXID] = 0xff;
	else
		buf[PI_NET_OFFSET_TXID] = data->txid;
	set_long(&buf[PI_NET_OFFSET_SIZE], len);
	memcpy(&buf[PI_NET_HEADER_LEN], msg, len);

	/* Write the header and body */
	bytes = next->write(ps, buf, PI_NET_HEADER_LEN + len, flags);
	if (bytes < (int)(PI_NET_HEADER_LEN + len)) {
		free(buf);
		return bytes;
	}

	CHECK(PI_DBG_NET, PI_DBG_LVL_INFO, net_dump_header(buf, 1));
	CHECK(PI_DBG_NET, PI_DBG_LVL_DEBUG, dumpdata((char *)msg, len));
	
	free(buf);
	return len;
}

/***********************************************************************
 *
 * Function:    net_rx
 *
 * Summary:     Receive NET Packets
 *
 * Parameters:  ps		--> socket to read from
 *				msg		<-- malloc()ed buffer containing the data
 *				len		--> unused
 *				flags   --> unused
 *
 * Returns:     A negative number on error, 0 on timeout, otherwise the
 *              length of the received packet.
 *
 ***********************************************************************/
ssize_t
net_rx(pi_socket_t *ps, pi_buffer_t *msg, size_t len, int flags)
{
	int 	bytes, 
		total_bytes, 
		packet_len,
		timeout;
	size_t	size;
	pi_protocol_t	*prot,
			*next;
	pi_buffer_t *header;
	pi_net_data_t *data;

	prot = pi_protocol(ps->sd, PI_LEVEL_NET);
	if (prot == NULL)
		return -1;
	
	data = (pi_net_data_t *)prot->data;
	next = pi_protocol_next(ps->sd, PI_LEVEL_NET);
	if (next == NULL)
		return -1;

	timeout = PI_NET_TIMEOUT;
	size = sizeof(timeout);
	pi_setsockopt(ps->sd, PI_LEVEL_DEV, PI_DEV_TIMEOUT, 
		      &timeout, &size);

	total_bytes = 0;
	header = pi_buffer_new (PI_NET_HEADER_LEN);
	if (header == NULL) {
		errno = ENOMEM;
		return -1;
	}

	if (data->txid == 0) {	
		/* Peek to see if it is a headerless packet */
		bytes = next->read(ps, header, 1, flags);
		if (bytes <= 0) {
			pi_buffer_free (header);
			return bytes;
		}

		LOG ((PI_DBG_NET, PI_DBG_LVL_INFO,
			"NET RX: Checking for headerless packet %d\n",
			header->data[0]));

		if (header->data[0] == 0x90) {
			/* Cause the header bytes to be skipped */
			LOG ((PI_DBG_NET, PI_DBG_LVL_INFO,
				"NET RX: Headerless packet\n"));
			total_bytes = PI_NET_HEADER_LEN;
			header->data[PI_NET_OFFSET_TYPE] = PI_NET_TYPE_DATA;
			header->data[PI_NET_OFFSET_TXID] = 0x01;
			set_long (&header->data[PI_NET_OFFSET_SIZE], 21);
		} else {
			total_bytes += bytes;
		}
	}

	/* bytes in what's left of the header */
	while (total_bytes < PI_NET_HEADER_LEN) {
		bytes = next->read(ps, header,
			(size_t)(PI_NET_HEADER_LEN - total_bytes), flags);
		if (bytes <= 0) {
			pi_buffer_free (header);
			return bytes;
		}
		total_bytes += bytes;
	}

	/* read the actual packet data */
	total_bytes = 0;
	packet_len = get_long(&header->data[PI_NET_OFFSET_SIZE]);

	while (total_bytes < packet_len) {
		bytes = next->read(ps, msg,
			(size_t)(packet_len - total_bytes), flags);
		if (bytes < 0) {
			pi_buffer_free (header);
			return bytes;
		}
		total_bytes += bytes;
	}

	CHECK(PI_DBG_NET, PI_DBG_LVL_INFO, net_dump_header(header->data, 0));
	CHECK(PI_DBG_NET, PI_DBG_LVL_DEBUG, net_dump(header->data, msg->data));

	/* Update the transaction id */
	if (ps->state == PI_SOCK_CONIN || ps->command == 1)
		data->txid = header->data[PI_NET_OFFSET_TXID];
	else {
		data->txid++;
		if (data->txid == 0xff)
			data->txid = 1;
	}

	pi_buffer_free (header);

	return packet_len;
}

/***********************************************************************
 *
 * Function:    net_getsockopt
 *
 * Summary:     get options on socket
 *
 * Parameters:  pi_socket*, level, option name, option value, option length
 *
 * Returns:     0 for success, -1 otherwise
 *
 ***********************************************************************/
static int
net_getsockopt(pi_socket_t *ps, int level, int option_name, 
	       void *option_value, size_t *option_len)
{
	pi_protocol_t *prot;
	pi_net_data_t *data;

	prot = pi_protocol(ps->sd, PI_LEVEL_NET);
	if (prot == NULL)
		return -1;
	data = (pi_net_data_t *)prot->data;

	switch (option_name) {
	case PI_NET_TYPE:
		if (*option_len < sizeof (data->type))
			goto error;
		memcpy (option_value, &data->type,
			sizeof (data->type));
		*option_len = sizeof (data->type);
		break;
	}
	
	return 0;
	
 error:
	errno = EINVAL;
	return -1;
}


/***********************************************************************
 *
 * Function:    net_setsockopt
 *
 * Summary:     set options on socket
 *
 * Parameters:  pi_socket*, level, option name, option value, option length
 *
 * Returns:     0 for success, -1 otherwise
 *
 ***********************************************************************/
static int
net_setsockopt(pi_socket_t *ps, int level, int option_name, 
	       const void *option_value, size_t *option_len)
{
	pi_protocol_t *prot;
	pi_net_data_t *data;

	prot = pi_protocol(ps->sd, PI_LEVEL_NET);
	if (prot == NULL)
		return -1;
	data = (pi_net_data_t *)prot->data;

	switch (option_name) {
	case PI_NET_TYPE:
		if (*option_len != sizeof (data->type))
			goto error;
		memcpy (&data->type, option_value,
			sizeof (data->type));
		*option_len = sizeof (data->type);
		break;
	}

	return 0;
	
 error:
	errno = EINVAL;
	return -1;
}


/***********************************************************************
 *
 * Function:    net_dump_header
 *
 * Summary:     Dump the NET packet header
 *
 * Parameters:  char* to net packet, TX boolean
 *
 * Returns:     void
 *
 ***********************************************************************/
void
net_dump_header(unsigned char *data, int rxtx)
{
	LOG((PI_DBG_NET, PI_DBG_LVL_NONE,
	    "NET %s type=%d txid=0x%.2x len=0x%.4x\n",
	    rxtx ? "TX" : "RX",
	    get_byte(&data[PI_NET_OFFSET_TYPE]),
	    get_byte(&data[PI_NET_OFFSET_TXID]),
	    get_long(&data[PI_NET_OFFSET_SIZE])));
}


/***********************************************************************
 *
 * Function:    net_dump
 *
 * Summary:     Dump the NET packet
 *
 * Parameters:  char* to net packet
 *
 * Returns:     void
 *
 ***********************************************************************/
void
net_dump(unsigned char *header, unsigned char *data)
{
	size_t 	size;

	size = get_long(&header[PI_NET_OFFSET_SIZE]);
	dumpdata((char *)data, size);
}
