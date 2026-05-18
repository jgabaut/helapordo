#include <stdio.h>
#define HELAPORDO_RAYLIB_BUILD
typedef struct Rectangle {
    int x;
    int y;
    int width;
    int height;
} Rectangle;

#include "../../src/core/game_core.h"
#include "../../src/core/game_init.h"

void enqueue_notification(char *text, int time, int color, RingaBuf* rb_notifications)
{
    return;
    //if (text) fprintf(stderr, "%s():    {%s}\n", __func__, text);
}

static void print_csv_header(void)
{
    printf("player_class,enemy_class,player_level,enemy_level,player_hp_start,enemy_hp_start,player_attack,enemy_attack,player_def,enemy_def,player_vel,enemy_vel,wins,total_fights\n");
}

static void print_csv_row(Fighter* player, Enemy* enemy, int wins, int total)
{
    EnemyBaseStats *enemy_base = &baseenemystats[enemy->class];
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        player->class,
        enemy->class,
        player->level,
        enemy->level,
        player->totalhp,
        enemy->totalhp,
        player->atk,
        enemy_base->atk,
        player->def,
        enemy_base->def,
        player->vel,
        enemy_base->vel,
        wins,
        total
    );
}

int main(int argc, char** argv)
{
    int seed = 0x0DEADBEEF;
    srand(seed);
    Koliseo* kls = kls_new(KLS_DEFAULT_SIZE*50);
    Koliseo* second_kls = kls_new(KLS_DEFAULT_SIZE*100);
    if (!kls) return 1;
    Fighter player = {0};
    Path game_path = {0};
    Koliseo_Temp* t_kls = NULL;
    int roomIndex = 0;
    int roomEnemyTotal = 1;
    int enemyIndex = 0;
    Enemy enemy = {0};
    RingaBuf* rb_notifications = NULL;

    G_DEBUG_ON = 1;
    G_DEBUG_ENEMYTYPE_ON = 1;
    GAMEMODE = -1;

    int fights_per_enclass = 1000;
    int player_lvl_cap = 5;

    print_csv_header();

    for (int player_lvl = 1; player_lvl < (player_lvl_cap+1); player_lvl++) {
        for (fighterClass pl_class = 0; pl_class < CLASSESMAX+1; pl_class++) {
            player = (Fighter){0};
            player.class = pl_class;
            initPlayerStats(&player, &game_path, kls);
            player.level = player_lvl;
            statReset(&player, 1);

            int total_wins = 0;
            for (enemyClass en_class = 0; en_class < ENEMYCLASSESMAX+1; en_class++) {
                t_kls = kls_temp_start(second_kls);
                G_DEBUG_ENEMYTYPE = en_class;
                //printf("%s():    preparing %s enemy\n", __func__, stringFromEClass(en_class));
                int wins = 0;
                for (int fight_count = 0; fight_count < fights_per_enclass; fight_count++) {
                    enemy = (Enemy){0};
                    prepareRoomEnemy(&enemy, roomIndex, roomEnemyTotal, enemyIndex, t_kls);
                    fightResult res = -1;
                    statReset(&player, 1);
                    player.hp = player.totalhp;
                    do {
                        res = defer_fight_enemy(&player, &enemy, enemyTurnPick(&enemy, &player), kls, rb_notifications);
                    } while (res != FIGHTRES_KILL_DONE && res != FIGHTRES_DEATH);
                    //printf("%s (lv %i) vs %s (lv %i) Res: %s\n", stringFromClass(player.class), player.level, stringFromEClass(enemy.class), enemy.level, stringFrom_fightResult(res));
                    if (res == FIGHTRES_KILL_DONE) wins++;
                }
                //printf("%s (lv %i) vs %s (lv %i) Wins: %i\n", stringFromClass(player.class), player.level, stringFromEClass(enemy.class), enemy.level, wins);
                kls_temp_end(t_kls);
                total_wins += wins;
                statReset(&player, 1);
                print_csv_row(&player, &enemy, wins, fights_per_enclass);
            }
            //printf("%s (lv %i) Total Wins: %i\n", stringFromClass(player.class), player.level, total_wins);
        }
    }
    kls_free(kls);
    kls_free(second_kls);
    return 0;
}
