#include <iostream>
#include "../Bmbz/Game/Components/MapDestroyer.hpp"

using namespace Bmbz;

int main(int argc, char* argv[])
{
    Game::Component::MapDestroyer _destroyer;
    _destroyer.Init(4, 4, 1);

    for(int i = 0; i < 12; i++)
    {
        auto cell = _destroyer.GetNextDestroyCell();
        if(cell.has_value())
        {
            std::cout << i << ") Cell: x = " << cell.value().x << ", y = " << cell.value().y << "\n";
        }
        else
        {
            std::cout << i << ") No cell" << "\n";
        }   
    }

    return 0;
}
