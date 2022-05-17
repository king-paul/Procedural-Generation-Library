using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace ProceduralGeneration
{
    [StructLayout(LayoutKind.Sequential)]
	struct Coord
	{
		int x;
		int y;

		Coord(int x, int y)
		{
			this.x = x;
			this.y = y;
		}

        public static Coord operator +(Coord lhs, Coord rhs)
		{
			return new Coord(lhs.x + rhs.x, lhs.y + rhs.y);
		}

		public static Coord operator -(Coord lhs, Coord rhs)
		{
			return new Coord(lhs.x - rhs.x, lhs.y - rhs.y);
		}

		/*
		public static Coord operator +=(Coord lhs, Coord rhs)
		{
			lhs.x += rhs.x;
			lhs.y += rhs.y;

			return lhs;
		}

		public static bool operator ==(Coord lhs, Coord rhs)
		{
			if (lhs.x == rhs.x && lhs.y == rhs.y)
				return true;

			return false;
		}*/


	};

	public class DungeonGenerator
	{
		private readonly IntPtr dungeonPointer;
		private int size;

		private int[,] map;

		[DllImport("Procedural Generation Library.dll")]
		private static extern void TestFunction();

		[DllImport("Procedural Generation Library.dll")]
		private static extern IntPtr CreateRandomWalkRoom(
			int size, int startX, int startY, int iterations, int walkLength, bool startRandomly = true);

		[DllImport("Procedural Generation Library.dll")]
		private static extern int GetSpaceValue(IntPtr dungeon, int x, int y);

		public DungeonGenerator(int size)
		{
			this.size = size;
			map = new int[size, size];

			dungeonPointer = CreateRandomWalkRoom(size, 25, 25, 50, 15, false);

			for (int y = 0; y < size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					map[x, y] = GetSpaceValue(dungeonPointer, x, y);
				}

			}

		}

		public int[,] GetMap()
		{
			return map;
		}

    }
}
