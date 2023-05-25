#pragma once
#include "KDtree.h"
#include "NodeParaters.h"

typedef std::function<bool (int , int , const char* )> PROGRESS_CB;

class Interpolation{
public:
	Interpolation();
	virtual ~Interpolation();

	void set_callback(PROGRESS_CB pcb) {
		progress_cb= pcb;
	}

private:
	// 解方程组ax=b,n为未知数个数,a为n*n矩阵,b为n*1向量
	// 全主元消元法 
	int agaus(double *a, double *b, int n);

	// 解方程组ax=b,n为未知数个数,a为n*n矩阵,b为n*1向量
	// 列主元消元法 
	int gausj(double *a, double *b, int n);

	// 解方程组ax=b,n为未知数个数,a为n*n矩阵,b为n*1向量
	// LU分解 非PLU
	int LUSolve(double *a, double *b, int n);

public:
	void setKDtree(KDTree *tree) {
		m_tree = tree;
	}

	//球形搜索
	void setSearchParameter(double radius, int minNum, int maxNum) {
		m_searchType = 1;
		m_radius = radius;
		m_minNum = minNum;
		m_maxNum = maxNum;
	}

	//各向异性 double* x，y，z方向半径
	void setSearchParameter(std::vector<double> radius, int minNum, int maxNum) {
		m_searchType = 2;
		m_radius_t = radius;
		m_minNum = minNum;
		m_maxNum = maxNum;
	}

	//所有数据
	void setSearchParameter() {
		m_searchType = 0;
	}

	// double *range所有特征点的Xmin,Ymin,Xmax,Ymax,Zmax,Zmax   
	// int mode, 计算半方差矩阵的模式 1球 2指数 3高斯 三种情况   
	// int *resol,x方向y方向z方向的分辨率。即，resol_x为x方向的间隔数；resol_y为y方向的间隔数,resol_z为z方向的间隔数最后总点数为：（resol_x+1）*（resol_y+1）*（resol_z+1）    
	// double c0块金, float c1基台值, float a变程 三个常量参数   
	// double *result 输出结果
	bool kriging(double *range, int mode, int *resol, double c0, double c1, double a, double *result, std::vector<pointFinal> & outputData);

	// double *range所有特征点的Xmin,Ymin,Xmax,Ymax,Zmax,Zmax   
	// int order, 多项式阶数 1 2 3三种情况   
	// double index 加权函数指数 
	// int *resol,x方向y方向z方向的分辨率。即，resol_x为x方向的间隔数；resol_y为y方向的间隔数,resol_z为z方向的间隔数最后总点数为：（resol_x+1）*（resol_y+1）*（resol_z+1）   
	// double *result 输出结果
	bool LocalPolynomialInterpolation(double *range, int order, double index, int *resol, double *result);

	// double *range所有特征点的Xmin,Ymin,Xmax,Ymax,Zmax,Zmax   
	// int order,阶数 1 2 3三种情况   
	// double smooth 平滑系数  
	// int *resol,x方向y方向z方向的分辨率。即，resol_x为x方向的间隔数；resol_y为y方向的间隔数,resol_z为z方向的间隔数最后总点数为：（resol_x+1）*（resol_y+1）*（resol_z+1）   
	// double *result 输出结果
	bool idw(double *range, int order, double smooth, int *resol, double *result, std::vector<pointFinal> & outputData);

private:
	KDTree *m_tree;
	double m_radius;
	std::vector<double> m_radius_t;
	int m_minNum;
	int m_maxNum;
	int m_searchType;
	PROGRESS_CB progress_cb;
};
