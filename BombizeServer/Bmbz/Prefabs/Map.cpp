#include "Map.hpp"
//#include <filesystem>

namespace Bmbz
{
	namespace Prefab
	{
		bool Map::LoadFieldMap()
		{
			// std::cout << _fileName << std::endl;
			// std::cout << "Current directory: " << std::filesystem::current_path() << std::endl;
			// std::cout << "File path: " << std::filesystem::absolute(_fileName) << std::endl;

			std::ifstream in(_fileName);
			std::string str;
			char arg;

			if (in.is_open())
			{
				in >> str;
				if (str == "#bombize.map")
				{
					in >> arg >> str;
					_scaleX = static_cast<uint8_t>(std::stoi(str));
					in >> arg >> str;
					_scaleY = static_cast<uint8_t>(std::stoi(str));

					if (in.eof() || in.fail())
					{
						in.close();
						return false;
					}

					int countCellsInField = _scaleX * _scaleY;
					_fieldMap.reserve(countCellsInField);

					for (int i = 0; i < countCellsInField; ++i)
					{
						in >> str;
						_fieldMap.push_back(static_cast<uint8_t>(std::stoi(str)));
					}

					if (in.fail())
					{
						in.close();
						return false;
					}

					in >> str;
					if (str != "#end")
					{
						in.close();
						return false;
					}
				}
				else
				{
					in.close();
					return false;
				}
			}
			else
			{
				return false;
			}
			in.close();
			return true;
		}

		uint8_t Map::GetCell(uint8_t x, uint8_t y) const
		{
			return _fieldMap[_scaleX * y + x];
		}

		void Map::AddCellValues(uint8_t x, uint8_t y, uint8_t maskAdd)
		{
			_fieldMap[_scaleX * y + x] |= maskAdd;
		}

		void Map::RemoveCellValues(uint8_t x, uint8_t y, uint8_t maskRemove)
		{
			_fieldMap[_scaleX * y + x] &= ~maskRemove;
		}

		void Map::ClearCellValue(uint8_t x, uint8_t y)
		{
			_fieldMap[_scaleX * y + x] = 0;
		}

		bool Map::TrySetBomb(uint8_t x, uint8_t y)
		{
			if(_fieldMap[_scaleX * y + x] & CellMask::Occupied)
			{
				return false;
			}
			AddCellValues(x, y, CellMask::Occupied | CellMask::LetThrough);
			return true;
		}

		void Map::RemoveBomb(uint8_t x, uint8_t y)
		{
			if(_fieldMap[_scaleX * y + x] & CellMask::Occupied)
				RemoveCellValues(x, y, CellMask::Occupied | CellMask::LetThrough);
		}
	}
}
