typedef enum { 
	Normal,
	Poison,
	Frozen,
	Burned,
	Weak,
	Strong
} fighterStatus;

typedef enum { 
	Knight=0,
	Archer=1,
	Mage=2
} fighterClass;
#define CLASSESMAX 2 

static const char* classesstrings[] = {
	"Knight",
	"Archer",
	"Mage"
};

typedef struct {
	fighterClass class;
	int hp;
	int atk;
	int def;
	int vel;
	int level;
	int totalxp;
        int currentlevelxp;
	int totallevelxp;
} BaseStats;

static BaseStats basestats[CLASSESMAX+1] = {

	{Knight,30,10,12,6,1,100,1,0},
	{Archer,20,11,10,9,1,100,1,0},
	{Mage,12,13,7,8,1,100,1,0}

};

typedef enum {
	Mummy=0,
	Ghost=1,
	Zombie=2,
	Goblin=3
} enemyClass;

#define ENEMYCLASSESMAX 3

static const char* classenemystrings[] = {
	"Mummy",
	"Ghost",
	"Zombie",
	"Goblin"
};

typedef struct {
	enemyClass class;
	int hp;
	int atk;
	int def;
	int vel;
	int level;
	int xp;
} EnemyBaseStats;
static EnemyBaseStats baseenemystats[ENEMYCLASSESMAX+1] = {

	{Mummy,20,12,5,5,1,1},
	{Ghost,40,10,28,6,1,2},
	{Zombie,18,10,6,5,1,1},
	{Goblin,32,19,8,12,1,3}
};

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
	fighterStatus status;
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
	fighterStatus status;
} Enemy;

typedef struct {
	int length;
	int luck;
	int prize;
} Path;

#define FIGHT 100
#define QUIT 101

#define KILL_DONE 1
#define DMG_DEALT 2
#define DMG_TAKEN 3
#define DEATH 4
#define NO_DMG 5
