// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "Cataclysm/Util/String/stringfunc.h"

using namespace Cataclysm;

TEST_CASE("Remove tags color to string")
{
	CHECK(removeColorTags("<c=green>Black<c=/>") == "Black");
	CHECK(removeColorTags("<c=blue>Black") == "Black");
	CHECK(removeColorTags("Black<c=blue>Black") == "BlackBlack");
	CHECK(removeColorTags("<c=blue> Black") == " Black");
	CHECK(removeColorTags(" Black<c=blue>Black") == " BlackBlack");
	CHECK(removeColorTags("Black<c=/>") == "Black");
	CHECK(removeColorTags("<c=blue><c=/>Black") == "Black");
}