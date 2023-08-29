# helapordo

  A roguelike terminal game.

  Small personal project.

  It uses ncurses to provide text interface.

  Here's a gif of a new game being started.

  ![Gif of game screen](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExNHF0Y2ZkdnYzY3Y3MmxhYzQ2Y25tenA0ajgzMmRscTRobDU5YzcyZiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/OxQAY7H9UwiXBUMt4h/giphy.gif)


## Dependencies

  `autoconf` and `automake` are needed to generate the Makefile used to compile the game.
  `ncurses-dev` is needed as a library dependecy.

  I included termios.h to have some of the messages be consistent in ignoring user input, so at the moment the code can't build on Windows.

## Initialising submodules

  You can initialise the submodules by running:
  ### `git submodule update --init`

## Autoreconf and ./configure

  You may need to run `automake --add-missing` to ensure your system provides the missing files needed by `autoreconf`.

  Run `autoreconf` to generate the `./configure` script. Run the script to generate the `Makefile` used for the project.

## Compiling and running

  Once you have the `Makefile`, you can compile the project by running, in its directory:

  ### `make`

  You can have a look at the CLI syntax with:

  ### `./helapordo -h`

## Installation

  You can install the game at `~/helapordo-local` and have a working alias for your PATH by running:

  ### `make install`
  ### `make do_alias`

  The second command needs root privileges to symlink `~/helapordo-local/helapordo` to `/usr/local/bin/helapordo`.

  You can skip the alias and maybe add the `helapordo-local` dir to your PATH instead.

## Generating documentation

  You can use `doxygen` to generate docs in both HTML and latex.

  To generate the documentation run, *from the repo dir*:

  ```
  doxygen docs/helapordo.doxyfile
  ```

  To have a pdf version, you can install a tex compiler and, inside `doxygen/latex`, run:
  ```
  make
  ```

  📦 Latest Release v1.0.11 29/08/2023
  https://github.com/jgabaut/helapordo/releases
