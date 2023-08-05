#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>
#include "../../src/helapordo.h"

int main(void) {
  if (GAMEMODE_MAX == 1) {
    printf("[SUCCESS]    GAMEMODE_MAX was 1!\n");
    return 0;
  }
    printf("[FAIL]    GAMEMODE_MAX was not 1!\n");
  return 1;
}
