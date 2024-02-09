#include "consumables.h"

/**
 * Takes a Fighter pointer and prepares its consumablesBag field by allocating a Consumable for each consumableClass.
 * @see Fighter
 * @see Consumable
 * @see consumableClass
 * @see CONSUMABLESMAX
 * @param f The Fighter pointer whose consumablesBag field will be initialised.
 * @param kls The Koliseo to do allocations.
 */
void initConsumableBag(Fighter *f, Koliseo *kls)
{

    for (int i = 0; i < CONSUMABLESMAX + 1; i++) {
        kls_log(kls, "DEBUG", "Prepping Consumable (%i)", i);
        Consumable *c =
            (Consumable *) KLS_PUSH_TYPED(kls, Consumable, HR_Consumable,
                                          "Consumable", "Consumable");
        c->class = i;

        Consumable *base = &consumablesBase[i];

        strcpy(c->name, base->name);
        strcpy(c->desc, base->desc);
        for (int j = 0; j < 8; j++) {
            strcpy(c->sprite[j], base->sprite[j]);
        }
        c->qty = 0;

        f->consumablesBag[i] = (struct Consumable *)c;
    }

}
