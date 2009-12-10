#include<iostream>
#include<vector>

using namespace std;

template <class T>
class Array2D
{
	vector< vector<T> > array2d;
	int col;
	int row;
public:
	// * Construtor;
	Array2D(int row_, int col_): array2d(row_,vector<T>(col_))
	{
		col = col_;
		row = row_;
	}

	~Array2D()
	{

	}

	vector<T> & operator[](int index){
		return array2d[index];
	}

	const vector<T> & operator[](int index) const{
		return array2d[index];
	}

	// * Redimensiona o array;
	void resizeArray(int rows, int cols){
		array2d.resize(rows);
		for (int i=0; i<rows; i++)
			array2d[i].resize(cols);
	}

	// * Retorna a quantidade de linhas do Array;
	int countRow(){
		return row;
	}

	// * Retorna a quantidade de colunas do Array;
	int countCol(){
		return col;
	}
};



template <class T>
class Array3D
{
	vector< vector <vector <T> > > array3d;
	int col;
	int row;
	int depht;
public:

	// * Construtor
	Array3D(int row_, int col_, int dep_)
	{
		col = col_;
		row = row_;
		depht = dep_;
		array3d.resize(row_);
		for (int i=0; i<row_; i++)
		{
			array3d[i].resize(col_);
			for (int j=0; j<col_; j++){
				array3d[i][j].resize(dep_);
			}
		}
	}

	vector <vector<T> > & operator[](int index){
		return array3d[index];
	}

	const vector< vector<T> > & operator[](int index) const{
		return array3d[index];
	}

	// * Retorna a quantidade de linhas do Array;
	int countRow(){
		return row;
	}

	// * Retorna a quantidade de colunas do Array;
	int countCol(){
		return col;
	}

	// * Return the depht of the Array;
	int countDepht(){
		return depht;
	}


};

