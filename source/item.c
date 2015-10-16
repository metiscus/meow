#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DATA_PATH "data/"

#define LINE_MAX 100

static bool consumable_create(FILE *fp, struct object_t*ptr);
static bool weapon_create(FILE *fp, struct object_t*ptr);
static bool armor_create(FILE *fp, struct object_t*ptr);

static bool FileGetLine(FILE *fp, char*buffer, unsigned length)
{
    char *ret = NULL;
    
    while(ret==NULL && !feof(fp))
    {
        ret = fgets(buffer, length, fp);
        unsigned len = strlen(buffer);
        if(len && buffer[len-1]=='\n')
        {
            buffer[len-1] = '\0';
        }
    
        if(len==0 || buffer[0] == '#')
        {
            ret = NULL;
        }
    }
    return ret == buffer;
}

struct object_t* object_create(unsigned id)
{
    struct object_t *ptr = NULL;

    // calculate filename
    char filename[LINE_MAX];
    sprintf(filename, "%s%u.txt", DATA_PATH, id);

    // open file
    FILE *fp = fopen(filename, "rt");
    if(fp)
    {
        char line[LINE_MAX];
        FileGetLine(fp, line, LINE_MAX);
        if(strcmp("object", line) != 0)
        {
            printf("(%s) : Invalid object file!\n", filename);
            return NULL;
        }

        ptr = (struct object_t*)calloc(1, sizeof(struct object_t));
        // id
        ptr->id = id;

        // object_type
        FileGetLine(fp, line, LINE_MAX);
        unsigned type = 0;
        sscanf(line, "%u", &type);
        ptr->type = (enum object_type)type;
        
        // mass
        FileGetLine(fp, line, LINE_MAX);
        sscanf(line, "%u", &ptr->mass);
        
        // name
        ptr->name = (char*)calloc(1, LINE_MAX);
        FileGetLine(fp, line, LINE_MAX);

        // call type constructor based on type
        switch(ptr->type)
        {
            case obj_consumable:
                if(!consumable_create(fp, ptr))
                {
                    goto cleanup;
                }
                break;

            case obj_weapon:
                if(!weapon_create(fp, ptr))
                {
                    goto cleanup;
                }
                break;

            case obj_armor:
                if(!armor_create(fp, ptr))
                {
                    goto cleanup;
                }
                break;

            default:
                printf("Invalid object type: %u\n", ptr->type);
                goto cleanup;
                break;
        }
    }
    return ptr;

cleanup:
    free(ptr->name);
    free(ptr);
    return NULL;
}

void object_destroy(struct object_t *obj);


static bool consumable_create(FILE *fp, struct object_t*ptr)
{
    char line[LINE_MAX];

    if(!fp || feof(fp) || !ptr)
    {
        return false;
    }

    // consumable type
    FileGetLine(fp, line, LINE_MAX);
    unsigned type = 0;
    sscanf(line, "%u", &type);
    ptr->consumable.type = (enum consumable_type)type;
    
    // consumable qty
    FileGetLine(fp, line, LINE_MAX);
    sscanf(line, "%u", &ptr->consumable.charge_qty);
    
    // consumable id
    FileGetLine(fp, line, LINE_MAX);
    sscanf(line, "%u", &ptr->consumable.charge_id);

    return true;
}

static bool weapon_create(FILE *fp, struct object_t*ptr)
{
    return false;
}

static bool armor_create(FILE *fp, struct object_t*ptr)
{
    return false;
}
