#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>
#include "../../src/helapordo.h"

int main(void) {
  if ( !(GAMEMODE_MAX == 2) ) {
    printf("\n[SUCCESS]    Assertion success: GAMEMODE_MAX is not 2 !\n");
    return 0;
  }
  printf("\n[FAIL]    Assertion was supposed to fail !\n");
  return 1;
}
