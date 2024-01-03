# Define the package name and version
AC_INIT([helapordo], [1.3.6], [jgabaut@github.com])

# Verify automake version and enable foreign option
AM_INIT_AUTOMAKE([foreign -Wall])

# Detect the host system and set PACK_PREFIX accordingly, later
AC_CANONICAL_HOST
build_linux=no
build_windows=no
build_mac=no
build_anvilpick=no

# Check for the --enable-debug option
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [Enable debug build])],
  [enable_debug=$enableval],
  [enable_debug=no])

# Define the DEBUG_BUILD conditional based on the --enable-debug option
AM_CONDITIONAL([DEBUG_BUILD], [test "$enable_debug" = "yes"])

# Check for the --enable-anvilpick option
AC_ARG_ENABLE([anvilpick],
  [AS_HELP_STRING([--enable-anvilpick], [Enable interactive selection of ./anvil impl])],
  [enable_anvilpick=$enableval],
  [enable_anvilpick=no])


# Define the include and library paths based on the host system
case "${host_os}" in
    mingw*)
        echo "Building for mingw32: [$host_cpu-$host_vendor-$host_os]"
        build_windows=yes
        # mingw32 specific flags
        AC_SUBST([HELAPORDO_CFLAGS], ["-I/usr/x86_64-w64-mingw32/include -fstack-protector -DWINDOWS_BUILD -DKOLISEO_HAS_CURSES -DNCURSES_STATIC"])
        AC_SUBST([HELAPORDO_LDFLAGS], ["-L/usr/x86_64-w64-mingw32/lib -lpanelw -lmenuw -lncursesw -static"])
        AC_SUBST([CCOMP], ["/usr/bin/x86_64-w64-mingw32-gcc"])
        AC_SUBST([OS], ["w64-mingw32"])
        AC_SUBST([TARGET], ["helapordo.exe"])
        if test "$enable_anvilpick" = "yes"; then
            echo "Building with anvil interactive selection"
            build_anvilpick=yes
            AC_SUBST([ANVIL_IMPL], ["-i repo_amboso"])
        else
            AC_SUBST([ANVIL_IMPL], ["repo_amboso"])
        fi
    ;;
    darwin*)
        echo "Building for macos: [$host_cpu-$host_vendor-$host_os]"
        build_mac=yes
        # macOS specific flags
        AC_SUBST([HELAPORDO_CFLAGS], ["-I/opt/homebrew/opt/ncurses/include -DKOLISEO_HAS_CURSES"])
        AC_SUBST([HELAPORDO_LDFLAGS], ["-L/opt/homebrew/opt/ncurses/lib -lmenu -lpanel -lncurses"])
        AC_SUBST([OS], ["darwin"])
        AC_SUBST([TARGET], ["helapordo"])
        if test "$enable_anvilpick" = "yes"; then
            echo "Building with anvil interactive selection"
            build_anvilpick=yes
            AC_SUBST([ANVIL_IMPL], ["-i repo_invil"])
        else
            AC_SUBST([ANVIL_IMPL], ["repo_invil"])
        fi
    ;;
    linux*)
        echo "Building for Linux: [$host_cpu-$host_vendor-$host_os]"
        build_linux=yes
        # Linux specific flags
        AC_SUBST([HELAPORDO_CFLAGS], ["-DKOLISEO_HAS_CURSES"])
        AC_SUBST([HELAPORDO_LDFLAGS], ["-lmenu -lpanel -lncurses"])
        AC_SUBST([OS], ["Linux"])
        AC_SUBST([CCOMP], ["gcc"]) #Yes
        AC_SUBST([TARGET], ["helapordo"])
        if test "$enable_anvilpick" = "yes"; then
            echo "Building with anvil interactive selection"
            build_anvilpick=yes
            AC_SUBST([ANVIL_IMPL], ["-i repo_amboso"])
        else
            AC_SUBST([ANVIL_IMPL], ["repo_amboso"])
        fi
    ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
    ;;
esac

AM_CONDITIONAL([ANVILPICK_BUILD], [test "$build_anvilpick" = "yes"])
AM_CONDITIONAL([WINDOWS_BUILD], [test "$build_windows" = "yes"])
AM_CONDITIONAL([DARWIN_BUILD], [test "$build_mac" = "yes"])
AM_CONDITIONAL([LINUX_BUILD], [test "$build_linux" = "yes"])

# Set a default version number if not specified externally
AC_ARG_VAR([VERSION], [Version number])
if test -z "$VERSION"; then
  VERSION="1.3.6"
fi

# Output variables to the config.h header
AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Version number])


AC_CHECK_PROGS([CCOMP], [gcc clang])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_FUNCS([malloc calloc])

# Output the generated files (Makefile and config.h)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
