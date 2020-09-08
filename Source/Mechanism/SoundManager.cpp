// Joan Andrés (@Andres6936) Github.

#include <Doryen/Doryen.hpp>
#include <Cataclysm/Entity/Player/player.h>
#include "Cataclysm/Mechanism/SoundManager.hpp"
#include <Cataclysm/Visual/Screen/MessageQueue.hpp>

using namespace Cataclysm;

void SoundManager::makeSound(Sound snd, Tripoint pos)
{
	if (snd.description.empty())
	{
		return;
	}
// TODO: Alert monsters
	Direction_full dir = get_general_direction(player->pos, pos);
	snd.volume -= rl_dist(pos, player->pos);
	if (snd.volume <= 0)
	{
		return; // To quiet to hear!
	}
// TODO: Don't hardcode color
// We don't punctuate the messages below - that's for the sound to do!
	if (dir == DIRFULL_NULL)
	{ // On top of the player!
		messageQueue.addMessage({ Doryen::format("<c=ltblue>You hear {}<c=/>", snd.description) });
	}
	else
	{
		messageQueue.addMessage(
				{ Doryen::format("<c=ltblue>To the <c=ltred>{}<c=ltblue>, you hear {}<c=/>", Direction_name(dir).c_str(),
						snd.description.c_str()) });
	}
}

void SoundManager::makeSound(std::string desc, int volume, Point pos)
{
	makeSound(desc, volume, Tripoint(pos.x, pos.y, 0));
}

void SoundManager::makeSound(std::string desc, int volume, Tripoint pos)
{
	makeSound(Sound(desc, volume), pos);
}

void SoundManager::makeSound(std::string desc, int volume, int x, int y)
{
	makeSound(desc, volume, Point(x, y));
}
