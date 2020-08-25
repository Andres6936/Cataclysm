// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_CREATEWORLD_HPP
#define CATACLYSM_CREATEWORLD_HPP

#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/World/worldmap.h>

namespace Cataclysm
{

	class CreateWorldScreen : public IScreen
	{

	private:

		Doryen::Console createWorldConsole {80, 24};

		cuss::interface i_editor;

		Worldmap tmp_world;

		std::string world_name;

	public:

		// Construct

		CreateWorldScreen();

		// Methods

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_CREATEWORLD_HPP
