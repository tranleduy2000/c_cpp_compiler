# Automake source file for the Parma Polyhedra Library.
# Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
# Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
#
# This file is part of the Parma Polyhedra Library (PPL).
#
# The PPL is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 3 of the License, or (at your
# option) any later version.
#
# The PPL is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
#
# For the most up-to-date information see the Parma Polyhedra Library
# site: http://bugseng.com/products/ppl/ .

# Tell aclocal where to find `.m4' files.
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = . utils src tests interfaces demos doc m4

dist_noinst_SCRIPTS = \
configure

nodist_noinst_HEADERS = \
ppl-config.h

dist_noinst_HEADERS = \
instchk.hh

ppl-config.h: config.h $(srcdir)/ppl-config.sed
	$(SED) -f $(srcdir)/ppl-config.sed $< >$@

ppl-config.h.in: config.h.in $(srcdir)/ppl-config.sed
	$(SED) -f $(srcdir)/ppl-config.sed $< >$@

DISTCHECK_CONFIGURE_FLAGS = \
--enable-shared

# Kludge: we need to force building `ppl-config.h' on `make dist',
# hence we list it in `EXTRA_DIST';  but we do not want to distribute
# it, hence we erase it in `dist-hook'.
dist-hook:
	rm -f $(distdir)/ppl-config.h \
	&& $(SED) -e "s/^enableval=thorough$$/enableval=quick/g" \
	          -e "s/^default_interfaces=\$${available_interfaces}$$/default_interfaces=\$${non_prolog_interfaces}/g" \
		<$(distdir)/configure.ac >$(distdir)/configure.ac.new \
	&& touch $(distdir)/configure.ac.new -r $(distdir)/configure.ac \
	&& mv -f $(distdir)/configure.ac.new $(distdir)/configure.ac
	$(SED) -e "s/^enableval=thorough$$/enableval=quick/g" \
	          -e "s/^default_interfaces=\$${available_interfaces}$$/default_interfaces=\$${non_prolog_interfaces}/g" \
		<$(distdir)/configure >$(distdir)/configure.new \
	&& touch $(distdir)/configure.new -r $(distdir)/configure \
	&& mv -f $(distdir)/configure.new $(distdir)/configure \
	&& chmod +x $(distdir)/configure

# Kludge: we need to force building `ppl-config.h' on `make dist',
# hence we list it in `EXTRA_DIST';  but we do not want to distribute
# it, hence we erase it in `dist-hook'.
EXTRA_DIST = \
ppl-config.h \
ppl-config.h.in \
ppl-config.sed \
ppl.lsm.in \
BUGS \
CREDITS \
ChangeLog_2001-2008 \
README.alpha \
README.arm \
README.configure \
README.solaris \
STANDARDS

DISTCLEANFILES = \
ppl-config.h

distcleancheck_listfiles = \
  find -type f -exec sh -c 'test -f $(srcdir)/{} || echo {}' ';'
