// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_BOOKGENRE_HPP
#define CATACLYSM_BOOKGENRE_HPP

#include <string>

enum Book_genre
{
	GENRE_NULL = 0,
	GENRE_AUTOBIOGRAPHY,
	GENRE_HISTORY,
	GENRE_NOVEL,    // Typeless novel.
	GENRE_SCIFI,
	GENRE_FANTASY,
	GENRE_MYSTERY,
	GENRE_ROMANCE,
	GENRE_MAX
};

Book_genre lookup_book_genre(std::string name);

std::string book_genre_name(Book_genre genre);

#endif //CATACLYSM_BOOKGENRE_HPP
