#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "object.h"

#define ST_Str 0
#define ST_Dex 1
#define ST_Con 2
#define ST_Int 3
#define ST_Wis 4
#define ST_Count 5

#define V_Hp 0
#define V_Sp 1
#define V_Mp 2
#define V_Count 3

#define R_Human 0
#define R_Elf 1
#define R_Orc 2
#define R_Count 3

#define SK_Attack  0
#define SK_Defense 1
#define SK_Dodge 2
#define SK_Concentration 3
#define SK_Blade 4
#define SK_Healing 5
#define SK_Count 6

#ifdef NAME_MAX
#undef NAME_MAX
#endif
#define NAME_MAX 64

struct way_t
{
    unsigned skill1;
    unsigned skill2;
    unsigned skill3;
};

struct skill_t
{
    char name [NAME_MAX];
    unsigned cost;
    unsigned multiplier;
};

struct stats_t
{
    char name[ST_Count][NAME_MAX];
    unsigned value[ST_Count];
};

struct race_t
{
    char name [NAME_MAX];
    struct stats_t stats;
};

struct vitals_t
{
    int value[V_Count];
    int max[V_Count];
};

struct character_t
{
    char name [NAME_MAX];
    unsigned race;
    struct stats_t stats;
    struct vitals_t vitals;
    unsigned experience;
    unsigned skills[SK_Count];
    unsigned skills_exp[SK_Count];
    unsigned weapon_id;
    unsigned inventory_count;
    struct object_t **inventory;
};

extern void char_create(struct character_t* pChar, char *name, unsigned race);
extern void char_explain(const struct character_t *pChar);
extern void char_add_inventory(struct character_t *pChar, struct object_t *object);

extern void stats_init(struct stats_t *pStats, unsigned str, unsigned dex, unsigned con, unsigned intel, unsigned wis);
extern void stats_set(struct stats_t *pStats, unsigned str, unsigned dex, unsigned con, unsigned intel, unsigned wis);
extern void skill_set(struct skill_t *pSkill, const char* name, unsigned cost, unsigned multiplier);
extern void race_set(struct race_t* pRace, const char* name, unsigned str, unsigned dex, unsigned con, unsigned intel, unsigned wis);
extern void compute_vitals(const struct stats_t *pStats, struct vitals_t *pVitals);
#endif // CHARACTER_H_INCLUDED
