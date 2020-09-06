// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Visual/Item/DictionaryItem.hpp"

using namespace Cataclysm;

// Construct

DictionaryItem::DictionaryItem(const DictionaryItem& _object) noexcept
{
	this->copy(_object);
}

DictionaryItem::DictionaryItem(std::string_view _name, int _weight, int _volume) noexcept
		: name(_name), weight(std::to_string(_weight)), volume(std::to_string(_volume))
{
	// String, Integer, Integer
}

DictionaryItem::DictionaryItem(std::string_view _name, std::string_view _weight, std::string_view _volume) noexcept
	: name(_name), weight(_weight), volume(_volume)
{
	// String, String, String
}

// Methods

void DictionaryItem::copy(const DictionaryItem& _object) noexcept
{
	name = _object.name;
	weight = _object.weight;
	volume = _object.volume;
}

// Operator

DictionaryItem& DictionaryItem::operator=(const DictionaryItem& _object)
{
	// The canonical copy-assignment operator is expected to perform no
	// action on self-assignment, and to return the lhs by reference

	// Invariant: verify that not perform action of self-assignment
	if (this not_eq &_object)
	{
		this->copy(_object);
	}

	// Invariant: Return the left value (this object) by reference
	return *this;

	// References: https://github.com/isocpp/CppCoreGuidelines/blob
	//  /master/CppCoreGuidelines.md#c62-make-copy-assignment-safe-
	//  for-self-assignment

	// References: https://en.cppreference.com/w/cpp/language/operators
	//  Section: Assignment operator
}

// Getters

const std::string& DictionaryItem::getName() const noexcept
{
	return name;
}

const std::string& DictionaryItem::getWeight() const noexcept
{
	return weight;
}

const std::string& DictionaryItem::getVolume() const noexcept
{
	return volume;
}
