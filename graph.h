#ifndef _GRAPH_H_CC_
#define _GRAPH_H_CC_

#include <iostream>
#include <vector>
#include <queue>
#include "matrix.h"

template <typename T = double>
class Graph
{
public:
	struct vertex;
	struct arc;

	struct vertex
	{
		unsigned int id;
		struct arc *link;
	};

	struct arc
	{
		T weight;
		struct vertex *vertex;
		struct arc *next;
	};

	typedef struct vertex Vertex;
	typedef struct arc Arc;

	typedef struct edge
	{
		Vertex *begin;
		Vertex *end;
		T weight;

		bool operator<(const edge &e) const { return weight < e.weight; }
		bool operator>(const edge &e) const { return weight > e.weight; }
	} Edge;

	//���캯��
	Graph(const Matrix<T> &adj_table);
	//�������캯��
	Graph(const Graph &graph);
	//��������
	~Graph();

	Matrix<bool> get_adj_table() const;
	template <typename To>
	friend std::ostream &operator<<(std::ostream &output, const Graph<To> &graph);
	inline unsigned int v_num() const { return this->n_ver; }
	unsigned int edge_num() const;

	std::vector<Edge> get_edges() const;

	void kruskal() const;

private:
	Vertex *adj_list;
	unsigned int n_ver;
};

template <typename T>
Graph<T>::Graph(const Matrix<T> &adj_table)
{
	if (!adj_table.is_square())
		exit(-1);

	this->n_ver = adj_table.size().n_row;
	this->adj_list = new Vertex[this->n_ver];

	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Vertex vertex = {i, nullptr};
		adj_list[i] = vertex;
	}

	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Arc *current = nullptr;
		for (unsigned int j = 0; j < this->n_ver; j++)
			if (adj_table[i][j])
			{
				Arc *arc = new Arc{adj_table[i][j], &this->adj_list[j], nullptr};
				if (current == nullptr)
					this->adj_list[i].link = arc;
				else
					current->next = arc;
				current = arc;
			}
	}
}

template <typename T>
Graph<T>::Graph(const Graph<T> &graph)
{
	Matrix<bool> adj_table = graph.get_adj_table();
	this->n_ver = adj_table.size().n_row;
	this->adj_list = new Graph<T>::Vertex[this->n_ver];
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::Vertex vertex;
		vertex.id = i;
		vertex.link = nullptr;
		adj_list[i] = vertex;
	}
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Graph<T>::Arc *current = nullptr;
		for (unsigned int j = 0; j < this->n_ver; j++)
			if (adj_table[i][j])
			{
				Graph<T>::Arc *node = new Graph<T>::Arc{adj_table[i][j], &this->adj_list[j], nullptr};
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
		Graph<T>::Arc *current = this->adj_list[i].link;
		while (current != nullptr)
		{
			Graph<T>::Arc *next = (current->next);
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

		Arc *current = this->adj_list[i].link;
		while (current != nullptr)
		{
			matrix[i][current->vertex->id] = current->weight;
			current = current->next;
		}
	}
	return matrix;
}

template <typename To>
std::ostream &operator<<(std::ostream &output, const Graph<To> &graph)
{
	for (unsigned int i = 0; i < graph.n_ver; i++)
	{
		output << graph.adj_list[i].id << ":";

		typename Graph<To>::Arc *current = graph.adj_list[i].link; //Ϊʲôһ��Ҫ��typename

		while (current != nullptr)
		{
			output << "->" << current->vertex->id << ":" << current->weight;
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
		Graph<T>::Arc *current = this->adj_list[i].link;
		while (current != nullptr)
		{
			n_egde++;
			current = current->next;
		}
	}

	return n_egde;
}

template <typename T>
std::vector<typename Graph<T>::Edge> Graph<T>::get_edges() const
{
	std::vector<Edge> edges = std::vector<Edge>();
	for (unsigned int i = 0; i < this->n_ver; i++)
	{
		Arc *current = this->adj_list[i].link;
		while (current != nullptr)
		{
			Edge edge = {&this->adj_list[i], current->vertex, current->weight};
			edges.push_back(edge);
			current = current->next;
		}
	}

	return edges;
}

unsigned int find(std::vector<int> parent, unsigned int id)
{
	while (parent[id] >= 0)
		id = parent[id];
	return id;
}

template <typename T>
void Graph<T>::kruskal() const
{
	std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>>
		pq_edges{std::greater<Edge>(), this->get_edges()};
	std::vector<int> parent(this->n_ver, -1);

	for (unsigned int i = 0; i < this->edge_num(); i++)
	{
		Edge edge = pq_edges.top();
		unsigned int begin_id = find(parent, edge.begin->id);
		unsigned int end_id = find(parent, edge.end->id);

		if (begin_id != end_id)
		{
			parent[begin_id] = end_id;
			std::cout << "��С�ķ��ǣ�" << edge.begin->id + 1 << "��" << edge.end->id + 1 << std::endl;
		}
		pq_edges.pop();
	}
}

template <typename T>
Graph<T> ug_input()
{
	unsigned int n_ver, n_edge;
	Matrix<T> mat;

	std::cout << "����������ͼ�Ķ�����Ŀ�ͱߵ���Ŀ(�Կո�ָ�����������������0����):";

	std::cin.clear();
	std::cin >> n_ver >> n_edge;

	if (n_ver != 0 && n_edge != 0)
	{
		mat = Matrix<T>(n_ver) * 0;

		std::cout << "�����������ߵĽڵ�����Լ����ǵ�Ȩֵ(�Կո�ָ�������):" << std::endl;
		while (n_edge--) //�������б����Լ����ǵ�Ȩֵ
		{
			unsigned int u, v;
			T w;
			std::cin.clear();
			std::cin >> u >> v >> w; //��������Լ����ǵ�Ȩֵ
			u--, v--;

			mat[u][v] = mat[v][u] = w;
		}
	}
	else if (n_ver == 0 || n_edge == 0)
		exit(-1);

	return Graph<T>(mat);
}

#endif // !_GRAPH_H_CC_