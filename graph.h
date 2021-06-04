#ifndef _GRAPH_H_CC_
#define _GRAPH_H_CC_

#include <iostream>
#include "matrix.h"

template <typename T = double>
class Graph;

template <typename T>
std::ostream &operator<<(std::ostream &output, const Graph<T> &graph);

template <typename T>
class Graph
{
public:
	struct vertex;
	struct edge;

	struct vertex
	{
		int id;
		struct edge *link;
	};

	struct edge
	{
		T value;
		struct vertex *vertex;
		struct edge *next;
	};

	typedef struct vertex VertexNode;
	typedef struct edge EdgeNode;

	Graph(const Matrix<T> &adj_table);
	Graph(const Graph &graph);
	~Graph();

	Matrix<bool> get_adj_table() const;

	friend std::ostream &operator<<<>(std::ostream &output, const Graph &graph);
	inline unsigned int v_num() const { return this->n_ver; }
	unsigned int edge_num() const;

	bool is_cyclic() const;
	bool is_directed() const;

private:
	VertexNode *adj_list;
	unsigned int n_ver;
};

template <typename T>
Graph<T>::Graph(const Matrix<T> &adj_table)
{
	if (!adj_table.is_square())
		exit(-1);

	this->n_ver = adj_table.size().n_row;
	this->adj_list = new Graph<T>::VertexNode[this->n_ver];
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::VertexNode vertex;
		vertex.id = i;
		vertex.link = nullptr;
		adj_list[i] = vertex;
	}

	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::EdgeNode *current = nullptr;
		for (unsigned int j = 0; j < this->n_ver; j++)
			if (adj_table[i][j])
			{
				Graph<T>::EdgeNode *node = new Graph<T>::EdgeNode({adj_table[i][j], &this->adj_list[j], nullptr});
				if (current == nullptr)
					this->adj_list[i].link = node;
				else
					current->next = node;
				current = node;
			}
	}
}

template <typename T>
Graph<T>::Graph(const Graph<T> &graph)
{
	Matrix<bool> adj_table = graph.get_adj_table();
	this->n_ver = adj_table.size().n_row;
	this->adj_list = new Graph<T>::VertexNode[this->n_ver];
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::VertexNode vertex;
		vertex.id = i;
		vertex.link = nullptr;
		adj_list[i] = vertex;
	}
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::EdgeNode *current = nullptr;
		for (unsigned int j = 0; j < this->n_ver; j++)
			if (adj_table[i][j])
			{
				Graph<T>::EdgeNode *node = new Graph<T>::EdgeNode({adj_table[i][j], &this->adj_list[j], nullptr});
				if (current == nullptr)
					this->adj_list[i].link = node;
				else
					current->next = node;
				current = node;
			}
	}
}

template <typename T>
Graph<T>::~Graph<T>()
{
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::EdgeNode *current = this->adj_list[i].link;
		while (current != nullptr)
		{
			Graph<T>::EdgeNode *next = (current->next);
			delete current;
			current = next;
		}
	}
	delete[] this->adj_list;
}

template <typename T>
Matrix<bool> Graph<T>::get_adj_table() const
{
	Matrix<bool> matrix = Matrix<bool>(this->n_ver) * false;
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		if (this->adj_list[i].link == nullptr)
			continue;

		Graph<T>::EdgeNode *current = this->adj_list[i].link;
		while (current != nullptr)
		{
			matrix[i][current->vertex->id] = current->value;
			current = current->next;
		}
	}
	return matrix;
}

template <typename T>
std::ostream &operator<<<>(std::ostream &output, const Graph<T> &graph)
{
	for (unsigned int i = 0; i < graph.n_ver; i++)
	{
		output << graph.adj_list[i].id << ":";

		typename Graph<T>::EdgeNode *current = graph.adj_list[i].link; //Ϊʲôһ��Ҫ��typename

		while (current != nullptr)
		{
			output << "->" << current->vertex->id;
			current = current->next;
		}
		output << std::endl;
	}
	return output;
}

template <typename T>
unsigned int Graph<T>::edge_num() const
{
	unsigned int n_egde = 0;
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::EdgeNode *current = this->adj_list[i].link;
		while (current != nullptr)
		{
			n_egde++;
			current = current->next;
		}
	}

	return n_egde;
}
#endif // !_GRAPH_H_CC_
