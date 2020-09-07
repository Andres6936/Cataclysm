// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_MESSAGEQUEUE_HPP
#define CATACLYSM_MESSAGEQUEUE_HPP

#include <deque>
#include <string>
#include <Cataclysm/Visual/Screen/Message.hpp>

namespace Cataclysm
{

	class MessageQueue : private std::deque<Message>
	{

	private:

		/**
		 * Identical messages within MESSAGE_GAP turns of each other are combined.
		 */
		const static std::uint8_t MESSAGE_GAP {3};

	public:

		// Methods

		void addMessage(const Message& _message) noexcept;

		std::string getMessageAt(const std::uint32_t _index) const;

		// Getters

		const std::uint32_t getSize() const noexcept;

	};

}

#endif //CATACLYSM_MESSAGEQUEUE_HPP
