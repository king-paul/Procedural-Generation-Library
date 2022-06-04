#include "CaveGenerator.h"
#include <windows.h>

using namespace ProceduralGeneration;

void CaveGenerator::PrintCave()
{
    system("cls");
    //system("Color 02"); // green
    //cout << endl << endl;

    for (int y = 0; y < m_map->getSize(0); y++)
    {
        for (int x = 0; x < m_map->getSize(1); x++)
        {
            int value = m_map->at(x, y);

            //std::this_thread::sleep_for(10ms);

            if (value == 0)
            {
                //cout << "\u001b[42m";
                cout << "\033[0;36m"; // 
                cout << ".";          
                //cout << "\u001b[0m";
            }
            else if (value == 1)
            {
                //cout << "\u001b[44m";
                cout << "\033[0;33m"; // 
                cout << "*";
                //cout << "\u001b[0m";
            }
            else
                cout << " ";
            //cout << "Error at: " << x << ", " << y << endl;            
        }

        cout << "\033[0;0m" << endl;
        //cout << "\u001b[40m" << endl;
    }

}

void CaveGenerator::PrintCaveWithGrid()
{   
    system("cls");
    //cout << endl << endl;

    // print column numbers
    cout << "  ";

    for (int x = 0; x < m_map->getSize(1); x++)
    {
        cout << "|" << x;

        if (x < 10)
            cout << ' ';
    }
    cout << endl;

    cout << "--";
    for (int x = 0; x < m_map->getSize(1); x++)
        cout << "---";
    cout << endl;

    for (int y = 0; y < m_map->getSize(0); y++)
    {
        cout << y;

        if (y < 10)
            cout << ' ';

        for (int x = 0; x < m_map->getSize(1); x++)
        {
            int value = m_map->at(x, y);

            if (value == 0)
                cout << "|  ";
            else if (value == 1)
                cout << "|**";
            else
                cout << "|. ";

            //cout << "Error at: " << x << ", " << y << endl;            
        }

        cout << "|" << endl;

        cout << "--";
        for (int x = 0; x < m_map->getSize(1); x++)
            cout << "---";

        cout << endl;
    }

}


void CaveGenerator::PrintCaveWithGrid(queue<Coord> coords)
{

    Coord nextCoord = coords.front();
    coords.pop();
    int queueIndex = 1;

    system("cls");
    //cout << endl << endl;

    // print column numbers
    cout << "  ";

    for (int x = 0; x < m_map->getSize(1); x++)
    {
        cout << "|" << x;

        if (x < 10)
            cout << ' ';
    }
    cout << endl;

    cout << "--";
    for (int x = 0; x < m_map->getSize(1); x++)
        cout << "---";
    cout << endl;

    for (int y = 0; y < m_map->getSize(0); y++)
    {
        cout << y;

        if (y < 10)
            cout << ' ';

        for (int x = 0; x < m_map->getSize(1); x++)
        {
            // draw queue position
            if (nextCoord.x == x && nextCoord.y == y)
            {
                cout << "|" << queueIndex;
                if (queueIndex < 10)
                    cout << ' ';

                if (coords.size() > 0)
                {
                    nextCoord = coords.front();
                    coords.pop();
                    queueIndex++;
                }

            }
            else // draw the tile
            {
                int value = m_map->at(x, y);

                if (value == 0)
                    cout << "|  ";
                else if (value == 1)
                    cout << "|**";
                else
                    cout << "|. ";
                //cout << "Error at: " << x << ", " << y << endl; 
            }
        }

        cout << "|" << endl;

        cout << "--";
        for (int x = 0; x < m_map->getSize(1); x++)
            cout << "---";

        cout << endl;
    }

}

void CaveGenerator::PrintCaveWithGrid(vector<Coord> allTiles, Coord currentTile, int searchX, int searchY)
{
    system("cls");
   
    //cout << endl << endl;

    // print column numbers
    cout << "  ";

    for (int x = 0; x < m_map->getSize(1); x++)
    {
        cout << "|" << x;

        if (x < 10)
            cout << ' ';
    }
    cout << endl;

    cout << "--";
    for (int x = 0; x < m_map->getSize(1); x++)
        cout << "---";
    cout << endl;

    // iterate through grid and print data
    for (int y = 0; y < m_map->getSize(0); y++)
    {
        cout << y;

        if (y < 10)
            cout << ' ';

        for (int x = 0; x < m_map->getSize(1); x++)
        {
            if (x == currentTile.x && y == currentTile.y) {
                cout << "|x ";
            }
            else if (x == searchX && y == searchY) {
                cout << "|o ";
            }
            else if (Helpers::CoordsInList(allTiles, x, y)){
                cout << "|+ ";
            } 
            else
            {
                int value = m_map->at(x, y);

                if (value == 0)
                    cout << "|  ";
                else if (value == 1)
                    cout << "|**";
                else
                    cout << "Error at: " << x << ", " << y << endl;
            }
        }

        // print grid outline
        cout << "|" << endl;

        cout << "--";
        for (int x = 0; x < m_map->getSize(1); x++)
            cout << "---";

        cout << endl;
    }

}

void CaveGenerator::PrintRoomOnMap(vector<Coord> coords)
{
    system("cls");
    //cout << endl << endl;

    for (int y = 0; y < m_map->getSize(0); y++)
    {
        for (int x = 0; x < m_map->getSize(1); x++)
        {
            if (Helpers::CoordsInList(coords, x, y))
                //cout << '.';
                cout << '[' << x << ',' << y << ']';// << '.';
            else
                cout << ' ';
        }

        cout << endl;
    }
}

void CaveGenerator::DrawCheckedPositions(Array2D<int>* flags)
{
    system("cls");

    for (int y = 0; y < m_map->getSize(0); y++)
    {
        for (int x = 0; x < m_map->getSize(1); x++)
        {
            cout << flags->at(x, y) << ' ';
        }

        cout << endl;
    }
}

template< class Rep, class Period>
void CaveGenerator::DrawAtPos(int x, int y, char character, unsigned int color, const std::chrono::duration<Rep, Period>& duration,
    bool usingGrid)
{
    if (!debugDraw)
        return;

    COORD cursorPos;
    if (usingGrid)
    {
        cursorPos.X = 2 + (short)x * 3;
        cursorPos.Y = (short)y + 3;
    }
    else
    {
        cursorPos.X = (short)x;
        cursorPos.Y = (short)y;
    }
    
    this_thread::sleep_for(duration);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
    cout << "\033[0;" << color << "m";
    cout << character;
}

void CaveGenerator::HighlightRoom(Room* room, unsigned int color)
{
    if (!debugDraw)
        return;

    auto roomCoords = room->GetTiles();

    for (Coord tile : roomCoords)
        DrawAtPos(tile.x, tile.y, '.', color, 0ms);
}