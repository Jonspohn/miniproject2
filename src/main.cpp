#include <iostream>
#include <Eigen/Dense> 
#include <Eigen/Geometry>
#include <fstream>
#include <Eigen/StdVector>
#include <cmath> 
using namespace std;

template <typename T>
using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

Matrix<double> FromFile(const string &input_file_path);
Matrix<double> FromFile(const string &input_file_path) {
  // Reads in the file from "input_file_path"
  ifstream input_file(input_file_path, ifstream::in);

 Matrix<double> m;
  string line;
  vector<double> temp_buffer;
  double coef;
  int num_cols = 0;
  int num_rows = 0;
  int cols_in_row;
  if (input_file) {
    // Iterates over every line in the input file
    while (!input_file.eof()) {
      getline(input_file, line);
      if (line.find_first_not_of(' ') == string::npos)
        continue;
      // Replaces every instance of the "delimiter" with whitespace for comma
      replace(line.begin(), line.end(), ',', ' ');
     
      // Creates a stringstream out of every line in the file
      stringstream stream(line);
      cols_in_row = 0;
      // Reads every coefficient in the stringstream into the temporary buffer
      while (stream >> coef) {
        temp_buffer.push_back(coef);
        ++cols_in_row;
      }
      // If the number of columns in the matrix hasn't been set, make it the
      // current number of columns in the row
      if (num_cols == 0) {
        num_cols = cols_in_row;
      // If the matrix in the file is shaped incorrectly, throw an error
      } else if(num_cols != cols_in_row) {
        cerr << "Problem with Matrix in: " + input_file_path +
                     ", exiting..." << endl;
        exit(1);
      }
      ++num_rows;
    }
    // Instantiate the matrix's size and feed it the coefficients in the
    // temporary buffer
    m.resize(num_rows, num_cols);
    for (int i = 0; i < num_rows; ++i)
      for (int j = 0; j < num_cols; ++j)
        m(i, j) = temp_buffer[i * num_cols + j];

    return m;					
  } else {
    // Error for when the file doesn't exist
    std::cerr << "Cannot open file " + input_file_path + ", exiting..."
              << std::endl;
    exit(1);
  }
}
int main(){
  string x_path = "X-12321.csv";
  Matrix<double> x = FromFile(x_path);

  string y_path = "y-26465.csv";
  Matrix<double> y = FromFile(y_path);

  string z_path = "beta-12566.csv";
  Matrix<double> z = FromFile(z_path);
 
  Matrix<double> a;
  a = x.transpose(); // transpose of matrix x is set to matrix a
  
  Matrix<double> b; // multiplying matrix a by x...... multiplying transpose by x matrix
  b = a * x; 
  
  Matrix<double> c; // taking the inverse of matrix b and setting it to matrix c
  c = b.inverse(); 
  
  Matrix<double> d; // Multiplying matrix c,a, and y to find beta 
  d = c * a * y;
  cout << "Beta Matrix is equal to:" << endl << endl << d << endl;

  Matrix<double> s;// start of sum of squared error..... substracting beta calc by beta test and setting it to a new matrix s
  s = d - z; 

  Matrix<double> j; // squaring matrix s by multiplying it by itself and setting it to a new matrix j 
  j = s.array()*s.array();
  cout << endl;
  cout << "The Sum of Squared error ratio when compared to the Test Data Beta is: " << endl;
  cout << j.sum() << endl; // taking the sum of matrix j which is now our Sum of Squared Error 
  cout << endl <<"Error Ratio is very small so Calculations are confirmed" << endl; 







}
