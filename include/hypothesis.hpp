#pragma once
#include "dataTable.hpp"


#define threshold 1e-24

//this class is where the training will take place
//So current thought is to "assign"
//a dataTable instance to each instance of hypothesis
//by having a unique_ptr store a reference to the dataTable class

//scratch that, it got way too complicated way too fast.
//I'm just gonna pass the dataTable 

template<typename thetaT = double>
class hypothesis
{
	public:

		hypothesis(double alpha = 0.001f);
		~hypothesis();

		//to implement
		double vectorizedDotProduct();
		void plot();

		void showTheta();

		//the main meat
		template <typename T, unsigned rows, unsigned labels, unsigned outputs>
		void train(dataTable<T, rows, labels, outputs>& data_table, unsigned mapToOutput = 0);

	private:
		thetaT* m_theta;
		thetaT* m_deviation;
		unsigned m_thetaSize;
		double m_alpha, m_error, m_previousError;
};

//definitions
template<typename thetaT>
hypothesis<thetaT>::hypothesis(double alpha)
	:
		m_theta(NULL),
		m_deviation(NULL),
		m_thetaSize(0),
		m_alpha(alpha),
		m_error(10),
		m_previousError(0)
{
}


template<typename thetaT>
hypothesis<thetaT>::~hypothesis()
{
	if(m_theta!=NULL && m_deviation!=NULL)
	{
		delete [] m_theta;
		delete [] m_deviation;
	}
}

template<typename thetaT>
void hypothesis<thetaT>::plot()
{
}

template<typename thetaT>
template <typename T, unsigned rows, unsigned labels, unsigned outputs>
void hypothesis<thetaT>::train(dataTable<T, rows, labels, outputs>& data_table, unsigned mapToOutput)
{
	//allocate the m_theta and m_deviation vector some memory, 
	m_thetaSize = data_table.m_xColumns;
	m_theta = new double[m_thetaSize];
	m_deviation = new double [data_table.m_rows];

	//display alpha and the table 
	showTheta();
	data_table.showData();
	std::cout<<"\n";

	//now then that's out of the way let's write the actual meat of the program

	//I've combined error function and gradient descent 
	//because separating them makes no sense computationally
	//(other than academic purposes)
	do
	{
		m_previousError = m_error;
		m_error = 0;

		//deviation in a mathematical sense is the difference between the predicted and the "actual" value
		//They are values unique to each Row i.e. constant for an iteration but is different for different 
		//iteration due to the values of theta/weights being changed every iteration.
		
		//I've noticed that every iteration's calculation requires multiple use of the value of deviation of each row so,
		//I've separated it and created a lookup table so as to not have to repeat calculation of it again
		for(unsigned i = 0; i<data_table.m_rows; i++)
		{
			double y_prediction = 0;
			//y_prediction = vectorizedDotProduct(); ideally
			//but for now; serial dot product
			for(unsigned j = 0; j<data_table.m_xColumns; j++)
			{
				y_prediction += m_theta[j] * data_table[i][j];
			}
			m_deviation[i] = y_prediction - data_table[i][data_table.m_xColumns + mapToOutput];
			//common term = y_prediction - y_actual^

			m_error += m_deviation[i] * m_deviation[i];
			//and it just so happens that the deviations can be
			//used here to calculate the exact value of the error
		}

		m_error /= (2.0f*data_table.m_rows);

		//Now that delta for each row has been found, we do:
		//The actual calculation and updating weights:

		//This is equivalent to simultaneously updating all weights as delta has already been calculated
		//with all of the previous theta values so, assume deviations as constants from this point
		for(unsigned j = 0; j<data_table.m_xColumns; j++)
		{
			double accumulator = 0;
			//for updating factor of theta0, take the deviations of all
			//rows previously obtained and multiply each them with 
			//corresponding x[row][0] and sum them
			for(unsigned i = 0; i<data_table.m_rows; i++)
			{
				//hmm this is also a dot product
				accumulator += m_deviation[i] * data_table[i][j];
			}

			m_theta[j] = m_theta[j] - m_alpha * (1.0f/data_table.m_rows) * accumulator;
			//if(m_theta[j] < 0.000000001f) m_theta[j] = 0; 
		}
	} while((m_previousError - m_error) > threshold || (m_error > threshold)); //just some random small number

	/*
	   {//error synchronization, please ignore
	   m_previousError = m_error;
	   m_error = 0;
	   std::cout<<"Previous Error:"<<m_previousError<<"\nFinal Error: ";
	//Because of the way I calculate stuff, the error 
	//value in m_error is actually the error value of 
	//the previous model so gotta do this again:
	for(unsigned i = 0; i<data_table.m_rows; i++)
	{
	double y_prediction = 0;
	for(unsigned j = 0; j<data_table.m_xColumns; j++)
	{
	y_prediction += m_theta[j] * data_table[i][j];
	}
	m_deviation[i] = y_prediction - data_table[i][data_table.m_xColumns + mapToOutput];
	//common term = y_prediction - y_actual^

	m_error += m_deviation[i] * m_deviation[i];
	}
	m_error /= (2.0f*data_table.m_rows);
	std::cout<<m_error<<'\n';
	}*/

	showTheta();
	data_table.showData();
}


//console output stuff
void coutFormatter(); //forward declaration
template<typename thetaT>
void hypothesis<thetaT>::showTheta()
{
	std::cout<<"   Theta   |";
	//printing the thetas
	for(unsigned j = 0; j<m_thetaSize; j++)
	{
		coutFormatter();
		std::cout<<m_theta[j]<<'|';
	}
	std::cout<<'\n';

	for(unsigned j = 0; j<m_thetaSize + 1; j++)
	{
		std::cout<<"------------";//11 _s + 1 _ because of |
	}
	std::cout<<"-\n";
}
