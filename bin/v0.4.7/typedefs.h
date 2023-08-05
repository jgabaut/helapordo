#define MAXPLAYERLUCK 30
#define MAXLENGTH 100
#define MAXLUCK 50

typedef enum { 
	Normal,
	Poison,
	Frozen,
	Burned,
	Weak,
	Strong
} fighterStatus;
#define STATUSMAX 5 

typedef enum {
	Potion=0,
	Rock=1,
	Bread=2,
	Coffee=3,
	Powergem=4
} itemClass;
#define ITEMSMAX 4 

typedef struct {
	itemClass class;
	char name[25];
	char desc[50];
	int qty;
} Item;

static const char* itemstrings[] = {
	"Potion",
	"Rock",
	"Bread",
	"Coffee",
	"Powergem"
};

static Item bag[ITEMSMAX+1] = {

	{Potion,"Amberosia","Completely restores your health.",0},
	{Rock,"Dwayne Johnson","Hurts the enemy and lowers its defences.",0},
	{Bread,"Carbowheat","Enhances your power but slows you a little.",0},
	{Coffee,"Espresso","Makes you faster and tense.",0},
	{Powergem,"Power Gem","Makes you wiser",0}
};


typedef enum { 
	Knight=0,
	Archer=1,
	Mage=2,
	Assassin=3
} fighterClass;
#define CLASSESMAX 3 

static const char* classesstrings[] = {
	"Knight",
	"Archer",
	"Mage",
	"Assassin"
};

typedef enum {
	ATK,
	DEF,
	VEL,
	ENR
} Stat;
#define STATMAX 3

typedef struct {
	fighterClass class;
	int hp;
	int atk;
	int def;
	int vel;
	int level;
	int totalxp;
        int totallevelxp;
	int currentlevelxp;
	int totalhp;
	int totalenergy;
} BaseStats;

static BaseStats basestats[CLASSESMAX+1] = {

	{Knight,30,10,20,6,1,0,1,0,30,5},
	{Archer,24,13,13,9,1,0,1,0,24,5},
	{Mage,17,19,10,7,1,0,1,0,17,5},
	{Assassin,20,16,12,8,1,0,1,0,20,5}

};

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

#define ENEMYCLASSESMAX 7

static const char* classenemystrings[] = {
	"Mummy",
	"Ghost",
	"Zombie",
	"Goblin",
	"Imp",
	"Troll",
	"Boar",
	"Werewolf"
};

typedef struct {
	enemyClass class;
	int hp;
	int atk;
	int def;
	int vel;
	int level;
	int xp;
	int totalhp;
	int beast;
	int totalenergy;
} EnemyBaseStats;

static EnemyBaseStats baseenemystats[ENEMYCLASSESMAX+1] = {

	{Mummy,22,11,7,5,1,1,22,0,5},
	{Ghost,35,5,16,6,1,2,35,0,5},
	{Zombie,20,6,7,5,1,1,20,0,5},
	{Goblin,32,10,8,11,2,2,32,0,5},
	{Imp,24,12,7,10,1,3,24,0,5},
	{Troll,45,4,18,4,1,4,45,0,5},
	{Boar,30,9,12,8,1,4,30,0,5},
	{Werewolf,20,14,5,7,1,3,20,0,5}
};

typedef struct {
	int enemieskilled;
	int itemsfound;
	int artifactsfound;
	int criticalhits;
	int roomscompleted;
	int specialsunlocked;
} countStats;

typedef enum {
	KSlash=0,
	KCover=1,
	KArmordrop=2,
	AHeadshot=3,
	AQuivercheck=4,
	APoisonshot=5,
	MFatewarp=6,
	MPowerup=7,
	MSpellstrike=8,
	XGrimdagger=9,
	XLeechknife=10,
	XDisguise=11
} specialMove;

#define SPECIALSMAX 2 //num of specials per class (0 inclusive)

static char* specialsstrings[CLASSESMAX+1][SPECIALSMAX+1] = {
	{"Slash Hit","Shield Cover","Armor Drop"} , //0 - Knight
	{"Headshot","Quiver Check","Poison Shot"} , //1 - Archer
	{"Fate Warp","Power Spell","Pity Spell"} , //2 - Mage
	{"Grim Dagger","Leech Knife","Disguise Kit"} , //3 - Assassin
};

static int specialscosts[CLASSESMAX+1][SPECIALSMAX+1] = {
	
	{2,2,3} , //0 - Knight
	{3,4,2} , //1 - Archer
	{4,3,3} , //2 - Mage
	{4,3,3} , //3 - Assassin
};


//Forward declarations for counter fields in entities
struct Turncounter;

//Number of counters per entity

#define COUNTERSMAX 15

typedef struct {
	int enabled;
	specialMove move;
	char* name;
	int cost;
} Specialslot;

typedef struct {
	char name[25];
	fighterClass class;
	int hp;
	int atk;
	int def;
	int vel;
	int level;
	int luck;
	int totalxp;
	int currentlevelxp;
	int totallevelxp;
	int totalhp;
	fighterStatus status;
	
	int energy;
	int totalenergy;
	Specialslot* specials[SPECIALSMAX+1];
	
	struct Turncounter* counters[COUNTERSMAX+1];
	int turnboost_atk;
	int turnboost_def;
	int turnboost_vel;
	int turnboost_enr;

	int permboost_atk;
	int permboost_def;
	int permboost_vel;
	int permboost_enr;

	countStats* stats ;
} Fighter;

typedef struct {
	enemyClass class;
	int hp;
	int atk;
	int def;
	int vel;
	int level;
	int luck;
	int xp;
	int totalhp;
	int energy;
	int totalenergy;
	fighterStatus status;
	int beast;
} Enemy;

typedef void (*effect_fun)( Fighter*);
typedef void (*effect_e_fun)( Enemy*);

typedef void (*boost_fun)( Fighter*, int boost);
typedef void (*artifact_fun)( Fighter*, Enemy*);

typedef enum {
	CNT_STATUS,
	CNT_ATKBOOST,
	CNT_DEFBOOST,
	CNT_VELBOOST,
	CNT_ENRBOOST
} Countertype;

typedef struct {
	int count;
	char* desc;
	int innerValue;
	Countertype type;
	void (*effect_fun)( Fighter*);
	void (*effect_e_fun)( Enemy*);
	void (*boost_fun)( Fighter*, int);
} Turncounter;

typedef enum {
	THKSKULL,
	TWINKIE,
	WRISTBAND,
	BOARTAIL
} artifactClass;
#define ARTIFACTSMAX 3 

typedef struct {
	artifactClass class;
	char name[25];
	char desc[50];
	int qty;
	int active;
	void * artifact_fun;
} Artifact;

static const char* artifactstrings[] = {
	"Thinking Skull",
	"Soft Twinkie",
	"Gold Wristband",
	"Boar Tail"
};

static Artifact artifactsBag[ARTIFACTSMAX+1] = {

	{THKSKULL,"Thinking Skull","You learn from questioning.",0,0},
	{TWINKIE,"Soft Twinkie","Zombies seem to not like this.",0,0},
	{WRISTBAND,"Gold Wristband","I'd have been cooler on your finger.",0,0},
	{BOARTAIL,"Boar Tail","The beast managed to run.",0,0}
};

typedef struct {
	int length;
	int luck;
	int prize;
} Path;

//Macros for special moves callback

#define REGISTER_CALLBACK(e,p) register_callback(e, (callback_void_t)p)
#define GET_CALLBACK(e,T) ((T)callback_func_ptrs[e])

//Macros for counters callback

//#define REGISTER_COUNTER_CALLBACK(c,x) register_counter_callback(c, (callback_void_t)x)
//#define GET_COUNTER_CALLBACK(c,C) ((C)callback_counter_ptrs[c])

typedef void (*callback_turncounter_fighter_t)(Fighter*);
typedef void (*callback_turncounter_enemy_t)(Enemy*);
typedef void (*callback_special_t)(Fighter*,Enemy*,Path*,int,int);
typedef void (*callback_void_t)(void);

#define FIGHT 100
#define SPECIAL 101
#define QUIT 102
#define ITEM 103
#define ARTIFACTS 104
#define STATS 105
#define DEBUG 1337

#define BEASTROOM 3
#define BSTFACTOR 1.5

#define ENEMYLVLRATIO 3
#define SPECIALLVLRATIO 2

#define KILL_DONE 1
#define DMG_DEALT 2
#define DMG_TAKEN 3
#define DEATH 4
#define NO_DMG 5

#define VERSION "0.4.7"
