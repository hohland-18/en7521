/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/miniupnpdctl.c#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

#if 0
static void sighandler(int sig)
{
	printf("received signal %d\n", sig);
}
#endif

int
main(int argc, char * * argv)
{
	/*char test[] = "test!";*/
	static const char command[] = "show all\n";
	char buf[256];
	int l;
	int s;
	struct sockaddr_un addr;

//	signal(SIGINT, sighandler);
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s<0)
	{
		perror("socket");
		return 1;
	}
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "/var/run/miniupnpd.ctl",
	        sizeof(addr.sun_path));
	if(connect(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0)
	{
		perror("connect");
		close(s);
		return 1;
	}

	printf("Connected.\n");
	write(s, command, sizeof(command));
	for(;;)
	{
		l = read(s, buf, sizeof(buf));
		if(l<0)
		{
			perror("read");
			break;
		}
		if(l==0)
			break;
		/*printf("%d bytes read\n", l);*/
		fflush(stdout);
		write(fileno(stdout), buf, l);
		/*printf("\n");*/
	}

	close(s);
	return 0;
}

