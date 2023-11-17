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
  + [Build with mingw32](#mingw32_build)
  + [Compiling](#compiling)
+ [Installation](#installation)
+ [Documentation](#doc)
+ [Latest Release](#latest_release)


## Building <a name = "building"></a>

  The code should build for:

  - `x86_64-Linux`
  - `aarch64-Linux`(see [this section](#latest_release))
  - `aarch64-apple-darwin`
  - `Windows` (through `x86_64-w64-mingw32`)

  **DISCLAIMER:**  The `Windows` build is proof-of-concept as:

  - The gameplay loop is a simplified version of `Rogue` gamemode from the main build.
    - Work in progress on running the original gameloop.
  - `Windows` terminal can't change its color pairs as far as I know, rendering it unsuitable for the game.
    - A solution to reject extra/modified colors could be implemented.

### Dependencies <a name = "dependencies"></a>

  - `autoconf` and `automake` are needed to generate the Makefile used to compile the game.
  - `ncurses-dev` is needed as a library dependecy.
  - `python3` is needed to generate `./src/palette.c` and `./src/palette.h`.


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
  - `-DKLS_DEBUG_CORE` (Enables debugging for `Koliseo` funcalls)

### Build with `x86_64-w64-mingw32` <a name = "mingw32_build"></a>

  After generating the `./configure` script, you may switch target host and use `mingw32` with:

    `./configure --host x86_64-w64-mingw32`

  Which should prepare a `Makefile` with the correct macro definitions. To reset the `Makefile` to release state, run:

    `./configure --host $(./config.guess)`

  You may need to correct the `configure.ac` flags if your installation does not have the expected layout, providing the correct paths for:

  - `mingw32`'s `lib` and `include` dirs, for `ncursesw`
  - `mingw32`'s correct `gcc`.

  Preprocessor macros based on the `Makefile` passing `-DMINGW32_BUILD` are present to enable/disable `Windows` specific code.

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

## Documentation <a name = "doc"></a>

  HTML docs are available at [this Github Pages link](https://jgabaut.github.io/helapordo-docs/index.html).

  You can also get the ready pdf version of the docs from the latest release.

  If you have `doxygen` installed you can generate the HTML yourself, or even the pdf if you have `doxygen-latex` or equivalents.

  You if you have those packages, running `make doc` should give you `./docs/docs.pdf`, just the pdf output from latex, and delete the doxygen folder by itself.

## Latest Release <a name = "latest_release"></a>

  I try to upload precompiled binaries for:

  - `x86_64-Linux`
  - `aarch64-Linux` (from [Termux](https://f-droid.org/packages/com.termux/) on Android).
  - `x86_64-w64-mingw32` (*JUST A DEMO.* Any help with debugging the full game is welcome.)

  Some releases may offer a precompiled binary for:

  - `darwin-arm64`

  📦 v1.3.1 17/11/2023
  https://github.com/jgabaut/helapordo/releases
