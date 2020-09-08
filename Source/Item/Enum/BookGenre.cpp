// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Item/Enum/BookGenre.hpp"
#include <Cataclysm/Util/String/stringfunc.h>

Book_genre lookup_book_genre(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 0; i < GENRE_MAX; i++)
	{
		Book_genre ret = Book_genre(i);
		if (name == no_caps(book_genre_name(ret)))
		{
			return ret;
		}
	}
	return GENRE_NULL;
}

std::string book_genre_name(Book_genre genre)
{
	switch (genre)
	{
	case GENRE_NULL:
		return "NULL";
	case GENRE_AUTOBIOGRAPHY:
		return "autobiography";
	case GENRE_HISTORY:
		return "history";
	case GENRE_NOVEL:
		return "novel";
	case GENRE_SCIFI:
		return "scifi";
	case GENRE_FANTASY:
		return "fantasy";
	case GENRE_MYSTERY:
		return "mystery";
	case GENRE_ROMANCE:
		return "romance";
	case GENRE_MAX:
		return "ERROR - GENRE_MAX";
	default:
		return "ERROR - Unnamed Book_genre";
	}
	return "ERROR - Escaped book_genre_name() switch";
}


