#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

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

enum weapon_type
{
    wpn_blunt,
    wpn_blade,
    wpn_spear,
};

struct weapon_t 
{
    enum weapon_type type;
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
#endif // ITEM_H_INCLUDED
