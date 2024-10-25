#include "MyRPGLib/item.h"

static std::uint32_t EQUIPMENTUNIQUEIDITERATOR = 0u;

EquipmentDelegate::EquipmentDelegate(std::string name, CoreStats cstats)
	: ItemDelegate(name), Stats(cstats), UniqueId(++EQUIPMENTUNIQUEIDITERATOR) {
}

const ItemDelegate* Item::GetData() { return _data; }

bool Item::GetMarkedForDeletion() const { return _marked_for_deletion; }

bool Item::GetMarkedAsEquipped() const { return _marked_as_just_equipped; }