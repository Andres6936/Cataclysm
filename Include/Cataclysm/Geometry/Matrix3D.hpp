// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_MATRIX3D_HPP
#define CATACLYSM_MATRIX3D_HPP

#include <array>
#include <cstdint>

namespace Cataclysm
{

	template<class T, std::uint32_t Width, std::uint32_t Height, std::uint32_t Depth>
	class Matrix3D : public std::array<T, Width * Height * Depth>
	{

	public:

		// Methods

		T& getAt(const std::uint32_t x, const std::uint32_t y, const std::uint32_t z)
		{
			return this->at(x * Height * Depth + y * Depth + z);
		}

		const T& getAt(const std::uint32_t x, const std::uint32_t y, const std::uint32_t z) const
		{
			return this->at(x * Height * Depth + y * Depth + z);
		}

		// Getters

		const std::uint32_t getWidth() const noexcept
		{
			return Width;
		}

		const std::uint32_t getHeight() const noexcept
		{
			return Height;
		}

		const std::uint32_t getDepth() const noexcept
		{
			return Depth;
		}

	};

}

#endif //CATACLYSM_MATRIX3D_HPP
