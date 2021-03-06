// Joan Andrés (@Andres6936) Github.

#include <utility>
#include "Cataclysm/Visual/Item/DictionaryItem.hpp"

using namespace Cataclysm;

// Construct

DictionaryItem::DictionaryItem(DictionaryItem&& _object) noexcept
	: key(_object.key), name(std::move(_object.name)), weight(std::move(_object.weight)), volume(std::move(_object.volume))
{
	// Move, Move, Move
}

DictionaryItem::DictionaryItem(const DictionaryItem& _object) noexcept
{
	this->copy(_object);
}

DictionaryItem::DictionaryItem(std::uint16_t _key, std::string_view _name, int _weight, int _volume) noexcept
	: key(_key) ,name(_name), weight(std::to_string(_weight)), volume(std::to_string(_volume))
{
	// String, Integer, Integer
}

DictionaryItem::DictionaryItem(std::uint16_t _key, std::string_view _name, std::string_view _weight, std::string_view _volume) noexcept
	: key(_key), name(_name), weight(_weight), volume(_volume)
{
	// String, String, String
}

// Methods

void DictionaryItem::copy(const DictionaryItem& _object) noexcept
{
	key = _object.key;
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

	// Invariant: Return the left value (this object) by reference non-const
	return *this;

	// References: https://github.com/isocpp/CppCoreGuidelines/blob
	//  /master/CppCoreGuidelines.md#c62-make-copy-assignment-safe-
	//  for-self-assignment

	// References: https://en.cppreference.com/w/cpp/language/operators
	//  Section: Assignment operator
}

DictionaryItem& DictionaryItem::operator=(DictionaryItem&& _object) noexcept
{
	// The canonical move assignment is expected to leave the moved-from
	// object in valid state (that is, a state with class invariants intact),
	// and either do nothing or at least leave the object in a valid state
	// on self-assignment, and return the lhs by reference to non-const,
	// and be noexcept

	// Invariant: The method is noexcept

	// Invariant: verify that not perform action of self-assignment
	if (this not_eq &_object)
	{
		// Invariant: Change of values and another object leave in valid state
		key = std::exchange(_object.key, 0);
		std::swap(name, _object.name);
		std::swap(weight, _object.weight);
		std::swap(volume, _object.volume);

		// In those situations where copy assignment cannot benefit from
		// resource reuse (it does not manage a heap-allocated array and does
		// not have a (possibly transitive) member that does, such as a member
		// std::vector or std::string), there is a popular convenient shorthand:
		// the copy-and-swap assignment operator, which takes its parameter by
		// value (thus working as both copy- and move-assignment depending on
		// the value category of the argument), swaps with the parameter, and
		// lets the destructor clean it up.
	}

	// Invariant: Return the left value (this object) by reference non-const
	return *this;

	// References: https://github.com/isocpp/CppCoreGuidelines/blob
	//  /master/CppCoreGuidelines.md#c62-make-copy-assignment-safe-
	//  for-self-assignment

	// References: https://en.cppreference.com/w/cpp/language/operators
	//  Section: Assignment operator
}

// Getters

const std::uint16_t DictionaryItem::getKey() const noexcept
{
	return key;
}

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

const std::string DictionaryItem::getNameWithLetter() const noexcept
{
	// For avoid write static_cast
	// The use of std::uint16_t more char throw warnings for
	// the compiler, so that first concat the name with " - "
	// then concat with the key (letter in this case)
	return (char) key + (" - " + name);
}

// Setters

void DictionaryItem::setName(const std::string& _name) noexcept
{
	name = _name;
}

void DictionaryItem::setWeight(const std::string& _weight) noexcept
{
	weight = _weight;
}

void DictionaryItem::setVolume(const std::string& _volume) noexcept
{
	volume = _volume;
}

void DictionaryItem::setKey(const std::uint16_t _key) noexcept
{
	key = _key;
}


// Definition class DictionaryItemCompare

bool DictionaryItemCompare::operator()(const DictionaryItem& lhs, const DictionaryItem& rhs) const noexcept
{
	// In C++, the "compare" predicate must be a strict weak ordering.

	// Invariant, if the key are equals, then compare for name, weight and volume
	//  this invariant allow use this class with std::multimap
	if (lhs.getKey() == rhs.getKey())
	{
		return lhs.getName() < rhs.getName() and lhs.getWeight() < rhs.getWeight() and lhs.getVolume() < rhs.getVolume();
	}
	else
	{
		return lhs.getKey() < rhs.getKey();
	}
}
