# Define the package name and version
AC_INIT([helapordo], [1.1.8], [jgabaut@github.com])

# Verify automake version and enable foreign option
AM_INIT_AUTOMAKE([foreign -Wall])

# Detect the host system and set PACK_PREFIX accordingly
AC_CANONICAL_HOST
AM_CONDITIONAL([OS_DARWIN], [test "$host_os" = "darwin"])

# Check for the --enable-debug option
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [Enable debug build])],
  [enable_debug=$enableval],
  [enable_debug=no])
# Define the DEBUG_BUILD conditional based on the --enable-debug option
AM_CONDITIONAL([DEBUG_BUILD], [test "$enable_debug" = "yes"])
AM_CONDITIONAL([MINGW32_BUILD], [test "$host_os" = "mingw32"])

# Define the include and library paths based on the host system
if test "$host_os" = "mingw32"; then
  echo "Building for mingw32: [$host_cpu-$host_vendor-$host_os]"
  # mingw32 specific flags
  AC_SUBST([HELAPORDO_CFLAGS], ["-I/usr/x86_64-w64-mingw32/include -fstack-protector -DMINGW32_BUILD -DKOLISEO_HAS_CURSES -DNCURSES_STATIC"])
  AC_SUBST([HELAPORDO_LDFLAGS], ["-L/usr/x86_64-w64-mingw32/lib -lpanelw -lmenuw -lncursesw -static"])
  AC_SUBST([CCOMP], ["/usr/bin/x86_64-w64-mingw32-gcc"])
  AC_SUBST([OS], ["w64-mingw32"])
  AC_SUBST([TARGET], ["helapordo.exe"])
fi
if test "$host_os" = "darwin"; then
  echo "Building for macos: [$host_cpu-$host_vendor-$host_os]"
  # macOS specific flags
  AC_SUBST([HELAPORDO_CFLAGS], ["-I/opt/homebrew/opt/ncurses/include -DKOLISEO_HAS_CURSES"])
  AC_SUBST([HELAPORDO_LDFLAGS], ["-L/opt/homebrew/opt/ncurses/lib -lmenu -lpanel -lncurses"])
  AC_SUBST([OS], ["darwin"])
  AC_SUBST([TARGET], ["helapordo"])
fi
if test "$host_os" = "linux-gnu"; then
  echo "Building for Linux: [$host_cpu-$host_vendor-$host_os]"
  # Linux specific flags
  AC_SUBST([HELAPORDO_CFLAGS], ["-DKOLISEO_HAS_CURSES"])
  AC_SUBST([HELAPORDO_LDFLAGS], ["-lmenu -lpanel -lncurses"])
  AC_SUBST([OS], ["Linux"])
  AC_SUBST([CCOMP], ["gcc"]) #Yes
  AC_SUBST([TARGET], ["helapordo"])
fi

# Set a default version number if not specified externally
AC_ARG_VAR([VERSION], [Version number])
if test -z "$VERSION"; then
  VERSION="1.1.8"
fi

# Output variables to the config.h header
AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Version number])


AC_CHECK_PROGS([CCOMP], [gcc clang])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_FUNCS([malloc calloc])

# Output the generated files (Makefile and config.h)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
