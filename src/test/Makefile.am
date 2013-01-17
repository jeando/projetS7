bin_PROGRAMS = test test_load_sound test_draw_spectro test_draw_spectro_mel test_save_sound

AM_CXXFLAGS = $(INTI_CFLAGS) $(SDL_CFLAGS)
test_SOURCES = test.cpp
test_LDADD = $(INTI_LIBS) $(SDL_LIBS)
test_load_sound_SOURCES = test_load_sound.cpp ../MyAL.cpp ../MyAL.h
test_load_sound_LDADD = $(INTI_LIBS) $(SDL_LIBS)
test_save_sound_SOURCES = test_save_sound.cpp ../MyAL.cpp ../MyAL.h
test_save_sound_LDADD = $(INTI_LIBS) $(SDL_LIBS)
test_draw_spectro_SOURCES = test_draw_spectro.cpp ../MyAL.cpp ../MyAL.h ../traitement.h ../traitement.cpp ../sdl.h ../ft.cpp ../ft.h
test_draw_spectro_LDADD = $(INTI_LIBS) $(SDL_LIBS)
test_draw_spectro_mel_SOURCES = test_draw_spectro_mel.cpp ../MyAL.cpp ../MyAL.h ../traitement.h ../traitement.cpp ../sdl.h ../ft.cpp ../ft.h
test_draw_spectro_mel_LDADD = $(INTI_LIBS) $(SDL_LIBS)
