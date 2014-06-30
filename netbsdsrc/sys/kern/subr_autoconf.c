/*	$NetBSD: subr_autoconf.c,v 1.27 1997/09/20 14:16:43 drochner Exp $	*/

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
 *
 * All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Lawrence Berkeley Laboratories.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * from: Header: subr_autoconf.c,v 1.12 93/02/01 19:31:48 torek Exp  (LBL)
 *
 *	@(#)subr_autoconf.c	8.1 (Berkeley) 6/10/93
 */

#include <sys/param.h>
#include <sys/device.h>
#include <sys/malloc.h>
#include <sys/systm.h>
#include <machine/limits.h>

/*
 * Autoconfiguration subroutines.
 */

/*
 * ioconf.c exports exactly two names: cfdata and cfroots.  All system
 * devices and drivers are found via these tables.
 */
extern struct cfdata cfdata[];
extern short cfroots[];

#define	ROOT ((struct device *)NULL)

#ifdef __BROKEN_INDIRECT_CONFIG
struct device *config_make_softc __P((struct device *, struct cfdata *));
#endif

struct matchinfo {
	cfmatch_t fn;
	struct	device *parent;
	void	*aux;
#ifdef __BROKEN_INDIRECT_CONFIG
	void	*match;
	int	indirect;
#else
	struct	cfdata *match;
#endif
	int	pri;
};

static char *number __P((char *, int));
static void mapply __P((struct matchinfo *, struct cfdata *));

struct devicelist alldevs;		/* list of all devices */
struct evcntlist allevents;		/* list of all event counters */

/*
 * Initialize autoconfiguration data structures.
 */
void
config_init()
{

	TAILQ_INIT(&alldevs);
	TAILQ_INIT(&allevents);
}

/*
 * Apply the matching function and choose the best.  This is used
 * a few times and we want to keep the code small.
 */
static void
mapply(m, cf)
	register struct matchinfo *m;
	register struct cfdata *cf;
{
	register int pri;
#ifdef __BROKEN_INDIRECT_CONFIG
	void *match;

	if (m->indirect)
		match = config_make_softc(m->parent, cf);
	else
		match = cf;
#endif

	if (m->fn != NULL)
#ifdef __BROKEN_INDIRECT_CONFIG
		pri = (*m->fn)(m->parent, match, m->aux);
#else
		pri = (*m->fn)(m->parent, cf, m->aux);
#endif
	else {
	        if (cf->cf_attach->ca_match == NULL) {
			panic("mapply: no match function for '%s' device\n",
			    cf->cf_driver->cd_name);
		}
#ifdef __BROKEN_INDIRECT_CONFIG
		pri = (*cf->cf_attach->ca_match)(m->parent, match, m->aux);
#else
		pri = (*cf->cf_attach->ca_match)(m->parent, cf, m->aux);
#endif
	}
	if (pri > m->pri) {
#ifdef __BROKEN_INDIRECT_CONFIG
		if (m->indirect && m->match)
			free(m->match, M_DEVBUF);
		m->match = match;
#else
		m->match = cf;
#endif
		m->pri = pri;
#ifdef __BROKEN_INDIRECT_CONFIG
	} else {
		if (m->indirect)
			free(match, M_DEVBUF);
#endif
	}
}

/*
 * Iterate over all potential children of some device, calling the given
 * function (default being the child's match function) for each one.
 * Nonzero returns are matches; the highest value returned is considered
 * the best match.  Return the `found child' if we got a match, or NULL
 * otherwise.  The `aux' pointer is simply passed on through.
 *
 * Note that this function is designed so that it can be used to apply
 * an arbitrary function to all potential children (its return value
 * can be ignored).
 */
#ifdef __BROKEN_INDIRECT_CONFIG
void *
#else
struct cfdata *
#endif
config_search(fn, parent, aux)
	cfmatch_t fn;
	register struct device *parent;
	void *aux;
{
	register struct cfdata *cf;
	register short *p;
	struct matchinfo m;

	m.fn = fn;
	m.parent = parent;
	m.aux = aux;
	m.match = NULL;
#ifdef __BROKEN_INDIRECT_CONFIG
	m.indirect = parent && parent->dv_cfdata->cf_driver->cd_indirect;
#endif
	m.pri = 0;
	for (cf = cfdata; cf->cf_driver; cf++) {
		/*
		 * Skip cf if no longer eligible, otherwise scan through
		 * parents for one matching `parent', and try match function.
		 */
		if (cf->cf_fstate == FSTATE_FOUND)
			continue;
		for (p = cf->cf_parents; *p >= 0; p++)
			if (parent->dv_cfdata == &cfdata[*p])
				mapply(&m, cf);
	}
	return (m.match);
}

#ifdef __BROKEN_INDIRECT_CONFIG
/*
 * Iterate over all potential children of some device, calling the given
 * function for each one.
 *
 * Note that this function is designed so that it can be used to apply
 * an arbitrary function to all potential children (its return value
 * can be ignored).
 */
void
config_scan(fn, parent)
	cfscan_t fn;
	register struct device *parent;
{
	register struct cfdata *cf;
	register short *p;
	void *match;
	int indirect;

	indirect = parent && parent->dv_cfdata->cf_driver->cd_indirect;
	for (cf = cfdata; cf->cf_driver; cf++) {
		/*
		 * Skip cf if no longer eligible, otherwise scan through
		 * parents for one matching `parent', and try match function.
		 */
		if (cf->cf_fstate == FSTATE_FOUND)
			continue;
		for (p = cf->cf_parents; *p >= 0; p++)
			if (parent->dv_cfdata == &cfdata[*p]) {
				if (indirect)
					match = config_make_softc(parent, cf);
				else
					match = cf;
				(*fn)(parent, match);
			}
	}
}
#endif /* __BROKEN_INDIRECT_CONFIG */

/*
 * Find the given root device.
 * This is much like config_search, but there is no parent.
 */
#ifdef __BROKEN_INDIRECT_CONFIG
void *
#else
struct cfdata *
#endif
config_rootsearch(fn, rootname, aux)
	register cfmatch_t fn;
	register char *rootname;
	register void *aux;
{
	register struct cfdata *cf;
	register short *p;
	struct matchinfo m;

	m.fn = fn;
	m.parent = ROOT;
	m.aux = aux;
	m.match = NULL;
#ifdef __BROKEN_INDIRECT_CONFIG
	m.indirect = 0;
#endif
	m.pri = 0;
	/*
	 * Look at root entries for matching name.  We do not bother
	 * with found-state here since only one root should ever be
	 * searched (and it must be done first).
	 */
	for (p = cfroots; *p >= 0; p++) {
		cf = &cfdata[*p];
		if (strcmp(cf->cf_driver->cd_name, rootname) == 0)
			mapply(&m, cf);
	}
	return (m.match);
}

static char *msgs[3] = { "", " not configured\n", " unsupported\n" };

/*
 * The given `aux' argument describes a device that has been found
 * on the given parent, but not necessarily configured.  Locate the
 * configuration data for that device (using the submatch function
 * provided, or using candidates' cd_match configuration driver
 * functions) and attach it, and return true.  If the device was
 * not configured, call the given `print' function and return 0.
 */
struct device *
config_found_sm(parent, aux, print, submatch)
	struct device *parent;
	void *aux;
	cfprint_t print;
	cfmatch_t submatch;
{
#ifdef __BROKEN_INDIRECT_CONFIG
	void *match;

	if ((match = config_search(submatch, parent, aux)) != NULL)
		return (config_attach(parent, match, aux, print));
#else
	struct cfdata *cf;

	if ((cf = config_search(submatch, parent, aux)) != NULL)
		return (config_attach(parent, cf, aux, print));
#endif
	if (print)
		printf(msgs[(*print)(aux, parent->dv_xname)]);
	return (NULL);
}

/*
 * As above, but for root devices.
 */
struct device *
config_rootfound(rootname, aux)
	char *rootname;
	void *aux;
{
#ifdef __BROKEN_INDIRECT_CONFIG
	void *match;

	if ((match = config_rootsearch((cfmatch_t)NULL, rootname, aux)) != NULL)
		return (config_attach(ROOT, match, aux, (cfprint_t)NULL));
#else
	struct cfdata *cf;

	if ((cf = config_rootsearch((cfmatch_t)NULL, rootname, aux)) != NULL)
		return (config_attach(ROOT, cf, aux, (cfprint_t)NULL));
#endif
	printf("root device %s not configured\n", rootname);
	return (NULL);
}

/* just like sprintf(buf, "%d") except that it works from the end */
static char *
number(ep, n)
	register char *ep;
	register int n;
{

	*--ep = 0;
	while (n >= 10) {
		*--ep = (n % 10) + '0';
		n /= 10;
	}
	*--ep = n + '0';
	return (ep);
}

/*
 * Attach a found device.  Allocates memory for device variables.
 */
#ifdef __BROKEN_INDIRECT_CONFIG
struct device *
config_attach(parent, match, aux, print)
	register struct device *parent;
	void *match;
	register void *aux;
	cfprint_t print;
{
	register struct cfdata *cf;
	register struct device *dev;
	register struct cfdriver *cd;
	register struct cfattach *ca;

	if (parent && parent->dv_cfdata->cf_driver->cd_indirect) {
		dev = match;
		cf = dev->dv_cfdata;
	} else {
		cf = match;
		dev = config_make_softc(parent, cf);
	}

	cd = cf->cf_driver;
	ca = cf->cf_attach;
	cd->cd_devs[cf->cf_unit] = dev;

	if (cf->cf_fstate == FSTATE_STAR)
		cf->cf_unit++;
	else {
		KASSERT(cf->cf_fstate == FSTATE_NOTFOUND);
		cf->cf_fstate = FSTATE_FOUND;
	}

	TAILQ_INSERT_TAIL(&alldevs, dev, dv_list);

	if (parent == ROOT)
		printf("%s (root)", dev->dv_xname);
	else {
		printf("%s at %s", dev->dv_xname, parent->dv_xname);
		if (print)
			(void) (*print)(aux, (char *)0);
	}

	/*
	 * Before attaching, clobber any unfound devices that are
	 * otherwise identical, or bump the unit number on all starred
	 * cfdata for this device.
	 */
	for (cf = cfdata; cf->cf_driver; cf++)
		if (cf->cf_driver == cd && cf->cf_unit == dev->dv_unit) {
			if (cf->cf_fstate == FSTATE_NOTFOUND)
				cf->cf_fstate = FSTATE_FOUND;
			if (cf->cf_fstate == FSTATE_STAR)
				cf->cf_unit++;
		}
#if defined(__alpha__) || defined(hp300) || defined(__i386__)
	device_register(dev, aux);
#endif
	(*ca->ca_attach)(parent, dev, aux);
	return (dev);
}

struct device *
config_make_softc(parent, cf)
	struct device *parent;
	struct cfdata *cf;
{
	register struct device *dev;
	register struct cfdriver *cd;
	register struct cfattach *ca;
	register size_t lname, lunit;
	register char *xunit;
	char num[10];

	cd = cf->cf_driver;
	ca = cf->cf_attach;
	if (ca->ca_devsize < sizeof(struct device))
		panic("config_make_softc");

	/* compute length of name and decimal expansion of unit number */
	lname = strlen(cd->cd_name);
	xunit = number(&num[sizeof num], cf->cf_unit);
	lunit = &num[sizeof num] - xunit;
	if (lname + lunit >= sizeof(dev->dv_xname))
		panic("config_attach: device name too long");

	/* get memory for all device vars */
	dev = (struct device *)malloc(ca->ca_devsize, M_DEVBUF, M_NOWAIT);
	if (!dev)
	    panic("config_attach: memory allocation for device softc failed");
	bzero(dev, ca->ca_devsize);
	dev->dv_class = cd->cd_class;
	dev->dv_cfdata = cf;
	dev->dv_unit = cf->cf_unit;
	bcopy(cd->cd_name, dev->dv_xname, lname);
	bcopy(xunit, dev->dv_xname + lname, lunit);
	dev->dv_parent = parent;

	/* put this device in the devices array */
	if (dev->dv_unit >= cd->cd_ndevs) {
		/*
		 * Need to expand the array.
		 */
		int old = cd->cd_ndevs, new;
		void **nsp;

		if (old == 0)
			new = MINALLOCSIZE / sizeof(void *);
		else
			new = old * 2;
		while (new <= dev->dv_unit)
			new *= 2;
		cd->cd_ndevs = new;
		nsp = malloc(new * sizeof(void *), M_DEVBUF, M_NOWAIT);	
		if (nsp == 0)
			panic("config_attach: %sing dev array",
			    old != 0 ? "expand" : "creat");
		bzero(nsp + old, (new - old) * sizeof(void *));
		if (old != 0) {
			bcopy(cd->cd_devs, nsp, old * sizeof(void *));
			free(cd->cd_devs, M_DEVBUF);
		}
		cd->cd_devs = nsp;
	}
	if (cd->cd_devs[dev->dv_unit])
		panic("config_attach: duplicate %s", dev->dv_xname);

	return (dev);
}
#else /* __BROKEN_INDIRECT_CONFIG */
struct device *
config_attach(parent, cf, aux, print)
	register struct device *parent;
	register struct cfdata *cf;
	register void *aux;
	cfprint_t print;
{
	register struct device *dev;
	register struct cfdriver *cd;
	register struct cfattach *ca;
	register size_t lname, lunit;
	register char *xunit;
	int myunit;
	char num[10];

	cd = cf->cf_driver;
	ca = cf->cf_attach;
	if (ca->ca_devsize < sizeof(struct device))
		panic("config_attach");
	myunit = cf->cf_unit;
	if (cf->cf_fstate == FSTATE_STAR)
		cf->cf_unit++;
	else {
		KASSERT(cf->cf_fstate == FSTATE_NOTFOUND);
		cf->cf_fstate = FSTATE_FOUND;
	}

	/* compute length of name and decimal expansion of unit number */
	lname = strlen(cd->cd_name);
	xunit = number(&num[sizeof num], myunit);
	lunit = &num[sizeof num] - xunit;
	if (lname + lunit >= sizeof(dev->dv_xname))
		panic("config_attach: device name too long");

	/* get memory for all device vars */
	dev = (struct device *)malloc(ca->ca_devsize, M_DEVBUF, M_NOWAIT);
	if (!dev)
	    panic("config_attach: memory allocation for device softc failed");
	bzero(dev, ca->ca_devsize);
	TAILQ_INSERT_TAIL(&alldevs, dev, dv_list);	/* link up */
	dev->dv_class = cd->cd_class;
	dev->dv_cfdata = cf;
	dev->dv_unit = myunit;
	bcopy(cd->cd_name, dev->dv_xname, lname);
	bcopy(xunit, dev->dv_xname + lname, lunit);
	dev->dv_parent = parent;
	if (parent == ROOT)
		printf("%s (root)", dev->dv_xname);
	else {
		printf("%s at %s", dev->dv_xname, parent->dv_xname);
		if (print)
			(void) (*print)(aux, (char *)0);
	}

	/* put this device in the devices array */
	if (dev->dv_unit >= cd->cd_ndevs) {
		/*
		 * Need to expand the array.
		 */
		int old = cd->cd_ndevs, new;
		void **nsp;

		if (old == 0)
			new = MINALLOCSIZE / sizeof(void *);
		else
			new = old * 2;
		while (new <= dev->dv_unit)
			new *= 2;
		cd->cd_ndevs = new;
		nsp = malloc(new * sizeof(void *), M_DEVBUF, M_NOWAIT);	
		if (nsp == 0)
			panic("config_attach: %sing dev array",
			    old != 0 ? "expand" : "creat");
		bzero(nsp + old, (new - old) * sizeof(void *));
		if (old != 0) {
			bcopy(cd->cd_devs, nsp, old * sizeof(void *));
			free(cd->cd_devs, M_DEVBUF);
		}
		cd->cd_devs = nsp;
	}
	if (cd->cd_devs[dev->dv_unit])
		panic("config_attach: duplicate %s", dev->dv_xname);
	cd->cd_devs[dev->dv_unit] = dev;

	/*
	 * Before attaching, clobber any unfound devices that are
	 * otherwise identical, or bump the unit number on all starred
	 * cfdata for this device.
	 */
	for (cf = cfdata; cf->cf_driver; cf++)
		if (cf->cf_driver == cd && cf->cf_unit == dev->dv_unit) {
			if (cf->cf_fstate == FSTATE_NOTFOUND)
				cf->cf_fstate = FSTATE_FOUND;
			if (cf->cf_fstate == FSTATE_STAR)
				cf->cf_unit++;
		}
#if defined(__alpha__) || defined(hp300) || defined(__i386__)
	device_register(dev, aux);
#endif
	(*ca->ca_attach)(parent, dev, aux);
	return (dev);
}
#endif /* __BROKEN_INDIRECT_CONFIG */

/*
 * Attach an event.  These must come from initially-zero space (see
 * commented-out assignments below), but that occurs naturally for
 * device instance variables.
 */
void
evcnt_attach(dev, name, ev)
	struct device *dev;
	const char *name;
	struct evcnt *ev;
{

#ifdef DIAGNOSTIC
	if (strlen(name) >= sizeof(ev->ev_name))
		panic("evcnt_attach");
#endif
	/* ev->ev_next = NULL; */
	ev->ev_dev = dev;
	/* ev->ev_count = 0; */
	strcpy(ev->ev_name, name);
	TAILQ_INSERT_TAIL(&allevents, ev, ev_list);
}