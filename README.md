Demonstrating autotools inconsistent quoting:

Generated with `autoreconf -fvi` (autoconf 2.71, automake 1.16.5). 


Then, trying with two levels of quoting:
```
$ CFLAGS='-Wno-builtin-macro-redefined -D__DATE__=\"redacted\" -Werror' ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a race-free mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking whether to enable maintainer-specific portions of Makefiles... no
checking for gcc... gcc
checking whether the C compiler works... no
configure: error: in `/tmp/autoconf-2.71':
configure: error: C compiler cannot create executables
See `config.log' for more details

$ grep -C2 3404 config.log
configure:3362: $? = 1
configure:3382: checking whether the C compiler works
configure:3404: gcc -Wno-builtin-macro-redefined -D__DATE__=\"redacted\" -Werror   conftest.c  >&5
<command-line>: error: missing terminating " character [-Werror]
cc1: all warnings being treated as errors
```

Okay. Let's remove a level of quotes:

```
$ CFLAGS='-Wno-builtin-macro-redefined -D__DATE__="redacted" -Werror' ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a race-free mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking whether to enable maintainer-specific portions of Makefiles... no
checking for gcc... gcc
checking whether the C compiler works... yes
checking for C compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether the compiler supports GNU C... yes
checking whether gcc accepts -g... yes
checking for gcc option to enable C11 features... none needed
checking whether gcc understands -c and -o together... yes
checking whether make supports the include directive... yes (GNU style)
checking dependency style of gcc... gcc3
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating config.h
config.status: executing depfiles commands

$ make
make  all-am
make[1]: Entering directory '/tmp/autoconf-2.71'
gcc -DHAVE_CONFIG_H -I.     -Wno-builtin-macro-redefined -D__DATE__="redacted" -Werror -MT hello-world.o -MD -MP -MF .deps/hello-world.Tpo -c -o hello-world.o hello-world.c
hello-world.c: In function ‘main’:
<command-line>: error: ‘redacted’ undeclared (first use in this function)
hello-world.c:5:19: note: in expansion of macro ‘__DATE__’
    5 |   char actual[] = __DATE__;
      |                   ^~~~~~~~
<command-line>: note: each undeclared identifier is reported only once for each function it appears in
hello-world.c:5:19: note: in expansion of macro ‘__DATE__’
    5 |   char actual[] = __DATE__;
      |                   ^~~~~~~~
make[1]: *** [Makefile:598: hello-world.o] Error 1
make[1]: Leaving directory '/tmp/autoconf-2.71'
make: *** [Makefile:481: all] Error 2
```

So, with the two-level quoting, the compiler check fails; with the one-level quoting, the compile run fails (because
`__DATE__` is set to `redacted` instead of `"redacted"`). So, what happens if I go back to two-level quoting but remove
`-Werror`?

```
$ CFLAGS='-Wno-builtin-macro-redefined -D__DATE__=\"redacted\"' ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a race-free mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking whether to enable maintainer-specific portions of Makefiles... no
checking for gcc... gcc
checking whether the C compiler works... yes
checking for C compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether the compiler supports GNU C... yes
checking whether gcc accepts -g... yes
checking for gcc option to enable C11 features... none needed
checking whether gcc understands -c and -o together... yes
checking whether make supports the include directive... yes (GNU style)
checking dependency style of gcc... gcc3
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating config.h
config.status: executing depfiles commands


$ make check
gcc -DHAVE_CONFIG_H -I.     -Wno-builtin-macro-redefined -D__DATE__=\"redacted\" -MT hello-world.o -MD -MP -MF .deps/hello-world.Tpo -c -o hello-world.o hello-world.c
mv -f .deps/hello-world.Tpo .deps/hello-world.Po
gcc  -Wno-builtin-macro-redefined -D__DATE__=\"redacted\"   -o hello-world hello-world.o  
make  hello-world
make[1]: Entering directory '/tmp/autoconf-2.71'
make[1]: 'hello-world' is up to date.
make[1]: Leaving directory '/tmp/autoconf-2.71'
make  check-TESTS
make[1]: Entering directory '/tmp/autoconf-2.71'
make[2]: Entering directory '/tmp/autoconf-2.71'
PASS: hello-world
============================================================================
Testsuite summary for hello 1.0
============================================================================
# TOTAL: 1
# PASS:  1
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[2]: Leaving directory '/tmp/autoconf-2.71'
make[1]: Leaving directory '/tmp/autoconf-2.71'
```

So, it appears that autoconf isn't even self-consistent; the built-in compiler checks use different quoting rules than
the makefiles that get generated.

