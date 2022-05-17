using System;

namespace ProceduralGeneration
{
    internal class Program
    {
        static void Main(string[] args)
        {
			int size = 50;

            DungeonGenerator dungeon = new DungeonGenerator(size);

            //Console.WriteLine("Dungeon generated successfully");

            var map = dungeon.GetMap();
			PrintDungeon(map);
		}

		static void PrintDungeon(int[,] map)
        {
			for (int y = 0; y < map.GetLength(0); y++)
			{
				for (int x = 0; x < map.GetLength(1); x++)
				{
					int value = map[y, x];

					if (value == 0)
						Console.Write(" ");
					else if (value == 1)
						Console.Write("o");
					else if (value == -1)
						Console.Write("*");
				}

				Console.WriteLine();
			}
		}
    }
}