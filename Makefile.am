# Makefile.am

# Set the automake options
AUTOMAKE_OPTIONS = foreign

# Variables
CFLAGS = @CFLAGS@
TARGET_INSTALL_DIR = $(HOME)/helapordo-local
SOURCE_INSTALL_DIR = "."
STATIC_DIR_NAME = "."
SLOT1_DIR_NAME = "hlpd-1"
SLOT2_DIR_NAME = "hlpd-2"
SLOT3_DIR_NAME = "hlpd-3"
INSTALLER_LOG_FILE = "./installer.log"
# Check local version
RUN_VERSION := $$(./$(TARGET) -v)
# Specify the shell to use in the makefile
SHELL := /bin/bash
# Set the .ONESHELL special target to use a specific shell for all recipes
.ONESHELL:
# Check system info
MACHINE := $$(uname -m)
PACK_NAME = $(TARGET)-$(HL_BUILD_STR)-$(VERSION)-$(OS)-$(MACHINE)
PALETTE_PATH = "./static/palette.gpl"

# Source files

# Linking rule
LDADD = $(HELAPORDO_LDFLAGS)
AM_LDFLAGS = -lm

if USE_ANVIL_BUILD
HELAPORDO_CFLAGS += -DANVIL_BUILD
endif

if USE_EMOJI_BUILD
HELAPORDO_CFLAGS += -DHELAPORDO_EMOJI_ICONS
endif

if DEBUG_BUILD
AM_LDFLAGS += -ggdb -O0
KLS_LDFLAGS = -ggdb -O0
HELAPORDO_CFLAGS += -DHELAPORDO_DEBUG_LOG -DHELAPORDO_DEBUG_ACCESS -DKLS_DEBUG_CORE -DKOLISEO_HAS_REGION
else
# Linker flag to strip symbols
AM_LDFLAGS += -s -O2
KLS_LDFLAGS = -O2
endif

# Compiler flags
AM_CFLAGS = $(HELAPORDO_CFLAGS) -Werror -Wpedantic -Wall -std=c11 -DHELAPORDO_BUILD_STR=\"$(HL_BUILD_STR)\" -DHELAPORDO_OS=\"$(OS)\" -DHELAPORDO_MACHINE=\"$(MACHINE)\"

if USE_ANVIL_BUILD
helapordo_SOURCES = src/anvil__helapordo.c
else
helapordo_SOURCES =
endif

helapordo_SOURCES += src/core/game_core.c src/utils/game_utils.c src/utils/game_init.c src/utils/game_fight.c src/utils/game_debug.c src/core/equips.c src/ser/ser_core.c src/utils/saves.c src/utils/bin_saves.c src/utils/turn_op.c src/utils/rooms.c src/utils/artifacts.c src/utils/specials.c src/main.c src/core/sprites.c src/utils/floors.c koliseo/src/koliseo.c sprites4curses/s4c-animate/animate.c src/animations/mummy_shuffle.c src/animations/ghost_spell.c src/animations/boar_scream.c src/animations/troll_club.c src/animations/goblin_shoot.c src/animations/zombie_walk.c src/animations/imp_fireball.c src/animations/werewolf_transform.c src/animations/knight_tapis.c src/animations/mage_spark.c src/animations/archer_drop.c src/animations/assassin_poof.c src/animations/crawlingdude_crawl.c src/animations/srwarthog_square.c src/animations/headlessninja_throw.c src/animations/bluetroll_wonder.c src/animations/enter_door.c src/animations/alt_chest_opening.c src/core/game_lore.c src/core/game_lore_alt.c src/palette.c
if HL_RAYLIB_BUILD
AM_CFLAGS += -DHELAPORDO_RAYLIB_BUILD
helapordo_SOURCES += src/build-rl/helapordo_raylib.c src/build-rl/game_rl.c
else
AM_CFLAGS += -DHELAPORDO_CURSES_BUILD
helapordo_SOURCES += src/build-nc/game_curses.c
endif

if WINDOWS_BUILD
if !HL_RAYLIB_BUILD
helapordo_SOURCES += src/build-nc-w64/helapordo_win.c src/build-nc-w64/floor_tester.c
endif
else
if HL_CURSES_BUILD
helapordo_SOURCES += src/build-nc/helapordo.c
endif
endif


./anvil:
	@echo -e "\033[1;35m[Makefile]\e[0m    Bootstrapping \"./anvil\":"
	./scripts/bootstrap_anvil.sh $(ANVIL_IMPL)
	@echo -e "\033[1;33mDone.\e[0m"

./src/anvil__helapordo.c: ./anvil
	@echo -e "\033[1;34m[Makefile]\e[0m    Using \033[1;33mamboso\e[0m to generate [\033[1;34m./src/anvil__helapordo.c\e[0m and \033[1;34m./src/anvil__helapordo.h\e[0m] for [ \033[1;36m$(VERSION)\e[0m ]:"
	-./anvil -qXG ./src $(VERSION)
	@echo -e "\033[1;33mDone.\e[0m"

./sprites4curses/s4c-animate/animate.o:
	@echo -e "    \033[1;35mAM_CFLAGS\e[0m: [ \"\033[1;34m$(AM_CFLAGS)\e[0m\" ]"
	@echo -en "\033[1;34m[Makefile]\e[0m    Building [\033[1;35m./sprites4curses/s4c-animate/animate.o\e[0m]:  "
	$(CCOMP) -c ./sprites4curses/s4c-animate/animate.c $(AM_CFLAGS) -o ./sprites4curses/s4c-animate/animate.o
	@echo -e "\033[1;33mDone.\e[0m"

./koliseo/src/koliseo.o:
	@echo -e "    \033[1;35mAM_CFLAGS\e[0m: [ \"\033[1;34m$(AM_CFLAGS)\e[0m\" ]"
	@echo -en "\033[1;34m[Makefile]\e[0m    Building [\033[1;35m./koliseo/src/koliseo.o\e[0m] with curses:  "
	$(CCOMP) -c ./koliseo/src/koliseo.c $(AM_CFLAGS) -o ./koliseo/src/koliseo.o
	@echo -e "\033[1;33mDone.\e[0m"

./src/palette.c:
	@echo -en "\033[1;34m[Makefile]\e[0m    Using \033[1;33mpython3\e[0m to generate C [\033[1;34m./src/palette.c\e[0m and \033[1;34m./src/palette.h\e[0m] files from \033[1;35m[$(PALETTE_PATH)]:\e[0m  "
	python3 -m s4c-scripts.s4c.core.palette C-impl $(PALETTE_PATH) .. > ./src/palette.c
	python3 -m s4c-scripts.s4c.core.palette C-header $(PALETTE_PATH) .. > ./src/palette.h
	@echo -e "\033[1;33mDone.\e[0m"

# Build rule for object files
%.o: %.c
	@echo -e "    \033[1;35mAM_CFLAGS\e[0m: [ \"\033[1;34m$(AM_CFLAGS)\e[0m\" ]"
	@echo -en "    [ $< ] -> [ \033[1;34m$@\e[0m ]:  "
	$(CCOMP) -c $(CFLAGS) $(AM_CFLAGS) $< -o $@
	@echo -e "\033[1;33mDone.\e[0m"

# Build rule for the final binary
$(TARGET): $(helapordo_SOURCES:.c=.o)
	@echo -e "    \033[1;35mAM_CFLAGS\e[0m: [ \"\033[1;34m$(AM_CFLAGS)\e[0m\" ]"
	@echo -e "    \033[1;35mAM_LDFLAGS\e[0m: [ \"\033[1;34m$(AM_LDFLAGS)\e[0m\" ]"
	@echo -e "    \033[1;35mLDADD\e[0m: [ \"\033[1;34m$(LDADD)\e[0m\" ]"
	@echo -en "\033[1;34m[Makefile]    Building [\033[1;35m $@ \033[1;34m]:\e[0m  "
	$(CCOMP) $(CFLAGS) $(AM_CFLAGS) $(helapordo_SOURCES:.c=.o) -o $@ $(LDADD) $(AM_LDFLAGS)
	@echo -e "\033[1;33mDone.\e[0m"

# Targets for additional rules
versioncheck:
	@echo -en "Checking helapordo local version.\n\n    Expected ($(VERSION)),  got: ($(RUN_VERSION)).\n"
	@test "$(VERSION)" = "$(RUN_VERSION)" || echo -en '\n    \033[1;31mFailed check for expected local version.\e[0m\n'
	@echo -e "\n\033[1;32mDone.\e[0m"

const.k:
	@echo -en "Building const.k test"
	$(CCOMP) kazoj/bone/const.c -o kazoj/bone/const.k
	@echo -e "\n\033[1;32mDone.\e[0m"

const_rl.k:
	@echo -en "Building const_rl.k test"
	$(CCOMP) kazoj/bone/const_rl.c -o kazoj/bone/const_rl.k -lraylib -DS4C_RAYLIB_EXTENSION
	@echo -e "\n\033[1;32mDone.\e[0m"

anviltest: const.k const_rl.k
	@echo -en "Running anvil tests.\n"
	./anvil -tX
	@echo -e "\n\033[1;32mDone.\e[0m"

doc:
	@echo -e "Using doxygen to create tex + html from $(VERSION) src/:    "
	doxygen ./docs/helapordo.doxyfile
	@echo -e "\n\033[1;33mDone.\e[0m\n"
	sleep 1
	clear
	@echo -e "Listing ./doxygen directory contents:\n"
	ls -R ./doxygen
	@echo -e "\n\033[1;32mDone.\e[0m"
	@echo -e "\ncd to ./doxygen/latex and make tex into pdf:\n"
	cd ./doxygen/latex ; make | grep "Output written"
	@echo -e "\n\033[1;32mDone.\e[0m"
	@echo -e "\ncd back to ../..:\n"
	cd ../..
	@echo -e "\nmv ./doxygen/latex/refman.pdf to (../../docs/docs.pdf) :\n"
	mv ./doxygen/latex/refman.pdf ./docs/docs.pdf
	@echo -e "\n\033[1;32mDone.\e[0m"
	@echo -en "\nRemove ./doxygen/:    "
	yes | rm -rd ./doxygen/
	@echo -e "\n\033[1;32mDone.\e[0m"

pack: versioncheck
	@echo -e "Packing helapordo:  make pack for $(VERSION)"
	@echo -en "\n    helapordo version:\n    "
	./helapordo -v
	@echo -e "\n    amboso version:"
	./anvil -v
	@echo -e "\033[1;32m[TREE] Prepping tree, pack for $(VERSION):\e[0m"
	[[ -d ~/helapordo-local/ ]] || mkdir ~/helapordo-local
	[[ -d ~/helapordo-local/share ]] || mkdir ~/helapordo-local/share
	[[ -d ~/helapordo-local/share/$(PACK_NAME) ]] || mkdir ~/helapordo-local/share/$(PACK_NAME)
	[[ -d ~/helapordo-local/share/$(PACK_NAME)/static ]] || mkdir ~/helapordo-local/share/$(PACK_NAME)/static

	@echo -e "\033[1;32m[TREE] Removing '~/helapordo-local/share/$(PACK_NAME).zip':\e[0m"
	touch ~/helapordo-local/share/$(PACK_NAME).zip
	rm ~/helapordo-local/share/$(PACK_NAME).zip
	@echo -e "\033[1;32m[TREE] Done.\e[0m"
	@echo -e "\033[1;35m[PACK] Installing:\e[0m"
	install $(TARGET) ~/helapordo-local/share/$(PACK_NAME)

	@echo -e "\033[1;35m[PACK] Done.\e[0m"
	@echo -e "\033[1;32m[ZIP] Compressing $(PACK_NAME):\e[0m"
	cd ~/helapordo-local/share && zip -r $(PACK_NAME).zip ./$(PACK_NAME)
	@echo -e "\033[1;33m[ZIP] Testing zip:\e[0m"
	echo -e "\033[1;36m[UNZIP] Result:\n"
	unzip -t ~/helapordo-local/share/$(PACK_NAME).zip | tail -n1
	@echo -e "\033[1;32m[ZIP] Done.\e[0m"
	@echo -e "\033[1;33mDone.\e[0m"

run: helapordo
	@echo -en "Running ./helapordo with debug flag.  "
	./helapordo -d
	@echo -e "\033[1;33mDone.\e[0m"

install: helapordo
	@echo -en "Installing helapordo $(VERSION):  "
	./installer "-i" $(VERSION) $(TARGET_INSTALL_DIR) $(SOURCE_INSTALL_DIR) $(STATIC_DIR_NAME) $(SLOT1_DIR_NAME) $(SLOT2_DIR_NAME) $(SLOT3_DIR_NAME) $(INSTALLER_LOG_FILE)
	@echo -e "\033[1;33mDone.\e[0m"

do_alias:
	@echo -en "Symlinking (~/helapordo-local/helapordo) to /usr/local/bin/helapordo:  "
	@[[ -e /usr/local/bin/helapordo ]] || sudo ln -s ~/helapordo-local/helapordo /usr/local/bin/helapordo
	@echo -e "\033[1;33mDone.\e[0m"

uninstall:
	@echo -en "Uninstalling helapordo:  "
	[[ -d ~/helapordo-local ]] && rm -rd ~/helapordo-local
	sudo rm /usr/local/bin/helapordo
	@echo -e "\033[1;33mDone.\e[0m"

clean:
	@echo -en "Cleaning build artifacts:  "
	-rm $(TARGET)
	-rm sprites4curses/s4c-animate/*.o
	-rm src/*.o
	-rm src/build-*/*.o
	-rm src/core/*.o
	-rm src/utils/*.o
	-rm src/animations/*.o
	-rm src/palette.*
	-rm src/ser/*.o
	-rm koliseo/src/*.o
	-rm ./*.o
	-rm src/anvil__helapordo.*
	-rm ./anvil
	@echo -e "\033[1;33mDone.\e[0m"

cleanob:
	@echo -en "Cleaning object build artifacts:  "
	-rm sprites4curses/s4c-animate/*.o
	-rm koliseo/src/*.o
	-rm src/*.o
	-rm src/build-*/*.o
	-rm src/core/*.o
	-rm src/utils/*.o
	-rm src/ser/*.o
	@echo -e "\033[1;33mDone.\e[0m"

# Default target (builds everything)
all: ./src/palette.c ./koliseo/src/koliseo.o ./sprites4curses/s4c-animate/animate.o $(TARGET)

# Target to clean and rebuild
rebuild: clean all

# By default, build the binary
.DEFAULT_GOAL := all
