#pragma once
#include <vector>
#include <math.h>
#include "MyHeaps.h"
#include "float.h"
#include "stdint.h"

#define DIM 3
using namespace std;

typedef vector<double> Point;

#define ROOT 0

struct Node{
    double key;
    int LIdx;
	int	RIdx;
	int	pIdx;

    Node(){ LIdx=RIdx=key=pIdx=-1; }
    inline bool isLeaf() const{ return pIdx>=0; }
};

class KDTree{
	// k维的二叉树? https://zhuanlan.zhihu.com/p/112246942
public:
	KDTree(){}
	KDTree(const vector<Point>& points);
	~KDTree();
private:
	int build_recursively(vector< vector<int> >& sortidx, vector<char> &sidehelper, int dim);  
	void knn_search(const Point& Xq, double radis, int nodeIdx = 0, int dim = 0);
	void knn_search(const Point& Xq, std::vector<double> &radis, int nodeIdx = 0, int dim = 0);
	double bounds_overlap_ball(const Point& Xq);

public:
	inline int size(){ return points.size(); }
	inline int ndims(){ return ndim; }

	//	Xq: 特征点
	//	xs: 最近邻搜索结果
	//	dist: 最近邻点与特征点距离
	void closest_point(const Point &p, vector<double>& x, double &dist);

	//	Xq: 特征点
	//	k: 近邻数量
	//  radis: 球形半径
	//	xs: 近邻搜索结果
	//	distances: 近邻点与特征点距离
	void k_closest_points(const Point& Xq, int k, double radis, vector<vector<double>>& xs, vector<double>& distances);

	//	Xq: 特征点
	//	k: 近邻数量
	//  radis: 坐标轴偏移
	//	xs: 近邻搜索结果
	//	distances: 近邻点与特征点距离
	void k_closest_points(const Point& Xq, int k, vector<double>& radis, vector<vector<double>>& xs, vector<double>& distances);

	//	获取所有点集合
	vector<Point> all_points() { return this->points; }

private:
	int ndim;
	int npoints;
	vector<Point> points;
	vector<Node*> nodesPtrs;

	int k;
	Point Bmin;
	Point Bmax;
	MaxHeap<double> pq;
	bool terminate_search;
};