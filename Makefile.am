# Makefile.am

# Set the automake options
AUTOMAKE_OPTIONS = foreign

# Variables
CC = @CC@
CFLAGS = @CFLAGS@
TARGET = helapordo
TARGET_INSTALL_DIR = $(HOME)/helapordo-local
SOURCE_INSTALL_DIR = "."
STATIC_DIR_NAME = "static"
INSTALLER_LOG_FILE = "./installer.log"
# Check local version
RUN_VERSION := $$(./$(TARGET) -v)
# Specify the shell to use in the makefile
SHELL := /bin/bash
# Set the .ONESHELL special target to use a specific shell for all recipes
.ONESHELL:
# Check system info
OS := $$(uname -s)
MACHINE := $$(uname -m)
PACK_NAME = $(TARGET)-$(VERSION)-$(OS)-$(MACHINE)
PALETTE_PATH = "./static/palette.gpl"

# Source files
helapordo_SOURCES = src/helapordo.c src/game_core.c src/game_utils.c src/game_curses.c src/rooms.c src/artifacts.c src/specials.c src/main.c src/sprites.c src/floors.c sprites4curses/s4c-animate/animate.c koliseo/src/koliseo.c src/anvil__helapordo.c src/animations/mummy_shuffle.c src/animations/ghost_spell.c src/animations/boar_scream.c src/animations/troll_club.c src/animations/goblin_shoot.c src/animations/zombie_walk.c src/animations/imp_fireball.c src/animations/werewolf_transform.c src/animations/knight_tapis.c src/animations/mage_spark.c src/animations/archer_drop.c src/animations/assassin_poof.c src/animations/crawlingdude_crawl.c src/animations/srwarthog_square.c src/animations/headlessninja_throw.c src/animations/bluetroll_wonder.c src/animations/enter_door.c src/animations/alt_chest_opening.c src/game_lore.c src/game_lore_alt.c src/palette.c

# Linking rule
LDADD = $(HELAPORDO_LDFLAGS) -lncurses -lmenu -lpanel -lm

if DEBUG_BUILD
AM_LDFLAGS = -ggdb -O0
HELAPORDO_CFLAGS += -DHELAPORDO_DEBUG_LOG -DHELAPORDO_DEBUG_ACCESS
else
# Linker flag to strip symbols
AM_LDFLAGS = -s -O2
endif

# Compiler flags
AM_CFLAGS = $(HELAPORDO_CFLAGS) -Werror -Wpedantic -Wall -DKOLISEO_HAS_CURSES

./src/anvil__helapordo.c:
	@echo -en "Generating C \033[1;34manvil__helapordo\e[0m for $(VERSION):  "
	-./anvil -qG ./src $(VERSION)
	@echo -e "\033[1;33mDone.\e[0m"

./koliseo/src/koliseo.o:
	@echo -en "Building \033[1;35mKoliseo\e[0m with curses:  "
	gcc -c ./koliseo/src/koliseo.c -DKOLISEO_HAS_CURSES -o ./koliseo/src/koliseo.o $(AM_LDFLAGS)
	@echo -e "\033[1;33mDone.\e[0m"

./src/palette.c:
	@echo -en "\033[1;34m[Makefile]\e[0m    Using \033[1;33mpython\e[0m to generate C palette files from \033[1;35m[$(PALETTE_PATH)]:\e[0m  "
	python ./sprites4curses/scripts/palette.py C-impl $(PALETTE_PATH) .. > ./src/palette.c
	python ./sprites4curses/scripts/palette.py C-header $(PALETTE_PATH) .. > ./src/palette.h
	@echo -e "\033[1;33mDone.\e[0m"

# Build rule for object files
%.o: %.c
	@echo -en "[ $< ] -> [ \033[1;34m$@\e[0m ]:  "
	$(CC) -c $(CFLAGS) $(AM_CFLAGS) $< -o $@
	@echo -e "\033[1;33mDone.\e[0m"

# Build rule for the final binary
$(TARGET): $(helapordo_SOURCES:.c=.o)
	@echo -en "\033[1;34mBuilding [\033[1;35m $@ \033[1;34m]:\e[0m  "
	$(CC) $(CFLAGS) $(AM_CFLAGS) $(helapordo_SOURCES:.c=.o) -o $@ $(LDADD) $(AM_LDFLAGS)
	@echo -e "\033[1;33mDone.\e[0m"

# Targets for additional rules
versioncheck:
	@echo -en "Checking helapordo local version.\n\n    Expected ($(VERSION)),  got: ($(RUN_VERSION)).\n"
	@test "$(VERSION)" = "$(RUN_VERSION)" || echo -en '\n    \033[1;31mFailed check for expected local version.\e[0m\n'
	@echo -e "\n\033[1;32mDone.\e[0m"

const.k:
	@echo -en "Building const.k test"
	$(CC) kazoj/bone/const.c -o kazoj/bone/const.k
	@echo -e "\n\033[1;32mDone.\e[0m"

anviltest: const.k
	@echo -en "Running anvil tests.\n"
	./anvil -t
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
	install ./helapordo ~/helapordo-local/share/$(PACK_NAME)

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
	./installer "-i" $(VERSION) $(TARGET_INSTALL_DIR) $(SOURCE_INSTALL_DIR) $(STATIC_DIR_NAME) $(INSTALLER_LOG_FILE)
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
	-rm src/*.o
	-rm src/animations/*.o
	-rm src/palette.*
	-rm koliseo/src/*.o
	-rm ./*.o
	-rm src/anvil__helapordo.*
	@echo -e "\033[1;33mDone.\e[0m"

cleanob:
	@echo -en "Cleaning object build artifacts:  "
	-rm src/*.o
	@echo -e "\033[1;33mDone.\e[0m"

# Default target (builds everything)
all: ./src/anvil__helapordo.c ./koliseo/src/koliseo.o ./src/palette.c $(TARGET)

# Target to clean and rebuild
rebuild: clean all

# By default, build the binary
.DEFAULT_GOAL := all
