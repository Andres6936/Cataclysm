// Joan Andrés (@Andres6936) Github.

#include <stdexcept>
#include "Cataclysm/Visual/Screen/MessageQueue.hpp"

using namespace Cataclysm;

// Methods

void MessageQueue::addMessage(const Message& _message) noexcept
{
	// Check if it's a duplicate of the last message -
	// if so, just increase count

	if (not empty())
	{
		auto& lastMessageAdded = back();

		if (_message.equalsText(lastMessageAdded))
		{
			lastMessageAdded.incrementByOneRepeatCounter();
		}
		else
		{
			emplace_back(_message);
		}
	}
	else
	{
		emplace_back(_message);
	}
}

// Getters

const std::uint32_t MessageQueue::getSize() const noexcept
{
	return size();
}

std::string MessageQueue::getMessageAt(const std::uint32_t _index) const
{
	try
	{
		const auto& message = at(_index);

		const std::string& text = message.getText();
		const std::uint32_t count = message.getCount();

		if (count > 1)
		{
			return {text + " x" + std::to_string(count) + '\n'};
		}
		else
		{
			return { text + '\n' };
		}
	}
	catch (std::out_of_range& exception)
	{
		throw std::out_of_range("Try access to element in the index: " + std::to_string(_index));
	}
}
