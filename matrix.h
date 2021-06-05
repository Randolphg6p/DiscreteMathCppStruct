/**
 * @file matrix.h
 * @author Chen Zhaorui (chenzhaorui@qq.com)
 * @brief A matrix template class
 * @version 0.1
 * @date 2021-06-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _MATRIX_H_CC_
#define _MATRIX_H_CC_

#include <iostream>

template <typename T = double>
class Matrix;

template <typename T>
std::ostream &operator<<(std::ostream &output, const Matrix<T> &mat);

template <typename T>
Matrix<T> matrix_input();

template <typename T>
class Matrix
{
	typedef T **MatrixArray;
	typedef struct
	{
		unsigned int n_row;
		unsigned int n_col;
	} Size;

public:
	//���캯��
	Matrix(unsigned int n_row = 0, unsigned int n_col = 0);
	//�������캯��
	Matrix(const Matrix &mat);
	//��������
	~Matrix();

	// ���������
	Matrix &operator=(const Matrix &);
	inline T *operator[](const int &n) const { return this->array[n]; }
	friend std::ostream &operator<<<>(std::ostream &output, const Matrix &mat);
	//����
	Matrix operator*(const T &value) const;
	inline friend Matrix operator*(const T &value, const Matrix &mat) { return mat * value; }
	//�˷�
	Matrix operator*(const Matrix &mat) const;
	//�ӷ�
	Matrix operator+(const Matrix &mat) const;
	//����
	inline Matrix operator-() const { return *this * -1; }
	//����
	inline Matrix operator-(const Matrix &mat) const { return *this + (-mat); }

	inline Size size() const { return Size{this->n_row, this->n_col}; }

	inline bool is_square() const { return this->n_col == this->n_row; }

	//�Է��հ�
	Matrix ref_clo() const;
	//���ݱհ�
	Matrix tran_clo() const;
	//�ԳƱհ�
	Matrix sym_clo() const;

private:
	MatrixArray array;
	unsigned int n_row;
	unsigned int n_col;
};

template <typename T>
Matrix<T>::Matrix(unsigned int n_row, unsigned int n_col)
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

	this->array = new T *[this->n_row];

	for (unsigned int i = 0; i < this->n_row; i++)
	{
		this->array[i] = new T[this->n_col];
		for (unsigned int j = 0; j < this->n_col; j++)
			this->array[i][j] = (T)1;
	}
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &mat)
{
	this->n_col = mat.n_col;
	this->n_row = mat.n_row;

	this->array = new T *[this->n_row];

	for (unsigned int i = 0; i < this->n_row; i++)
	{
		this->array[i] = new T[this->n_col];
		for (unsigned int j = 0; j < this->n_col; j++)
			this->array[i][j] = mat.array[i][j];
	}
}

template <typename T>
Matrix<T>::~Matrix()
{
	for (unsigned int i = 0; i < this->n_row; i++)
		delete[] this->array[i];

	delete[] this->array;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &mat)
{
	this->~Matrix<T>();

	this->n_col = mat.n_col;
	this->n_row = mat.n_row;

	this->array = new T *[this->n_row];

	for (unsigned int i = 0; i < this->n_row; i++)
	{
		this->array[i] = new T[this->n_col];
		for (unsigned int j = 0; j < this->n_col; j++)
			this->array[i][j] = mat.array[i][j];
	}

	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &mat) const
{
	Matrix<T> product = Matrix<T>(this->n_row, mat.n_col);
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

template <typename T>
Matrix<T> Matrix<T>::operator*(const T &num) const
{
	Matrix<T> sm = Matrix<T>(this->n_row, this->n_col);
	for (unsigned int i = 0; i < n_row; i++)
		for (unsigned int j = 0; j < n_col; j++)
			sm.array[i][j] = this->array[i][j] * num;
	return sm;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &mat) const
{
	Matrix<T> addition = Matrix<T>(this->n_row, this->n_col);
	for (unsigned int i = 0; i < n_row; i++)
		for (unsigned int j = 0; j < n_col; j++)
			addition.array[i][j] = this->array[i][j] + mat.array[i][j];
	return addition;
}

template <typename T>
std::ostream &operator<<<>(std::ostream &output, const Matrix<T> &mat)
{
	output << "[";
	for (unsigned int i = 0; i < mat.n_row; i++)
	{
		if (i > 0)
			output << " [";
		else
			output << "[";
		for (unsigned int j = 0; j < mat.n_col; j++)
			if (j < mat.n_col - 1)
				output << mat.array[i][j] << ",";
			else
				output << mat.array[i][j];
		if (i < mat.n_row - 1)
			output << "]" << std::endl;
		else
			output << "]";
	}
	output << "]";

	return output;
}

template <typename T>
Matrix<T> matrix_input()
{
	unsigned int n_row, n_col;

	std::cout << "Input matrix rows:";
	std::cin.clear();
	std::cin >> n_row;
	std::cout << "Input matrix columns:" std::cin.clear();
	std::cin >> n_col;

	Matrix<T> mat = Matrix<T>(n_row, n_col);

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

template <typename T>
Matrix<T> Matrix<T>::ref_clo() const
{
	Matrix<T> ref_mat = *this;

	for (unsigned int i = 0; i < ref_mat.n_row; i++)
		ref_mat[i][i] = T(1);

	return ref_mat;
}

template <typename T>
Matrix<T> Matrix<T>::sym_clo() const
{
	Matrix<T> sym_mat = *this;

	for (unsigned int i = 0; i < sym_mat.n_row; i++)
		for (unsigned int j = 0; j < sym_mat.n_col; j++)
			if (sym_mat[i][j])
				sym_mat[j][i] = sym_mat[i][j];

	return sym_mat;
}

template <typename T>
Matrix<T> Matrix<T>::tran_clo() const
{
	Matrix<T> tran_mat = *this;

	for (unsigned i = 0; i < tran_mat.n_row; i++)
		for (unsigned j = 0; j < tran_mat.n_col; j++)
			if (tran_mat[j][i])

				for (unsigned int k = 0; k < tran_mat.n_row; k++)
					tran_mat[j][k] = tran_mat[j][k] + tran_mat[i][k];

	return tran_mat;
}
#endif