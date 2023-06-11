#include "dataTable.hpp"
#include "hypothesis.hpp"

int main()
{
	//define the data
	dataTable<unsigned, 4, 2, 1>  housingPrice {
		{1, 2, 4},
		{2, 3, 6},
		{3, 4, 8},
		{4, 5, 10},
	};

	hypothesis<double> model0;
	model0.train(housingPrice);



	return 0;
}
