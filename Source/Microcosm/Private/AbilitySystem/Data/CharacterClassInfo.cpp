#include "CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(const ECharacterClass CharacterClass)
{
	return CharacterClassInfo.FindChecked(CharacterClass);
}