# helapordo

  A roguelike terminal game.

  Small personal project.

  It uses ncurses to provide text interface.

  Here's a gif of a new game being started.

  ![Gif of game screen](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExNHF0Y2ZkdnYzY3Y3MmxhYzQ2Y25tenA0ajgzMmRscTRobDU5YzcyZiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/OxQAY7H9UwiXBUMt4h/giphy.gif)


## Table of Contents <a name = "index"></a>

+ [Building](#building)
  + [Dependencies](#dependencies)
  + [Initialitising Submodules](#init_submodules)
  + [Autoreconf and ./configure](#autotools)
  + [Enable game debug](#debug_access)
  + [Compiling](#compiling)
+ [Installation](#installation)
+ [Generating Documentation](#make_doc)
+ [Latest Release](#latest_release)


## Building <a name = "building"></a>

### Dependencies <a name = "dependencies"></a>

  - `autoconf` and `automake` are needed to generate the Makefile used to compile the game.
  - `ncurses-dev` is needed as a library dependecy.
  - `python` is needed to generate `./src/palette.c` and `./src/palette.h`.


  I included termios.h to have some of the messages be consistent in ignoring user input, so at the moment the code can't build on Windows.

### Initialising submodules <a name = "init_submodules"></a>

  You can initialise the submodules by running:

    `git submodule update --init`

### Autoreconf and ./configure <a name = "autotools"></a>

  You may need to run `automake --add-missing` to ensure your system provides the missing files needed by `autoreconf`.

  Run `autoreconf` to generate the `./configure` script. Run the script to generate the `Makefile` used for the project.

### Enable game debug  <a name = "debug_access"></a>

  After generating the `./configure` script, you may enable game debug mode by running:

    `./configure --enable-debug=yes`

  Which should prepare a `Makefile` with the correct macro definitions to turn on game debug at compile time. To reset the `Makefile` to release state, run:

    `./configure --enable-debug=no`

  If you're compiling without using `autotools`, you may want to define the compiler macros:

  - `-DHELAPORDO_DEBUG_LOG`  (Enables debug logging)
  - `-DHELAPORDO_DEBUG_ACCESS` (Enables the -d flag to turn on debug mode)

### Compiling and running <a name = "compiling"></a>

  Once you have the `Makefile`, you can compile the project by running, in its directory:

    `make`

  The build chain expects some files to be generated from already given ones.

  If you want to build without using `make`, you can see in the `Makefile.am` how to generate `./src/palette.c`, `./src/palette.h`, `./src/anvil__helapordo.c` and `./src/anvil__helapordo.h`

  You can have a look at the CLI syntax with:

    `./helapordo -h`

## Installation <a name = "installation"></a>

  You can install the game at `~/helapordo-local` and have a working alias for your PATH by running:

    `make install`
    `make do_alias`

  The second command needs root privileges to symlink `~/helapordo-local/helapordo` to `/usr/local/bin/helapordo`.

  You can skip the alias and maybe add the `helapordo-local` dir to your PATH instead.

## Generating documentation <a name = "make_doc"></a>

  You can use `doxygen` to generate docs in both HTML and latex.

  To generate the documentation run, *from the repo dir*:

  ```
  doxygen docs/helapordo.doxyfile
  ```

  To have a pdf version, you can install a tex compiler and, inside `doxygen/latex`, run:
  ```
  make
  ```

## Latest Release <a name = "latest_release"></a>

  I try to upload precompiled binaries for:

  - `linux-x86_64`
  - `linux-aarch64` (from [Termux](https://f-droid.org/packages/com.termux/) on Android).

  Some releases may offer a precompiled binary for:

  - `apple-arm64`

  📦 v1.1.4 06/09/2023
  https://github.com/jgabaut/helapordo/releases
