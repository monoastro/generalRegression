#pragma once
#include <iostream>
#include <string>

//For most people just starting the default values will suffice
//But for certain applications that may arise when learning more advanced 
//or general forms of linear regression use this constructor that
//gives finer control over the creation of the data-table:
//To make resizing the table easier, use linked lists
template <typename T = double, unsigned rows = 0, unsigned labels = 1, unsigned outputs = 1>
class dataTable
{
public:
	dataTable();
	dataTable(std::initializer_list<std::initializer_list<T>> data);
	~dataTable();

	//this is nice 
	T* operator[](unsigned row) { return m_data[row]; }

	//console input has no use outside of initial testing purposes
	//I mean who's gonna input their dataset to the console 
	//every time they run the program
	//void readData();
	//the constructor method is much better for the time being

	//console output stuff
	void showData();

	//this work with the data
	template<typename>
	friend class hypothesis;
private:

	//separating this from the constructor because we
	//need to allocate memory at different times
	//depending on the constructor called 
	void allocateMem();

	//this is a member function because it requires
	//information about the table
	void printTheBar();

	T **m_data;
	unsigned m_rows, m_columns, m_xColumns, m_yColumns;
	//I think for further applications there needs to be a distinction
	//between what is dependent and what is independent in the data table
	//so there are variables to store the number of columns of x and y
};

//B R U H templated class's implementation should
//be kept in the header file. Makes sense if I think
//about it

//default arguments for the traditional univariate linear regression
//one dependent, one independent y = f(x) situation
//also can accomodate console input of any size dataTable if initializer
//list is not used but why would you do that?
template <typename T, unsigned rows, unsigned labels, unsigned outputs>
dataTable<T, rows, labels, outputs>::dataTable()
: 
	m_data(nullptr),
	m_rows(rows),
	m_columns(1+labels+outputs), /*the 1 is for bias neuron*/
	m_xColumns(labels+1), m_yColumns(outputs)
{
	if(!m_rows)
	{
		std::cout<<"Enter the number of dataPoints: ";
		std::cin>>m_rows;
	}

	allocateMem();

	std::cout<<"Provide the data to do regression on \nx_1 x_2 ... y_0 y_1....\nform:\n";
	//read data-table from the user 
	for(unsigned i = 0; i<m_rows; i++)
	{
		m_data[i][0] = 1;
		std::cout<<"Insert the elements of row "<<i<<":\n";
		for(unsigned j = 1; j<m_columns; j++)
		{
			std::cin>>m_data[i][j];
		}
	}

}

template <typename T, unsigned rows, unsigned labels, unsigned outputs>
dataTable<T, rows, labels, outputs>::dataTable(std::initializer_list<std::initializer_list<T>> list)
:
	m_data(nullptr),
	m_rows(list.size()), 
	m_columns(list.begin()->size() + 1), /*again, the +1 is for bias neuron*/
	m_xColumns(labels + 1), m_yColumns(outputs)
{
	//as a result of ^, there is an assumption that the matrix is a rectangular matrix 
	//I could iterate through the list and allocate memory according to the columns which
	//should allow for variable row length I can't find a reason why I'd have variable
	//column length in the first place so yeah.
	
	//If there's a mismatch, insult the user and refuse to proceed.


	if(m_rows != rows || m_columns != (m_xColumns + m_yColumns))
	{
		std::cout<<"Learn to count dumbass\n";
		exit(-1);
	}

	allocateMem();
	
	//initialize the array with the values
	{
		unsigned i = 0;
		for(auto &row: list)
		{
			unsigned j = 0;
			m_data[i][j++] = 1; //initialize the first column as 1 for bias
			for(auto &value: row)
			{
				m_data[i][j] = value;
				j++;
			}
			i++;
		}
	}	
}

template <typename T, unsigned rows, unsigned labels, unsigned outputs>
dataTable<T, rows, labels, outputs>::~dataTable()
{
	for(unsigned i = 0; i<m_rows; i++)
	{
		delete [] m_data[i];
	}
	delete [] m_data; 
}

template <typename T, unsigned rows, unsigned labels, unsigned outputs>
void dataTable<T, rows, labels, outputs>::allocateMem()
{
	m_data = new T*[m_rows];
	for(unsigned i = 0; i<m_rows; i++)
	{
		m_data[i] = new T[m_columns];
	}
	//notable thing here is that we interpret the memory
	//allocated using m_columns as the row and that for m_rows 
	//as the first column of the matrix all because 
	//number of rows == size of column
	//number of columns == size of rows
	//(assuming a matrix)
}


//console display stuff, irrelevant, just ignore
void coutFormatter()
{
	std::cout.width(11);
	std::cout.fill(' ');
	std::cout<<std::right;
}

template <typename T, unsigned rows, unsigned labels, unsigned outputs>
void dataTable<T, rows, labels, outputs>::printTheBar()
{
	std::cout<<"Row/Column |";

	for(unsigned j = 0; j<m_xColumns; j++)
	{
		coutFormatter();
		std::cout<<("x_" + std::to_string(j))<<'|';
	}
	for(unsigned j = 0; j<m_yColumns; j++)
	{
		coutFormatter();
		std::cout<<("y_" + std::to_string(j))<<'|';
	}
	std::cout<<"\n";

	for(unsigned j = 0; j<m_columns+1; j++) //the +1 here is for the index column
	{
		std::cout<<"------------";
	}
	std::cout<<"-\n";
}

template <typename T, unsigned rows, unsigned labels, unsigned outputs>
void dataTable<T, rows, labels, outputs>::showData()
{

	printTheBar();
	for(unsigned i = 0; i<m_rows; i++)
	{
		//the separate loops are for formatting, 
		coutFormatter();
		std::cout<<i<<'|';
	
		//printing the dependent variables
		for(unsigned j = 0; j<m_xColumns; j++)
		{
			coutFormatter();
			std::cout<<m_data[i][j]<<'|';
		}

		//printing the independent variables
		for(unsigned j = m_xColumns; j<m_columns; j++)
		{
			coutFormatter();
			std::cout<<m_data[i][j]<<'|';
		}

		std::cout<<"\n";
	}
	std::cout<<"\n";
}



