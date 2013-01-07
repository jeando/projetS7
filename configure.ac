#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([reconnaissance_voix], [0.1])
AM_INIT_AUTOMAKE(reconnaissance_voix, 0.1)

CXXFLAGS="-g -ansi -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -D_DEBUG_ -std=c++11"

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl Check for sdl-config
AC_CHECK_PROG(HAVE_SDL_CONFIG, sdl-config, true, false)
AM_CONDITIONAL(HAVE_SDL_CONFIG,$HAVE_SDL_CONFIG)

#AM_PATH_SDL(,[
#			 	HAVE_SDL=yes
#					SDL_LIBS=`$SDL_CONFIG --libs`
#					SDL_CFLAGS=`$SDL_CONFIG --cflags`
#					],AC_MSG_WARN([*** Unable to find SDL -- Not compiling example players ***]))
AM_PATH_SDL(,
			:,
			AC_MSG_WARN([*** Unable to find SDL -- Not compiling example players ***]))
AC_SUBST([SDL_LIBS])
AC_SUBST([SDL_CFLAGS])
#AC_ARG_ENABLE([$SDL_LIBS]);


#AM_CFLAGS="$AM_CFLAGS $SDL_CFLAGS"
#AM_CXXFLAGS="$AM_CXXFLAGS $SDL_CFLAGS"
#AM_LDFLAGS="$AM_LDFLAGS $SDL_LIBS"
#AC_CHECK_LIB($SDL_LIBS,[],[],[])
# Checks for libraries.
AC_SEARCH_LIBS([sqrt], [m], [], [])
AC_SEARCH_LIBS([sf_open, sf_close, sf_format_check, sf_writef_short], [sndfile], [], [sh install_env_script/install_libsndfile.sh])
#AC_SEARCH_LIBS([sf_open, sf_close, sf_format_check, sf_writef_short], [sndfile], [], [could not install libsndfile])
AC_SEARCH_LIBS([alcOpenDevice], [openal], [], [sh install_env_script/install_openal.sh])
#AC_SEARCH_LIBS([alcOpenDevice], [openal], [], [AC_MSG_ERROR(could not install openal)])
#AC_SEARCH_LIBS([SDL_Init], [SDL], [], [AC_MSG_ERROR([unable to find SDL_Init, which is a part of SDL lib])])
#AC_SEARCH_LIBS([SDL_Init], [SDL], [], [AC_MSG_ERROR([unable to find SDL_Init, which is a part of SDL lib])])
AC_SEARCH_LIBS([vlineRGBA], [SDL_gfx], [],[sh install_env_script/install_gfx.sh])
#AC_SEARCH_LIBS([vlineRGBA], [SDL_gfx], [],[AC_MSG_ERROR(could not install SDL_gfx)])
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
#AC_CHECK_FUNCS([sqrt])



AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
