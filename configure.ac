# Define the package name and version
AC_INIT([helapordo], [1.4.4-dev], [jgabaut@github.com])

# Verify automake version and enable foreign option
AM_INIT_AUTOMAKE([foreign -Wall])

# Detect the host system and set PACK_PREFIX accordingly, later
AC_CANONICAL_HOST
build_linux=no
build_windows=no
build_mac=no
build_anvilpick=no

build_curses=yes
build_raylib=no

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

AC_ARG_ENABLE([raylib],
  [AS_HELP_STRING([--enable-raylib], [Enable raylib build])],
  [enable_raylib=$enableval],
  [enable_raylib=no])

AC_ARG_ENABLE([anvil],
  [AS_HELP_STRING([--enable-anvil], [Enable anvil-backed build])],
  [enable_anvil=$enableval],
  [enable_anvil=yes])

AC_ARG_ENABLE([emoji],
  [AS_HELP_STRING([--enable-emoji], [Enable emoji icons of equips])],
  [enable_emoji=$enableval],
  [enable_emoji=yes])

if test "$enable_anvil" = "yes"; then
    echo "CONF    Building using anvil."
else
    echo "CONF    Building without using anvil."
fi

# Define the include and library paths based on the host system
case "${host_os}" in
    mingw*)
        # mingw32 specific flags
        echo "CONF    Building for mingw32: [$host_cpu-$host_vendor-$host_os]"
        build_windows=yes
        if test "$enable_raylib" = "yes"; then
            echo "CONF    Building with raylib"
            build_raylib=yes
            AC_SUBST([HELAPORDO_CFLAGS], ["-I/usr/x86_64-w64-mingw32/include -fstack-protector -DWINDOWS_BUILD -DS4C_RAYLIB_EXTENSION"])
            AC_SUBST([HELAPORDO_LDFLAGS], ["-L/usr/x86_64-w64-mingw32/lib -lraylib -lm -lgdi32 -lwinmm -static"])
            AC_SUBST([CCOMP], ["/usr/bin/x86_64-w64-mingw32-gcc"])
            AC_SUBST([OS], ["w64-mingw32"])
            AC_SUBST([TARGET], ["helapordo.exe"])
            if test "$enable_anvilpick" = "yes"; then
                echo "CONF    Building with anvil interactive selection"
                build_anvilpick=yes
                AC_SUBST([ANVIL_IMPL], ["-i repo_invil"])
            else
                AC_SUBST([ANVIL_IMPL], ["repo_amboso"])
            fi
            AC_SUBST([HL_BUILD_STR], ["rl"])
        else
            echo "CONF    Building with ncurses"
            #build_raylib=no
            #build_curses=yes
            AC_SUBST([HELAPORDO_CFLAGS], ["-I/usr/x86_64-w64-mingw32/include -fstack-protector -DWINDOWS_BUILD -DKOLISEO_HAS_CURSES -DNCURSES_NO_PADDING -DNCURSES_STATIC -DS4C_UNCHECKED"])
            AC_SUBST([HELAPORDO_LDFLAGS], ["-L/usr/x86_64-w64-mingw32/lib -lpanelw -lmenuw -lncursesw -static"])
            AC_SUBST([CCOMP], ["/usr/bin/x86_64-w64-mingw32-gcc"])
            AC_SUBST([OS], ["w64-mingw32"])
            AC_SUBST([TARGET], ["helapordo.exe"])
            if test "$enable_anvilpick" = "yes"; then
                echo "CONF    Building with anvil interactive selection"
                build_anvilpick=yes
                AC_SUBST([ANVIL_IMPL], ["-i repo_invil"])
            else
                AC_SUBST([ANVIL_IMPL], ["repo_amboso"])
            fi
            AC_SUBST([HL_BUILD_STR], ["nc"])
        fi
    ;;
    darwin*)
        # macOS specific flags
        echo "CONF    Building for macos: [$host_cpu-$host_vendor-$host_os]"
        build_mac=yes
        if test "$enable_raylib" = "yes"; then
            echo "CONF    Building with raylib"
            build_raylib=yes
            build_curses=no
            AC_SUBST([HELAPORDO_CFLAGS], ["-I/opt/homebrew/opt/raylib/include -DS4C_RAYLIB_EXTENSION"])
            AC_SUBST([HELAPORDO_LDFLAGS], ["-L/opt/homebrew/opt/raylib/lib -lraylib"])
            AC_SUBST([OS], ["darwin"])
            AC_SUBST([TARGET], ["helapordo"])
            if test "$enable_anvilpick" = "yes"; then
                echo "CONF    Building with anvil interactive selection"
                build_anvilpick=yes
                AC_SUBST([ANVIL_IMPL], ["-i repo_invil"])
            else
                AC_SUBST([ANVIL_IMPL], ["repo_invil"])
            fi
            AC_SUBST([HL_BUILD_STR], ["rl"])
        else
            echo "CONF    Building with ncurses"
            #build_raylib=no
            #build_curses=yes
            AC_SUBST([HELAPORDO_CFLAGS], ["-I/opt/homebrew/opt/ncurses/include -DKOLISEO_HAS_CURSES -DNCURSES_NO_PADDING -DS4C_UNCHECKED"])
            AC_SUBST([HELAPORDO_LDFLAGS], ["-L/opt/homebrew/opt/ncurses/lib -lmenu -lpanel -lncurses"])
            AC_SUBST([OS], ["darwin"])
            AC_SUBST([TARGET], ["helapordo"])
            if test "$enable_anvilpick" = "yes"; then
                echo "CONF    Building with anvil interactive selection"
                build_anvilpick=yes
                AC_SUBST([ANVIL_IMPL], ["-i repo_invil"])
            else
                AC_SUBST([ANVIL_IMPL], ["repo_invil"])
            fi
            AC_SUBST([HL_BUILD_STR], ["nc"])
        fi
    ;;
    linux*)
        # Linux specific flags
        echo "CONF    Building for Linux: [$host_cpu-$host_vendor-$host_os]"
        build_linux=yes
        if test "$enable_raylib" = "yes"; then
            echo "CONF    Building with raylib"
            build_raylib=yes
            build_curses=no
            AC_SUBST([HELAPORDO_CFLAGS], ["-DS4C_RAYLIB_EXTENSION"])
            AC_SUBST([HELAPORDO_LDFLAGS], ["-lraylib"])
            AC_SUBST([OS], ["Linux"])
            AC_SUBST([CCOMP], ["gcc"]) #Yes
            AC_SUBST([TARGET], ["helapordo"])
            if test "$enable_anvilpick" = "yes"; then
                echo "CONF    Building with anvil interactive selection"
                build_anvilpick=yes
                AC_SUBST([ANVIL_IMPL], ["-i repo_invil"])
            else
                AC_SUBST([ANVIL_IMPL], ["repo_amboso"])
            fi
            AC_SUBST([HL_BUILD_STR], ["rl"])

        else
            echo "CONF    Building with ncurses"
            #build_raylib=no
            #build_curses=yes
            AC_SUBST([HELAPORDO_CFLAGS], ["-DKOLISEO_HAS_CURSES -DNCURSES_NO_PADDING -DS4C_UNCHECKED"])
            AC_SUBST([HELAPORDO_LDFLAGS], ["-lmenu -lpanel -lncurses"])
            AC_SUBST([OS], ["Linux"])
            AC_SUBST([CCOMP], ["gcc"]) #Yes
            AC_SUBST([TARGET], ["helapordo"])
            if test "$enable_anvilpick" = "yes"; then
                echo "CONF    Building with anvil interactive selection"
                build_anvilpick=yes
                AC_SUBST([ANVIL_IMPL], ["-i repo_invil"])
            else
                AC_SUBST([ANVIL_IMPL], ["repo_amboso"])
            fi
            AC_SUBST([HL_BUILD_STR], ["nc"])
        fi
    ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
    ;;
esac

AM_CONDITIONAL([HL_RAYLIB_BUILD], [test "$build_raylib" = "yes"])
AM_CONDITIONAL([HL_CURSES_BUILD], [test "$build_curses" = "yes"])
AM_CONDITIONAL([ANVILPICK_BUILD], [test "$build_anvilpick" = "yes"])
AM_CONDITIONAL([USE_ANVIL_BUILD], [test "$enable_anvil" = "yes"])
AM_CONDITIONAL([WINDOWS_BUILD], [test "$build_windows" = "yes"])
AM_CONDITIONAL([DARWIN_BUILD], [test "$build_mac" = "yes"])
AM_CONDITIONAL([LINUX_BUILD], [test "$build_linux" = "yes"])
AM_CONDITIONAL([USE_EMOJI_BUILD], [test "$enable_emoji" = "yes"])

# Set a default version number if not specified externally
AC_ARG_VAR([VERSION], [Version number])
if test -z "$VERSION"; then
  VERSION="1.4.4-dev"
fi

# Output variables to the config.h header
AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Version number])


AC_CHECK_PROGS([CCOMP], [gcc clang])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_FUNCS([malloc calloc])

# Output the generated files (Makefile and config.h)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
