Uhh idk how start this.

Soo this is what the cool kids would call a multivariable linear regression model trainer. Written in pure cpp of course. It can train a n parameter linear regression model based on the data provided.
The project provides two self-contained header files dataTable.hpp and hypothesis.hpp, providing two classes; dataTable and hypothesis.

1. dataTable class
I made this class mainly so I could make a few assumptions on the structure of the data while I was writing the hypothesis class. So if you want to use the hypothesis class(which is the main star of the show) you must conform your data to the structure of this table. 
Usage:
dataTable<data_type_of_data, number_of_rows, number_of_inputs, number_of_outputs> table;

2. hypothesis class
As discussed, the star of the show, simply define your data table and pass it to the hypothesis object's .train() method and watch it spit out the nth dimensional linear model that fits your data.

Usage:
hypothesis<data_type_for_theta> model;
model.train(table);
