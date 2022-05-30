#include "Pattern.h"

void Pattern::SetValue(int x, int y, int value)
{
	//m_grid.set(x, y, value);
}

int Pattern::GetValue(int x, int y)
{
	return 0;// m_grid.get(x, y);
}

bool Pattern::ValueEqualAtPosition(int x, int y, int value)
{
	return value == GetValue(x, y);
}

bool Pattern::EqualToPattern(Direction direction, Pattern* pattern)
{
	Array2D<int> gridA = GetValuesInDirection(direction);
	Array2D<int> gridB = GetValuesInDirection(Direction2D::GetOpposite(direction));

	return false;
}

Array2D<int> Pattern::GetValuesInDirection(Direction direction)
{
	Array2D<int> gridSection;

	switch (direction)
	{
        case Direction::Up:
            gridSection = JaggedArray<int>(m_grid->Length() - 1, m_grid->Length());
            // xMin, xMax, yMin, yMax
            CreateSection(0, m_grid->Length(), 1, m_grid->Length(), gridSection);

            break;
        case Direction::Down:
            gridSection = JaggedArray<int>(m_grid->Length() - 1, m_grid->Length());
            CreateSection(0, m_grid->Length(), 0, m_grid->Length(), gridSection);
            break;

        case Direction::Left:
            gridSection = JaggedArray<int>(m_grid->Length(), m_grid->Length() - 1);
            CreateSection(0, m_grid->Length() - 1, 0, m_grid->Length(), gridSection);
            break;

        case Direction::Right:
            gridSection = JaggedArray<int>(m_grid->Length(), m_grid->Length() - 1);
            CreateSection(1, m_grid->Length(), 0, m_grid->Length(), gridSection);
            break;

        default:
            return *m_grid;
	}

	return gridSection;
}

void Pattern::CreateSection(int xMin, int xMax, int yMin, int yMax, Array2D<int> gridPartToCompare)
{
    vector<int> templ1DList;

    // adds all values in the grid to the temporary list
    for (int row = 0; row < yMax; row++)
    {
        for (int col = xMin; col < xMax; col++)
        {
            templ1DList.Add(m_grid->get(row, col));
        }
    }

    // add each value in the tempoary list into the parts to compare
    for (int i = 0; i < templ1DList.size(); i++)
    {
        // coverts 1 dimensional index into x and y coordinates
        // for 2D array
        int x = i % gridPartToCompare.Length();
        int y = i / gridPartToCompare.Length();

        gridPartToCompare.set(x, y, templ1DList[i]);
    }

}
