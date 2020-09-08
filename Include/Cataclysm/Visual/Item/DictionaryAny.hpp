// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_DICTIONARYANY_HPP
#define CATACLYSM_DICTIONARYANY_HPP

#include <map>
#include <Cataclysm/Visual/Item/DictionaryItem.hpp>

namespace Cataclysm
{
	using DictionaryAny = std::multimap<DictionaryItem, bool, DictionaryItemCompare>;
}

#endif //CATACLYSM_DICTIONARYANY_HPP
