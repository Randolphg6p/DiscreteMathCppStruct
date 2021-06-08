#ifndef _MATRIX_H_CC_
#define _MATRIX_H_CC_

#include<iostream>
#include<utility>

template<typename Tm = double>
class Matrix
{
	typedef Tm** MatrixArray;
	typedef struct {
		unsigned int n_row;
		unsigned int n_col;
	}Size;

public:

	//���캯��
	Matrix(unsigned int n_row = 0, unsigned int n_col = 0);
	//�������캯��
	Matrix(const Matrix& mat);
	//��������
	~Matrix();

	// ���������
	Matrix& operator=(const Matrix&);
	inline Tm* operator[](const int& n)const { return this->array[n]; }
	template <typename To>
	friend std::ostream& operator<< (std::ostream& output, const Matrix<To>& mat);
	//����
	Matrix operator*(const Tm& value)const;
	inline friend Matrix operator*(const Tm& value, const Matrix& mat) { return mat * value; }
	//�˷�
	Matrix operator*(const Matrix& mat)const;
	//�ӷ�
	Matrix operator+(const Matrix& mat)const;
	//����
	inline Matrix operator-() const { return *this * -1; }
	//����
	inline Matrix operator-(const Matrix& mat) const { return *this + (-mat); }

	inline Size size()const { return Size{ this->n_row,this->n_col }; }

	inline bool is_square()const { return this->n_col == this->n_row; }

	Matrix T() const;

	//�Է��հ�
	Matrix ref_clo()const;
	//���ݱհ�
	Matrix tran_clo()const;
	//�ԳƱհ�
	inline Matrix sym_clo()const { return *this + this->T(); }

private:
	MatrixArray array;
	unsigned int n_row;
	unsigned int n_col;
};

template<typename Tm>
Matrix<Tm>::Matrix(unsigned int n_row, unsigned int n_col)
{
	if (n_row == 0)
	{
		n_row = 1;
		n_col = 1;
	}
	else if (n_col == 0)
		n_col = n_row;

	this->n_col = n_col;
	this->n_row = n_row;

	this->array = new Tm * [this->n_row];

	for (unsigned int i = 0; i < this->n_row; i++)
	{
		this->array[i] = new Tm[this->n_col];
		for (unsigned int j = 0; j < this->n_col; j++)
			this->array[i][j] = (Tm)1;
	}

}

template<typename Tm>
Matrix<Tm>::Matrix(const Matrix<Tm>& mat)
{
	this->n_col = mat.n_col;
	this->n_row = mat.n_row;

	this->array = new Tm * [this->n_row];

	for (unsigned int i = 0; i < this->n_row; i++)
	{
		this->array[i] = new Tm[this->n_col];
		for (unsigned int j = 0; j < this->n_col; j++)
			this->array[i][j] = mat.array[i][j];
	}
}

template<typename Tm>
Matrix<Tm>::~Matrix()
{
	for (unsigned int i = 0; i < this->n_row; i++)
		delete[] this->array[i];

	delete[] this->array;
}

template<typename Tm>
Matrix<Tm>& Matrix<Tm>::operator=(const Matrix<Tm>& mat)
{
	this->~Matrix<Tm>();

	this->n_col = mat.n_col;
	this->n_row = mat.n_row;

	this->array = new Tm * [this->n_row];

	for (unsigned int i = 0; i < this->n_row; i++)
	{
		this->array[i] = new Tm[this->n_col];
		for (unsigned int j = 0; j < this->n_col; j++)
			this->array[i][j] = mat.array[i][j];
	}

	return *this;
}

template<typename Tm>
Matrix<Tm> Matrix<Tm>::operator*(const Matrix<Tm>& mat)const
{
	Matrix<Tm> product = Matrix<Tm>(this->n_row, mat.n_col);
	for (unsigned int i = 0; i < this->n_row; i++)
		for (unsigned int j = 0; j < mat.n_col; j++)
		{
			int e = 0;
			for (unsigned k = 0; k < this->n_col; k++)
				e += this->array[i][k] * mat.array[k][j];
			product.array[i][j] = e;
		}
	return product;
}

template<typename Tm>
Matrix<Tm> Matrix<Tm>::operator*(const Tm& num)const
{
	Matrix<Tm> sm = Matrix<Tm>(this->n_row, this->n_col);
	for (unsigned int i = 0; i < n_row; i++)
		for (unsigned int j = 0; j < n_col; j++)
			sm.array[i][j] = this->array[i][j] * num;
	return sm;
}

template<typename Tm>
Matrix<Tm> Matrix<Tm>::operator+(const Matrix<Tm>& mat)const
{
	Matrix<Tm> addition = Matrix<Tm>(this->n_row, this->n_col);
	for (unsigned int i = 0; i < n_row; i++)
		for (unsigned int j = 0; j < n_col; j++)
			addition.array[i][j] = this->array[i][j] + mat.array[i][j];
	return addition;
}

template<typename To>
std::ostream& operator<<(std::ostream& output, const Matrix<To>& mat)
{
	for (unsigned int i = 0; i < mat.n_row; i++)
	{
		for (unsigned int j = 0; j < mat.n_col; j++)

			output << mat.array[i][j] << " ";
		if (i < mat.n_row - 1)
			output << std::endl;
	}

	return output;
}

template<typename Tmi>
Matrix<Tmi> matrix_input()
{
	unsigned int n_row, n_col;

	std::cout << "��������������:";
	std::cin.clear();
	std::cin >> n_row;
	std::cout << "��������������:";
	std::cin.clear();
	std::cin >> n_col;

	Matrix<Tmi> mat = Matrix<Tmi>(n_row, n_col);

	std::cout << "���������:" << std::endl;
	for (unsigned int i = 0; i < n_row; i++)
	{
		std::cout << std::endl;
		std::cout << "���������ĵ�" << i << "��Ԫ��(Ԫ���Կո�ָ�) :";
		std::cin.clear();
		for (unsigned int j = 0; j < n_col; j++)
		{
			std::cin >> mat[i][j];
		}
	}

	return mat;
}

template<typename Tm>
Matrix<Tm> Matrix<Tm>::T()const
{
	Matrix<Tm> Tmat = Matrix<Tm>(this->n_col, this->n_row);

	for (unsigned int i = 0; i < this->n_row; i++)
		for (unsigned int j = 0; j < this->n_col; j++)
			Tmat[j][i] = this->array[i][j];

	return Tmat;
}

template<typename Tm>
Matrix<Tm> Matrix<Tm>::ref_clo()const
{
	Matrix<Tm> ref_mat = *this;

	for (unsigned int i = 0; i < ref_mat.n_row; i++)
		ref_mat[i][i] = Tm(1);

	return ref_mat;
}

template<typename Tm>
Matrix<Tm> Matrix<Tm>::tran_clo()const
{
	Matrix<Tm> tran_mat = *this;

	for (unsigned i = 0; i < tran_mat.n_row; i++)
		for (unsigned j = 0; j < tran_mat.n_col; j++)
			if (tran_mat[j][i])

				for (unsigned int k = 0; k < tran_mat.n_row; k++)
					tran_mat[j][k] = tran_mat[j][k] + tran_mat[i][k];

	return tran_mat;
}
#endif