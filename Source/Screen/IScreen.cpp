// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/IScreen.hpp"

using namespace Cataclysm;

bool IScreen::showQueryYesNo(std::string_view _text)
{
	console.writeWrapText({0, 0}, {80, 24}, Doryen::BlendModes::SET, _text);
	console.draw();

	flushinp();

	long key = getch();

	while(key not_eq 'y' or key not_eq 'n')
	{
		if (key == 'y')
		{
			return true;
		}
		else if (key == 'n')
		{
			return false;
		}

		flushinp();

		key = getch();
	}

	return false;
}

void IScreen::showDebugMessage(std::string_view _text)
{
	console.writeWrapText({0, 0}, {80, 24}, Doryen::BlendModes::SET, _text);
	console.draw();

	flushinp();

	long ch = getch();

	while (ch not_eq '\n')
	{
		flushinp();

		ch = getch();
	}
}

void IScreen::showMessagePopup(std::string_view _text)
{
	console.writeWrapText({0, 0}, {80, 24}, Doryen::BlendModes::SET, _text);
	console.draw();

	flushinp();

	long key = getch();

	while(key not_eq '\n' or key not_eq ' ')
	{
		flushinp();
		key = getch();
	}
}
