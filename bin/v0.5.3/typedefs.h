/**
 * Ceiling for Fighter luck.
 * @see Fighter
 */
#define MAXPLAYERLUCK 30

/**
 * Ceiling for Path length.
 * @see Path
 */
#define MAXLENGTH 100

/**
 * Ceiling for Path luck.
 * @see Path
 */
#define MAXLUCK 50
    
/** 
 * The different kinds of Consumable.
 * Ordering is directly related to consumablestrings
 * @see consumablestrings
 * @see Consumable
 */
typedef enum {
	Potion=0,
	Rock=1,
	Bread=2,
	Coffee=3,
	Powergem=4,
	Magicbean=5
} consumableClass;

/** Maximum index of consumableClass, so that the size has to add 1 for the 0th index
 * @see consumableClass
 */
#define CONSUMABLESMAX 5 

/** 
 * Holds a certain qty of a consumableClass.
 * @see useConsumable()
 */
typedef struct {
	consumableClass class; /**< Defines which kind of Consumable this is*/
	char name[25]; /**< Contains the name of the consumable*/
	char desc[50]; /**< Contains a brief description of the consumable*/
	int qty; /**< Indicates how many copies of the consumable the instance of this class holds*/
} Consumable;

/**
 * Array with the name strings for Consumable.
 * @see consumableClass
 * @see Consumable
 */
static char* consumablestrings[] = {
	"Potion",
	"Rock",
	"Bread",
	"Coffee",
	"Powergem",
	"Magic Bean"
};

/**
 * Array containing all Consumable instances.
 * All the Consumable->qty values are initialised to 0
 * @see Consumables
 * @see consumableClass
 */
static Consumable consumablesBag[CONSUMABLESMAX+1] = {

	{Potion,"Amberosia","Completely restores your health.",0},
	{Rock,"Dwayne Johnson","Hurts the enemy and lowers its defences.",0},
	{Bread,"Carbowheat","Enhances your power but slows you a little.",0},
	{Coffee,"Espresso","Makes you faster and tense.",0},
	{Powergem,"Power Gem","You will feel wiser",0},
	{Magicbean,"Magic Bean","Grants you new magic strength.",0}
};

/** 
 * The different kinds of fighterStatus.
 */
typedef enum { 
	Normal,
	Poison,
	Frozen,
	Burned,
	Weak,
	Strong
} fighterStatus;

/** Maximum index of fighterStatus, so that the size has to add 1 for the 0th index
 * @see fighterStatus
 */
#define STATUSMAX 5 

/** 
 * The different kinds of Fighter.
 * Ordering is directly related to classesstrings
 * @see classesstrings
 * @see Fighter
 */
typedef enum { 
	Knight=0,
	Archer=1,
	Mage=2,
	Assassin=3
} fighterClass;

/** Maximum index of fighterClass, so that the size has to add 1 for the 0th index
 * @see fighterClass
 */
#define CLASSESMAX 3 

/**
 * Array with the name strings for fighterClass.
 * @see fighterClass
 */
static char* classesstrings[] = {
	"Knight",
	"Archer",
	"Mage",
	"Assassin"
};

/** 
 * The different kinds of Stat.
 */
typedef enum {
	ATK,
	DEF,
	VEL,
	ENR
} Stat;

/** Maximum index of Stat, so that the size has to add 1 for the 0th index
 * @see Stat
 */
#define STATMAX 3

/** 
 * Holds the base stats for a fighterClass.
 * @see Fighter
 * @see fighterClass
 * @see initStats()
 */
typedef struct {
	fighterClass class; /**< Defines which kind of fighterClass the stats relate to*/
	int hp; /**< Hp value*/
	int atk; /**< Atk value*/
	int def; /**< Def value*/
	int vel; /**< Vel value*/
	int level; /**< Level value*/
	int totalxp; /**< Lifetime xp value*/
        int totallevelxp; /**< Xp value needed to level up from current level*/
	int currentlevelxp; /**< Total xp value for current level*/
	int totalhp; /**< Total hp value*/
	int totalenergy; /**< Total energy value*/
} BaseStats;

/**
 * Array containing all BaseStats instances.
 * All the classes get their starting stats from here.
 * Ordering of parameters is: hp, atk, def, vel, level, totalxp (0), totallevelxp (1), currentlevelxp (0), totalhp, totalenergy
 * @see Fighter
 * @see fighterClass
 * @see initStats()
 */
static BaseStats basestats[CLASSESMAX+1] = {

	{Knight,32,12,22,7,1,0,1,0,32,5},
	{Archer,24,15,14,11,1,0,1,0,24,5},
	{Mage,17,19,10,8,1,0,1,0,17,5},
	{Assassin,20,16,12,10,1,0,1,0,20,5}

};

/** 
 * The different kinds of Enemy.
 * Ordering is directly related to classenemystrings
 * @see classenemystrings
 * @see Enemy
 */
typedef enum {
	Mummy=0,
	Ghost=1,
	Zombie=2,
	Goblin=3,
	Imp=4,
	Troll=5,
	Boar=6,
	Werewolf=7
} enemyClass;

/** Maximum index of enemyClass, so that the size has to add 1 for the 0th index
 * @see enemyClass
 */
#define ENEMYCLASSESMAX 7

/**
 * Array with the name strings for enemyClass.
 * @see enemyClass
 * @see printStringFromEClass()
 */
static char* classenemystrings[] = {
	"Mummy",
	"Ghost",
	"Zombie",
	"Goblin",
	"Imp",
	"Troll",
	"Boar",
	"Werewolf"
};

/** 
 * Holds the base stats for a enemyClass.
 * @see Enemy
 * @see enemyClass
 * @see initEnemyStats()
 */
typedef struct {
	enemyClass class; /**< Defines which kind of enemyClass the stats relate to*/
	int hp; /**< Hp value*/
	int atk; /**< Atk value*/
	int def; /**< Def value*/
	int vel; /**< Vel value*/
	int level; /**< Level value*/
	int xp; /**< Xp value given on death */
	int totalhp; /**< Total hp value*/
	int beast; /**< Flag used for "beast" enemies*/
	int totalenergy;/**< Total energy value*/
} EnemyBaseStats;

/**
 * Array containing all EnemyBaseStats instances.
 * All the classes get their starting stats from here.
 * Ordering of parameters is: hp, atk, def, vel, level, xp, totalhp, beast(0), totalenergy
 * @see Enemy
 * @see enemyClass
 * @see initEnemyStats()
 */
static EnemyBaseStats baseenemystats[ENEMYCLASSESMAX+1] = {

	{Mummy,22,11,7,5,1,1,22,0,5},
	{Ghost,35,5,16,6,1,2,35,0,5},
	{Zombie,20,6,7,5,1,1,20,0,5},
	{Goblin,32,10,8,11,1,2,32,0,5},
	{Imp,24,12,7,10,1,3,24,0,5},
	{Troll,42,4,17,4,2,3,42,0,5},
	{Boar,30,9,12,8,2,4,30,0,5},
	{Werewolf,20,14,5,7,1,3,20,0,5}
};

/** 
 * Holds the lifetime stats of the player.
 * @see Fighter
 */
typedef struct {
	int enemieskilled; /**< How many enemies were defeated*/
	int consumablesfound; /**< How many consumabls were found*/
	int equipsfound; /**< How many equips were found*/
	int artifactsfound; /**< How many artifacts were found*/
	int criticalhits; /**< How many criticals were dealt*/
	int roomscompleted; /**< How many rooms were completed*/
	int specialsunlocked; /**< How many special move were unlocked*/
} countStats;

/** 
 * The different kinds of special moves.
 * Ordering is directly related to specialsstrings and specialscosts.
 * There must always be exactly SPECIALSMAX +1 values for each fighterClass.
 * @see specialsstrings
 * @see specialscosts
 */
typedef enum {
	KSlash=0,
	KCover=1,
	KArmordrop=2,
	KBerserk=3,
	AHeadshot=4,
	AQuivercheck=5,
	APoisonshot=6,
	AFireshot=7,
	MFatewarp=8,
	MPowerup=9,
	MSpellstrike=10,
	MFlamering=11,
	XGrimdagger=12,
	XLeechknife=13,
	XDisguise=14,
	XVenomblade=15
} specialMove;


/** Maximum index of specialMove per class, so that the size for each Fighter has to add 1 for the 0th index
 * @see specialMove
 */
#define SPECIALSMAX 3 //num of specials per class (0 inclusive)

/**
 * Array with the name strings for special moves.
 * @see specialMove
 */
static char* specialsstrings[CLASSESMAX+1][SPECIALSMAX+1] = {
	{"Slash Hit","Shield Cover","Armor Drop","Berserk Rush"} , /**< names for Knight, fighterClass 0*/
	{"Headshot","Quiver Check","Poison Shot","Burning Arrow"} , /**< names for Archer, fighterClass 1*/
	{"Fate Warp","Power Spell","Pity Spell","Flame Ring"} , /**< names for Mage, fighterClass 2*/
	{"Grim Dagger","Leech Knife","Disguise Kit","Venomous Needles"} , /**< names for Assassin, fighterClass 3*/
};

/**
 * Array with the cost integer values for special moves.
 * @see specialMove
 */
static int specialscosts[CLASSESMAX+1][SPECIALSMAX+1] = {
	
	{1,2,3,3} , /**< costs for Knight, fighterClass 0*/
	{2,4,2,3} , /**< costs for Archer, fighterClass 1*/
	{4,3,2,3} , /**< costs for Mage, fighterClass 2*/
	{4,3,3,3} ,  /**< costs for Assassin, fighterClass 3*/
};

/** 
 * Holds state for a Fighter specials.
 * @see Fighter
 * @see specialMove
 */
typedef struct {
	int enabled; /**< Flag defining if the current slot is initialised*/
	specialMove move; /**< Defines which kind of specialMove the slot is holding*/
	char* name; /**< Name string*/
	int cost; /**< Cost of use*/
} Specialslot;

//Forward declarations for counter fields in entities
struct Turncounter;

/** 
 * The different indexes for Fighter->counters.
 * Ordering is directly related to Fighter->counters.
 * There must always be exactly COUNTERSMAX +1 values.
 * @see Turncounter
 * @see Fighter
 */
typedef enum {
	NORMAL=0,
	POISON=1,
	BURNED=2,
	FROZEN=3,
	WEAK=4,
	STRONG=5,
	TURNBOOST_ATK=6,
	TURNBOOST_DEF=7,
	TURNBOOST_VEL=8,
	TURNBOOST_ENR=9
} counterIndexes;
//Number of counters per entity

/** Maximum index of counterIndexes, so that the size has to add 1 for the 0th index
 * @see counterIndexes
 */
#define COUNTERSMAX 9

//Forward declarations for equipslot fields in entities
struct Equipslot;
struct Equip;

/** 
 * The different kinds of equip zones.
 * Ordering is directly related to equipzonestrings.
 * There must always be exactly EQUIPZONES +1 values.
 * @see equipzonestrings
 */
typedef enum {
	HEAD=0,
	TORSO=1,
	LEGS=2
} Equipzone;

/** Maximum index of Equipzone, so that the size has to add 1 for the 0th index
 * @see Equipzone
 */
#define EQUIPZONES 2

/**
 * Array with the name strings for equip zones.
 * @see Equipzone
 */
static char* equipzonestrings[] = {
	"Head",
	"Torso",
	"Legs"	
};

/* Ceiling for Fighter equipsBag indexes. 
 * @see Fighter
 */
#define EQUIPSBAGSIZE 8

/** 
 * The different kinds of perks.
 * Ordering is directly related to perksstrings.
 * There must always be exactly PERKSMAX +1 values.
 * @see perksstrings
 */
typedef enum {
	CRITBOOST_DMG=0,
	CRITBOOST_CHANCE=1,
	VAMPIRISM=2,
	RUNIC_MAGNET=3,
	HARVESTER=4,
	HOT_BODY=5,
	BIOHAZARD=6,
	ORACLE_GIFT=7,
	PENICILLIN=8
} perkClass;

/** Maximum index of perkClass, so that the size has to add 1 for the 0th index
 * @see perkClass
 */
#define PERKSMAX 8 

/** Maximum number of Perk per Equip
 * @see Perk
 * @see Equip
 */
#define EQUIPPERKSMAX 2

/**
 * Array with the name strings for perkClass.
 * @see perkClass
 */
static char* perksnamestrings[] = {
	"Mighty Strikes",
	"Lucky Charm",
	"Garlic Fever",
	"Runic Circle",
	"Hot Body",
	"Harvester",
	"Biohazard",
	"Oracle Gift",
	"Penicillin"
};

/**
 * Array with the desc strings for perkClass.
 * @see perkClass
 */
static char* perksdescstrings[] = {
	"When you hit hard, you hit REALLY HARD.",
	"You may have better chances at smashing.",
	"Enemy blood seems attractive.",
	"Monks don't have to rest.",
	"Feels burning to the touch.",
	"You reap more fruits.",
	"You shall bring the plague upon them.",
	"A second call from the door.",
	"You are prepared for pollen season."
};

/** 
 * Represents the entity initialised from a perkClass.
 * @see perkClass
 */
typedef struct {
	perkClass class; /**< Defines which kind of perk the instance relates to*/
	char name[25]; /**< Name string*/
	char desc[50]; /**< Description string*/
	int innerValue; /**< Contains an integer for perks that need a state*/
} Perk;

/** 
 * Represents the entity initialised from a fighterClass.
 * @see fighterClass
 * @see fighterStatus
 * @see initStats()
 * @see Specialslot
 * @see Turncounter
 * @see Equipslot
 * @see countStats
 */
typedef struct {
	char name[25]; /**< Name string*/
	fighterClass class; /**< Defines which kind of fighterClass the instance relates to*/
	int hp; /**< Current hp value*/
	int atk; /**< Current atk value*/
	int def; /**< Current def value*/
	int vel; /**< Current vel value*/
	int level; /**< Current level value*/
	int luck; /**< Current luck value*/
	int totalxp; /**< Lifetime xp value*/
	int currentlevelxp; /**< Xp gained for the current level*/
	int totallevelxp; /**< Xp needed to level up*/
	int totalhp; /**< Full hp value*/
	fighterStatus status; /**< Defines active fighterStatus*/
		
	int energy; /**< Current energy value*/
	int totalenergy; /**< Full energy value*/
	Specialslot* specials[SPECIALSMAX+1]; /**< Array with all the Specialslot*/
	
	struct Turncounter* counters[COUNTERSMAX+1]; /**< Array with all the Turncounter*/
	int turnboost_atk; /**< Current temp boost value for atk*/
	int turnboost_def; /**< Current temp boost value for def*/
	int turnboost_vel; /**< Current temp boost value for vel*/
	int turnboost_enr; /**< Current temp boost value for enr*/
	
	int perksCount; /**< Keeps track of how many perks are active.*/
	Perk* perks[PERKSMAX+1]; /**< Array with all the Perk*/

	struct Equipslot* equipslots[EQUIPZONES+1]; /**< Array with all the Equipslot*/
	struct Equip* equipsBag[EQUIPSBAGSIZE+1]; /**< Array with all the Equip found*/
	
	int equipsBagOccupiedSlots; /**< Keeps track of how many slots are occupied.*/
	int earliestBagSlot; /**< Keeps track of first available spot.*/
		//To alwasy use the array efficiently (???) I sense linked lists may be better

	int permboost_atk; /**< Current temp boost value for atk*/
	int permboost_def; /**< Current temp boost value for atk*/
	int permboost_vel; /**< Current temp boost value for atk*/
	int permboost_enr; /**< Current temp boost value for atk*/
	
	int equipboost_atk; /**< Current equip boost value for atk*/
	int equipboost_def; /**< Current equip boost value for atk*/
	int equipboost_vel; /**< Current equip boost value for atk*/
	int equipboost_enr; /**< Current equip boost value for atk*/

	countStats* stats ; /**< Pointer to countStats instance*/
} Fighter;

/** 
 * Represents the entity initialised from a enemyClass.
 * @see enemyClass
 * @see fighterStatus
 * @see initEnemyStats()
 * @see Turncounter
 * @see counterIndexes
 */
typedef struct {
	enemyClass class; /**< Defines which kind of enemyClass the instance relates to*/
	int hp; /**< Current hp value*/
	int atk; /**< Current atk value*/
	int def; /**< Current def value*/
	int vel; /**< Current vel value*/
	int level; /**< Level value*/
	int luck; /**< Luck value*/
	int xp; /**< Xp value given on death*/
	int totalhp; /**< Full hp value*/
	
	int energy; /**< Current energy value*/
	int totalenergy; /**< Full energy value*/
	
	fighterStatus status; /**< Defines active fighterStatus*/
	int beast; /**< Flag defining the instance as "beast" if set*/

	struct Turncounter* counters[COUNTERSMAX+1]; /**< Array with all the Turncounter*/
	int turnboost_atk; /**< Current temp boost value for atk*/
	int turnboost_def; /**< Current temp boost value for def*/
	int turnboost_vel; /**< Current temp boost value for vel*/
	int turnboost_enr; /**< Current temp boost value for enr*/
} Enemy;

/**
 * Defines a function pointer returning void and taking a Fighter pointer.
 * @see Fighter
 * @see Turncounter
 */
typedef void (*effect_fun)( Fighter*);
/**
 * Defines a function pointer returning void and taking a Enemy pointer.
 * @see Enemy
 * @see Turncounter
 */
typedef void (*effect_e_fun)( Enemy*);

/**
 * Defines a function pointer returning void and taking a Fighter pointer and an integer.
 * @see Fighter
 * @see Turncounter
 */
typedef void (*boost_fun)( Fighter*, int boost);
/**
 * Defines a function pointer returning void and taking a Enemy pointer and an integer.
 * @see Enemy
 * @see Turncounter
 */
typedef void (*boost_e_fun)( Enemy*, int boost);

/**
 * Defines a function pointer returning void and taking a Fighter and a Enemy pointers.
 * @see Enemy
 * @see Turncounter
 */
typedef void (*artifact_fun)( Fighter*, Enemy*);

/** 
 * The different kinds of Countertype.
 * @see Turncounter
 */
typedef enum {
	CNT_STATUS, /**< Counter for a fighterStatus.*/
	CNT_ATKBOOST, /**< Counter for a temp boost on atk.*/
	CNT_DEFBOOST,  /**< Counter for a temp boost on def.*/
	CNT_VELBOOST, /**< Counter for a temp boost on vel.*/
	CNT_ENRBOOST  /**< Counter for a temp boost on enr.*/
} Countertype;

/** 
 * Represents the entity initialised from a CounterType.
 * @see Countertype
 * @see Fighter
 * @see Enemy
 */
typedef struct {
	int count; /**< Defines the current count.*/
	char* desc; /**< Description string*/
	int innerValue; /**< Contains an integer for counters that need a state*/
	Countertype type; /**< Defines which kind of Countertype the instance relates to*/
	void (*effect_fun)( Fighter*); /**< Pointer to effect function applying to Fighter*/
	void (*effect_e_fun)( Enemy*); /**< Pointer to effect function applying to Enemy*/
	void (*boost_fun)( Fighter*, int); /**< Pointer to temp boost function applying to Fighter, takes a int*/
	void (*boost_e_fun)( Enemy*, int); /**< Pointer to temp boost function applying to Enemy, takes a int*/
} Turncounter;

/** 
 * The different kinds of artifactClass.
 * Ordering is directly related to artifactstrings.
 * There must always be exactly ARTIFACTSMAX +1 values.
 * @see artifactstrings
 * @see Artifact
 */
typedef enum {
	THKSKULL=0,
	TWINKIE=1,
	WRISTBAND=2,
	BOARTAIL=3,
	CHAOSORB=4,
	POWERSYPHON=5,
	GIANTFOSSILE=6
} artifactClass;

/** Maximum index of artifactClass, so that the size has to add 1 for the 0th index
 * @see Equipzone
 */
#define ARTIFACTSMAX 6 

/** 
 * Represents the entity initialised from a artifactClass.
 * @see artifactClass
 */
typedef struct {
	artifactClass class; /**< Defines which kind of artifactClass the instance relates to*/
	char name[25]; /**< Name string*/
	char desc[50]; /**< Description string*/
	int qty; /**< Indicates how many copies of the artifact the instance of this class holds*/
	int active; /**< Flag used to mark instance whose function pointer has been called already*/
	int innerValue; /**< Indicates interal state of the item when needed*/
	void * artifact_fun; /**< Pointer to artifact function */
} Artifact;

/**
 * Array with the name strings for artifactClass.
 * @see artifactClass
 */
static char* artifactstrings[] = {
	"Thinking Skull",
	"Soft Twinkie",
	"Gold Wristband",
	"Boar Tail",
	"Dark Orb",
	"Power Syphon",
	"Giant Fossile"
};
 /**
  * Array with all the Artifact found.
  * @see Artifact
  */
static Artifact artifactsBag[ARTIFACTSMAX+1] = {

	{THKSKULL,"Thinking Skull","You learn from questioning.",0,0,0},
	{TWINKIE,"Soft Twinkie","Zombies seem to not like this.",0,0,0},
	{WRISTBAND,"Gold Wristband","I'd have been cooler on your finger.",0,0,0},
	{BOARTAIL,"Boar Tail","The beast managed to run.",0,0,0},
	{CHAOSORB,"Dark Orb","This feels ominous.",0,0,0},
	{POWERSYPHON,"Power Syphon","Energy can only be transformed.",0,0,0},
	{GIANTFOSSILE,"Giant Fossile","Maybe some species really are related.",0,0,0}
};

/** 
 * Holds the state of game progression.
 */
typedef struct {
	int length; /**< Defines how many rooms there are in total.*/
	int luck; /**< Defines global luck value.*/
	int prize; /**< Defines the reward for getting to length*/
} Path;

/** 
 * The different kinds of Equip.
 * Ordering is directly related to equipstring.
 * There must always be exactly EQUIPSMAX +1 values.
 * @see equipstrings
 */
typedef enum {
	Vest=0,
	Helmet=1,
	Boots=2,
	Cape=3,
	Tophat=4,
	Loafers=5
} equipClass;

/** Maximum index of equipClass, so that the size has to add 1 for the 0th index
 * @see equipClass
 */
#define EQUIPSMAX 5 

/** 
 * The different grades of Equip.
 * Ordering is directly related to qualitytrings.
 * There must always be exactly QUALITIESMAX +1 values.
 * @see qualitytrings
 */
typedef enum {
	Bad=0,
	Average=1,
	Good=2,
} quality;

/** Maximum index of quality, so that the size has to add 1 for the 0th index
 * @see quality
 */
#define QUALITIESMAX 2 

/**
 * Array with the name strings for quality.
 * @see quality
 */
static char* qualitytrings[] = {
	"Bad",
	"Average",
	"Good"	
};

/** 
 * Represents the entity initialised from a equipClass.
 * @see equipClass
 * @see Equipzone
 * @see quality
 */
typedef struct {
	equipClass class; /**< Defines which kind of equipClass the instance relates to*/
	Equipzone type; /**< Defines which kind of Equipzone the instance relates to*/
	char name[25]; /**< Name string*/
	char desc[50]; /**< Description string*/
	int qty; /**< Defines how many copies of the equipClass the instance holds. Should be <=1, >=0 */

	int equipped; /**< Flag indicating the instance as currently in use if set.*/

	int level; /**< Level value*/
	//Modifiers
	//  modifier
	int atk; /**< equip boost atk value*/
	int def; /**< equip boost def value*/
	int vel; /**< equip boost vel value*/
	int enr; /**< equip boost enr value*/
	
	int bonus; /**< Defines how many pointers to bonus functions the instance holds.*/
	int perksCount; /**< Defines how many Perk pointers the instance holds.*/
	
	quality qual; /**< Defines which kind of quality the instances relates to*/
	void (*equip_fun)(Fighter*,Enemy*,Path*); /**< Pointer to temp boost function applying to Enemy, takes a int*/
	Perk* perks[EQUIPPERKSMAX]; /**< Array of Perk of the instance*/
} Equip;

/**
 * Array with the name strings for equipClass.
 * @see equipClass
 */
static char* equipstrings[] = {
	"Vest",
	"Helmet",
	"Boots",
	"Cape",
	"Tophat",
	"Loafers"
};

/**
 * Array containing all Equip instances.
 * All the Equip->qty values are initialised to 0
 * @see Equip
 * @see equipClass
 */
static Equip equips[EQUIPSMAX+1] = {

	{Vest,TORSO,"Vest","Nice cloth.",0,0,1,0,1,0,0,0,0},
	{Helmet,HEAD,"Helmet","Fits on your head.",0,0,1,0,2,0,0,0,0},
	{Boots,LEGS,"Boots","NOT made for crawling.",0,0,1,1,0,1,0,0,0},
	{Cape,TORSO,"Cape","Doesn't make you fly.",0,0,1,2,0,0,1,0,0},
	{Tophat, HEAD,"Top Hat","Dapper you say?",0,0,1,0,1,0,1,0,0},
	{Loafers,LEGS,"Loafers","Soft and comfy for battles.",0,0,1,0,1,2,0,0,0}
};

/**
 * Defines a function pointer returning void and taking a Fighter and a Enemy pointers.
 * @see Equip
 * @see Fighter
 * @see equipClass
 */
typedef void (*equip_fun)(Fighter*,Enemy*);

/** 
 * Represents the entity containing an Equip instance.
 * @see Equip
 * @see Equipzone
 */
typedef struct {
	int active; /**< Flag indicating the instance as currently ready to use if set.*/
	char* desc; /**< Description string.*/
	Equipzone type; /**< Defines which kind of Equipzone the instance relates to*/
	Equip* item; /**< Pointer to the Equip instance associated*/
	//void (*equip_fun)( Fighter*,Enemy*); /**< Pointer to temp boost function applying to Enemy, takes a int
} Equipslot;

//Macros for special moves callback

/**
 * Defines a macro used for special moves, calling register_callback() with the first argument and the second casted to callback_void_t.
 * @see register_callback
 * @see callback_void_t
 */
#define REGISTER_CALLBACK(e,p) register_callback(e, (callback_void_t)p)

/**
 * Defines a macro used for special moves, casting the function pointer in callback_func_ptrs to the first argument.
 */
#define GET_CALLBACK(e,T) ((T)callback_func_ptrs[e])

//Macros for counters callback

//#define REGISTER_COUNTER_CALLBACK(c,x) register_counter_callback(c, (callback_void_t)x)
//#define GET_COUNTER_CALLBACK(c,C) ((C)callback_counter_ptrs[c])

/**
 * Defines a function pointer returning void and taking a Fighter pointer.
 * @see Fighter
 * @see Turncounter
 */
typedef void (*callback_turncounter_fighter_t)(Fighter*);

/**
 * Defines a function pointer returning void and taking a Enemy pointer.
 * @see Enemy
 * @see Turncounter
 */
typedef void (*callback_turncounter_enemy_t)(Enemy*);

/**
 * Defines a function pointer returning void and taking a Fighter, a Enemy and a Path pointers, along with two ints.
 * @see Fighter
 * @see Enemy
 * @see Path
 * @see SpecialSlot
 */
typedef void (*callback_special_t)(Fighter*,Enemy*,Path*,int,int);

/**
 * Defines a function pointer returning void and taking NO parameters (different than saying just (), unspecified parameters).
 */
typedef void (*callback_void_t)(void);

/**
 * Value of turn choice indicating normal fight.
 * @see getTurnChoice()
 * @see fight()
 */
#define FIGHT 100
/**
 * Value of turn choice indicating special attack.
 * @see getTurnChoice()
 * @see specialMove
 */
#define SPECIAL 101
/**
 * Value of turn choice indicating exiting the game.
 * @see getTurnChoice()
 * @see quit()
 */
#define QUIT 102
/**
 * Value of turn choice indicating managing consumables.
 * @see getTurnChoice()
 * @see Consumable
 */
#define CONSUMABLE 103
/**
 * Value of turn choice indicating managing artifacts.
 * @see getTurnChoice()
 * @see Artifact
 */
#define ARTIFACTS 104
/**
 * Value of turn choice indicating managing equippable items.
 * @see getTurnChoice()
 * @see Equip
 * @see Equipzone
 */
#define EQUIPS 105
/**
 * Value of turn choice indicating visualizing active perks.
 * @see getTurnChoice()
 * @see Perk
 */
#define PERKS 106
/**
 * Value of turn choice indicating managing game statistics.
 * @see getTurnChoice()
 * @see countStats
 */
#define STATS 107
/**
 * Value of turn choice indicating debug menu.
 * @see getTurnChoice()
 * @see debug()
 */
#define DEBUG 1337

/**
 * Ratio of beast rooms to normal ones. Every Xth room will have a beast.
 * @see initEnemyStats()
 */
#define BEASTROOM 3
/**
 * Factor for stat boost to beast enemies.
 * @see initEnemyStats()
 */
#define BSTFACTOR 1.5

/**
 * Ratio of Fighter levels to Enemy levels. Every Xth level up will apply to enemies.
 * @see initEnemyStats()
 */
#define ENEMYLVLRATIO 2
/**
 * Ratio of Fighter levels to unlock a new special move. Every Xth level up will unlock one.
 * @see unlockSpecial() 
 */
#define SPECIALLVLRATIO 2
/**
 * Ratio of Fighter levels to Equip levels. Every Xth level up will apply to equips.
 * @see dropEquip()
 */
#define EQUIPLVLBOOSTRATIO 3

/**
 * Value of fight status indicating the enemy died.
 * @see fight()
 */
#define KILL_DONE 1
/**
 * Value of fight status indicating the fighter was damaged.
 * @see fight()
 */
#define DMG_DEALT 2
/**
 * Value of fight status indicating the enemy was damaged.
 * @see fight()
 */
#define DMG_TAKEN 3
/**
 * Value of fight status indicating the fighter died.
 * @see fight()
 */
#define DEATH 4
/**
 * Value of fight status indicating no damage occurred.
 * Used to reset the fight status to neutral after the end of a combat..
 * @see fight()
 * @see room()
 */
#define NO_DMG 5
