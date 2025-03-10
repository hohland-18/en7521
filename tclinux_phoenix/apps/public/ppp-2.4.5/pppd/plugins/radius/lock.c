/*
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/ppp-2.4.5/pppd/plugins/radius/lock.c#1 $
 *
 * Copyright (C) 1997 Lars Fenneberg
 *
 * See the file COPYRIGHT for the respective terms and conditions.
 * If the file is missing contact me at lf@elemental.net
 * and I'll send you a copy.
 *
 */

#include "includes.h"
#include <unistd.h>
#include <fcntl.h>

int do_lock_exclusive(int fd)
{
	struct flock fl;
	int res;

	memset((void *)&fl, 0, sizeof(fl));

	fl.l_type = F_WRLCK;
	fl.l_whence = fl.l_start = 0;
	fl.l_len = 0; /* 0 means "to end of file" */

	res = fcntl(fd, F_SETLK, &fl);

	if ((res == -1) && (errno == EAGAIN))
		errno = EWOULDBLOCK;

	return res;
}

int do_unlock(int fd)
{
	struct flock fl;

	memset((void *)&fl, 0, sizeof(fl));

	fl.l_type = F_UNLCK;
	fl.l_whence = fl.l_start = 0;
	fl.l_len = 0; /* 0 means "to end of file" */

	return fcntl(fd, F_SETLK, &fl);
}
