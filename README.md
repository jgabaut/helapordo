# helapordo

  A roguelike terminal game.

  Small personal project.

  It uses ncurses to provide text interface.

  Here's a gif of a new game being started.

  ![Gif of game screen](https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExZjgycTdlazZzbHJ4MnljZm1jOW9rOGY0Y3NydXlreWl4YTNlc212NSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/DG0aoSpfx92NYjNOKO/source.gif)


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

  - Some source files are expected to be generated at build time, by a symlink executable named `anvil`. By default, the configuration will pick an implementation for `anvil` depending on passed host:
    - `darwin*`: default is `repo_invil`. This means you need the Rust build toolchain to be installed.
    - `linux*`, `mingw*`: default is `repo_amboso`. This means you need `bash >=4.x`, and your `awk` should not be `mawk`.
  - To readily override the default implementation for `anvil`, you can pass `--enable-anvilpick` to the `./configure` script:
    `./configure --enable-anvilpick`
    This will ensure you get an interactive prompt to pick an implementation (and can still use the default one by pressing Enter without typing anything), whenever `make` needs to create `./anvil`.

  - Needed programs, depending on chosen `ANVIL_IMPL`:
    - `awk` is needed by `amboso` to generate `./src/anvil__helapordo.h`.
      - `bash >=4` is needed to run `amboso`.
      - Unfortunately, `mawk` is not compatible with `amboso`.
    - If you dont' have those, you can use `invil` to generate `./src/anvil__helapordo.h`.
      - You will need `cargo` to build `invil`.
    - If you want to just build the code without `./src/anvil__helapordo.h`., you can apply the patch file provided at [this link](https://github.com/jgabaut/helapordo/issues/52#issuecomment-1871877437).

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

  Preprocessor macros based on the `Makefile` passing `-DWINDOWS_BUILD` are present to enable/disable `Windows` specific code.

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

  I try to upload precompiled binaries for the ncurses build:

  - `x86_64-Linux` : [download latest](https://github.com/jgabaut/helapordo/releases/download/1.4.0/helapordo-nc-1.4.0-Linux-x86_64.zip)
  - `aarch64-Linux` (from [Termux](https://f-droid.org/packages/com.termux/) on Android).
  - `x86_64-w64-mingw32` (*JUST A DEMO.* Any help with debugging the full game is welcome.) : [download latest](https://github.com/jgabaut/helapordo/releases/download/1.4.0/helapordo.exe-nc-1.4.0-w64-mingw32-x86_64.zip)

  Some releases may offer a precompiled binary for:

  - `darwin-arm64` : [download latest](https://github.com/jgabaut/helapordo/releases/download/1.3.5/helapordo-1.3.5-darwin-arm64.zip) (Available = `1.3.5`. Latest = `1.4.0`)

  📦 v1.4.0 05/01/2023
  https://github.com/jgabaut/helapordo/releases
