/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-99 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/boa-asp/src/response.c#1 $*/

#include "boa.h"

#define HTML "text/html; charset=ISO-8859-1"
#define CRLF "\r\n"

#if defined(TCSUPPORT_CD_NEW_GUI) || defined(TCSUPPORT_CT_C5_HEN_SFU)
char	pre_sid[10] = {0};
#endif
#if defined(TCSUPPORT_CT_E8GUI)
#define REDIRECT_URL(x)	"<HTML><HEAD><TITLE>Login</TITLE>" \
						"<script language=javascript>" \
						"top.location.replace(\"/cgi-bin/"x"\");" \
						"</script>" \
						"</HEAD><body></body></HTML>\n"
#endif

void print_content_type(request * req)
{
    req_write(req, "Content-Type: ");
    req_write(req, get_mime_type(req->request_uri));
    req_write(req, "\r\n");
#if defined(TCSUPPORT_CUC) || defined(TCSUPPORT_CT)
	if(strstr(req->request_uri,"syslog.txt") != NULL){
		req_write(req, "Content-Disposition: attachment; filename=\"syslog.txt\"\r\n");
	}
#endif
}

void print_content_length(request * req)
{
    req_write(req, "Content-Length: ");
    req_write(req, simple_itoa(req->filesize));
    req_write(req, "\r\n");
}

void print_last_modified(request * req)
{
    static char lm[] = "Last-Modified: "
        "                             " "\r\n";
    rfc822_time_buf(lm + 15, req->last_modified);
    req_write(req, lm);
}

void print_ka_phrase(request * req)
{
    if (req->kacount > 0 &&
        req->keepalive == KA_ACTIVE && req->response_status < 500) {
        req_write(req, "Connection: Keep-Alive\r\nKeep-Alive: timeout=");
        req_write(req, simple_itoa(ka_timeout));
        req_write(req, ", max=");
        req_write(req, simple_itoa(req->kacount));
        req_write(req, "\r\n");
    } else
    {
        req_write(req, "Connection: close\r\n");
}
}

void print_http_headers(request * req)
{
    static char stuff[] = "Date: "
        "                             "
        "\r\nServer: " SERVER_VERSION "\r\n";

    rfc822_time_buf(stuff + 6, 0);
    req_write(req, stuff);
#if defined(TCSUPPORT_WEBSERVER_SSL)
	if(req->ssl==NULL)
#endif
    print_ka_phrase(req);
}

void print_no_cache(request * req)
{
#if defined(TCSUPPORT_WEBSERVER_SSL)
	if(strstr(req->request_uri,"romfile.cfg") == NULL) 
#endif
	{
    req_write(req, "Cache-Control: no-cache\r\n");
    req_write(req, "Pragma: no-cache\r\n");
    req_write(req, "Expires: 0\r\n");
	}
}


/* The routines above are only called by the routines below.
 * The rest of Boa only enters through the routines below.
 */

/* R_REQUEST_OK: 200 */
void send_r_request_ok(request * req)
{
#ifdef BOA_DEBUG
	struct timeval tv;
	gettimeofday(&tv,NULL);
	fprintf(stderr,"send_r_request_ok:%d,%d\n",tv.tv_usec,tv.tv_sec);
#endif
    req->response_status = R_REQUEST_OK;
    if (req->simple)
        return;

    req_write(req, "HTTP/1.0 200 OK\r\n");
    print_http_headers(req);
    print_no_cache(req);

    if (!req->is_cgi) {
        print_content_length(req);
        print_last_modified(req);
        print_content_type(req);
        req_write(req, "\r\n");
    }
}

/* R_MOVED_PERM: 301 */
void send_r_moved_perm(request * req, char *url)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_PERM;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 301 Moved Permanently\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");
        req_write_escape_http(req, url);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>301 Moved Permanently</TITLE></HEAD>\n"
                  "<BODY>\n<H1>301 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_MOVED_TEMP: 302 */
void send_r_moved_temp(request * req, char *url, char *more_hdr)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_TEMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 302 Moved Temporarily\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");
        req_write_escape_http(req, url);
        req_write(req, "\r\n");
        req_write(req, more_hdr);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>302 Moved Temporarily</TITLE></HEAD>\n"
                  "<BODY>\n<H1>302 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

#if !defined(TCSUPPORT_CT)
/* R_MOVED_TEMP: 302 */
void send_r_moved_temp_new(request * req, char *url, char *more_hdr)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_TEMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 302 Moved Temporarily\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");

	/* In request url, the ipv6 ip address will be enclosed in the square brackets '[' and ']'. 
	And this square brackets should not be transferred.*/
        //req_write_escape_http(req, url);
	req_write_escape_http_new(req, url);
	 
        req_write(req, "\r\n");
        req_write(req, more_hdr);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>302 Moved Temporarily</TITLE></HEAD>\n"
                  "<BODY>\n<H1>302 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}
#endif


/* R_NOT_MODIFIED: 304 */
void send_r_not_modified(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_MODIFIED;
    req_write(req, "HTTP/1.0 304 Not Modified\r\n");
    print_http_headers(req);
    print_content_type(req);
    req_write(req, "\r\n");
    req_flush(req);
}

/* R_BAD_REQUEST: 400 */
void send_r_bad_request(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_REQUEST;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 400 Bad Request\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD)
        req_write(req,
                  "<HTML><HEAD><TITLE>400 Bad Request</TITLE></HEAD>\n"
                  "<BODY><H1>400 Bad Request</H1>\nYour client has issued "
                  "a malformed or illegal request.\n</BODY></HTML>\n");
    req_flush(req);
}
#if defined(TCSUPPORT_CD_NEW_GUI) || defined(TCSUPPORT_CT_C5_HEN_SFU)
void send_r_web_unauthorized(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_UNAUTHORIZED;
    
    if (!req->simple) {
	req_write(req, "HTTP/1.0 401 Unauthorized\r\n");
        print_http_headers(req);
        if(req->cookie == NULL)//lee 2006-4-12:set cookie
        {
        	char id[10];
		char head[256],path[256];
		int r;
		FILE *fd;
		
		srand (time(NULL));
		r = rand();
		sprintf (id, "%.8x",r);
		
		if(strlen(pre_sid) > 0)//delete old sid file
		{
			if(strcmp(pre_sid, id) != 0)
			{
				sprintf(path,"/var/tmp/%s",pre_sid);
				unlink(path);
				memset(pre_sid, 0, sizeof(pre_sid));
			}
		}
		else
		{
			strcpy(pre_sid, id);
		}

		sprintf(path,"/var/tmp/%s",id);
		fd = fopen(path,"w");
		if(fd)
			fclose(fd);
					
      		//sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s;\r\n\r\n",id);
      		sprintf(head,"Content-Type: text/html; charset=gb2312\r\nSet-Cookie: SESSIONID=%s;\r\n\r\n",id);
      		
		  #ifdef BOA_DEBUG	
          	fprintf(stderr,"authorize set cookie:%s\n",head);
		  #endif
          	req_write(req,head); /* terminate header */
        }
        else
        	//req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
        	req_write(req, "Content-Type: text/html; charset=gb2312\r\n\r\n");
	}
    if (req->method != M_HEAD){
        req_write(req,
                  "<HTML><HEAD><TITLE>Login</TITLE>"
									"<script language=javascript>"
									"top.location.replace(\"/cgi-bin/login.asp\");"
									"</script>"
									"</HEAD><body></body></HTML>\n");
    }
    req_flush(req);
}
#endif

#if defined(TCSUPPORT_CT_E8GUI)
void send_r_web_unauthorized(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_UNAUTHORIZED;
    if (!req->simple) {
	req_write(req, "HTTP/1.0 401 Unauthorized\r\n");
        print_http_headers(req);
        //if(req->cookie == NULL)//lee 2006-4-12:set cookie
        if( NULL == req->cookie
		 || (NULL != req->cookie && NULL == strstr(req->cookie, "SESSIONID"))) // should check the session key.
        {
        	char id[16];
		char head[256],path[256];
		int r;
		FILE *fd;
		
		srand (time(NULL));
		r = rand();
		sprintf (id, "boasid%.8x",r);
		
		sprintf(path,"/var/tmp/%s",id);
		fd = fopen(path,"w");
		if(fd)
			fclose(fd);
					
      		//sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s;\r\n\r\n",id);
      		sprintf(head,"Content-Type: text/html; charset=gb2312\r\nSet-Cookie: SESSIONID=%s;path=/;\r\n\r\n",id);
      		
		  #ifdef BOA_DEBUG	
          	fprintf(stderr,"authorize set cookie:%s\n",head);
		  #endif
          	req_write(req,head); /* terminate header */
        }
        else
        	//req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
        	req_write(req, "Content-Type: text/html; charset=gb2312\r\n\r\n");
    }
    if (req->method != M_HEAD){
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		if ( req->isSimCard_redirect )
		{
			req_write(req, REDIRECT_URL("InsertSimcardMsg.cgi"));
		}
		else
#endif
#if defined(CT_COM_DEVICEREG)
		if ( req->iswan_redirect )
		{
 			req_write(req, REDIRECT_URL("register.asp"));
		}
		else
#endif
#if defined(TCSUPPORT_CT_JOYME)
		if ( req->ispushview_upg)
		{
			req_write(req, REDIRECT_URL("pushviewupgrade.asp"));
		}
		else
#endif
		{
			req_write(req,
					  req->validsession ? REDIRECT_URL("logout.cgi") : REDIRECT_URL("index2.asp"));
		}
    }
    req_flush(req);
}
#endif

/* R_UNAUTHORIZED: 401 */
void send_r_unauthorized(request * req, char *realm_name)
{	
#ifdef TCSUPPORT_WEB_LOGOUT
	 SQUASH_KA(req);
	 req->response_status = R_UNAUTHORIZED;
	 if (!req->simple) {
	 req_write(req, "HTTP/1.0 401 Unauthorized\r\n");
		 print_http_headers(req);
		 if(req->cookie == NULL)//lee 2006-4-12:set cookie
		 {
			 char id[10];
			 char head[256],path[256];
			 int r;
			 FILE *fd;
			 
			 srand (time(NULL));
			 r = rand();
			 sprintf (id, "%.8x",r);
			 
			 sprintf(path,"/var/tmp/%s",id);
			 fd = fopen(path,"w");
			 if(fd)
				 fclose(fd);
					 
			 //sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s;\r\n\r\n",id);
			 sprintf(head,"Content-Type: text/html; charset=gb2312\r\nSet-Cookie: SESSIONID=%s;\r\n\r\n",id);
			 
	   #ifdef BOA_DEBUG  
			 fprintf(stderr,"authorize set cookie:%s\n",head);
	   #endif
			 req_write(req,head); /* terminate header */
		 }
		 else
			 //req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
			 req_write(req, "Content-Type: text/html; charset=gb2312\r\n\r\n");
	 }
	 if (req->method != M_HEAD){
		 req_write(req,
				   "<HTML><HEAD><TITLE>Login</TITLE>"
		   "<script language=javascript>"
		   "top.location.replace(\"/cgi-bin/login.asp\");"
		   "</script>"
				   "</HEAD><body></body></HTML>\n");
	 }
	 req_flush(req);
		
#else		
    SQUASH_KA(req);
    
    req->response_status = R_UNAUTHORIZED;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 401 Unauthorized\r\n");
        print_http_headers(req);
        req_write(req, "WWW-Authenticate: Basic realm=\"");
        req_write(req, realm_name);
        req_write(req, "\"\r\n");
#if !defined(TCSUPPORT_CZ_GENERAL) 
        if(req->cookie == NULL)//lee 2006-4-12:set cookie
#endif
        {
        	char id[10];
					char head[256],path[256];
					int r;
					FILE *fd;
					
					srand (time(NULL));
					r = rand();
					sprintf (id, "%.8x",r);
					
					sprintf(path,"/var/tmp/%s",id);
					fd = fopen(path,"w");
					if(fd)
						fclose(fd);
					
        	//sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s;expires=Wednesday,1-Jan-2020 00:00:00 GMT\r\n",id);
        	sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s;\r\n\r\n",id);
		  #ifdef BOA_DEBUG	
          fprintf(stderr,"authorize set cookie:%s\n",head);
		  #endif
          req_write(req,head); /* terminate header */
        }
        else
        	req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>401 Unauthorized</TITLE></HEAD>\n"
                  "<BODY><H1>401 Unauthorized</H1>\nYour client does not "
                  "have permission to get URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " from this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
#endif
}

/* R_FORBIDDEN: 403 */
void send_r_forbidden(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_FORBIDDEN;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 403 Forbidden\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req, "<HTML><HEAD><TITLE>403 Forbidden</TITLE></HEAD>\n"
                  "<BODY><H1>403 Forbidden</H1>\nYour client does not "
                  "have permission to get URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " from this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_NOT_FOUND: 404 */
void send_r_not_found(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_FOUND;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 404 Not Found\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req, "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>\n"
                  "<BODY><H1>404 Not Found</H1>\nThe requested URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " was not found on this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_ERROR: 500 */
void send_r_error(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_ERROR;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 500 Server Error\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>500 Server Error</TITLE></HEAD>\n"
                  "<BODY><H1>500 Server Error</H1>\nThe server encountered "
                  "an internal error and could not complete your request.\n"
                  "</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_NOT_IMP: 501 */
void send_r_not_implemented(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_IMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 501 Not Implemented\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>501 Not Implemented</TITLE></HEAD>\n"
                  "<BODY><H1>501 Not Implemented</H1>\nPOST to non-script "
                  "is not supported in Boa.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_BAD_GATEWAY: 502 */
void send_r_bad_gateway(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_GATEWAY;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 502 Bad Gateway" CRLF);
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML CRLF CRLF); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>502 Bad Gateway</TITLE></HEAD>\n"
                  "<BODY><H1>502 Bad Gateway</H1>\nThe CGI was "
                  "not CGI/1.1 compliant.\n" "</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_SERVICE_UNAVAILABLE: 503 */
void send_r_service_unavailable(request * req) /* 503 */
{
    static char body[] =
        "<HTML><HEAD><TITLE>503 Service Unavailable</TITLE></HEAD>\n"
        "<BODY><H1>503 Service Unavailable</H1>\n"
        "There are too many connections in use right now.\r\n"
        "Please try again later.\r\n</BODY></HTML>\n";
    static int _body_len;
    static char *body_len;

    if (!_body_len)
        _body_len = strlen(body);
    if (!body_len)
        body_len = strdup(simple_itoa(_body_len));
    if (!body_len) {
        log_error_time();
        perror("strdup of _body_len from simple_itoa");
    }


    SQUASH_KA(req);
    req->response_status = R_SERVICE_UNAV;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 503 Service Unavailable\r\n");
        print_http_headers(req);
        if (body_len) {
            req_write(req, "Content-Length: ");
            req_write(req, body_len);
            req_write(req, "\r\n");
        }
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header
                                                               */
    }
    if (req->method != M_HEAD) {
        req_write(req, body);
    }
    req_flush(req);
}


/* R_NOT_IMP: 505 */
void send_r_bad_version(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_VERSION;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 505 HTTP Version Not Supported\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>505 HTTP Version Not Supported</TITLE></HEAD>\n"
                  "<BODY><H1>505 HTTP Version Not Supported</H1>\nHTTP versions "
                  "other than 0.9 and 1.0 "
                  "are not supported in Boa.\n<p><p>Version encountered: ");
        req_write(req, req->http_version);
        req_write(req, "<p><p></BODY></HTML>\n");
    }
    req_flush(req);
}
