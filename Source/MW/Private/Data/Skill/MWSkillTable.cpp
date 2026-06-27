#include "Data/Skill/MWSkillTable.h"

FMWSkillTable::FMWSkillTable()
{
}

bool FMWSkillTable::IsValidRow() const
{
    return Id != INDEX_NONE
        && Name != NAME_None
        && Asset.ToSoftObjectPath().IsValid();
}