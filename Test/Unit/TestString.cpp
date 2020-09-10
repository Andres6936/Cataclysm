// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "Cataclysm/Util/String/stringfunc.h"

TEST_CASE("Remove tags color to string")
{
	CHECK(remove_color_tags("<c=green>Black<c=/>") == "Black");
}