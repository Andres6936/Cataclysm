// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_DICTIONARYCLOTHING_HPP
#define CATACLYSM_DICTIONARYCLOTHING_HPP

#include <map>
#include <Cataclysm/Visual/Item/DictionaryItem.hpp>

namespace Cataclysm
{
	using DictionaryClothing = std::multimap<DictionaryItem, bool, DictionaryItemCompare>;
}

#endif //CATACLYSM_DICTIONARYCLOTHING_HPP
