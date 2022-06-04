using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace ProceduralGeneration
{
    public class CaveGenerator
    {
        private bool[,] map;
        private int[,] marchingSquares;

        // External Functions
        [DllImport("Procedural Generation Library.dll")]
        protected static extern IntPtr GenerateCave(int width, int height, int fillPercent, int smoothingIterations, int borderSize,
        int wallThresholdSize, int roomThresholdSize, int passageWidth, bool forceAccessToMain, bool useRandomSeed, int seed);

        [DllImport("Procedural Generation Library.dll")]
        protected static extern bool IsWall(int x, int y, IntPtr cavePointer);

        [DllImport("Procedural Generation Library.dll")]
        protected static extern int GetMarchingSquareValue(int x, int y, IntPtr cavePointer);

        // Properties
        /// <summary>
        /// Returns map of solid and vacant spaces
        /// </summary>
        public bool[,] MAP { get => map; }

        /// <summary>
        /// Returnes a grid of marching square configurations
        /// </summary>
        public int[,] MarchingSquares { get => marchingSquares; }


        // Constructor
        public CaveGenerator(int width = 128, int height = 72, int fillPercent = 53, int smoothingIterations = 5, int borderSize = 3,
                        int wallThresholdSize = 50, int roomThresholdSize = 50, int passageWidth = 1, bool forceAccessToMain = true,
                        bool useRandomSeed = false, int seed = 0)
        {
            IntPtr cave =
                GenerateCave(width, height, fillPercent, smoothingIterations, borderSize, wallThresholdSize, roomThresholdSize,
                             passageWidth, forceAccessToMain, useRandomSeed, seed);

            map = new bool[height + (borderSize), width + (borderSize)];
            marchingSquares = new int[height + (borderSize), width + (borderSize)];

            // fill map array
            for (int y = 0; y < map.GetLength(0); y++)
            {
                for (int x = 0; x < map.GetLength(1); x++)
                {
                    map[y, x] = IsWall(x, y, cave);
                }
            }

            // fill marching squares array
            for (int y = 0; y < marchingSquares.GetLength(0); y++)
            {
                for (int x = 0; x < marchingSquares.GetLength(1); x++)
                {
                    marchingSquares[y, x] = GetMarchingSquareValue(x, y, cave);
                }
            }
        }
    }
}
