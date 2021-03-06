#include <iostream>
#include "matrix.h"
#include "graph.h"
using namespace std;

void test_graph();
void test_matrix();

int main()
{
    test_matrix();
    test_graph();

    return 0;
}

void test_matrix()
{
    Matrix<int> mat = Matrix<int>(3, 3);
    cout << mat.T() << endl;
}

void test_graph()
{
    Graph<int> graph = Graph<int>(Matrix<int>(5) * 4);
    cout << graph << endl;
}