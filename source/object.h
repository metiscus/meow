#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

enum consumable_type
{
    con_potion,
    con_torch,
};

struct consumable_t
{
    enum consumable_type type;
    unsigned charge_qty;
    unsigned charge_id;
};

struct weapon_t 
{
    unsigned hit_stat;
    unsigned damage_stat;
    unsigned skill;
    unsigned damage_base;
};

enum armor_type
{
    arm_none   = 0x0,
    arm_chest  = 0x1,
    arm_head   = 0x2,
    arm_legs   = 0x4,
    arm_l_arm  = 0x8,
    arm_r_arm  = 0x16
};

struct armor_t 
{
    enum armor_type type;
};

enum object_type
{
    obj_consumable,
    obj_weapon,
    obj_armor,
};

struct object_t
{
    unsigned id;
    enum object_type type;
    unsigned mass;
    char *name;
    
    union {
        struct consumable_t consumable;
        struct weapon_t weapon;
        struct armor_t armor;
    };
};


struct object_t* object_create(unsigned id);
void object_destroy(struct object_t *obj);
#endif // OBJECT_H_INCLUDED
