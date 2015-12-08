/*
 * Copyright (c) 2015 Micro Systems Marc Balmer, CH-5073 Gipf-Oberfrick.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * "nanomsg" is a trademark of Martin Sustrik.  Used with permission.
 */

/* nanomsg for Lua constants */

#include <nanomsg/nn.h>
#include <nanomsg/pair.h>
#include <nanomsg/reqrep.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/survey.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/bus.h>

#include "luananomsg.h"

#define NN(NAME)		{ #NAME, NN_##NAME }
#define PLAIN(NAME)		{ #NAME, NAME }

struct int_constant nn_int[] = {
	/* Socket types */
	NN(PAIR),

	/* reqrep */
	NN(REQ),
	NN(REP),

	/* pubsub */
	NN(PUB),
	NN(SUB),
	NN(SUB_SUBSCRIBE),
	NN(SUB_UNSUBSCRIBE),

	/* survey */
	NN(SURVEYOR),
	NN(RESPONDENT),
	NN(SURVEYOR_DEADLINE),

	/* pipeline */
	NN(PUSH),
	NN(PULL),

	/* bus */
	NN(BUS),

	/* ABI versioning */
	NN(VERSION_CURRENT),
	NN(VERSION_REVISION),
	NN(VERSION_AGE),

	/* Error codes */
	PLAIN(ENOTSUP),
	PLAIN(EPROTONOSUPPORT),
	PLAIN(ENOBUFS),
	PLAIN(ENETDOWN),
	PLAIN(EADDRINUSE),
	PLAIN(EADDRNOTAVAIL),
	PLAIN(ECONNREFUSED),
	PLAIN(EINPROGRESS),
	PLAIN(ENOTSOCK),
	PLAIN(EAFNOSUPPORT),
	PLAIN(EPROTO),
	PLAIN(EAGAIN),
	PLAIN(EBADF),
	PLAIN(EINVAL),
	PLAIN(EMFILE),
	PLAIN(EFAULT),
	PLAIN(EACCESS),
	PLAIN(ENETRESET),
	PLAIN(ENETUNREACH),
	PLAIN(EHOSTUNREACH),
	PLAIN(ENOTCONN),
	PLAIN(EMSGSIZE),
	PLAIN(ETIMEDOUT),
	PLAIN(ECONNABORTED),
	PLAIN(ECONNRESET),
	PLAIN(ENOPROTOOPT),
	PLAIN(EISCONN),
	PLAIN(ESOCKTNOSUPPORT),

	/* Native nanomsg error codes. */
	PLAIN(ETERM),
	PLAIN(EFSM),

	/*
	 * Constants that are returned in `ns` member of
	 * nn_symbol_properties.
	 */
	NN(NS_NAMESPACE),
	NN(NS_VERSION),
	NN(NS_DOMAIN),
	NN(NS_TRANSPORT),
	NN(NS_PROTOCOL),
	NN(NS_OPTION_LEVEL),
	NN(NS_SOCKET_OPTION),
	NN(NS_TRANSPORT_OPTION),
	NN(NS_OPTION_TYPE),
	NN(NS_OPTION_UNIT),
	NN(NS_FLAG),
	NN(NS_ERROR),
	NN(NS_LIMIT),
	NN(NS_EVENT),

	/*
	 * Constants that are returned in `type` member of
	 * nn_symbol_properties.
	 */
	NN(TYPE_NONE),
	NN(TYPE_INT),
	NN(TYPE_STR),

	/*
	 * Constants that are returned in the `unit` member of
	 * nn_symbol_properties.
	 */
	NN(UNIT_NONE),
	NN(UNIT_BYTES),
	NN(UNIT_MILLISECONDS),
	NN(UNIT_PRIORITY),
	NN(UNIT_BOOLEAN),

	/* SP address families. */
	PLAIN(AF_SP),
	PLAIN(AF_SP_RAW),

	/* Max size of an SP address. */
	NN(SOCKADDR_MAX),

	/*
	 * Socket option levels: Negative numbers are reserved for transports,
    	 * positive for socket types.
    	 */
	NN(SOL_SOCKET),

	/* Generic socket options (NN_SOL_SOCKET level). */
	NN(LINGER),
	NN(SNDBUF),
	NN(RCVBUF),
	NN(SNDTIMEO),
	NN(RCVTIMEO),
	NN(RECONNECT_IVL),
	NN(RECONNECT_IVL_MAX),
	NN(SNDPRIO),
	NN(RCVPRIO),
	NN(SNDFD),
	NN(RCVFD),
	NN(DOMAIN),
	NN(PROTOCOL),
	NN(IPV4ONLY),
	NN(SOCKET_NAME),
	NN(RCVMAXSIZE),

	/* Send/recv options. */
	NN(DONTWAIT),

	/* Ancillary data. */
	PLAIN(PROTO_SP),
	PLAIN(SP_HDR)
};

size_t
num_nn_int(void)
{
	return sizeof(nn_int)/sizeof(nn_int[0]);
}
