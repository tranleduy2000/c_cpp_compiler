# Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
# Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.

The YAP Prolog interface to the PPL is available as a dynamically
linked module.  Only YAP version 5.1.1 and following are supported.

In order to dynamically load the library from YAP you should simply
load `<prefix>/lib/ppl/ppl_yap.pl', where <prefix> is the root of your
installation (`/usr' or `/usr/local' in most cases).  This will invoke
`ppl_initialize/0' automatically; it is the programmer's
responsibility to call `ppl_finalize/0' when the PPL library is no
longer needed.  Notice that, for dynamic linking to work, you should
have configured the library with the `--enable-shared' option (on most
platforms this is the default).
