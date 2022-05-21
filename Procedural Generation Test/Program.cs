﻿using System;

namespace ProceduralGeneration
{
    internal class Program
    {
        static void Main(string[] args)
        {
			DungeonGenerator dungeon = new CorridorFirstDungeon(200, 200, 100, 100, 30, 10, 0.5f);
									       //RandomWalkRoom();

            var map = dungeon.GetMap();
			PrintDungeon(map);

			//Console.ReadKey();
		}

		static void PrintDungeon(Tile[,] map)
        {
			for (int y = 0; y < map.GetLength(0); y++)
			{
				for (int x = 0; x < map.GetLength(1); x++)
				{
					switch (map[y, x])
					{
						case Tile.Empty: 
							Console.Write(" ");
							break;

						case Tile.Floor: 
							Console.Write(" ");
							break;

						case Tile.WallFull:	
							Console.Write("*");
							break;

						case Tile.Top: 
							Console.Write("_");
							break;

						case Tile.Bottom: case Tile.SideBottom:	
							Console.Write("-");
							break;

						case Tile.SideLeft: case Tile.SideRight:
							Console.Write("|");
							break;

						case Tile.InnerCornerDownLeft: 
							Console.Write("<");
							break;

						case Tile.InnerCornerDownRight:	
							Console.Write(">");
							break;

						case Tile.DiagonalCornerDownLeft: case Tile.DiagonalCornerUpLeft: 
								Console.Write("/");
							break;

						case Tile.DiagonalCornerDownRight: case Tile.DiagonalCornerUpRight: 
								Console.Write("\\");
							break;
					}
				}

				Console.WriteLine();
			}
		}
    }
}