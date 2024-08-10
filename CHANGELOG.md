## [1.4.11] - 2024-08-10

### Added

- Add display_floor_layout_with_player() to have a still view of the Floor
- Add display_explored_layout_with_player() to have a live view of the Floor
  - Improved colors for display_floor_layout(), display_explored_layout()
- Add ROOM_VIEW
- Add SAVES_PICK
- Add Gui_State
- Add Gui_Button

### Changed

- Factor out draw and update functions to game_rl.c

- Handle Floor regen by passing double pointers to update_GameScreen()
- Rename GAMEPLAY screen to FLOOR_VIEW
- Roll seed for rl build
- Prep current_save_path for rl build
- Push current floor on kls_t, set starting room as explored
- Init Path, Fighter for rl build
- Pressing R also resets Path and Player
- Added room init on floor_kls
- Added Gamestate prep for rl build
- Fix: writeSerSaveHeader() now tries to create a missing directory even when not forced
- Moved animation to ROOM_VIEW, only for Enemy rooms
- Use current enemy class for animation instead of a set Zombie class
- Ending screen goes to quit
- Fix: update tutorial text to drop Story mode mention
- Fix: proper transform on virtualMouse
- Fix: use PRId64 to print int64_t blob_size in readSerGamestate()
- Fix: install doesn't try copying invalid .c file from ringabuf/src
- Take name, class from CLI args for rl build
- Fix: wrap anvil header inclusion for rl build
- Added stats print to ROOM_VIEW
- Added rb_notifications prep
- Added rectangle for drawing notifications
- Added name/class drawing
- Refactor hlpd_dbg_features()
- Added CHANGELOG
- Bump koliseo to 0.4.5, turning on the block of Koliseo usage while it has an active Koliseo_Temp
- Debug build uses Koliseo's caller location extension
- Bump invil to 0.2.16
- Bump s4c-scripts to 0.1.2
- Bump sprites4curses to 0.4.7
- Raylib build uses S4C_QUIETER to avoid frame drops due to transparent tiles in some animations

## [1.4.10] - 2024-06-14

### Changed

- Fix: add release_data.o to make clean
- Fix: Use int64_t for start_time in SerGameState, instead of platform-specific-width clock_t
- Fix: Refactor hlpd_default_keybinds and oterh places needed to build with raylib
- Use texture rendering to scale the whole texture
- The raylib build gets to see more of the lower API thanks to the new notifications interface
- Drop s4c-gui inclusion for raylib build
- Trim gameloop_rl(), use hlpd_getopts() and hlpd_reset_logfile()
- Changed some error messages for save loading
- Add sizeof(SerGamestate), sizeof(SerSaveHeader) to const.c, const_rl.c anvil tests
- Use int32_t for a couple of SerChest values
- Bump HELAPORDO_BINSAVEFILE_VERSION to 0.0.5

## [1.4.9] - 2024-06-02

### Added

- Embed info on previous releases for later use
- WIP: add plot_line_in_ncurses()
- Add format macros for more Ser structs, for hlpd-save2l
- Add Notification to hold state for enqueue_notification()
- Binary file save/load does not require -X anymore
- Bps floor generation does not require -X anymore
- Add ringabuf at 0.0.4

### Changed

- Fix: Use s4c_display_sprite_at_coords() instead of s4c_animate_rangeof_sprites_at_coords() to handle SIGINT while on animation screen
- Fix: load roomscompleted for roomsDone when loading game
- Fix: load floorscompleted for floors_done when loading game
- Fix: use int32_t for status field in Ser structures
- Move transparent terminal color support behind a feature flag
  - Now needs either
    - ./configure --enable-default-background to be used
    - or HELAPORDO_SUPPORT_DEFAULT_BACKGROUND to be defined before building
- Only one call to getopt() per execution, rather than once per run when using retry()
- Slim down gameloop() by collecting some prep step in new functions
- Debug command 'd' in debug_generic() prints release data (currently unreachable)
- Refactor to drop notify_win from lower interfaces
- Refactor display_notification() to enqueue_notification()
- Increase terminal size requirement for warning
- Increase notification window size
- Refactor to pass color to printStatusText()
- Drop Story, Standard from Gamemode
- Drop -s to run with Story mode
- Drop textfile save/load interfaces
- Drop loreCounter from Path, SerPath
- Drop HELAPORDO_SAVEPATH_1

## [1.4.8] - 2024-05-17

### Added

- Experimental: improved TUI for pickClass(), pickName(9 using s4c-gui
- Add GameOptions for runtime settings
  - Can be changed at any time from menu (open with m)
- Add new binary file to store the setting
- Add HLPD_Key, hlpd_default_keybinds as base to support customizable keybinds
  - For now, just a rigid set of 3 schems is available (mutually exclusive): ARROW_KEYS, VIM_KEYS, WASD_KEYS
  - Run with -j to use VIM_KEYS at start
  - Run with -w to use WASD_KEYS at start
- Add optional usage of terminal's default colors (to support transparent background for the game)
  - Run with -b to use this option at start
  - Needs support from your ncurses library for reset_color_pairs(), which is an extension to ncurses. It may be available when building using ncurses's ABI 6.
    - Builds missing this support are unable to use this option. If your build is stuck with this option off, you may try to rebuild your ncurses library from source.
- Collect BSP_Room tree for each bsp gen in gamestate_kls
  - Currently only used by dbg_BSP_Room() on the root
- Add WIP draw_BSP_room()
- Add s4c-gui at 0.0.7

### Changed

- Fix: proper check for ncurses version for log
- Fix: clear gamestate->current_room after finishing one
- Fix: typo in header
- Fix: drop useless left/right option pick for get_saveslots_index()
- Fix: move anvil__helapordo.h inclusion to game_utils.h
  - Should make the starting info show anvil info instead of Built without anvil incorrectly
- Fix: place player at home coords with -X after Boss kill
- Most curses menu wrap around when going up from first item or down from last, wrap to the other side
- Experimental: use int32_t for enum types in ser.h
- Renamed ctrl_c_handler() to hlpd_sigint_handler()
- Drop .md extension from LICENSE
- Bump sprites4curses to 0.4.5

## [1.4.7] - 2024-04-29

- Fixes broken anvil tag for 1.4.6

## [1.4.6] - 2024-04-29

### Added

- Add SIGINT handler
  - Try to free arenas when Ctrl+C is received
- Add bsp implementation, based on mtsdurica's rendition of the popular algorithm
  - With the author's permission, the code is relicensed under GPL-3.0-only, to have only one license for all the game sources
  - Thanks to Matus for its availability and politeness
- Add from_bsp field to Floor

### Changed

- Fix: drop repetitive done_loading log message
- Fix: avoid leaving the terminal messed up on binary savefile load errors
- Fix: free arenas in initRoom() for invalid classes BASIC, WALL
- Fix: removed some UB
- Enlarge seed field in Path by 1
- Update experimental binary savefile version to 0.0.3
- Recompute floor are in floor_set_room_types() with G_EXPERIMENTAL_ON
- Floors generated with bsp will have a random position for HOME room
- Use enemy_area_ratio to loop in enemy room placement for new floors
  - Floors generated with bsp will now have relatively less enemies
- Use -X in raylib build to toggle floor layout render
- Fix: wrong doxygen comments for Path and SerPath
- Bump koliseo to 0.4.3
- Bump s4c-scripts to 0.1.1
- Bump invil to 0.2.13

## [1.4.5] - 2024-04-19

### Added

- Experimental:
  - Add logic to save/load the RNG state across runs
- Add hlpd_rand(), G_RNG_ADVANCEMENTS, gen_random_seed(), hlpd_hash()
- Add seeded runs with -S \<seed\>
- Add is_seeded to Gamestate to handle seeded runs
- Add -D to use current dir for saves
  - When -D is not present, default directory is $HOME/helapordo-local
- Add proof-of-concept binary tool for savefile handling

### Changed

- Fix: loading/saving an Equip with perksCount == EQUIPPERKSMAX fails
- Fix: loading a zero-initialised Room led to failed enemy index read
  - ser_Room() now sets class as BASIC when passed Room is NULL, rather than letting 0 be used for the class field
- Fix: move enemyTotal randomisation call in initRoom() into the ENEMIES class case
- Fix: updateSelectedSpecialW() iteration index
- Fix: avoid restarting colors, window box in draw_floor_view()
- Fix: partial fixes to errors on missing save directories
  - Goal is not to fail if we can create the directory ourselves
  - Skip prep of save.txt with G_EXPERIMENTAL_ON
- Seeds are now strings between 8 and 19 chars long
- Drop src/utils/bin_saves.h
- Drop is_localexe from Gamestate
- Bump amboso to 2.0.6
- Bump invil to 0.2.12
- Bump koliseo to 0.4.2
- Upgraded helapordo.doxyfile for doxygen 1.10

## [1.4.4] - 2024-03-25

### Added

- Print run seed when exiting from retry()
- Experimental: these features are available only when running with -X flag.
  - Icon indicators for when you have an equipped item
  - Save/load using binary files in Rogue gamemode
    - Add all packed SerXYZ structures for saving/loading
    - Add ser_XYZ()/deser_XYZ() functions for conversion
- Add --enable-anvil configure option to allow easy building without anvil
- Add FLOORMENU_SAVE to saveType
- Add seed field to Path

### Changed

- Fix: use strlen() for whoami push
- Bump koliseo to 0.4.1
- Use kls_conf_init() to prep the used Koliseos
  - Now, only the debug build uses KLS_Region
- Update installer to 0.1.2
  - Now creates hlpd-{1,3} directories, used for Saveslot handling
- Bump amboso to 2.0.5
  - Uses a newer bash on macOS, if the shell setup was done
- Update const.c test

## [1.4.3] - 2024-02-22

### Added

- Rough implementation of FLOORS_H declarations for raylib build

### Changed

- Drop circular dependency of rooms.h on helapordo.h
- Refactor:
  - Slim helapordo.h to the bare minimum
  - New files
  - New subdirectories to src
  - Improved Makefile.amm configuration ambiguity
- Bump amboso to 2.0.4
- Bump invil to 0.2.8

## [1.4.2] - 2024-02-09

### Added

- Add Gamescreen

### Changed

- Fix: clang warnings
- Check screen size before save pick
- Drop forced KLS_DEBUG_CORE from ncurses build
- Define NCURSES_NO_PADDING
- Add NOGDI, NOUSER, WIN32_LEAN_AND_MEAN to use KLS_DEBUG_CORE on mingw32 raylib build
- Bump koliseo to 0.4.0
- Bump amboso to 2.0.3
- Bump invil to 0.2.6

## [1.4.1] - 2024-01-22

### Added

- Add s4c-scripts at 0.1.0

### Changed

- Fix: string literal usage for time print
- Fix: unuser var in gameloop_rl()
- Fix: window size checks for gameloop_rl()
- Fix: failed build with raylib on mingw32, when passing --enable-debug
- Print commit desc when building using invil
- Bump sprites4curses to 0.4.3
- Bump invil to 0.2.5
- Bump amboso to 2.0.2

## [1.4.0] - 2024-01-05

### Added

- Add --enable-raylib option for ./configure
- Add --enable-anvilpick option for ./configure
  - Set repo_invil default for darwin
  - Use repo_invil default when using interactive selection for anvil
- Add -V to print build info
- Add WIP raylib build
- Add helapordo_build_string
- Add clock_t start_time to Gamestate
  - Add time under G_EXPERIMENTAL_ON for Enemy, Boss rooms
- Add mandatory define for build (mutually exclusive, HELAPORDO_CURSES_BUILD or HELAPORDO_RAYLIB_BUILD)
- Add hlpd_dbg_features()

### Changed

- Update PACK_NAME to $(TARGET)-$(HL_BUILD_STR)-$(VERSION)-$(OS)-$(MACHINE)
- Print build in title screen
- Bump sprites4curses to 0.4.2
- Bump amboso to 2.0.1

## [1.3.6] - 2024-01-03

### Added

- Add cargo_invil source for anvil in bootstrap_anvil.sh
- Add menu hint in handleTutorial()
- Add --enable-anvilpick argument for ./configure
- Add ANVIL_IMPL, substituted by repo_invil on macOS by default

### Changed

- Fix: build time in info panel is now the correct value
- Refactor printTitle()
- Bump sprites4curses to 0.4.0
- Update Copyright to 2022-2024

## [1.3.5] - 2023-12-29

### Changed

- Fix: memory leaks of ITEMS from Boss, Items, Enemies and Shop rooms
- Rename MINGW32_BUILD to WINDOWS_BUILD
- Improve configure.ac to recognize darwin on macOS
- Bump amboso to 2.0.0
- Bump invil to 0.2.1
- Bump koliseo to 0.3.20
- Bump sprites4curses to 0.3.6

## [1.3.4] - 2023-12-17

### Changed

- Bump koliseo to 0.3.18
- Bump amboso to 1.9.9
- Bump invil to 0.1.6
- Drop 1.3.3 from stego.lock

## [1.3.3] - 2023-12-10

### Added

- Add invil at 0.1.1, to optionally replace amboso
- Add scripts/bootstrap_anvil.sh to automate linking ./anvil

### Changed

- Add ./anvil to .gitignore

## [1.3.2] - 2023-12-03

### Changed

- Add doxygen index
- Update issue templates
- Bump koliseo to 0.3.12
- Bump amboso to 1.9.6
- Update stego.lock format

## [1.3.1] - 2023-11-17

### Added

- Add Skillslot, skillType
- Add basic functions for skillType
- Add skills array to Fighter, Enemy, Boss
- Add defer_skill_enemy(). defer_skill_boss()
- Add do_Skill(), do_Skill_boss() (WIP)

### Changed

- Update text savefile version to 0.1.7
- Update turnOP_args to hold picked_skill

## [1.3.0] - 2023-11-16

### Changed

- Add back --std=c11 to build and drop -Wno-trigraphs
- Regen animations with 0.2.2 format from sprites4curses
- Bump sprites4curses to 0.3.5
- Bump koliseo to 0.3.11
- Bump amboso to 1.9.0

## [1.2.10] - 2023-11-13

### Changed

- Drop --std=c11 to avoid broken animations

## [1.2.9] - 2023-11-12

### Changed

- Rename format.sh to format
- Fix: add astyle install to make-anviltest.yml
- Add --std=c11 to build
- Bump koliseo to 0.3.10
- Bump amboso to 1.7.4

## [1.2.8] - 2023-11-06

### Added

- Add format.sh to use astyle -s4 --style=linux
- Add base for OP_SKILL
- Add check_format anvil test

### Changed

- Fix: remove K&R C =- confusion
- Fix: broken build for nasty git tag description
- Drop commented setlocale() calls
- Add SPDX license identifier to header files
- Bump koliseo to 0.3.7
- Bump amboso to 1.6.10

## [1.2.7] - 2023-10-26

### Added

- Better loggig for floor info, including explored matrix

### Changed

- Fix: call update_Gamestate() with current room class
- Fix: animations are not restarted on every turn taken
- Fix: kls_free() is called on short flag runs (-h, -v, -T)
- Fix: refactor retry() for a better handling of input
- Update log messages for defer_fight

## [1.2.6] - 2023-10-24

### Added

- Add set_Saveslot_name() to try reading the save name
- Add dbg_print_floor_layout() and dbg_print_roomclass_layout() to log current floor info

### Changed

- Gamestate holds current floor
- Fix: fix minw32 build
- Bump koliseo to 0.3.4

## [1.2.5] - 2023-10-20

### Changed

- Bump koliseo to 0.3.3

## [1.2.4] - 2023-10-12

### Changed

- Bump koliseo to 0.3.0
- Changes kls_new() calls to kls_new_conf()
- Moves some define directives used in rooms.c from helapordo.h to game_core.h

## [1.2.3] - 2023-10-06

### Added

- Add the reset call needed for Enemy and Boss Turncounter for stat boosts
- Add the logic needed to display info for stat boosts

### Changed

- Fix: make pack no longer botches the zip file for mingw32
- Fix: -r and -E flags no longer cause a crash
- Bump sprites4curses to 0.3.4
- Bump amboso to 1.6.6

## [1.2.2] - 2023-09-21

### Added

- Implements boss_attack()

### Changed

- Change bossTurnPick() to be able to return FOE_FIGHT
- Fix: crash on retry() from gameloop()

## [1.2.1] - 2023-09-20

### Changed

- Enemies can pick a turn without G_EXPERIMENTAL_ON

## [1.2.0] - 2023-09-20

### Changed

- Regenerated animations to use sprites4curses v0.3.2 format (0.2.1)
- Bump koliseo to 0.2.2
- Bump sprites4curses to 0.3.3

## [1.1.9] - 2023-09-17

### Added

- Add proof-of-concept basic logic to handle foe actions (ATM only runs with G_EXPERIMENTAL_ON can make the enemies not idle
- Introduce simplified Rogue run as gameloop_Win() for Windows build
- Add fight-wrapping functions to allow for foes to pick a turn

### Changed

- Change make to expect python3 instead of python
- Fix: bad color codes in specials.c

## [1.1.8] - 2023-09-14

### Added

- Add gameloop_Win() to give something to do to Windows build users
- Minimal logs for Windows build.
- Add some defines for Windows color indexes, representing cmd.exe default pairs

### Changed

- Fix: -L and -I flags for building on macOS
- Halve binary size
- Bump sprites4curses to 0.3.0
- Replace MINGW32_BUILD preprocessor conditionals with _WIN32

## [1.1.7] - 2023-09-13

### Changed

- Drop usage of init_game_color_pairs(), and in general should remove usage of the legacy color pairs
- Update colors for Rogue mode floor map, to aid in navigation
- Bump koliseo to 0.2.1, with Region mode disabled for any Koliseo_Temp
- Fix: improve build flags in Makefile.am, removing unused linker flags from object file targets

## [1.1.6] - 2023-09-10

### Added

- Add MINGW32_BUILD to enable some lines when building for x86_64-w64-mingw32

### Changed

- Drop termios.h inclusion
- Compilation is successful for mingw32 but the output binary is not working ATM
- Removes a couple of init_game_color_pairs(), starting the cleanup of old color definitions
- Bump koliseo to 0.1.21
- Bump amboso to 1.6.5

## [1.1.5] - 2023-09-07

### Added

- Add foeTurnOption and foeTurnOption_OP for later use
- Add stamina to Fighter, Enemy and Boss for later use

### Changed

- Use bigger sizes for kls_new(), 2x for default_kls and 4x for temporary_kls
- Changes calls to kls_free() to be made when the game is won
- Bump koliseo to 0.1.20
- Bump amboso to 1.6.4
- Prettier Makefile output

## [1.1.4] - 2023-09-06

### Added

- Adds Gamestate, already passed to turnOP()

### Changed

- Fix: Memory leaks from menu_items not being freed
- Makes log_tag() variadic
- Bump koliseo to 0.1.19

## [1.1.3] - 2023-09-02

### Changed

- Bump koliseo to 0.1.18

## [1.1.2] - 2023-09-02

### Added

- Add HLP_Region_Type, to type the allocated regions

### Changed

- Bump koliseo to 0.1.17
- Switches allocations to KLS_PUSH_TYPED()
- Rework 'K' and 'T' queries for debug menu, old ones moved to 'k' and 't'

## [1.1.1] - 2023-09-01

### Changed

- Bump sprites4curses to 0.2.12
- Drop static runtime dependency on palette.gpl
- Disable debug mode by default. Build by using ./configure --enable-debug to enable debug log and debug flag usage

## [1.1.0] - 2023-09-01

### Changed

- Animations and lores are now compiled rather than being loaded at runtime.
  - Causes a noticeable size increase for the output binary, but lets us drop the static dependency on the text files at runtime.

- Animation files and lore files are no longer installed by `installer`, nor packed by `make pack`.

### Notes

- If you wish to install a local version of `helapordo` that can run versions `<1.1.0`, you must do so from a previous version and *not* uninstall, as that will remove the whole `helapordo-local` dir and you will lose the correct animation files again.
- Running older versions is always best when calling the game from a corrently checked out repo (which should guarantee the file format version of the animation files is compliant with the one expected by binary).

## [1.0.11] - 2023-08-29

### Added

- New minimal menu to select from 3 saveslots

### Changed

- Bump koliseo to 0.1.11
- Update 'd' debug command to show current saveslot

## [1.0.10] - 2023-08-27

### Changed

- Fix: working load/save by removing a typo from load_args push to default_kls
- Marks versions >=1.0.3, <1.0.10 as deprecated in stego.lock
## [1.0.9] - 2023-08-26

### Changed

- Bump koliseo to 0.1.10
- Builds koliseo/src/koliseo.o with KOLISEO_HAS_CURSES defined
- Fix: Free temporary_kls on death()

## [1.0.8] - 2023-08-25

### Changed

- Bump koliseo to 0.1.9
- Enable logging of default_kls and temporary_kls to the debug log file

## [1.0.7] - 2023-08-24

### Added

- Adds temporary_kls globally, to handle allocations with less-than-program lifetime
- Adds a menu for Rogue map walking, it is opened by pressing m while on the map.

### Changed

- Fix: force usage of Enter inside Home room to pick turn choice

### Notes

- Currently, the Rogue menu won't show a Debug option, since it didn't get a valid Room pointer for that specific turnOP(DEBUG) call. To see the crash, run with G_EXPERIMENTAL_ON.

## [1.0.6] - 2023-08-23

### Added

- Uses Koliseo_Temp in many places to wrap the previous malloc()
- Disables the related free() calls
- For now, turnOP_args is still allocated with malloc()

### Changed

- Bump amboso to 1.6.3
- Fix: wrong error message for Turncounter generation

## [1.0.5] - 2023-08-22

### Changed

- Bump koliseo to 0.1.6

## [1.0.4] - 2023-08-22

### Changed

- Bump amboso to 1.6.1
- Bump koliseo to 0.1.5

## [1.0.3] - 2023-08-21

### Changed

- Bump koliseo to 0.1.4

## [1.0.2] - 2023-08-19

### Added

- First introduction on default_kls for allocation.

### Changed

- Bump sprites4curses to 0.1.2
- Bump koliseo to 0.1.2
- Update the gif in README

## [1.0.1] - 2023-08-08

### Added

- Add Koliseo submodule

### Changed

- Fix: memory leak in unlockSpecial()
- Fix: use after free in handleRoom_Enemies()
- Fix: handleStats() being called twice on death in an ENEMIES room.

## [1.0.0] - 2023-08-05

- First release
