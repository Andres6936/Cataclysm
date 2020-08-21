// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_BINDING_HPP
#define CATACLYSM_BINDING_HPP

#include <string>
#include <Cuss/Enum/ActionID.hpp>

namespace cuss
{

	struct binding
	{
		action_id act;
		std::string target;
		int a, b;

		binding(action_id ACT = ACT_NULL, std::string T = "", int A = 0, int B = 0) :
				act(ACT), target(T), a(A), b(B)
		{
		};

		std::string save_data();

		void load_data(std::istream& datastream);
	};

}

#endif //CATACLYSM_BINDING_HPP
