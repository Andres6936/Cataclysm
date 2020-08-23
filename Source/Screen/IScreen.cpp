// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/IScreen.hpp"

using namespace Cataclysm;

bool IScreen::showQueryYesNo(std::string_view _text)
{
	console.writeText({0, 0}, {80, 24}, Doryen::BlendModes::SET, _text);
	console.draw();

	Doryen::KeyCode key = console.getKeyPressed().getKeyCode();

	while(key not_eq Doryen::KeyCode::Y or key not_eq Doryen::KeyCode::N)
	{
		if (key == Doryen::KeyCode::Y)
		{
			return true;
		}
		else if (key == Doryen::KeyCode::N)
		{
			return false;
		}

		key = console.getKeyPressed().getKeyCode();
	}
}

void IScreen::showDebugMessage(std::string_view _text)
{
	console.writeText({0, 0}, {80, 24}, Doryen::BlendModes::SET, _text);
	console.draw();
}
