#ifndef BMBZ_PREFAB_MAPPREFAB_HPP
#define BMBZ_PREFAB_MAPPREFAB_HPP

#include <cstdint>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include "NetworkTypes/Vector2.hpp"
#include "CellMask.hpp"

namespace Bmbz
{
	namespace Prefab
	{
		struct Map
		{		
			Map() = default;
			Map(std::chrono::minutes timeLife, std::string fileName)
			: timeLife(timeLife)
			, _fileName(std::move(fileName))
			{}

			std::chrono::minutes timeLife;

			bool LoadFieldMap();
			// Undefined behavior if the map is not loaded:

			NetType::Vector2<uint8_t> Scale() const
			{
				return NetType::Vector2<uint8_t>{_scaleX, _scaleY};
			}

			uint8_t GetCell(uint8_t x, uint8_t y) const;
			void AddCellValues(uint8_t x, uint8_t y, uint8_t mask);
			void RemoveCellValues(uint8_t x, uint8_t y, uint8_t mask);
			void ClearCellValue(uint8_t x, uint8_t y);

			bool TrySetBomb(uint8_t x, uint8_t y);
			void RemoveBomb(uint8_t x, uint8_t y);

		private:
			std::string _fileName;
			std::vector<uint8_t> _fieldMap;
			uint8_t _scaleX, _scaleY;
		};
	}
}
#endif