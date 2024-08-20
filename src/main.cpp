#include "dataTable.hpp"
#include "hypothesis.hpp"

int main()
{
	dataTable<unsigned, 11, 1, 1>  housingPrice
	{
		{1, 1},
		{2, 2},
		{3, 3},
		{4, 4},
		{5, 5},
		{6, 6},
		{7, 7},
		{8, 8},
		{9, 9},
		{10, 10},
		{11, 11},
	};

	hypothesis model0, model1;
	model0.train(housingPrice);
	model1.train(housingPrice);
	return 0;
}
