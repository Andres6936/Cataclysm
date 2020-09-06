// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_DICTIONARYITEM_HPP
#define CATACLYSM_DICTIONARYITEM_HPP

#include <string>
#include <string_view>

namespace Cataclysm
{

	/**
	 * This properties are necessary for use this class as Key for
	 * structs define for ISO (aka. Standard).
	 *
	 * Properties: Copyable, Copy Assignable, Move Assignable, Move-able
	 */
	class DictionaryItem
	{

	private:

		// Variables

		std::string name;

		std::string weight;

		std::string volume;

	public:

		// Construct

		/**
		 * Construct Move, set the property Move-able.
		 *
		 * @param _object Another objecto to move.
		 */
		DictionaryItem(DictionaryItem&& _object) noexcept;

		/**
		 * Construct Copy, set the property Copyable.
		 *
		 * @param _object Another object to copy.
		 */
		DictionaryItem(const DictionaryItem& _object) noexcept;

		DictionaryItem(std::string_view _name, int _weight, int _volume) noexcept;

		DictionaryItem(std::string_view _name, std::string_view _weight, std::string_view _volume) noexcept;

		// Methods

		void copy(const DictionaryItem& _object) noexcept;

		// Operators

		/**
		 * Assignment Operator, set the property of Copy Assignable.
		 *
		 * @param _object Another object to copy.
		 * @return Reference to this object.
		 */
		DictionaryItem& operator= (const DictionaryItem& _object);

		/**
		 * Assignment Move, set the property of Move Assignable.
		 *
		 * @param _object Another objecto to copy.
		 * @return Reference to this object.
		 */
		DictionaryItem& operator= (DictionaryItem&& _object) noexcept;

		// Getters

		const std::string& getName() const noexcept;

		const std::string& getWeight() const noexcept;

		const std::string& getVolume() const noexcept;

	};

}

#endif //CATACLYSM_DICTIONARYITEM_HPP
