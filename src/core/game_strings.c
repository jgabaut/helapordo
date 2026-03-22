#include "game_strings.h"
/**
 * Takes a saveType and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see saveType
 * @param s The saveType value.
 * @return String corresponding to the save type.
 */
char *stringFrom_saveType(saveType s)
{

    return savetypestrings[s];
}

/**
 * Takes a OP_res and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see OP_res
 * @param r The OP_res.
 * @return String corresponding to the OP result.
 */
char *stringFrom_OP_res(OP_res r)
{

    return opresstrings[r];
}

/**
 * Takes a turnOption_OP and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see turnOption_OP
 * @param t The turn choice.
 * @return String corresponding to the turn choice.
 */
char *stringFromTurnOP(turnOption_OP t)
{

    return turnopstrings[t];
}

/**
 * Takes a foeTurnOption_OP and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see foeTurnOption_OP
 * @param ft The turn choice.
 * @return String corresponding to the turn choice.
 */
char *stringFromFoeTurnOP(foeTurnOption_OP ft)
{
    return foeturnopstrings[ft];
}

/**
 * Takes a fightResult fr and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fightResult
 * @param fr The fight result value.
 * @return String corresponding to the fight result value.
 */
char *stringFrom_fightResult(fightResult fr)
{

    return fightresultstrings[fr];
}

/**
 * Takes a fighterStatus and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterStatus
 * @param s The status.
 * @return String corresponding to the status.
 */
char *stringFromStatus(fighterStatus s)
{
    static char *strings[] = {
        "Normal",
        "Poison",
        "Frozen",
        "Burned",
        "Weak",
        "Strong"
    };

    return strings[s];
}

/**
 * Takes a winconClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see winconClass
 * @param w The class.
 * @return String corresponding to the class.
 */
char *stringFromWinconClass(winconClass w)
{

    return winconstrings[w];
}

/**
 * Takes a fighterClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @param f The class.
 * @return String corresponding to the class.
 */
char *stringFromClass(fighterClass f)
{

    return classesstrings[f];
}

/**
 * Takes a enemyClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see enemyClass
 * @param e The class.
 * @return String corresponding to the class.
 */
char *stringFromEClass(enemyClass e)
{

    return classenemystrings[e];
}

/**
 * Takes a bossClass and returns the corresponding string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see bossClass
 * @param b The class.
 * @return String corresponding to the class.
 */
char *stringFromBossClass(bossClass b)
{

    return classbossstrings[b];
}

/**
 * Takes a integer and returns the corresponding consumable string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see consumableClass
 * @param c The integer/consumableClass.
 * @return String corresponding to the consumable.
 */
char *stringFromConsumables(int c)
{

    return consumablestrings[c];
}

/**
 * Takes a integer and returns the corresponding equip string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see equipClass
 * @param e The integer/equipClass.
 * @return String corresponding to the equip.
 */
char *stringFromEquips(int e)
{

    return equipstrings[e];
}

/**
 * Takes a integer and returns the corresponding equipzone string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see Equipzone
 * @param z The integer/Equipzone.
 * @return String corresponding to the equipzone.
 */
char *stringFromEquipzones(int z)
{

    return equipzonestrings[z];
}

/**
 * Takes a integer and returns the corresponding quality string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see quality
 * @param q The integer/quality
 * @return String corresponding to the quality.
 */
char *stringFromQuality(int q)
{

    return qualitytrings[q];
}

/**
 * Takes a integer and returns the corresponding artifact string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see artifactClass
 * @param a The integer/artifactClass.
 * @return String corresponding to the artifact.
 */
char *stringFromArtifacts(int a)
{

    return artifactstrings[a];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return String corresponding to the special move.
 */
char *nameStringFromSpecial(fighterClass class, specialMove move)
{
    return specialsnamestrings[class][(move % (SPECIALSMAX + 1))];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The fighterClass.
 * @param move The specialMove.
 * @return String corresponding to the special move.
 */
char *descStringFromSpecial(fighterClass class, specialMove move)
{
    return specialsdescstrings[class][(move % (SPECIALSMAX + 1))];
}

/**
 * Takes a skillType and returns the corresponding name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see specialMove
 * @param class The skillType.
 * @return String corresponding to the skillType.
 */
char *nameStringFromSkill(skillType class)
{
    return skillsnamestrings[class];
}

/**
 * Takes a fighterClass and a specialMove and returns the corresponding desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see fighterClass
 * @see skillType
 * @param class The skillType.
 * @return String corresponding to the skillType.
 */
char *descStringFromSkill(skillType class)
{
    return skillsdescstrings[class];
}

/**
 * Takes a integer and returns the corresponding perk name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param p The integer/perkClass.
 * @return String corresponding to the perk name.
 */
char *nameStringFromPerk(int p)
{

    return perksnamestrings[p];
}

/**
 * Takes a integer and returns the corresponding perk desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see perkClass
 * @see Perk
 * @param p The integer/perkClass.
 * @return String corresponding to the perk desc.
 */
char *descStringFromPerk(int p)
{

    return perksdescstrings[p];
}

/**
 * Takes a integer and returns the corresponding treasure desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see treasureClass
 * @param t The integer/treasureClass.
 * @return String corresponding to the treasure desc.
 */
char *descStringFromTreasure(int t)
{

    return treasuredescstrings[t];
}

/**
 * Takes a integer and returns the corresponding chest desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see chestClass
 * @param c The integer/chestClass.
 * @return String corresponding to the chest desc.
 */
char *descStringFromChest(int c)
{
    return chestdescstrings[c];
}

/**
 * Takes a integer and returns the corresponding FoeParty desc string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see foePartyClass
 * @param fp The integer/foePartyClass.
 * @return String corresponding to the FoeParty desc.
 */
char *stringFromFoePartyClass(foePartyClass fp)
{
    return foepartystrings[fp];
}

/**
 * Takes a integer and returns the corresponding HLP_Region_Type string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see HLP_Region_Type
 * @param t The HLP type.
 * @return String corresponding to the HLP_Region_Type name.
 */
char *stringFrom_HLP_Region_Type(HLP_Region_Type t)
{
    return hlp_regiontype_strings[t];
}

/**
 * Takes a integer and returns the corresponding room name string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see roomClass
 * @param r The integer/roomClass.
 * @return String corresponding to the room name.
 */
char *stringFromRoom(roomClass r)
{
    return roomnamestrings[r];
}

/**
 * Takes a integer and returns the corresponding gamemode string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see Gamemode
 * @param g The integer/Gamemode.
 * @return String corresponding to the gamemode.
 */
char *stringFromGamemode(Gamemode g)
{
    return gamemodenamestrings[g];
}

/**
 * Takes a integer and returns the corresponding floorClass string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see floorClass
 * @param fc The integer/floorClass.
 * @return String corresponding to the floorClass.
 */
char *stringFromFloorclass(floorClass fc)
{
    return floornamestrings[fc];
}

/**
 * Takes a integer and returns the corresponding HLPD_KeyClass string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see HLPD_KeyClass
 * @param k The integer/HLPD_KeyClass.
 * @return String corresponding to the HLPD_KeyClass.
 */
char *stringFrom_HLPD_KeyClass(HLPD_KeyClass k)
{
    return hlpd_keyclass_strings[k];
}

/**
 * Takes a integer and returns the corresponding HLPD_DirectionalKeys_Schema string by the inner array position.
 * Correct result is only possible by having the enum values in a consistent order with the string array.
 * @see HLPD_DirectionalKeys_Schema
 * @param dks The integer/HLPD_DirectionalKeys_Schema.
 * @return String corresponding to the HLPD_DirectionalKeys_Schema.
 */
const char *stringFrom_HLPD_DirectionalKeys_Schema(int dks)
{
    return hlpd_directionalkeyschemas_strings[dks];
}
