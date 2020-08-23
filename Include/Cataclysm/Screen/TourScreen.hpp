// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TOURSCREEN_HPP
#define CATACLYSM_TOURSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class TourScreen : public IScreen
	{

	private:

		Doryen::Console tourScreen {80, 24};

		std::string tour_name;

		cuss::interface i_tour;

		std::uint32_t indexFile = 1;

	public:

		TourScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_TOURSCREEN_HPP
