#include "game_rng.h"

int64_t G_RNG_ADVANCEMENTS = 0;

/**
 * Wraps over rand() to update G_RNG_ADVANCEMENTS if passed flag is true.
 * @see G_RNG_ADVANCEMENTS
 * @see rand()
 * @param count When true, advance G_RNG_ADVANCEMENTS.
 * @return A random integer from rand().
 */
int hlpd_rand_docount(bool count)
{
    //log_tag("debug_log.txt", "[RAND]", "%s():    Rolling. G_RNG_ADVANCEMENTS: {%i}", __func__, G_RNG_ADVANCEMENTS);
    if (count) G_RNG_ADVANCEMENTS += 1;
    return rand();
}

/**
 * Wraps over hlpd_rand_docount() to update G_RNG_ADVANCEMENTS.
 * @see G_RNG_ADVANCEMENTS
 * @see hlpd_rand_docount()
 * @return A random integer from hlpd_rand_docount().
 */
int hlpd_rand(void)
{
    return hlpd_rand_docount(true);
}

/**
 * djb2 by Dan Bernstein.
 * See:
 * http://www.cse.yorku.ca/~oz/hash.html
 * https://stackoverflow.com/questions/7666509/hash-function-for-string
 * @param str The string to hash.
 * @return The resulting hash.
 */
unsigned long hlpd_hash(unsigned char *str)
{
    log_tag("debug_log.txt", "[DEBUG]", "%s():    Hashing {%s}.", __func__, str);
    unsigned long hash = 5381;
    int c;

    do {
        c = *str++;
        if (c) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    } while (c);

    return hash;
}

/**
 * Sets the passed buffer up to be a random seed. Only chars >= 0, <= Z; not including the symbols between digits and letters.
 * @param buffer The buffer to set.
 */
void gen_random_seed(char buffer[PATH_SEED_BUFSIZE+1])
{
    log_tag("debug_log.txt", "[DEBUG]", "%s():    Creating a random seed.", __func__);
    int len = (hlpd_rand_docount(false) % (PATH_SEED_BUFSIZE-8)) +8; // Min len should be 8
    for (size_t i=0; i < len; i++) {
        int r_ch = -1;
        do {
            r_ch = (hlpd_rand_docount(false) % ('Z' - '0' +1)) + '0'; // We want a char from 0 to Z included.
        } while (r_ch >= ':' && r_ch <= '@'); // We reject chars between the digits and upperscore letters
        buffer[i] = r_ch;
    }
    buffer[PATH_SEED_BUFSIZE] = '\0';
}

/**
 * Checks the passed buffer as a seed. Only chars >= 0, <= Z; not including the symbols between digits and letters.
 * Notably, the passed buffer is checked by applying toupper() to each char.
 * @param buffer The buffer to check.
 * @return True for a valid seed.
 */
bool check_seed(char buffer[PATH_SEED_BUFSIZE])
{
    if (buffer == NULL) {
        log_tag("debug_log.txt", "[DEBUG]", "%s():    Passed buffer was NULL.\n");
        return false;
    }

    int buf_len = strlen(buffer);
    char ch = -1;
    for (size_t i=0; i < buf_len; i++) {
        ch = toupper(buffer[i]);
        if (ch < '0' || ch > 'Z' || (ch >= ':' && ch <= '@')) {
            log_tag("debug_log.txt", "[DEBUG]", "%s():    Found invalid char. {%c}", __func__, ch);
            return false;
        }
    }
    return true;
}
