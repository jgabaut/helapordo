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

# Source files
helapordo_SOURCES = src/helapordo.c src/game_core.c src/game_utils.c src/game_curses.c src/rooms.c src/artifacts.c src/specials.c src/main.c src/sprites.c src/floors.c sprites4curses/s4c-animate/animate.c koliseo/src/koliseo.c

# Linking rule
LDADD = $(HELAPORDO_LDFLAGS) -lncurses -lmenu -lpanel -lm

# Linker flag to strip symbols
AM_LDFLAGS = -s

# Compiler flags
AM_CFLAGS = $(HELAPORDO_CFLAGS) -O2 -Werror -Wpedantic -Wall

# Build rule for object files
%.o: %.c
	$(CC) -c $(CFLAGS) $(AM_CFLAGS) $< -o $@

# Build rule for the final binary
$(TARGET): $(helapordo_SOURCES:.c=.o)
	$(CC) $(CFLAGS) $(AM_CFLAGS) $(helapordo_SOURCES:.c=.o) -o $@ $(LDADD) $(AM_LDFLAGS)

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
	[[ -d ~/helapordo-local/share/$(PACK_NAME)/static/animations ]] || mkdir ~/helapordo-local/share/$(PACK_NAME)/static/animations
	[[ -d ~/helapordo-local/share/$(PACK_NAME)/static/lore ]] || mkdir ~/helapordo-local/share/$(PACK_NAME)/static/lore
	@echo -e "\033[1;32m[TREE] Removing '~/helapordo-local/share/$(PACK_NAME).zip':\e[0m"
	touch ~/helapordo-local/share/$(PACK_NAME).zip
	rm ~/helapordo-local/share/$(PACK_NAME).zip
	@echo -e "\033[1;32m[TREE] Done.\e[0m"
	@echo -e "\033[1;35m[PACK] Installing:\e[0m"
	install ./helapordo ~/helapordo-local/share/$(PACK_NAME)
	install ./static/animations/* ~/helapordo-local/share/$(PACK_NAME)/static/animations
	install ./static/lore/* ~/helapordo-local/share/$(PACK_NAME)/static/lore
	install ./static/palette.gpl ~/helapordo-local/share/$(PACK_NAME)/static/
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
	rm $(TARGET)
	rm src/*.o
	@echo -e "\033[1;33mDone.\e[0m"

cleanob:
	@echo -en "Cleaning object build artifacts:  "
	rm src/*.o
	@echo -e "\033[1;33mDone.\e[0m"

# Default target (builds everything)
all: $(TARGET)

# Target to clean and rebuild
rebuild: clean all

# By default, build the binary
.DEFAULT_GOAL := all
