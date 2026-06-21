#include "Data/Character/MWCharacterTable.h"

FMWCharacterTable::FMWCharacterTable()
{
}

bool FMWCharacterTable::IsValidRow() const
{
	return Id != INDEX_NONE
		&& Name != NAME_None
		&& Asset.ToSoftObjectPath().IsValid();
}