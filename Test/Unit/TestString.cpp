// Joan Andrés (@Andres6936) Github.

#include <doctest/doctest.h>
#include "../../Include/Cataclysm/Util/String/stringfunc.h"

TEST_CASE("Remove tags color to string")
{
	CHECK(remove_color_tags("<c=green>Black<c=/>") == "Black");
}