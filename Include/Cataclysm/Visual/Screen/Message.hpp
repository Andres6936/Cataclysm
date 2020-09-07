// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_MESSAGE_HPP
#define CATACLYSM_MESSAGE_HPP

#include <string>
#include <cstdint>
#include <string_view>

namespace Cataclysm
{

	class Message
	{

	private:

		std::string text;

		std::uint32_t turn = 0;

		/**
		 * The number of times this message is repeated
		 */
		std::uint32_t count = 1;

	public:

		Message() noexcept = default;

		/**
		 * Construct Move, set the property Move-able.
		 *
		 * @param _object Another objecto to move.
		 */
		Message(Message&& _object) noexcept;

		/**
		 * Construct Copy, set the property Copyable.
		 *
		 * @param _object Another object to copy.
		 */
		Message(const Message& _object) noexcept;

		Message(std::string_view _text, std::uint32_t _turn) noexcept;

		// Methods

		void copy(const Message& _object) noexcept;

		/**
		 * Increment by one the number of times that it message is repeated
		 */
		void incrementByOneRepeatCounter() noexcept;

		/**
		 * Only compare the texts
		 *
		 * @param _object Another object to compare
		 * @return True if the text is equals to another object.
		 */
		bool equalsText(const Message& _object) const noexcept;

		// Operators

		/**
		 * Assignment Operator, set the property of Copy Assignable.
		 *
		 * @param _object Another object to copy.
		 * @return Reference to this object.
		 */
		Message& operator= (const Message& _object) noexcept;

		Message& operator= (Message&& _object) noexcept;

		// Getters

		const std::string& getText() const noexcept;

		const std::uint32_t getTurn() const noexcept;

		const std::uint32_t getCount() const noexcept;

		// Setters

		void setText(const std::string& _text) noexcept;

		void setTurn(const std::uint32_t _turn) noexcept;

		void setCount(const std::uint32_t _count) noexcept;

	};

}

#endif //CATACLYSM_MESSAGE_HPP
