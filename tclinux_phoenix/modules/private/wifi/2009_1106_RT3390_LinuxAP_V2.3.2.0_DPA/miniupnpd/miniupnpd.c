/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/miniupnpd.c#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2008 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/file.h>
#include <syslog.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <sys/param.h>
#if defined(sun)
#include <kstat.h>
#else
/* for BSD's sysctl */
#include <sys/sysctl.h>
#endif

/* unix sockets */
#include "config.h"
#ifdef USE_MINIUPNPDCTL
#include <sys/un.h>
#endif

#include "upnpglobalvars.h"
#include "upnphttp.h"
#include "upnpdescgen.h"
#include "miniupnpdpath.h"
#include "getifaddr.h"
#include "upnpsoap.h"
#include "options.h"
#include "minissdp.h"
#include "upnpredirect.h"
#include "miniupnpdtypes.h"
#include "daemonize.h"
#include "upnpevents.h"
#ifdef ENABLE_NATPMP
#include "natpmp.h"
#endif
#include "commonrdr.h"
#ifdef ENABLE_WSC_SERVICE
#include "wsc/wsc_common.h"
//#include "wsc/wsc_netlink.h"
#ifdef RT_DEBUG
/* To be improved */
int
xml_print(const char * s, int len, FILE * f)
{
	int n = 0, i;
	int elt_close = 0;
	int c, indent = 0;

	while(len > 0)
	{
		c = *(s++);	len--;
		switch(c)
		{
		case '<':
			if(len>0 && *s == '/')
				elt_close++;
			else if(len>0 && *s == '?')
				elt_close = 1;
			else
				elt_close = 0;
			if(elt_close!=1)
			{
				if(elt_close > 1)
					indent--;
				fputc('\n', f); n++;
				for(i=indent; i>0; i--)
					fputc(' ', f);
				n += indent;
			}
			fputc(c, f); n++;
			break;
		case '>':
			fputc(c, f); n++;
			if(elt_close==1)
			{
				/*fputc('\n', f); n++; */
				//elt_close = 0;
				if(indent > 0)
					indent--;
			}
			else if(elt_close == 0)
				indent++;
			break;
		default:
			fputc(c, f); n++;
		}
	}
	return n;
}

int DescDump()
{
	char * Desc;
	int DescLen;
	char * s;
	int l;

	Desc = genWSCRootDesc(&DescLen);
	xml_print(Desc, DescLen, stdout);
	free(Desc);
	printf("\n-------------\n");

	s = genWSC(&l);
	xml_print(s, l, stdout);
	free(s);
	printf("\n-------------\n");
#ifdef ENABLE_EVENTS
	s = getVarsWSC(&l);
	xml_print(s, l, stdout);
	free(s);
	printf("\n-------------\n");
#endif /* ENABLE_EVENTS */
	return 0;
}
#endif /* RT_DEBUG */
#endif /* ENABLE_WSC_SERVICE */

#ifdef USE_MINIUPNPDCTL
struct ctlelem {
	int socket;
	LIST_ENTRY(ctlelem) entries;
};
#endif

/* MAX_LAN_ADDR : maximum number of interfaces
 * to listen to SSDP traffic */
/*#define MAX_LAN_ADDR (4)*/

static volatile int quitting = 0;

/* OpenAndConfHTTPSocket() :
 * setup the socket used to handle incoming HTTP connections. */
static int
OpenAndConfHTTPSocket(unsigned short port)
{
	int s;
	int i = 1;
	struct sockaddr_in listenname;

	if( (s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		syslog(LOG_ERR, "socket(http): %m");
		return -1;
	}

	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
	{
		syslog(LOG_WARNING, "setsockopt(http, SO_REUSEADDR): %m");
	}

	memset(&listenname, 0, sizeof(struct sockaddr_in));
	listenname.sin_family = AF_INET;
	listenname.sin_port = htons(port);
	listenname.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(s, (struct sockaddr *)&listenname, sizeof(struct sockaddr_in)) < 0)
	{
		syslog(LOG_ERR, "bind(http): %m");
		close(s);
		return -1;
	}

	if(listen(s, 6) < 0)
	{
		syslog(LOG_ERR, "listen(http): %m");
		close(s);
		return -1;
	}

	return s;
}

/* Functions used to communicate with miniupnpdctl */
#ifdef USE_MINIUPNPDCTL
static int
OpenAndConfCtlUnixSocket(const char * path)
{
	struct sockaddr_un localun;
	int s;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	localun.sun_family = AF_UNIX;
	strncpy(localun.sun_path, path,
	          sizeof(localun.sun_path));
	if(bind(s, (struct sockaddr *)&localun,
	        sizeof(struct sockaddr_un)) < 0)
	{
		syslog(LOG_ERR, "bind(sctl): %m");
		close(s);
		s = -1;
	}
	else if(listen(s, 5) < 0)
	{
		syslog(LOG_ERR, "listen(sctl): %m");
		close(s);
		s = -1;
	}
	return s;
}

static void
write_upnphttp_details(int fd, struct upnphttp * e)
{
	char buffer[256];
	int len;
	while(e)
	{
		len = snprintf(buffer, sizeof(buffer),
		               "%d %d %s req_buf=%p(%dbytes) res_buf=%p(%dbytes alloc)\n",
		               e->socket, e->state, e->HttpVer,
		               e->req_buf, e->req_buflen,
		               e->res_buf, e->res_buf_alloclen);
		write(fd, buffer, len);
		e = e->entries.le_next;
	}
}

static void
write_ctlsockets_list(int fd, struct ctlelem * e)
{
	char buffer[256];
	int len;
	while(e)
	{
		len = snprintf(buffer, sizeof(buffer),
		               "struct ctlelem: socket=%d\n", e->socket);
		write(fd, buffer, len);
		e = e->entries.le_next;
	}
}

static void
write_option_list(int fd)
{
	char buffer[256];
	int len;
	int i;
	for(i=0; i<num_options; i++)
	{
		len = snprintf(buffer, sizeof(buffer),
		               "opt=%02d %s\n",
		               ary_options[i].id, ary_options[i].value);
		write(fd, buffer, len);
	}
}

#endif

/* Handler for the SIGTERM signal (kill) 
 * SIGINT is also handled */
static void
sigterm(int sig)
{
	/*int save_errno = errno;*/
	signal(sig, SIG_IGN);	/* Ignore this signal while we are quitting */

	syslog(LOG_NOTICE, "received signal %d, good-bye", sig);

	quitting = 1;
	/*errno = save_errno;*/
}

/* record the startup time, for returning uptime */
static void
set_startup_time(int sysuptime)
{
	startup_time = time(NULL);
	if(sysuptime)
	{
		/* use system uptime instead of daemon uptime */
#if defined(__linux__)
		char buff[64];
		int uptime, fd;
		fd = open("/proc/uptime", O_RDONLY);
		if(fd < 0)
		{
			syslog(LOG_ERR, "open(\"/proc/uptime\" : %m");
		}
		else
		{
			memset(buff, 0, sizeof(buff));
			read(fd, buff, sizeof(buff) - 1);
			uptime = atoi(buff);
			syslog(LOG_INFO, "system uptime is %d seconds", uptime);
			close(fd);
			startup_time -= uptime;
		}
#elif defined(SOLARIS_KSTATS)
		kstat_ctl_t *kc;
		kc = kstat_open();
		if(kc != NULL)
		{
			kstat_t *ksp;
			ksp = kstat_lookup(kc, "unix", 0, "system_misc");
			if(ksp && (kstat_read(kc, ksp, NULL) != -1))
			{
				void *ptr = kstat_data_lookup(ksp, "boot_time");
				if(ptr)
					memcpy(&startup_time, ptr, sizeof(startup_time));
				else
					syslog(LOG_ERR, "cannot find boot_time kstat");
			}
			else
				syslog(LOG_ERR, "cannot open kstats for unix/0/system_misc: %m");
			kstat_close(kc);
		}
#else
		struct timeval boottime;
		size_t size = sizeof(boottime);
		int name[2] = { CTL_KERN, KERN_BOOTTIME };
		if(sysctl(name, 2, &boottime, &size, NULL, 0) < 0)
		{
			syslog(LOG_ERR, "sysctl(\"kern.boottime\") failed");
		}
		else
		{
			startup_time = boottime.tv_sec;
		}
#endif
	}
}

#ifdef ENABLE_WSC_SERVICE
/* moved to upnpglobalvars.h */
#if 0
/* structure containing variables used during "main loop"
 * that are filled during the init */
struct runtime_vars {
	/* LAN IP addresses for SSDP traffic and HTTP */
	/* moved to global vars */
	/*int n_lan_addr;*/
	/*struct lan_addr_s lan_addr[MAX_LAN_ADDR];*/
	int port;	/* HTTP Port */
	int notify_interval;	/* seconds between SSDP announces */
	/* unused rules cleaning related variables : */
	int clean_ruleset_threshold;	/* threshold for removing unused rules */
	int clean_ruleset_interval;		/* (minimum) interval between checks */
};
#endif
#endif /* ENABLE_WSC_SERVICE */

/* parselanaddr()
 * parse address with mask
 * ex: 192.168.1.1/24
 * return value : 
 *    0 : ok
 *   -1 : error */
static int
parselanaddr(struct lan_addr_s * lan_addr, const char * str)
{
	const char * p;
	int nbits = 24;
	int n;
	p = str;
	while(*p && *p != '/' && !isspace(*p))
		p++;
	n = p - str;
	if(*p == '/')
	{
		nbits = atoi(++p);
		while(*p && !isspace(*p))
			p++;
	}
	if(n>15)
	{
		fprintf(stderr, "Error parsing address/mask : %s\n", str);
		return -1;
	}
	memcpy(lan_addr->str, str, n);
	lan_addr->str[n] = '\0';
	if(!inet_aton(lan_addr->str, &lan_addr->addr))
	{
		fprintf(stderr, "Error parsing address/mask : %s\n", str);
		return -1;
	}
	lan_addr->mask.s_addr = htonl(nbits ? (0xffffffff << (32 - nbits)) : 0);
#ifdef MULTIPLE_EXTERNAL_IP
	while(*p && isspace(*p))
		p++;
	if(*p) {
		n = 0;
		while(p[n] && !isspace(*p))
			n++;
		if(n<=15) {
			memcpy(lan_addr->ext_ip_str, p, n);
			lan_addr->ext_ip_str[n] = '\0';
			if(!inet_aton(lan_addr->ext_ip_str, &lan_addr->ext_ip_addr)) {
				/* error */
				fprintf(stderr, "Error parsing address : %s\n", lan_addr->ext_ip_str);
			}
		}
	}
#endif
	return 0;
}

/* init phase :
 * 1) read configuration file
 * 2) read command line arguments
 * 3) daemonize
 * 4) open syslog
 * 5) check and write pid file
 * 6) set startup time stamp
 * 7) compute presentation URL
 * 8) set signal handlers */
static int
init(int argc, char * * argv, struct runtime_vars * v)
{
	int i;
	int pid;
	int debug_flag = 0;
	int options_flag = 0;
	int openlog_option;
	struct sigaction sa;
	/*const char * logfilename = 0;*/
	const char * presurl = 0;
	const char * optionsfile = "/etc/miniupnpd.conf";

	/* first check if "-f" option is used */
	for(i=2; i<argc; i++)
	{
		if(0 == strcmp(argv[i-1], "-f"))
		{
			optionsfile = argv[i];
			options_flag = 1;
			break;
		}
	}

#ifdef ENABLE_WSC_SERVICE
	/* default turn on WPS service */
	SETFLAG(ENABLEWPSMASK);
	/* default turn off IGD service */
#else
	/* set initial values */
	SETFLAG(ENABLEUPNPMASK);
#endif /* ENABLE_WSC_SERVICE */

	/*v->n_lan_addr = 0;*/
	v->port = -1;
	v->notify_interval = 30;	/* seconds between SSDP announces */
	v->clean_ruleset_threshold = 20;
	v->clean_ruleset_interval = 0;	/* interval between ruleset check. 0=disabled */

	/* read options file first since
	 * command line arguments have final say */
	if(readoptionsfile(optionsfile) < 0)
	{
		/* only error if file exists or using -f */
		if(access(optionsfile, F_OK) == 0 || options_flag)
			fprintf(stderr, "Error reading configuration file %s\n", optionsfile);
	}
	else
	{
		for(i=0; i<num_options; i++)
		{
			switch(ary_options[i].id)
			{
			case UPNPEXT_IFNAME:
				ext_if_name = ary_options[i].value;
				break;
			case UPNPEXT_IP:
				use_ext_ip_addr = ary_options[i].value;
				break;
			case UPNPLISTENING_IP:
				if(n_lan_addr < MAX_LAN_ADDR)/* if(v->n_lan_addr < MAX_LAN_ADDR)*/
				{
					/*if(parselanaddr(&v->lan_addr[v->n_lan_addr],*/
					if(parselanaddr(&lan_addr[n_lan_addr],
					             ary_options[i].value) == 0)
						n_lan_addr++; /*v->n_lan_addr++; */
				}
				else
				{
					fprintf(stderr, "Too many listening ips (max: %d), ignoring %s\n",
			    		    MAX_LAN_ADDR, ary_options[i].value);
				}
				break;
			case UPNPPORT:
				v->port = atoi(ary_options[i].value);
				break;
			case UPNPBITRATE_UP:
				upstream_bitrate = strtoul(ary_options[i].value, 0, 0);
				break;
			case UPNPBITRATE_DOWN:
				downstream_bitrate = strtoul(ary_options[i].value, 0, 0);
				break;
			case UPNPPRESENTATIONURL:
				presurl = ary_options[i].value;
				break;
#ifdef USE_NETFILTER
			case UPNPFORWARDCHAIN:
				miniupnpd_forward_chain = ary_options[i].value;
				break;
			case UPNPNATCHAIN:
				miniupnpd_nat_chain = ary_options[i].value;
				break;
#endif
			case UPNPNOTIFY_INTERVAL:
				v->notify_interval = atoi(ary_options[i].value);
				break;
			case UPNPSYSTEM_UPTIME:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(SYSUPTIMEMASK);	/*sysuptime = 1;*/
				break;
			case UPNPPACKET_LOG:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(LOGPACKETSMASK);	/*logpackets = 1;*/
				break;
			case UPNPUUID:
				strncpy(uuidvalue+5, ary_options[i].value,
				        strlen(uuidvalue+5) + 1);
				break;
			case UPNPSERIAL:
				strncpy(serialnumber, ary_options[i].value, SERIALNUMBER_MAX_LEN);
				serialnumber[SERIALNUMBER_MAX_LEN-1] = '\0';
				break;				
			case UPNPMODEL_NUMBER:
				strncpy(modelnumber, ary_options[i].value, MODELNUMBER_MAX_LEN);
				modelnumber[MODELNUMBER_MAX_LEN-1] = '\0';
				break;
			case UPNPCLEANTHRESHOLD:
				v->clean_ruleset_threshold = atoi(ary_options[i].value);
				break;
			case UPNPCLEANINTERVAL:
				v->clean_ruleset_interval = atoi(ary_options[i].value);
				break;
#ifdef USE_PF
			case UPNPQUEUE:
				queue = ary_options[i].value;
				break;
			case UPNPTAG:
				tag = ary_options[i].value;
				break;
#endif
#ifdef ENABLE_NATPMP
			case UPNPENABLENATPMP:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(ENABLENATPMPMASK);	/*enablenatpmp = 1;*/
				else
					if(atoi(ary_options[i].value))
						SETFLAG(ENABLENATPMPMASK);
					/*enablenatpmp = atoi(ary_options[i].value);*/
				break;
#endif
#ifdef PF_ENABLE_FILTER_RULES
			case UPNPQUICKRULES:
				if(strcmp(ary_options[i].value, "no") == 0)
					SETFLAG(PFNOQUICKRULESMASK);
				break;
#endif
			case UPNPENABLE:
				if(strcmp(ary_options[i].value, "yes") != 0)
					CLEARFLAG(ENABLEUPNPMASK);
				break;
			case UPNPSECUREMODE:
				if(strcmp(ary_options[i].value, "yes") == 0)
					SETFLAG(SECUREMODEMASK);
				break;
#ifdef ENABLE_LEASEFILE
			case UPNPLEASEFILE:
				lease_file = ary_options[i].value;
				break;
#endif
			case UPNPMINISSDPDSOCKET:
				minissdpdsocketpath = ary_options[i].value;
				break;
			default:
				fprintf(stderr, "Unknown option in file %s\n",
				        optionsfile);
			}
		}
	}

	/* command line arguments processing */
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]!='-')
		{
			fprintf(stderr, "Unknown option: %s\n", argv[i]);
		}
		else switch(argv[i][1])
		{
		case 'o':
			if(i+1 < argc)
				use_ext_ip_addr = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 't':
			if(i+1 < argc)
				v->notify_interval = atoi(argv[++i]);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'u':
			if(i+1 < argc)
				strncpy(uuidvalue+5, argv[++i], strlen(uuidvalue+5) + 1);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 's':
			if(i+1 < argc)
				strncpy(serialnumber, argv[++i], SERIALNUMBER_MAX_LEN);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			serialnumber[SERIALNUMBER_MAX_LEN-1] = '\0';
			break;
#ifdef ENABLE_WSC_SERVICE
		case 'M':
			if(i+1 < argc)
				strncpy(modelnumber, argv[++i], MODELNUMBER_MAX_LEN);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			modelnumber[MODELNUMBER_MAX_LEN-1] = '\0';
			break;
		case 'm':
			i++;	/* discarding */
			break;
#else
		case 'm':
			if(i+1 < argc)
				strncpy(modelnumber, argv[++i], MODELNUMBER_MAX_LEN);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			modelnumber[MODELNUMBER_MAX_LEN-1] = '\0';
			break;
		case 'M':
			i++;	/* discarding */
			break;
#endif /* ENABLE_WSC_SERVICE */

#ifdef ENABLE_NATPMP
		case 'N':
			/*enablenatpmp = 1;*/
			SETFLAG(ENABLENATPMPMASK);
			break;
#endif
#ifdef ENABLE_WSC_SERVICE
		case 'G':
			/*enable IGD service*/
			SETFLAG(ENABLEUPNPMASK);
			break;
		case 'n':
			i++;
			break;
#endif /* ENABLE_WSC_SERVICE */
		case 'U':
			/*sysuptime = 1;*/
			SETFLAG(SYSUPTIMEMASK);
			break;
		/*case 'l':
			logfilename = argv[++i];
			break;*/
		case 'L':
			/*logpackets = 1;*/
			SETFLAG(LOGPACKETSMASK);
			break;
		case 'S':
			SETFLAG(SECUREMODEMASK);
			break;
		case 'i':
			if(i+1 < argc)
				ext_if_name = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
#ifdef USE_PF
		case 'q':
			if(i+1 < argc)
				queue = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'T':
			if(i+1 < argc)
				tag = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
#endif
		case 'p':
			if(i+1 < argc)
				v->port = atoi(argv[++i]);
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'P':
			if(i+1 < argc)
				pidfilename = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'd':
			debug_flag = 1;
			break;
		case 'w':
			if(i+1 < argc)
				presurl = argv[++i];
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'B':
			if(i+2<argc)
			{
				downstream_bitrate = strtoul(argv[++i], 0, 0);
				upstream_bitrate = strtoul(argv[++i], 0, 0);
			}
			else
				fprintf(stderr, "Option -%c takes two arguments.\n", argv[i][1]);
			break;
		case 'a':
			if(i+1 < argc)
			{
				int address_already_there = 0;
				int j;
				i++;
				for(j=0; j<n_lan_addr; j++)/* for(j=0; j<v->n_lan_addr; j++)*/
				{
					struct lan_addr_s tmpaddr;
					parselanaddr(&tmpaddr, argv[i]);
					/*if(0 == strcmp(v->lan_addr[j].str, tmpaddr.str))*/
					if(0 == strcmp(lan_addr[j].str, tmpaddr.str))
						address_already_there = 1;
				}
				if(address_already_there)
					break;
				if(n_lan_addr < MAX_LAN_ADDR) /*if(v->n_lan_addr < MAX_LAN_ADDR)*/
				{
					/*v->lan_addr[v->n_lan_addr++] = argv[i];*/
					/*if(parselanaddr(&v->lan_addr[v->n_lan_addr], argv[i]) == 0)*/
					if(parselanaddr(&lan_addr[n_lan_addr], argv[i]) == 0)
						n_lan_addr++; /*v->n_lan_addr++;*/
				}
				else
				{
					fprintf(stderr, "Too many listening ips (max: %d), ignoring %s\n",
				    	    MAX_LAN_ADDR, argv[i]);
				}
			}
			else
				fprintf(stderr, "Option -%c takes one argument.\n", argv[i][1]);
			break;
		case 'f':
			i++;	/* discarding, the config file is already read */
			break;
		default:
			fprintf(stderr, "Unknown option: %s\n", argv[i]);
		}
	}
	if(!ext_if_name || (/*v->*/n_lan_addr==0))
	{
		fprintf(stderr, "Usage:\n\t"
		        "%s [-f config_file] [-i ext_ifname] [-o ext_ip]\n"
#ifndef ENABLE_NATPMP
				"\t\t[-a listening_ip] [-p port] [-d] [-L] [-U] [-S]\n"
#else
				"\t\t[-a listening_ip] [-p port] [-d] [-L] [-U] [-S] [-N]\n"
#endif
				/*"[-l logfile] " not functionnal */
#ifdef ENABLE_WSC_SERVICE
				"\t\t[-u uuid] [-s serial] [-M model_number] \n"
#else
				"\t\t[-u uuid] [-s serial] [-m model_number] \n"
#endif /* ENABLE_WSC_SERVICE */


				"\t\t[-t notify_interval] [-P pid_filename]\n"
#ifdef USE_PF
				"\t\t[-B down up] [-w url] [-q queue] [-T tag]\n"
#else
				"\t\t[-B down up] [-w url]\n"
#endif
		        "\nNotes:\n\tThere can be one or several listening_ips.\n"
		        "\tNotify interval is in seconds. Default is 30 seconds.\n"
				"\tDefault pid file is %s.\n"
				"\tWith -d miniupnpd will run as a standard program.\n"
				"\t-L sets packet log in pf and ipf on.\n"
				"\t-S sets \"secure\" mode : clients can only add mappings to their own ip\n"
				"\t-U causes miniupnpd to report system uptime instead "
				"of daemon uptime.\n"
				"\t-B sets bitrates reported by daemon in bits per second.\n"
				"\t-w sets the presentation url. Default is http address on port 80\n"
#ifdef USE_PF
				"\t-q sets the ALTQ queue in pf.\n"
				"\t-T sets the tag name in pf.\n"
#endif
#ifdef ENABLE_WSC_SERVICE
				"\t-G turns on IGD Device service.\n"
#endif /* ENABLE_WSC_SERVICE */
		        "", argv[0], pidfilename);
		return 1;
	}

	if(debug_flag)
	{
		pid = getpid();
	}
	else
	{
#ifdef USE_DAEMON
		if(daemon(0, 0)<0) {
			perror("daemon()");
		}
		pid = getpid();
#else
		pid = daemonize();
#endif
	}

	openlog_option = LOG_PID|LOG_CONS;
	if(debug_flag)
	{
		openlog_option |= LOG_PERROR;	/* also log on stderr */
	}

	openlog("miniupnpd", openlog_option, LOG_MINIUPNPD);

	if(!debug_flag)
	{
		/* speed things up and ignore LOG_INFO and LOG_DEBUG */
		setlogmask(LOG_UPTO(LOG_NOTICE));
	}

	if(checkforrunning(pidfilename) < 0)
	{
		syslog(LOG_ERR, "MiniUPnPd is already running. EXITING");
		return 1;
	}	

	set_startup_time(GETFLAG(SYSUPTIMEMASK)/*sysuptime*/);

	/* presentation url */
	if(presurl)
	{
		strncpy(presentationurl, presurl, PRESENTATIONURL_MAX_LEN);
		presentationurl[PRESENTATIONURL_MAX_LEN-1] = '\0';
	}
	else
	{
		snprintf(presentationurl, PRESENTATIONURL_MAX_LEN,
		         "http://%s/", lan_addr[0].str);
		         /*"http://%s:%d/", lan_addr[0].str, 80);*/
		         /*"http://%s:%d/", v->lan_addr[0].str, 80);*/
	}

	/* set signal handler */
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigterm;

	if (sigaction(SIGTERM, &sa, NULL))
	{
		syslog(LOG_ERR, "Failed to set %s handler. EXITING", "SIGTERM");
		return 1;
	}
	if (sigaction(SIGINT, &sa, NULL))
	{
		syslog(LOG_ERR, "Failed to set %s handler. EXITING", "SIGINT");
		return 1;
	}

	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		syslog(LOG_ERR, "Failed to ignore SIGPIPE signals");
	}

	if(init_redirect() < 0)
	{
		syslog(LOG_ERR, "Failed to init redirection engine. EXITING");
		return 1;
	}

	writepidfile(pidfilename, pid);

#ifdef ENABLE_LEASEFILE
	/*remove(lease_file);*/
	syslog(LOG_INFO, "Reloading rules from lease file");
	reload_from_lease_file();
#endif

	return 0;
}

/* === main === */
/* process HTTP or SSDP requests */
int
main(int argc, char * * argv)
{
	int i;
	int sudp = -1, shttpl = -1;
#ifdef ENABLE_NATPMP
	int snatpmp = -1;
#endif
#ifdef ENABLE_WSC_SERVICE
	int shttpwpsl = -1;
#endif /* ENABLE_WSC_SERVICE */
	int snotify[MAX_LAN_ADDR];
	LIST_HEAD(httplisthead, upnphttp) upnphttphead;
	struct upnphttp * e = 0;
	struct upnphttp * next;
	fd_set readset;	/* for select() */
#ifdef ENABLE_EVENTS
	fd_set writeset;
#endif
	struct timeval timeout, timeofday, lasttimeofday = {0, 0};
	int max_fd = -1;
#ifdef USE_MINIUPNPDCTL
	int sctl = -1;
	LIST_HEAD(ctlstructhead, ctlelem) ctllisthead;
	struct ctlelem * ectl;
	struct ctlelem * ectlnext;
#endif
	struct runtime_vars v;
	/* variables used for the unused-rule cleanup process */
	struct rule_state * rule_list = 0;
	struct timeval checktime = {0, 0};

	if(init(argc, argv, &v) != 0)
		return 1;

#ifdef ENABLE_WSC_SERVICE
	if (GETFLAG(ENABLEWPSMASK))
	{
		if (WPSInit(argc, argv))
		{
	    	DBGPRINTF(RT_DBG_OFF, "WPSInit() ok !\n");
		}
		else
		{
	    	DBGPRINTF(RT_DBG_OFF, "WPSInit() failed !\n");
			CLEARFLAG(ENABLEWPSMASK);
			return 1;
		}
	}
#endif /* ENABLE_WSC_SERVICE */

	LIST_INIT(&upnphttphead);
#ifdef USE_MINIUPNPDCTL
	LIST_INIT(&ctllisthead);
#endif

	if(
#ifdef ENABLE_NATPMP
        !GETFLAG(ENABLENATPMPMASK) &&
#endif
#ifdef ENABLE_WSC_SERVICE
        !GETFLAG(ENABLEWPSMASK) &&
#endif /* ENABLE_WSC_SERVICE */
        !GETFLAG(ENABLEUPNPMASK) ) {
		syslog(LOG_ERR, "Why did you run me anyway?");
		return 0;
	}

#ifdef ENABLE_WSC_SERVICE
	if (GETFLAG(ENABLEWPSMASK))
	{
		/* open socket for HTTP connections. Listen on the 1st LAN address */
		shttpwpsl = OpenAndConfHTTPSocket(WPS_PORT);
		if(shttpwpsl < 0)
		{
			syslog(LOG_ERR, "Failed to open socket for WPS. EXITING");
			return 1;
		}
		syslog(LOG_NOTICE, "WPS listening on port %d", WPS_PORT);

		{
			int flags;

			flags = fcntl(netlink_sock, F_GETFL, 0); 
			if (flags == -1) 
		    	return 1; 
		    fcntl(netlink_sock, F_SETFL, flags | O_NONBLOCK);
		}		
	}
#endif /* ENABLE_WSC_SERVICE */

	if(GETFLAG(ENABLEUPNPMASK)/*enableupnp*/)
	{
		/* open socket for HTTP connections. Listen on the 1st LAN address */
		shttpl = OpenAndConfHTTPSocket((v.port > 0) ? v.port : 0);
		if(shttpl < 0)
		{
			syslog(LOG_ERR, "Failed to open socket for HTTP. EXITING");
			return 1;
		}
		if(v.port <= 0) {
			struct sockaddr_in sockinfo;
			socklen_t len = sizeof(struct sockaddr_in);
			if (getsockname(shttpl, (struct sockaddr *)&sockinfo, &len) < 0) {
				syslog(LOG_ERR, "getsockname(): %m");
				return 1;
			}
			v.port = ntohs(sockinfo.sin_port);
		}
		syslog(LOG_NOTICE, "HTTP listening on port %d", v.port);
	}

	if(GETFLAG(ENABLEUPNPMASK)/*enableupnp*/
#ifdef ENABLE_WSC_SERVICE
		|| GETFLAG(ENABLEWPSMASK)/*enablewps*/
#endif /* ENABLE_WSC_SERVICE */
		)
	{
		/* open socket for SSDP connections */
		sudp = OpenAndConfSSDPReceiveSocket(n_lan_addr, lan_addr);
		if(sudp < 0)
		{
			/*syslog(LOG_ERR, "Failed to open socket for receiving SSDP. EXITING");
			return 1;*/
			syslog(LOG_INFO, "Failed to open socket for receiving SSDP. Trying to use MiniSSDPd");
			if(SubmitServicesToMiniSSDPD(lan_addr[0].str, v.port) < 0) {
				syslog(LOG_ERR, "Failed to connect to MiniSSDPd. EXITING");
				return 1;
			}
		}

		/* open socket for sending notifications */
		if(OpenAndConfSSDPNotifySockets(snotify) < 0)
		{
			syslog(LOG_ERR, "Failed to open sockets for sending SSDP notify "
		                "messages. EXITING");
			return 1;
		}
	}

#ifdef ENABLE_NATPMP
	/* open socket for NAT PMP traffic */
	if(GETFLAG(ENABLENATPMPMASK))
	{
		snatpmp = OpenAndConfNATPMPSocket();
		if(snatpmp < 0)
		{
			syslog(LOG_ERR, "Failed to open socket for NAT PMP.");
			/*syslog(LOG_ERR, "Failed to open socket for NAT PMP. EXITING");
			return 1;*/
		} else {
			syslog(LOG_NOTICE, "Listening for NAT-PMP traffic on port %u",
			       NATPMP_PORT);
		}
		ScanNATPMPforExpiration();
	}
#endif

	/* for miniupnpdctl */
#ifdef USE_MINIUPNPDCTL
	sctl = OpenAndConfCtlUnixSocket("/var/run/miniupnpd.ctl");
#endif

#ifdef ENABLE_WSC_SERVICE
	if (GETFLAG(ENABLEWPSMASK))
	{
		DescDump();
	}
#endif /* ENABLE_WSC_SERVICE */

	/* main loop */
	while(!quitting)
	{
		/* Check if we need to send SSDP NOTIFY messages and do it if
		 * needed */
		if(gettimeofday(&timeofday, 0) < 0)
		{
			syslog(LOG_ERR, "gettimeofday(): %m");
			timeout.tv_sec = v.notify_interval;
			timeout.tv_usec = 0;
		}
		else
		{
			/* the comparaison is not very precise but who cares ? */
			if(timeofday.tv_sec >= (lasttimeofday.tv_sec + v.notify_interval))
			{
				{
					// how often to verify the timeouts, in seconds
					// default value is equal to notify interval
					WscMsgQVerifyTimeouts(v.notify_interval);
				}

				if (GETFLAG(ENABLEUPNPMASK))
				{
					SendSSDPNotifies2(snotify,
				                  (unsigned short)v.port,
				                  v.notify_interval << 1);

				}

#ifdef ENABLE_WSC_SERVICE
				if (GETFLAG(ENABLEWPSMASK))
				{
					WSC_SendSSDPNotifies2(snotify,
				                  (unsigned short)WPS_PORT,
				                  v.notify_interval << 1);
				}
#endif /* ENABLE_WSC_SERVICE */

				memcpy(&lasttimeofday, &timeofday, sizeof(struct timeval));
				timeout.tv_sec = v.notify_interval;
				timeout.tv_usec = 0;
			}
			else
			{
				timeout.tv_sec = lasttimeofday.tv_sec + v.notify_interval
				                 - timeofday.tv_sec;
				if(timeofday.tv_usec > lasttimeofday.tv_usec)
				{
					timeout.tv_usec = 1000000 + lasttimeofday.tv_usec
					                  - timeofday.tv_usec;
					timeout.tv_sec--;
				}
				else
				{
					timeout.tv_usec = lasttimeofday.tv_usec - timeofday.tv_usec;
				}
			}
		}
		/* remove unused rules */
		if( v.clean_ruleset_interval
		  && (timeofday.tv_sec >= checktime.tv_sec + v.clean_ruleset_interval))
		{
			if(rule_list)
			{
				remove_unused_rules(rule_list);
				rule_list = NULL;
			}
			else
			{
				rule_list = get_upnp_rules_state_list(v.clean_ruleset_threshold);
			}
			memcpy(&checktime, &timeofday, sizeof(struct timeval));
		}
#ifdef ENABLE_NATPMP
		/* Remove expired NAT-PMP mappings */
		while( nextnatpmptoclean_timestamp && (timeofday.tv_sec >= nextnatpmptoclean_timestamp + startup_time))
		{
			/*syslog(LOG_DEBUG, "cleaning expired NAT-PMP mappings");*/
			if(CleanExpiredNATPMP() < 0) {
				syslog(LOG_ERR, "CleanExpiredNATPMP() failed");
				break;
			}
		}
		if(nextnatpmptoclean_timestamp && timeout.tv_sec >= (nextnatpmptoclean_timestamp + startup_time - timeofday.tv_sec))
		{
			/*syslog(LOG_DEBUG, "setting timeout to %d sec", nextnatpmptoclean_timestamp + startup_time - timeofday.tv_sec);*/
			timeout.tv_sec = nextnatpmptoclean_timestamp + startup_time - timeofday.tv_sec;
			timeout.tv_usec = 0;
		}
#endif

		/* select open sockets (SSDP, HTTP listen, and all HTTP soap sockets) */
		FD_ZERO(&readset);

		if (sudp >= 0)
		{
			FD_SET(sudp, &readset);
			max_fd = MAX( max_fd, sudp);
		}
		
		if (shttpl >= 0) 
		{
			FD_SET(shttpl, &readset);
			max_fd = MAX( max_fd, shttpl);
		}

#ifdef ENABLE_WSC_SERVICE
		if(shttpwpsl >= 0)
		{
			FD_SET(shttpwpsl, &readset);
			max_fd = MAX(max_fd, shttpwpsl);
		}
		if(netlink_sock >= 0)
		{
			FD_SET(netlink_sock, &readset);
			max_fd = MAX(max_fd, netlink_sock);
		}
#endif /* ENABLE_WSC_SERVICE */

		i = 0;	/* active HTTP connections count */
		for(e = upnphttphead.lh_first; e != NULL; e = e->entries.le_next)
		{
			if((e->socket >= 0) && (e->state <= 2))
			{
				FD_SET(e->socket, &readset);
				max_fd = MAX( max_fd, e->socket);
				i++;
			}
		}
		/* for debug */
#ifdef DEBUG
		if(i > 1)
		{
			syslog(LOG_DEBUG, "%d active incoming HTTP connections", i);
		}
#endif
#ifdef ENABLE_NATPMP
		if(snatpmp >= 0) {
			FD_SET(snatpmp, &readset);
			max_fd = MAX( max_fd, snatpmp);
		}
#endif
#ifdef USE_MINIUPNPDCTL
		if(sctl >= 0) {
			FD_SET(sctl, &readset);
			max_fd = MAX( max_fd, sctl);
		}
		
		for(ectl = ctllisthead.lh_first; ectl; ectl = ectl->entries.le_next)
		{
			if(ectl->socket >= 0) {
				FD_SET(ectl->socket, &readset);
				max_fd = MAX( max_fd, ectl->socket);
			}
		}
#endif

#ifdef ENABLE_EVENTS
		FD_ZERO(&writeset);
		upnpevents_selectfds(&readset, &writeset, &max_fd);
#endif

#ifdef ENABLE_EVENTS
		if(select(max_fd+1, &readset, &writeset, 0, &timeout) < 0)
#else
		if(select(max_fd+1, &readset, 0, 0, &timeout) < 0)
#endif
		{
			if(quitting) goto shutdown;
			syslog(LOG_ERR, "select(all): %m");
			syslog(LOG_ERR, "Failed to select open sockets. EXITING");
			return 1;	/* very serious cause of error */
		}
#ifdef USE_MINIUPNPDCTL
		for(ectl = ctllisthead.lh_first; ectl;)
		{
			ectlnext =  ectl->entries.le_next;
			if((ectl->socket >= 0) && FD_ISSET(ectl->socket, &readset))
			{
				char buf[256];
				int l;
				l = read(ectl->socket, buf, sizeof(buf));
				if(l > 0)
				{
					/*write(ectl->socket, buf, l);*/
					write_option_list(ectl->socket);
					write_permlist(ectl->socket, upnppermlist, num_upnpperm);
					write_upnphttp_details(ectl->socket, upnphttphead.lh_first);
					write_ctlsockets_list(ectl->socket, ctllisthead.lh_first);
					write_ruleset_details(ectl->socket);
#ifdef ENABLE_EVENTS
					write_events_details(ectl->socket);
#endif
					/* close the socket */
					close(ectl->socket);
					ectl->socket = -1;
				}
				else
				{
					close(ectl->socket);
					ectl->socket = -1;
				}
			}
			if(ectl->socket < 0)
			{
				LIST_REMOVE(ectl, entries);
				free(ectl);
			}
			ectl = ectlnext;
		}
		if((sctl >= 0) && FD_ISSET(sctl, &readset))
		{
			int s;
			struct sockaddr_un clientname;
			struct ctlelem * tmp;
			socklen_t clientnamelen = sizeof(struct sockaddr_un);
			//syslog(LOG_DEBUG, "sctl!");
			s = accept(sctl, (struct sockaddr *)&clientname,
			           &clientnamelen);
			syslog(LOG_DEBUG, "sctl! : '%s'", clientname.sun_path);
			tmp = malloc(sizeof(struct ctlelem));
			tmp->socket = s;
			LIST_INSERT_HEAD(&ctllisthead, tmp, entries);
		}
#endif
#ifdef ENABLE_EVENTS
		upnpevents_processfds(&readset, &writeset);
#endif
#ifdef ENABLE_NATPMP
		/* process NAT-PMP packets */
		if((snatpmp >= 0) && FD_ISSET(snatpmp, &readset))
		{
			ProcessIncomingNATPMPPacket(snatpmp);
		}
#endif
		/* process SSDP packets */
		if(sudp >= 0 && FD_ISSET(sudp, &readset))
		{
			/*syslog(LOG_INFO, "Received UDP Packet");*/
			ProcessSSDPRequest(sudp, (unsigned short)v.port); 
		}
		/* process active HTTP connections */
		/* LIST_FOREACH macro is not available under linux */
		for(e = upnphttphead.lh_first; e != NULL; e = e->entries.le_next)
		{
			if((e->socket >= 0) && (e->state <= 2)
				&&(FD_ISSET(e->socket, &readset)) )
			{
				Process_upnphttp(e);
			}
#ifdef ENABLE_WSC_SERVICE
			else if(e->state == 3)
			{
				/* to wait for K2U http response from netlink socket */
			}
#endif /* ENABLE_WSC_SERVICE */
		}

#ifdef ENABLE_WSC_SERVICE
		if (GETFLAG(ENABLEWPSMASK))
		{
			/* process incoming HTTP connections */
			if(shttpwpsl >= 0 && FD_ISSET(shttpwpsl, &readset))
			{
				int shttpwps;
				socklen_t clientnamelen;
				struct sockaddr_in clientname;
				clientnamelen = sizeof(struct sockaddr_in);
				shttpwps = accept(shttpwpsl, (struct sockaddr *)&clientname, &clientnamelen);
				if (shttpwps<0)
				{
					syslog(LOG_ERR, "accept(shttpwpsl): %m");
				}
				else
				{
					struct upnphttp * tmp = 0;
					syslog(LOG_INFO, "HTTP connection from %s:%d",
						inet_ntoa(clientname.sin_addr),
						ntohs(clientname.sin_port) );
					/* Create a new upnphttp object and add it to
					 * the active upnphttp object list */
					tmp = New_upnphttp(shttpwps);
					if(tmp)
					{
						tmp->clientaddr = clientname.sin_addr;
						LIST_INSERT_HEAD(&upnphttphead, tmp, entries);
					}
					else
					{
						syslog(LOG_ERR, "New_upnphttp() failed");
						close(shttpwps);
						shttpwps = -1;
					}
				}
			}

			if(FD_ISSET(netlink_sock, &readset))
			{
				DBGPRINTF(RT_DBG_LOUD, "(%s):netlink socket data is available now.\n", __FUNCTION__);
				wscNLSockRecv(netlink_sock);
			}
		}
#endif /* ENABLE_WSC_SERVICE */
		/* process incoming HTTP connections */
		if(shttpl >= 0 && FD_ISSET(shttpl, &readset))
		{
			int shttp;
			socklen_t clientnamelen;
			struct sockaddr_in clientname;
			clientnamelen = sizeof(struct sockaddr_in);
			shttp = accept(shttpl, (struct sockaddr *)&clientname, &clientnamelen);
			if(shttp<0)
			{
				syslog(LOG_ERR, "accept(http): %m");
			}
			else
			{
				struct upnphttp * tmp = 0;
				syslog(LOG_INFO, "HTTP connection from %s:%d",
					inet_ntoa(clientname.sin_addr),
					ntohs(clientname.sin_port) );
				/*if (fcntl(shttp, F_SETFL, O_NONBLOCK) < 0) {
					syslog(LOG_ERR, "fcntl F_SETFL, O_NONBLOCK");
				}*/
				/* Create a new upnphttp object and add it to
				 * the active upnphttp object list */
				tmp = New_upnphttp(shttp);
				if(tmp)
				{
					tmp->clientaddr = clientname.sin_addr;
					LIST_INSERT_HEAD(&upnphttphead, tmp, entries);
				}
				else
				{
					syslog(LOG_ERR, "New_upnphttp() failed");
					close(shttp);
				}
			}
		}
		/* delete finished HTTP connections */
		for(e = upnphttphead.lh_first; e != NULL; )
		{
			next = e->entries.le_next;
			if(e->state >= 100)
			{
				LIST_REMOVE(e, entries);
				Delete_upnphttp(e);
			}
			e = next;
		}
	}

shutdown:
	/* close out open sockets */
	while(upnphttphead.lh_first != NULL)
	{
		e = upnphttphead.lh_first;
		LIST_REMOVE(e, entries);
		Delete_upnphttp(e);
	}

	if (sudp >= 0) close(sudp);
	if (shttpl >= 0) close(shttpl);
#ifdef ENABLE_NATPMP
	if(snatpmp>=0)
	{
		close(snatpmp);
		snatpmp = -1;
	}
#endif
#ifdef ENABLE_WSC_SERVICE
	if(shttpwpsl >= 0)
	{
		close(shttpwpsl);
		shttpwpsl = -1;
	}
	if(netlink_sock >= 0)
	{
		close(netlink_sock);
		netlink_sock = -1;
	}
#endif /* ENABLE_WSC_SERVICE */
#ifdef USE_MINIUPNPDCTL
	if(sctl>=0)
	{
		close(sctl);
		sctl = -1;
		if(unlink("/var/run/miniupnpd.ctl") < 0)
		{
			syslog(LOG_ERR, "unlink() %m");
		}
	}
#endif
	
#ifdef ENABLE_WSC_SERVICE
	if (GETFLAG(ENABLEWPSMASK))
	{
		if(WSC_SendSSDPGoodbye(snotify, n_lan_addr) < 0)
		{
			syslog(LOG_ERR, "Failed to broadcast good-bye notifications for WSC");
		}
	}

	if (!(GETFLAG(ENABLEUPNPMASK)))
	{
		for(i=0; i<n_lan_addr; i++)/* for(i=0; i<v.n_lan_addr; i++)*/
			close(snotify[i]);
	}
#endif /* ENABLE_WSC_SERVICE */

	/*if(SendSSDPGoodbye(snotify, v.n_lan_addr) < 0)*/
	if (GETFLAG(ENABLEUPNPMASK))
	{
		if(SendSSDPGoodbye(snotify, n_lan_addr) < 0)
		{
			syslog(LOG_ERR, "Failed to broadcast good-bye notifications");
		}

		for(i=0; i<n_lan_addr; i++)/* for(i=0; i<v.n_lan_addr; i++)*/
			close(snotify[i]);
	}

	if(unlink(pidfilename) < 0)
	{
		syslog(LOG_ERR, "Failed to remove pidfile %s: %m", pidfilename);
	}

	closelog();	
	freeoptions();
#ifdef ENABLE_WSC_SERVICE
	CLEARFLAG(ENABLEWPSMASK);
    DBGPRINTF(RT_DBG_INFO, "wscSystemStop()...\n");
    wscSystemStop();

#if 1 // def MULTIPLE_CARD_SUPPORT
	unlink(pid_file_path);
#else
	unlink(DEFAULT_PID_FILE_PATH);
#endif // MULTIPLE_CARD_SUPPORT //
#endif /* ENABLE_WSC_SERVICE */
	return 0;
}

