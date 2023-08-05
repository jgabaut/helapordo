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
} BaseStats;

static BaseStats basestats[CLASSESMAX+1] = {

	{Knight,30,10,20,6,1,0,1,0,30},
	{Archer,24,13,13,9,1,0,1,0,24},
	{Mage,17,19,10,7,1,0,1,0,17},
	{Assassin,20,16,12,8,1,0,1,0,20}

};

typedef enum {
	Mummy=0,
	Ghost=1,
	Zombie=2,
	Goblin=3,
	Imp=4,
	Troll=5,
	Boar=6
} enemyClass;

#define ENEMYCLASSESMAX 6

static const char* classenemystrings[] = {
	"Mummy",
	"Ghost",
	"Zombie",
	"Goblin",
	"Imp",
	"Troll",
	"Boar"
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
} EnemyBaseStats;

static EnemyBaseStats baseenemystats[ENEMYCLASSESMAX+1] = {

	{Mummy,22,11,7,5,1,0,22,0},
	{Ghost,35,5,16,6,1,1,35,0},
	{Zombie,20,6,7,5,1,0,20,0},
	{Goblin,32,10,8,11,1,2,32,0},
	{Imp,24,12,7,10,1,3,24,0},
	{Troll,45,4,18,4,1,4,45,0},
	{Boar,30,9,12,8,1,4,30,0}
};

typedef struct {
	int enemieskilled;
	int itemsfound;
	int criticalhits;
	int roomscompleted;
} countStats;

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
	fighterStatus status;
	int beast;
} Enemy;

typedef struct {
	int length;
	int luck;
	int prize;
} Path;

#define REGISTER_CALLBACK(e,p) register_callback(e, (callback_void_t)p)
#define GET_CALLBACK(e,T) ((T)callback_func_ptrs[e])

typedef void (*callback_special_t)(Fighter*,Enemy*,Path*,int,int);
typedef void (*callback_void_t)(void);

#define FIGHT 100
#define QUIT 101
#define ITEM 102
#define STATS 103
#define DEBUG 1337

#define BEASTROOM 3
#define BSTFACTOR 1.5

#define ENEMYLVLRATIO 3

#define KILL_DONE 1
#define DMG_DEALT 2
#define DMG_TAKEN 3
#define DEATH 4
#define NO_DMG 5

#define VERSION "0.3.1"
