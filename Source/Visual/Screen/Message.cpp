// Joan Andrés (@Andres6936) Github.

#include <utility>
#include "Cataclysm/Visual/Screen/Message.hpp"

using namespace Cataclysm;

Message::Message(Message&& _object) noexcept : text(std::move(_object.text)), turn(_object.turn)
{

}

Message::Message(const Message& _object) noexcept
{
	this->copy(_object);
}

Message::Message(std::string_view _text, std::uint32_t _turn) noexcept : text(_text), turn(_turn)
{
	// String, Integer
}

// Methods

void Message::copy(const Message& _object) noexcept
{
	text = _object.text;
	turn = _object.turn;
	count = _object.count;
}

Message& Message::operator=(const Message& _object) noexcept
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

Message& Message::operator=(Message&& _object) noexcept
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
		std::swap(text, _object.text);
		turn = std::exchange(_object.turn, 0);
		count = std::exchange(_object.count, 0);

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

const std::string& Message::getText() const noexcept
{
	return text;
}

const std::uint32_t Message::getTurn() const noexcept
{
	return turn;
}

const std::uint32_t Message::getCount() const noexcept
{
	return count;
}

// Setters

void Message::setText(const std::string& _text) noexcept
{
	text = _text;
}

void Message::setTurn(const std::uint32_t _turn) noexcept
{
	turn = _turn;
}

void Message::setCount(const std::uint32_t _count) noexcept
{
	count = _count;
}
