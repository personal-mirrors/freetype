# FreeType Compilation Guide

FreeType's official git repository is located at

>https://gitlab.freedesktop.org/freetype
 
from  which the  'freetype.git' and  'freetype-demos.git' repositories
can be cloned in the usual way.

FreeType:

```bash
git clone https://gitlab.freedesktop.org/freetype/freetype.git
```

FreeType-demos:
```bash
git clone https://gitlab.freedesktop.org/freetype/freetype-demos.git
```

If you  want to  use the  Savannah mirror  instead, you  have to  do a
slightly different  incantation because  the repository  names contain
digit '2' for historical reasons.

```bash
git clone https://git.savannah.nongnu.org/git/freetype/freetype2.git
```

```bash
git clone https://git.savannah.nongnu.org/git/freetype/freetype2-demos.git
```

---
# Building FreeType
There are  several ways  to build the  FreeType library,  depending on
your system and the level of  customization you need.  Here is a short
overview of the documentation available:


## A. Prerequisites and dependencies

  FreeType is a low level C library  that only depends on the standard
  C library with very few platform-dependent optimizations utilized at
  build time.  System libraries, such as  zlib,  Gzip,  bzip2, Brotli,
  and libpng,  might  be  used  to handle  compressed fonts  or decode
  embedded PNG glyphs.

  FreeType  auto-configuration scripts  should be  able to detect  the
  prerequisites if the necessary headers are available  at the default
  locations.  Otherwise,  modify  `include/freetype/config/ftoption.h`
  to control how the FreeType library gets built.  Normally, you don't
  need to change anything.

  Applications have very limited control over FreeType's behaviour  at
  run-time;  look at the documentation of function `FT_Property_Set`.


## B. Normal installation and upgrades
 - [Unix and unix-like systems](#1-unix-and-unix-like-systems)
 - [Mac OS](#2-mac-os)
 - [Other systems using GNU make](#3-other-systems-using-gnu-make)
 - [On VMS with the `mms` build tool](#6-on-vms-with-the-mms-build-tool)
### 1. Unix and Unix-like systems
---

1. Ensure that you are using GNU Make

    The FreeType build system  _exclusively_ works with GNU Make.  You
    will  not be  able to  compile the  library with  the instructions
    below using any other alternative (including BSD Make).

    Check that you have GNU make by running the command:
    ```bash
    make -v
    ```

    This should dump some text that begins with:

       GNU Make  <version number>
       Copyright (C) <year> Free Software Foundation Inc.

    Note that version  3.81 or higher is *required* or the  build will
    fail.

    It is also fine to have GNU Make under another name (e.g. 'gmake')
    if you use the MAKE variable as described below.

    As  a  special exception,  'makepp'  can  also  be used  to  build
    FreeType 2.  See the file docs/MAKEPP for details.

    For builds  with `cmake` please check file `CMakeLists.txt`;  this
    is a contributed file not directly supported by the FreeType team.


2. Regenerate the configure script if needed

    This only applies if you  are building a git snapshot or checkout,
    *not* if you grabbed the sources of an official release.

    You  need  to invoke  the  `autogen.sh`  script  in the  top-level
    directory  in order  to  create the  `configure`  script for  your
    platform.  Normally, this simply means typing:
    ```shell
    sh autogen.sh
    ```

    In case of problems, you may  need to install or upgrade Automake,
    Autoconf or Libtool.  See  `README.md` in the top-level directory
    for more information.


3. Build and install the library

    Say

       ./configure --help

    to see  the list of  possible configuration options  and important
    environment  variables.  The ./configure script  will detect  some
    prerequisite  system  libraries  (libpng, brotli, etc.)  if  their
    headers are available at the default locations.

    The following  should work  on all Unix  systems where  the `make`
    command invokes GNU Make:
    ```bash
    ./configure [options]
    make
    make install           (as root)
    ```

    The default installation path  is `/usr/local`.  It can be changed
    with the `--prefix=<path>` option.  Example:
    ```bash
    ./configure --prefix=/usr
    ```

    When using  a different command  to invoke GNU Make,  use the MAKE
    variable.  For example,  if `gmake' is the command  to use on your
    system, do something like:
    ```bash
    MAKE=gmake ./configure [options]
    gmake
    gmake install            (as root)
    ```

    If  this still doesn't  work, there  must be  a problem  with your
    system (e.g., you are using a very old version of GNU Make).

    For library identification, FreeType's `configure` script uses the
    `pkg-config` interface: Assuming it  needs library `foo`, it calls
    the  `pkg-config` program  to find  information on  library `foo`,
    which in turn  looks for a `foo.pc` file installed  at the system.
    Some platforms,  however, don't come with  `pkg-support`; you then
    have  to  use environment  variables  as  described by  `configure
    --help`.  Example:
    ```bash
    LIBPNG_CFLAGS="-I/path/to/libpng/include/directory" \
    LIBPNG_LIBS="-L/path/to/libpng/lib/directory" \
    configure ...
    ```
    It  is possible  to  compile FreeType  in  a different  directory.
    Assuming the FreeType source  files in directory `/src/freetype` a
    compilation in directory `foo` works as follows:
    ```bash
       cd foo
       /src/freetype/configure [options]
       make
       make install
    ```


4. Note: Interdependency with HarfBuzz

    Note that there  is a chicken-and-egg problem  currently since the
    HarfBuzz library  (used by the  auto-hinter to improve  support of
    OpenType  fonts)  depends on  FreeType,  which  can be  solved  as
    follows in case HarfBuzz is not yet installed on your system.

    1. Call    FreeType's     `configure`    script     with    option
       `--without-harfbuzz`, then compile and install FreeType.

    2. Compile and install HarfBuzz.

    3. Call    FreeType's    `configure`   script    without    option
       `--without-harfbuzz` (after  executing `make  distclean`), then
       compile and install FreeType again.


### 2. Mac OS
---

Please follow the instructions in  INSTALL.UNIX to install FreeType on
Mac OS X.

Currently  FreeType2 functions  based on  some deprecated  Carbon APIs
return  `FT_Err_Unimplemented_Feature` always,  even  if FreeType2  is
configured and  built on  the system that  deprecated Carbon  APIs are
available.   To  enable  deprecated  FreeType2  functions  as  far  as
possible, replace `src/base/ftmac.c` by `builds/mac/ftmac.c`.

Starting with  Mac OS X  10.5, gcc  defaults the deployment  target to
10.5.  In previous  versions of Mac OS X, this  defaulted to 10.1.  If
you want your built binaries to run only on 10.5, this change does not
concern you.  If  you want them to  also run on older  versions of Mac
OS  X,   then  you   must  either  set   the  MACOSX_DEPLOYMENT_TARGET
environment  variable  or  pass `-mmacosx-version-min`  to  gcc.   You
should specify the oldest  version of Mac OS you want  the code to run
on.  For example, if you use Bourne shell:
```bash
export MACOSX_DEPLOYMENT_TARGET=10.2
```
or, if you use C shell:

```bash
setenv MACOSX_DEPLOYMENT_TARGET 10.2
```
Alternatively, you could pass `-mmacosx-version-min=10.2` to gcc.

Here the number 10.2 is the lowest version that the built binaries can
run on.  In the  above cases, the built binaries will run  on Mac OS X
10.2 and later, but _not_ earlier.  If you want to run on earlier, you
have to set lower version, e.g., 10.0.

For classic Mac OS (Mac OS 7, 8, 9) please refer to builds/mac/README.

### 3. Other systems using GNU Make
---

> Note  that if you are
running  Cygwin  or  MinGW/MSYS  in Windows,  you  should  follow  the
instructions in the file `INSTALL.UNIX' instead.

> Note that *NO OTHER  MAKE TOOL
WILL  WORK*[1]!   This  methods   supports  several  compilers  on
Windows, OS/2, and BeOS,  including MinGW* (without MSYS*), Visual
C++, Borland C++, and more.


1. Install GNU Make

   The FreeType 2 build system relies on many features special to GNU
   Make.

   NEARLY ALL OTHER MAKE TOOLS  FAIL, INCLUDING `BSD MAKE`, SO REALLY
   INSTALL A RECENT VERSION OF GNU MAKE ON YOUR SYSTEM!

   Note that  make++, a  make tool written  in Perl,  supports enough
   features of GNU make to compile FreeType.  See

       https://makepp.sourceforge.net

   for more information;  you need version 2.0 or newer, and you must
   pass option `--norc-substitution`.

   Make sure that you are invoking GNU Make from the command line, by
   typing something like:

   ```bash
   make -v
   ```

   to display its version number.

   VERSION 3.81 OR NEWER IS NEEDED!


2. Invoke `make`

   Go to  the root  directory of FreeType  2, then simply  invoke GNU
   Make from the command line.   This will launch the FreeType 2 host
   platform  detection routines.   A summary  will be  displayed, for
   example, on Win32.

       FreeType build system -- automatic system detection

       The following settings are used:

       platform                     windows
       compiler                     gcc
       configuration directory      .\builds\windows
       configuration rules          .\builds\windows\w32-gcc.mk

   If this does not correspond to your system or settings please
   remove the file 'config.mk' from this directory then read the
   INSTALL file for help.

   Otherwise, simply type 'make' again to build the library
   or 'make refdoc' to build the API reference (the latter needs
   Python >= 3.5).


   If the detected settings correspond to your platform and compiler,
   skip to step 5.  Note that if your platform is completely alien to
   the build system, the detected platform will be `ansi`.


3. Configure the build system for a different compiler

   If the build system correctly detected your platform, but you want
   to use a different compiler  than the one specified in the summary
   (for most platforms, gcc is the default compiler), invoke GNU Make
   with

       make setup <compiler>

   Examples:

   | To use | type |
   | ---  | ---     |
   |Visual C++ on Win32 | `make setup visualc`|
   |Borland C++ on Win32 | `make setup bcc32 `|
   |Watcom C++ on Win32 | `make setup watcom`|
   |Intel C++ on Win32 | `make setup intelc`|
   |LCC-Win32 on Win32 | `make setup lcc`|
   |Watcom C++ on OS/2 | `make setup watcom`|
   |VisualAge C++ on OS/2 | `make setup visualage`|

   The  <compiler> name to  use is  platform-dependent.  The  list of
   available  compilers for  your  system is  available  in the  file
   `builds/<system>/detect.mk`.

   If you  are satisfied  by the new  configuration summary,  skip to
   step 5.

   - Use clang instead of gcc

     The `clang`  compiler can  use FreeType's setup  for `gcc`;  it is
     sufficient to set the `CC` variable, for example
     ```bash
     make CC=clang
     ```


   - Compiling with a C++ compiler

     FreeType can be built with a C++ compiler, for example

     ```bash
     make CC="g++"
     ```

     If `clang++` should  be used it is necessary to  also override the
     `ANSIFLAGS` variable:

     ```bash
     make CC="clang++" ANSIFLAGS=""
     ```


4. Configure the build system for an unknown platform/compiler

   The auto-detection/setup  phase of the build system  copies a file
   to the current directory under the name `config.mk`.

   For    example,    on    OS/2+gcc,    it   would    simply    copy
   `builds/os2/os2-gcc.mk` to `./config.mk`.

   If for  some reason your  platform isn't correctly  detected, copy
   manually the configuration sub-makefile to `./config.mk` and go to
   step 5.

   Note  that  this file  is  a  sub-Makefile  used to  specify  Make
   variables  for compiler  and linker  invocation during  the build.
   You can  easily create your own  version from one  of the existing
   configuration files,  then copy it to the  current directory under
   the name `./config.mk`.


5. Build the library

   The auto-detection/setup  phase should have  copied a file  in the
   current  directory,  called  `./config.mk`.   This  file  contains
   definitions of various Make  variables used to invoke the compiler
   and linker during the build.  [It has also generated a file called
   `ftmodule.h`   in  the  objects   directory  (which   is  normally
   `<toplevel>/objs/`);  please read  the  file `docs/CUSTOMIZE`  for
   customization of FreeType.]

   To  launch  the build,  simply  invoke  GNU  Make again:  The  top
   Makefile will detect the configuration file and run the build with
   it.  If you have used variables in  step 3, you must use  the same
   variables here, too.


   Final note

   The above instructions build a  _statically_ linked library of the
   font engine in the `objs` directory.   On Windows, you can build a
   DLL  either  with  MinGW  (within an  MSYS  shell,  following  the
   instructions in `INSTALL.UNIX`), or you  use one of the Visual C++
   project files; see  the  subdirectories  of `builds/windows`.  For
   everything else,  you are on  your own,  and you might  follow the
   instructions in `INSTALL.ANY` to create your own Makefiles.


### 3. With an IDE Project File
---

We provide a  small number of `project files'  for various IDEs to
automatically build  the library as  well.  Note that  these files
are  not supported  and only  sporadically maintained  by FreeType
developers, so don't expect them to work in each release.

To find them, have a  look at the content of the `builds/<system>`
directory, where `<system>` stands for your OS or environment.


### 4. Using cmake
---

See the top-level `CMakeLists.txt` file for more information.


### 5. From you own IDE, or own Makefiles
---

If  you  want  to  create   your  own  project  file,  follow  the
instructions   given  in  the   `INSTALL.ANY`  document   of  this
directory.

### 6. On VMS with the `mms` build tool
---

It is actually very straightforward to install the FreeType 2 library.
Just  execute vms_make.com from  the toplevel  directory to  build the
library.  This procedure currently accepts the following options:

 - `DEBUG`: Build the library with debug information and without optimization.

 - `lopts=<value>`: Options to pass to the link command e.g. lopts=/traceback

 - `ccopt=<value>`: Options to pass to the C compiler e.g. ccopt=/float=ieee

In case you did download the demos, place them in a separate directory
sharing the same top level  as the directory of FreeType 2  and follow
the same instructions as above for  the demos from  there.  The  build
process relies on this  to figure out  the location  of the FreeType 2
include files.


To rebuild  the  sources it is necessary to  have MMS/MMK installed on
the system.

The library is available in the directory

    [.LIB]

To  compile applications  using  FreeType  2 you  have  to define  the
logical FREETYPE pointing to the directory

    [.INCLUDE.FREETYPE]

i.e., if  the directory in which this INSTALL.VMS file  is located is
`$disk:[freetype]` then define the logical with

    define freetype $disk:[freetype.include.freetype]

This version has  been tested with Compaq C  V6.2-006 on OpenVMS Alpha
V7.2-1.


Any problems can be reported to

    Jouk Jansen <joukj@hrem.stm.tudelft.nl> or
    Martin P.J. Zinser <zinser@zinser.no-ip.info>

## C. Custom builds of the library

Customizing the compilation  of FreeType is easy, and  allows you to
select only the components of  the font engine that you really need.
For more details read the file [CONFIG.md](docs/CONFIG.md).


## D. Standard builds with `configure`

The git repository doesn't contain pre-built configuration scripts for
UNIXish platforms.  To generate them say

    sh autogen.sh

which in turn depends on the following packages:

    automake (1.10.1)
    libtool (2.2.4)
    autoconf (2.62)

The versions given  in parentheses are known to  work.  Newer versions
should  work too,  of course.   Note  that `autogen.sh`  also sets  up
proper file permissions for the `configure` and auxiliary scripts.

The `autogen.sh` script checks whether the versions of the above three
tools match the numbers above.  Otherwise it will complain and suggest
either  upgrading or  using  environment variables  to  point to  more
recent versions of the required tools.

Note that  `aclocal` is provided  by the 'automake' package  on Linux,
and that `libtoolize` is called `glibtoolize` on Darwin (OS X).


## E. Alternative build methods

For static  builds that don't use  platform-specific optimizations, no
configure script is necessary at all; saying

    make setup ansi
    make

should work on all platforms that have GNU `make` (or `makepp`).

A build  with `cmake`  or `meson`  can be done  directly from  the git
repository.  However, if you want  to use the `FT_DEBUG_LOGGING` macro
(see file `docs/DEBUG` for more information) it is currently mandatory
to execute `autogen.sh`  in advance; this script clones  the 'dlg' git
submodule and copies some files into FreeType's source tree.

___
[1] make++, a make tool written in Perl, has sufficient support of GNU
make extensions to build FreeType.  See

      https://makepp.sourceforge.net

for more information;  you need version 2.0 or newer, and you must
pass option `--norc-substitution`.
___

```
Copyright (C) 2000-2021 by
David Turner, Robert Wilhelm, and Werner Lemberg.

This  file is  part of  the FreeType  project, and  may only  be used,
modified,  and distributed  under the  terms of  the  FreeType project
license,  COPYING.md.  By  continuing to  use, modify,  or distribute
this file you  indicate that you have read  the license and understand
and accept it fully.
```
