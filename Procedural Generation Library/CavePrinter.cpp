#include "CaveGenerator.h"
//#include <consoleapi2.h>

using namespace ProceduralGeneration;

void CaveGenerator::PrintMapToConsole()
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

void CaveGenerator::PrintMapToConsole(queue<Coord> coords)
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

void CaveGenerator::PrintMapToConsole(vector<Coord> allTiles, Coord currentTile, int searchX, int searchY)
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

/*
void ProceduralGeneration::CaveGenerator::DrawRegionsPositions(vector<Coord> region)
{
    system("cls");
    //cout << endl << endl;

    for (int y = 0; y < m_map->getSize(0); y++)
    {
        for (int x = 0; x < m_map->getSize(1); x++)
        {
            if (Helpers::CoordsInList(region, x, y))
                cout << '.';
            else
                cout << ' ';
        }

        cout << endl;
    }

}
*/