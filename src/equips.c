#include "equips.h"

/**
 * Takes a Equip and a Fighter pointers.
 * Iterates over the equip's perks and adds them to the fighter perks.
 * @see Perk
 * @see Equip
 * @see Fighter
 * @param e An Equip pointer.
 * @param f A Fighter pointer.
 */
void applyEquipPerks(Equip *e, Fighter *f)
{

    for (int i = 0; i < (e->perksCount); i++) {
        Perk *p = e->perks[i];

        Perk *fighterPerk = f->perks[p->class];
        fighterPerk->innerValue += 1;
    }
}

/**
 * Takes a Equip and a Fighter pointers.
 * Iterates over the equip's perks and removes them to the fighter perks.
 * @see Perk
 * @see Equip
 * @see Fighter
 * @param e An Equip pointer.
 * @param f A Fighter pointer.
 */
void removeEquipPerks(Equip *e, Fighter *f)
{

    for (int i = 0; i < (e->perksCount); i++) {
        Perk *p = e->perks[i];

        Perk *fighterPerk = f->perks[p->class];
        fighterPerk->innerValue -= 1;
    }
}
