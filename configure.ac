# Define the package name and version
AC_INIT([helapordo], [1.1.2], [jgabaut@github.com])

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

# Define the include and library paths based on the host system
if test "$OS_DARWIN" = "yes"; then
  echo "Building for: [$host_cpu-$host_vendor-$host_os]"
  # macOS specific flags
  AC_SUBST([HELAPORDO_CFLAGS], ["-I/usr/local/opt/ncurses/include"])
  AC_SUBST([HELAPORDO_LDFLAGS], ["-L/usr/local/opt/ncurses/lib"])
else
  echo "Building for: [$host_cpu-$host_vendor-$host_os]"
  # Linux specific flags
  AC_SUBST([HELAPORDO_CFLAGS], [""])
  AC_SUBST([HELAPORDO_LDFLAGS], [""])
fi

# Set a default version number if not specified externally
AC_ARG_VAR([VERSION], [Version number])
if test -z "$VERSION"; then
  VERSION="1.1.2"
fi

# Output variables to the config.h header
AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Version number])


AC_CHECK_PROGS([CC], [gcc clang])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_FUNCS([malloc calloc])

# Output the generated files (Makefile and config.h)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
