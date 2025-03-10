/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/minixml.h#1 $ */
/* minimal xml parser
 *
 * Project : miniupnp
 * Website : http://miniupnp.free.fr/
 * Author : Thomas Bernard
 * Copyright (c) 2005 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution.
 * */
#ifndef __MINIXML_H__
#define __MINIXML_H__
#define IS_WHITE_SPACE(c) ((c==' ') || (c=='\t') || (c=='\r') || (c=='\n'))

/* if a callback function pointer is set to NULL,
 * the function is not called */
struct xmlparser {
	const char *xmlstart;
	const char *xmlend;
	const char *xml;	/* pointer to current character */
	int xmlsize;
	void * data;
	void (*starteltfunc) (void *, const char *, int);
	void (*endeltfunc) (void *, const char *, int);
	void (*datafunc) (void *, const char *, int);
	void (*attfunc) (void *, const char *, int, const char *, int);
};

/* parsexml()
 * the xmlparser structure must be initialized before the call
 * the following structure members have to be initialized :
 * xmlstart, xmlsize, data, *func
 * xml is for internal usage, xmlend is computed automatically */
void parsexml(struct xmlparser *);

#endif

