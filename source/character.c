#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "character.h"
#include "object.h"

static struct skill_t *G_Skills   = NULL;
static struct race_t *G_Races     = NULL;
static struct object_t *G_Weapons[4];
static char *G_Vitals[] = { "Hp", "Sp", "Mp" };

static void char_init_all()
{
    if(!G_Skills)
    {
        G_Skills = (struct skill_t*)calloc(SK_Count, sizeof(struct skill_t));
        skill_set(&G_Skills[SK_Attack],        "Attack", 100, 1);
        skill_set(&G_Skills[SK_Defense],       "Defense", 400, 2);
        skill_set(&G_Skills[SK_Dodge],         "Dodge", 400, 4);
        skill_set(&G_Skills[SK_Concentration], "Concentration", 200, 1);
        skill_set(&G_Skills[SK_Blade],         "Blade", 200, 1);
        skill_set(&G_Skills[SK_Healing],       "Healing", 400, 2);
    }

    if(!G_Races)
    {
        G_Races = (struct race_t*)calloc(R_Count, sizeof(struct race_t));
        race_set(&G_Races[R_Human], "Human", 10, 10, 10, 10, 10);
        race_set(&G_Races[R_Elf],   "Elf", 8, 12, 8, 11, 11);
        race_set(&G_Races[R_Orc],   "Orc", 15, 10, 15, 5, 5);
    }

    G_Weapons[0] = object_create(2);
    G_Weapons[1] = object_create(3);
    G_Weapons[2] = object_create(4);
    
    if(!G_Weapons[0] || !G_Weapons[1] || !G_Weapons[2])
    {
        printf("unable to load weapons\n");
        exit(1);
    }
}

inline unsigned roll()
{
    return rand() % 20;
}

void char_create(struct character_t* pChar, char *name, unsigned race)
{
    if(G_Skills == NULL || G_Races == NULL)
    {
        char_init_all();
    }

    memset(pChar, 0, sizeof(struct character_t));
    strncpy(pChar->name, name, NAME_MAX);
    // Assign racial base stats
    memcpy(&pChar->stats, &G_Races[race].stats, sizeof(struct stats_t));

    // Initialize skills
    int i;
    for(i=0; i<SK_Count; ++i)
    {
        pChar->skills[i] = 2;
    }

    compute_vitals(&pChar->stats, &pChar->vitals);
    pChar->weapon_id = 0; // bare fists
}

void char_explain(const struct character_t *pChar)
{
    printf("Character\n");
    printf("Name:\"%s\"\n", pChar->name);
    printf("Vitals:\n");
    int i;
    for(i=0; i<V_Count; ++i)
    {
        printf("\t%-3s : %d / %d\n", G_Vitals[i], pChar->vitals.value[i], pChar->vitals.max[i]);
    }
    printf("Stats:\n");
    for(i=0; i<ST_Count; ++i)
    {
        printf("\t%-10s : %d\n", pChar->stats.name[i], pChar->stats.value[i]);
    }
    printf("Skills:\n");
    for(i=0; i<SK_Count; ++i)
    {
        printf("\t%-20s : %d\n", G_Skills[i].name, pChar->skills[i]);
    }
}

void char_add_inventory(struct character_t *pChar, struct object_t *object)
{
    printf("[debug] %p\n", pChar->inventory);
    void *ptr = realloc(pChar->inventory, sizeof(void*)*(pChar->inventory_count+1));
    if(ptr != NULL)
    {
        pChar->inventory = ptr;
        pChar->inventory[pChar->inventory_count] = object;
        ++pChar->inventory_count;
        printf("[debug] inventory item added\n");
    }
    else
    {
        printf("[debug] inventory item failed to be added\n");
    }
}

void char_remove_inventory(struct character_t *pChar, struct object_t *object)
{
    unsigned i;
    for(i=0; i<pChar->inventory_count; ++i)
    {
        if(pChar->inventory[i] == object)
        {
            object_destroy(object);
            --pChar->inventory_count;
            printf("[debug] inventory item removed\n");
            
            if(pChar->inventory_count > 0 && i < pChar->inventory_count)
            {
                // swap the last item with the current item and set the last item pointer to null
                pChar->inventory[i] = pChar->inventory[pChar->inventory_count];
                pChar->inventory[pChar->inventory_count] = NULL;
            }
            else
            {
                pChar->inventory[i] = NULL;
            }
            
            return;
        }
    }

    printf("[debug] inventory item failed to be removed\n");
}

void char_heal(struct character_t *pChar, unsigned qty)
{
    pChar->vitals.value[0] += qty;
    if(pChar->vitals.value[0] > pChar->vitals.max[0])
    {
        pChar->vitals.value[0] = pChar->vitals.max[0];
    }
}

void char_consumable_apply(struct character_t *pChar, struct object_t *pObj)
{
    if(pObj->consumable.type == con_potion)
    {
        // apply a potion
        if(pObj->consumable.charge_qty == 0)
        {
            printf("%s tries to quaff a %s but it was empty.\n", pChar->name, pObj->name);
        }
        else
        {
            switch(pObj->consumable.charge_id)
            {
                case 0: // healing potion
                {
                    printf("%s quaffs a %s and is \x1B[38;5;4mhealed\x1B[0m.\n", pChar->name, pObj->name);
                    char_heal(pChar, roll()+roll());
                    --pObj->consumable.charge_qty;
                    
                    if(pObj->consumable.charge_qty == 0)
                    {
                        char_remove_inventory(pChar, pObj);
                    }
                    break;
                }
            }
        }
    }
}

void compute_vitals(const struct stats_t *pStats, struct vitals_t *pVitals)
{
    pVitals->value[V_Hp] = pVitals->max[V_Hp] = 10 * pStats->value[ST_Con];
    pVitals->value[V_Sp] = pVitals->max[V_Sp] = 2 * pStats->value[ST_Dex] + 5 * pStats->value[ST_Con] + 3 * pStats->value[ST_Str];
    pVitals->value[V_Mp] = pVitals->max[V_Mp] = 8 * pStats->value[ST_Wis] + 2 * pStats->value[ST_Int];
}

void skill_set(struct skill_t *pSkill, const char* name, unsigned cost, unsigned multiplier)
{
    pSkill->name[0] = '\0';
    strcat(pSkill->name, name);
    pSkill->cost = cost;
    pSkill->multiplier = multiplier;
}

void stats_init(struct stats_t *pStats, unsigned str, unsigned dex, unsigned con, unsigned intel, unsigned wis)
{
    strcpy(pStats->name[ST_Str], "Strength");
    strcpy(pStats->name[ST_Dex], "Dexterity");
    strcpy(pStats->name[ST_Con], "Constitution");
    strcpy(pStats->name[ST_Int], "Intelligence");
    strcpy(pStats->name[ST_Wis], "Wisdom");
    stats_set(pStats, str, dex, con, intel, wis);
}

void stats_set(struct stats_t *pStats, unsigned str, unsigned dex, unsigned con, unsigned intel, unsigned wis)
{
    pStats->value[ST_Str] = str;
    pStats->value[ST_Dex] = dex;
    pStats->value[ST_Con] = con;
    pStats->value[ST_Int] = intel;
    pStats->value[ST_Wis] = wis;
}

void race_set(struct race_t* pRace, const char* name,
    unsigned str, unsigned dex, unsigned con, unsigned intel, unsigned wis)
{
    pRace->name[0] = '\0';
    strcat(pRace->name, name);
    stats_init(&pRace->stats, str, dex, con, intel, wis);
}

void combat_round(struct character_t *attacker, struct character_t *defender);

int main(int argc, char** argv)
{
    struct character_t me, orc;

    char name[NAME_MAX];
    sprintf(name, "Metiscus");

    char* username = getenv("USER");
    if(username)
    {
        if(isalpha(username[0]))
        {
            username[0] = toupper(username[0]);
        }
        sprintf(name, "%s", username);
    }

    char_create(&orc, "Munkres", R_Human);
    orc.skills[SK_Concentration] = 10;
    orc.weapon_id = 2; // bo staff
    char_add_inventory(&orc, object_create(1));
    char_add_inventory(&orc, object_create(1));

    char_create(&me, name, R_Human);
    me.weapon_id = 1; // katana
    me.skills[SK_Blade] = 10;

    srand(time(0));

    int j;
    int meWins = 0;
    int trials = 1;
    for(j=0; j<trials; ++j)
    {
        me.vitals.value[0] = me.vitals.max[0];
        orc.vitals.value[0] = orc.vitals.max[0];

        for( ; ; )
        {
            printf("Me: %d\tOrc: %d\n", me.vitals.value[0], orc.vitals.value[0]);
            combat_round(&me, &orc);
            
            // If the ORC health is near critical have him chug a potion
            unsigned criticalHealth = orc.vitals.max[0] / 2 + 1;
            
            unsigned canAttack = 1;
            if((orc.vitals.value[0] <= criticalHealth) && (orc.inventory_count > 0))
            {
                // look for a potion to use
                unsigned item = 0;
                for(item=0; item<orc.inventory_count; ++item)
                {
                    struct object_t *pObj = orc.inventory[item];
                    if(pObj->type == obj_consumable && pObj->consumable.type == con_potion)
                    {
                        // we have a potion
                        // Check for charges and then use it!
                        if(pObj->consumable.charge_qty > 0)
                        {
                            printf("Me: %d\tOrc: %d\n", me.vitals.value[0], orc.vitals.value[0]);
                            char_consumable_apply(&orc, pObj);
                            canAttack = 0;
                            printf("Me: %d\tOrc: %d\n", me.vitals.value[0], orc.vitals.value[0]);
                            break;
                        }
                    }
                }
            }
            
            if(canAttack)
            {
                combat_round(&orc, &me);
            }

            if(me.vitals.value[0]<=0 || orc.vitals.value[0]<=0)
            {
                meWins += (me.vitals.value[0]<=0) ? 0 : 1;
                printf("Battle complete!\n");
                break;
            }
        }
        printf("Me: %d\tOrc: %d\n", me.vitals.value[0], orc.vitals.value[0]);
    }

    char_explain(&me);
    char_explain(&orc);

    printf("Final result: me %d, Orc %d\n", meWins, trials - meWins);
    return 0;
}

char* get_damage_string(unsigned damage)
{
    //Note: ANSI color sequences are \x1B[38;5;<color here>m
    //\x1B[0m cancels the color sequence
    if(damage > 50)
        return "\x1B[38;5;1mannihilates\x1B[0m";
    if(damage > 25)
        return "\x1B[38;5;2mpowerfully hits\x1B[0m";
    if(damage > 10)
        return "\x1B[38;5;3mhits\x1B[0m";
    if(damage > 5)
        return "\x1B[38;5;4mhits lightly\x1B[0m";

    return "\x1B[38;5;5mtickles\x1B[0m";
}

void char_exercise_skill(struct character_t *pChar, unsigned skill, unsigned qty)
{
    pChar->skills_exp[skill] += qty;
    // check for advancement
    if(pChar->skills_exp[skill] > (G_Skills[skill].cost + G_Skills[skill].multiplier * pChar->skills[skill]))
    {
        printf("\x1B[38;5;93m%s has increased his %s level.\x1B[0m\n", pChar->name, G_Skills[skill].name);
        ++pChar->skills[skill];
        pChar->skills_exp[skill] = 0;
    }
}

void char_exercise_stat(struct character_t *pChar, unsigned stat, unsigned qty)
{

}

void combat_round(struct character_t *pAttacker, struct character_t *pDefender)
{
    struct object_t *wpn = G_Weapons[pAttacker->weapon_id];
    unsigned AttackRoll = roll();
    int criticalAttack = AttackRoll == 19;

    AttackRoll += pAttacker->stats.value[wpn->weapon.hit_stat];

    unsigned DefenseRoll = roll();
    unsigned DefenseStat = ST_Dex;
    switch(wpn->weapon.hit_stat)
    {
        case ST_Str:
        case ST_Dex:
            DefenseStat = ST_Dex;
            break;
        case ST_Int:
        case ST_Wis:
            DefenseStat = ST_Wis;
            break;
    }

    char_exercise_skill(pAttacker, SK_Attack, 1);
    char_exercise_skill(pAttacker, wpn->weapon.skill, 1);
    char_exercise_skill(pDefender, SK_Defense, 1);

    DefenseRoll += pDefender->stats.value[DefenseStat];

    if(criticalAttack || AttackRoll > DefenseRoll)
    {
        unsigned Damage = wpn->weapon.damage_base;
        Damage += pAttacker->stats.value[wpn->weapon.damage_stat] / pDefender->skills[SK_Defense] + 1;
        if(criticalAttack)
        {
            if(roll() > DefenseRoll)
            {
                Damage *= 10;
            }
            else
            {
                Damage *= 2;
            }
        }

        printf("%s %s %s with his %s!\n", pAttacker->name, get_damage_string(Damage), pDefender->name, G_Weapons[pAttacker->weapon_id]->name);
        pDefender->vitals.value[V_Hp] -= Damage;

        char_exercise_stat(pAttacker, wpn->weapon.damage_stat, Damage);
        char_exercise_stat(pDefender, DefenseStat, Damage);
    }
    else
    {
        printf("%s misses %s with his %s!\n", pAttacker->name, pDefender->name, G_Weapons[pAttacker->weapon_id]->name);
    }
}
