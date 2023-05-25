#include <math.h>
#include "Interpolation.h"
#include <iostream>
#include <ctime>
#include <assert.h>

Interpolation::Interpolation()
{
	m_radius = 0;
	m_maxNum = 0;
	m_radius_t = std::vector<double>();
}

Interpolation::~Interpolation()
{

}

int Interpolation::agaus(double *a, double *b, int n) {

	int i, j, k;
	int *indexj = new int[n];
	for (k = 0; k < n - 1; k++) {
		double max = 0;
		int maxi = k;
		int maxj = k;
		for (i = k + 1; i < n; ++i)
			for (j = k + 1; j < n; ++j)
				if (fabs(a[i*n + j] > max)) {
					max = a[i*n + j];
					maxi = i;
					maxj = j;
					indexj[k] = j;
				}
		if (!max) {
			delete[] indexj;
			return 0;
		}
		if (maxi != k) { 
			for (j = k; j < n; ++j) {
				double temp = a[k*n + j];
				a[k*n + j] = a[maxi*n + j];
				a[maxi*n + j] = temp;
			}
			double temp = b[k];
			b[k] = b[maxi];
			b[maxi] = temp;
		}
		if (maxj != k) {
			for (i = 0; i < n; ++i) {
				double temp = a[i*n + k];
				a[i*n + k] = a[i*n + maxj];
				a[i*n + maxj] = temp;
			}
		}
		for (i = k + 1; i < n; ++i) {
			double d = a[i*n + k] / a[k*n + k];
			b[i] -= b[k] * d;
			for (j = k + 1; j < n; ++j) {
				a[i*n + j] -= a[k*n + j] * d;
			}
		}
	}
	if (!a[n*n - 1]) {
		delete[] indexj;
		return 0;
	}
	for (i = n - 1; i >= 0; --i) {
		double sum = 0;
		for (j = n - 1; j > i; --j) {
			sum += a[i*n + j] * b[j];
		}
		b[i] = (b[i] - sum) / a[i*n + i];
	}
	indexj[n - 1] = n - 1;
	for (k = n - 1; k >= 0; --k) {
		if (indexj[k] != k) {
			double temp = b[k];
			b[k] = b[indexj[k]];
			b[indexj[k]] = temp;
		}
	}
	return 1;
}

int Interpolation::gausj(double * a, double * b, int n)
{
	int i, j, k;
	for (k = 0; k < n - 1; ++k) {
		double max = 0;
		int maxi = k;
		for (i = k + 1; i < n; ++i)
			if (fabs(a[i*n + k]) > max) {
				max = a[i*n + k];
				maxi = i;
			}
		if (!max)
			return 0;
		if (maxi != k) {
			for (j = k; j < n; ++j) {
				double temp = a[k*n + j];
				a[k*n + j] = a[maxi*n + j];
				a[maxi*n + j] = temp;
			}
			double temp = b[k];
			b[k] = b[maxi];
			b[maxi] = temp;
		}
		for (i = k + 1; i < n; ++i) {
			double d = a[i*n + k] / a[k*n + k];
			b[i] -= b[k] * d;
			a[i*n + k] = 0;
			for (j = k + 1; j < n; ++j) {
				a[i*n + j] -= a[k*n + j] * d;
			}
		}
	}
	if (!a[n*n - 1]) {
		return 0;
	}
	for (i = n - 1; i >= 0; --i) {
		double sum = 0;
		for (j = n - 1; j > i; --j) {
			sum += a[i*n + j] * b[j];
		}
		b[i] = (b[i] - sum) / a[i*n + i];
	}
	return 1;
}

int Interpolation::LUSolve(double * a, double * b, int n)
{
	int i, j, k;
	for (k = 1; k < n; ++k)
		a[k*n] /= a[0];
	for (k = 1; k < n; ++k) {
		if (!a[k*n + k])
			return 0;
		for (j = k; j < n; ++j) {
			double sum = 0;
			for (i = 0; i < k; ++i) {
				sum += a[i*n + j] * a[k*n + i];
			}
			a[k*n + j] -= sum;
		}
		for (i = k + 1; i < n; ++i) {
			double sum = 0;
			for (j = 0; j < k; ++j) {
				sum += a[i*n + j] * a[j*n + k];
			}
			a[i*n + k] = (a[i*n + k] - sum) / a[k*n + k];
		}
	}
	for (i = 0; i < n; ++i) {
		double sum = b[0];
		for (j = 0; j < i; ++j) {
			sum += a[i*n + j] * b[j];
		}
		b[i] = sum;
	}
	if (!a[n*n - 1]) {
		return 0;
	}
	for (i = n - 1; i >= 0; --i) {
		double sum = 0;
		for (j = n - 1; j > i; --j) {
			sum += a[i*n + j] * b[j];
		}
		b[i] = (b[i] - sum) / a[i*n + i];
	}
	return 1;
}

bool Interpolation::kriging(double *range, int mode, int *resol, double c0, double c1, double a, double *result, std::vector<pointFinal> & outputData) {

	double begin_row = *(range);
	double begin_col = *(range + 1);
	double begin_high = *(range + 2);
	double end_row = *(range + 3);
	double end_col = *(range + 4);
	double end_high = *(range + 5);

	int resol_x = *(resol);
	int resol_y = *(resol + 1);
	int resol_z = *(resol + 2);

	double cnt_x = (end_row - begin_row) / (double)resol_x;
	double cnt_y = (end_col - begin_col) / (double)resol_y;
	double cnt_z = (end_high - begin_high) / (double)resol_z;

	bool stop = progress_cb(resol_x* resol_y, 0, "");

	int count = 0;
	double lastResult = 0;
	for (int i = 0; i < resol_x; i++) {
		double i_f = cnt_x*i + begin_row;
		for (int j = 0; j < resol_y; j++) {
			//progress_cb(resol_x* resol_y, i*resol_x + j, "");
			double j_f = cnt_y*j + begin_col;
			for (int k = 0; k < resol_z; k++) {
				double k_f = cnt_z*k + begin_high;

				std::vector<double> itemPoint = { i_f,j_f,k_f };
				std::vector<std::vector<double>> searchPoints;
				std::vector<double> disVec;
				if (m_searchType == 0)
					searchPoints = m_tree->all_points();
				if (m_searchType == 1)
					m_tree->k_closest_points(itemPoint, m_maxNum, m_radius, searchPoints, disVec);
				if (m_searchType == 2)
					m_tree->k_closest_points(itemPoint, m_maxNum, m_radius_t, searchPoints, disVec);

				if (searchPoints.size() < m_minNum)
				{
					*(result + count) = NAN;
					outputData.push_back(pointFinal(i_f, j_f, k_f, NAN));
					count++;
					continue;
				}
					
				int dim = searchPoints.size() + 1;

				double* V = new double[dim * dim];
				double* D = new double[dim];
				double* Cd = new double[dim * dim];

				double distance;
				for (int i = 0; i< dim - 1; i++)
					for (int j = i; j < dim - 1; j++) {
						distance = (searchPoints[i][0] - searchPoints[j][0])*(searchPoints[i][0] - searchPoints[j][0]) + (searchPoints[i][1] - searchPoints[j][1])*(searchPoints[i][1] - searchPoints[j][1]) + (searchPoints[i][2] - searchPoints[j][2])*(searchPoints[i][2] - searchPoints[j][2]);
						Cd[i*dim + j] = sqrt(distance);
					}

				for (int i = 0; i< dim - 1; i++) {
					V[i*dim + dim - 1] = 1;
					V[(dim - 1)*(dim)+i] = 1;
				}

				V[dim*dim - 1] = 0;

				for (int i = 0; i< dim - 1; i++)
					for (int j = i; j < dim - 1; j++) {
						switch (mode) {
						case 1:
							if (Cd[i*dim + j] < a)
								V[i*dim + j] = V[j*dim + i] = (double)(c0 + c1*(1.5*Cd[i*dim + j] / a - 0.5*(Cd[i*dim + j] / a)*(Cd[i*dim + j] / a)*(Cd[i*dim + j] / a)));
							else
								V[i*dim + j] = V[j*dim + i] = c0 + c1;
							break;
						case 2:
							V[i*dim + j] = V[j*dim + i] = (double)(c0 + c1 *(1 - exp(-3 * Cd[i*dim + j] / a)));
							break;
						case 3:
							V[i*dim + j] = V[j*dim + i] = (double)(c0 + c1 *(1 - exp(-3 * Cd[i*dim + j] * Cd[i*dim + j] / a / a)));
							break;
						default:
							V[i*dim + j] = V[j*dim + i] = (double)(1 * Cd[i*dim + j]);
							break;
						}
					}

				delete[] Cd;

				double* temp = new double[dim * dim];
				for (int i = 0; i<dim*dim; i++)
					temp[i] = V[i];

				for (int m = 0; m < dim - 1; m++) {
					distance = (i_f - searchPoints[m][0]) *(i_f - searchPoints[m][0]) + (j_f - searchPoints[m][1])*(j_f - searchPoints[m][1]) + (k_f - searchPoints[m][2])*(k_f - searchPoints[m][2]);
					distance = sqrt(distance);
					switch (mode) {
					case 1:
						if (distance < a)
							D[m] = (double)(c0 + c1*(1.5*distance / a - 0.5*(distance / a)*(distance / a)*(distance / a)));
						else
							D[m] = c0 + c1;
						break;
					case 2:
						D[m] = (double)(c0 + c1 *(1 - exp(-3 * distance / a)));
						break;
					case 3:
						D[m] = (double)(c0 + c1 *(1 - exp(-3 * distance*distance / a / a)));
						break;
					default: D[m] = (double)(1 * distance);
						break;
					}
				}

				D[dim - 1] = 1;

				for (int m = 0; m < dim*dim; m++)
					V[m] = temp[m];

				//int res = agaus(V, D, dim);
				int res = gausj(V, D, dim);
				//CUDASolve(V, D, dim);

				if (!res) {
					*(result + count) = lastResult;
					outputData.push_back(pointFinal(i_f, j_f, k_f, lastResult));
					count++;
					delete[] V;
					delete[] D;
					delete[] temp;
					continue;
				}

				double scalar = 0;
				for (int m = 0; m < dim - 1; m++)
					scalar += D[m] * searchPoints[m][3];

				*(result + count) = scalar;
				outputData.push_back(pointFinal(i_f, j_f, k_f, scalar));
				lastResult = scalar;
				count++;

				delete[] V;
				delete[] D;
				delete[] temp;
			}
			stop= progress_cb(resol_x* resol_y, i*resol_x + j+1, "p"); // 性能为p
			if (stop)
				break;
		}
		if (stop)
			break;
	}
	return !stop;
}

bool Interpolation::LocalPolynomialInterpolation(double * range, int order, double index, int * resol, double * result){

	double begin_row = *(range);
	double begin_col = *(range + 1);
	double begin_high = *(range + 2);
	double end_row = *(range + 3);
	double end_col = *(range + 4);
	double end_high = *(range + 5);

	int resol_x = *(resol);
	int resol_y = *(resol + 1);
	int resol_z = *(resol + 2);

	double cnt_x = (end_row - begin_row) / (double)resol_x;
	double cnt_y = (end_col - begin_col) / (double)resol_y;
	double cnt_z = (end_high - begin_high) / (double)resol_z;

	int count = 0;
	double lastResult = 0;
	for (int i = 0; i < resol_x; i++) {
		double i_f = cnt_x*i + begin_row;
		for (int j = 0; j < resol_y; j++) {
			double j_f = cnt_y*j + begin_col;
			for (int k = 0; k < resol_z; k++) {
				double k_f = cnt_z*k + begin_high;

				std::vector<double> itemPoint = { i_f,j_f,k_f };
				std::vector<std::vector<double>> searchPoints;
				std::vector<double> disVec;
				if (m_searchType == 0)
					searchPoints = m_tree->all_points();
				if (m_searchType == 1)
					m_tree->k_closest_points(itemPoint, m_maxNum, m_radius, searchPoints, disVec);
				if (m_searchType == 2)
					m_tree->k_closest_points(itemPoint, m_maxNum, m_radius_t, searchPoints, disVec);

				if (searchPoints.size() < m_minNum)
				{
					*(result + count) = NAN;
					count++;
					continue;
				}

				double *search = new double[searchPoints.size() * 3];
				double *scalars = new double[searchPoints.size()];
				for (int i = 0; i < searchPoints.size(); ++i) {
					for (int j = 0; j < 3; ++j) {
						search[i * 3 + j] = searchPoints.at(i).at(j);
					}
					scalars[i] = searchPoints.at(i).at(3);
				}

				double *B,*Z;
				int Dsize = 0;
				switch (order) {
				case 1:
				{
					Dsize = 4;
					B = new double[Dsize * searchPoints.size()];
					for (int i = 0; i < searchPoints.size(); ++i) {
						B[Dsize * i + 0] = 1;
						B[Dsize * i + 1] = search[i * 3 + 0];
						B[Dsize * i + 2] = search[i * 3 + 1];
						B[Dsize * i + 3] = search[i * 3 + 2];
					}
					Z = new double[Dsize];
					Z[Dsize * i + 0] = 1;
					Z[Dsize * i + 1] = i_f;
					Z[Dsize * i + 2] = j_f;
					Z[Dsize * i + 3] = k_f;
					break;
				}					
				case 2:
				{
					Dsize = 10;
					B = new double[Dsize * searchPoints.size()];
					for (int i = 0; i < searchPoints.size(); ++i) {
						B[Dsize * i + 0] = 1;
						B[Dsize * i + 1] = search[i * 3 + 0];
						B[Dsize * i + 2] = search[i * 3 + 1];
						B[Dsize * i + 3] = search[i * 3 + 2];
						B[Dsize * i + 4] = search[i * 3 + 0] * search[i * 3 + 0];
						B[Dsize * i + 5] = search[i * 3 + 0] * search[i * 3 + 1];
						B[Dsize * i + 6] = search[i * 3 + 0] * search[i * 3 + 2];
						B[Dsize * i + 7] = search[i * 3 + 1] * search[i * 3 + 1];
						B[Dsize * i + 8] = search[i * 3 + 1] * search[i * 3 + 2];
						B[Dsize * i + 9] = search[i * 3 + 2] * search[i * 3 + 2];
					}
					Z = new double[Dsize];
					Z[Dsize * i + 0] = 1;
					Z[Dsize * i + 1] = i_f;
					Z[Dsize * i + 2] = j_f;
					Z[Dsize * i + 3] = k_f;
					Z[Dsize * i + 4] = i_f * i_f;
					Z[Dsize * i + 5] = i_f * j_f;
					Z[Dsize * i + 6] = i_f * k_f;
					Z[Dsize * i + 7] = j_f * j_f;
					Z[Dsize * i + 8] = j_f * k_f;
					Z[Dsize * i + 9] = k_f * k_f;
					break;
				}					
				case 3:
				{
					Dsize = 20;
					B = new double[Dsize * searchPoints.size()];
					for (int i = 0; i < searchPoints.size(); ++i) {
						B[Dsize * i + 0] = 1;
						B[Dsize * i + 1] = search[i * 3 + 0];
						B[Dsize * i + 2] = search[i * 3 + 1];
						B[Dsize * i + 3] = search[i * 3 + 2];
						B[Dsize * i + 4] = search[i * 3 + 0] * search[i * 3 + 0];
						B[Dsize * i + 5] = search[i * 3 + 0] * search[i * 3 + 1];
						B[Dsize * i + 6] = search[i * 3 + 0] * search[i * 3 + 2];
						B[Dsize * i + 7] = search[i * 3 + 1] * search[i * 3 + 1];
						B[Dsize * i + 8] = search[i * 3 + 1] * search[i * 3 + 2];
						B[Dsize * i + 9] = search[i * 3 + 2] * search[i * 3 + 2];
						B[Dsize * i + 10] = search[i * 3 + 0] * search[i * 3 + 0] * search[i * 3 + 0];
						B[Dsize * i + 11] = search[i * 3 + 0] * search[i * 3 + 0] * search[i * 3 + 1];
						B[Dsize * i + 12] = search[i * 3 + 0] * search[i * 3 + 0] * search[i * 3 + 2];
						B[Dsize * i + 13] = search[i * 3 + 1] * search[i * 3 + 1] * search[i * 3 + 0];
						B[Dsize * i + 14] = search[i * 3 + 1] * search[i * 3 + 1] * search[i * 3 + 1];
						B[Dsize * i + 15] = search[i * 3 + 1] * search[i * 3 + 1] * search[i * 3 + 2];
						B[Dsize * i + 16] = search[i * 3 + 2] * search[i * 3 + 2] * search[i * 3 + 0];
						B[Dsize * i + 17] = search[i * 3 + 2] * search[i * 3 + 2] * search[i * 3 + 1];
						B[Dsize * i + 18] = search[i * 3 + 2] * search[i * 3 + 2] * search[i * 3 + 2];
						B[Dsize * i + 19] = search[i * 3 + 0] * search[i * 3 + 1] * search[i * 3 + 2];
					}
					Z = new double[Dsize];
					Z[Dsize * i + 0] = 1;
					Z[Dsize * i + 1] = i_f;
					Z[Dsize * i + 2] = j_f;
					Z[Dsize * i + 3] = k_f;
					Z[Dsize * i + 4] = i_f * i_f;
					Z[Dsize * i + 5] = i_f * j_f;
					Z[Dsize * i + 6] = i_f * k_f;
					Z[Dsize * i + 7] = j_f * j_f;
					Z[Dsize * i + 8] = j_f * k_f;
					Z[Dsize * i + 9] = k_f * k_f;
					Z[Dsize * i + 10] = i_f * i_f * i_f;
					Z[Dsize * i + 11] = i_f * i_f * j_f;
					Z[Dsize * i + 12] = i_f * i_f * k_f;
					Z[Dsize * i + 13] = j_f * j_f * i_f;
					Z[Dsize * i + 14] = j_f * j_f * j_f;
					Z[Dsize * i + 15] = j_f * j_f * k_f;
					Z[Dsize * i + 16] = k_f * k_f * i_f;
					Z[Dsize * i + 17] = k_f * k_f * j_f;
					Z[Dsize * i + 18] = k_f * k_f * k_f;
					Z[Dsize * i + 19] = i_f * j_f * k_f;
					break;
				}
				default:
					return false;
				}

				double kernel = 1;
				double* P = new double[searchPoints.size() * searchPoints.size()];
				for (int i = 0; i < searchPoints.size(); ++i) {
					for (int j = 0; j < searchPoints.size(); ++j) {
						P[i*searchPoints.size() + j] = 0;
						if (i == j) {
							//P[i*searchPoints.size() + j] = 1 / ((search[i * 3] - i_f)*(search[i * 3] - i_f) + (search[i * 3 + 1] - j_f)*(search[i * 3 + 1] - j_f) + (search[i * 3 + 2] - k_f)*(search[i * 3 + 2] - k_f));//反距离权重
							P[i*searchPoints.size() + j] = 1;
						}
					}
				}
				
				double* temp = new double[Dsize * searchPoints.size()];
				for (int i = 0; i < Dsize; ++i) {
					for (int j = 0; j < searchPoints.size(); ++j) {
						temp[i*searchPoints.size() + j] = 0;
						for (int k = 0; k < searchPoints.size(); ++k) {
							temp[i*searchPoints.size() + j] += B[k*Dsize + i] * P[k*searchPoints.size() + j];
						}
					}
				}				

				double* V = new double[Dsize * Dsize];
				for (int i = 0; i < Dsize; ++i) {
					for (int j = 0; j < Dsize; ++j) {
						V[i*Dsize + j] = 0;
						for (int k = 0; k < searchPoints.size(); ++k) {
							V[i*Dsize + j] += temp[i*searchPoints.size() + k] * B[k*Dsize + j];
						}
					}
				}			

				double* D = new double[Dsize];
				for (int i = 0; i < Dsize; ++i) {
					D[i] = 0;
					for (int k = 0; k < searchPoints.size(); ++k) {
						D[i] += temp[i*searchPoints.size() + k] * scalars[k];
					}
				}				

				if (!gausj(V, D, Dsize)) {
					*(result + count) = lastResult;
					count++;

					delete[] D;
					delete[] V;
					delete[] temp;
					delete[] P;
					delete[] Z;
					delete[] B;

					delete[] scalars;
					delete[] search;
					continue;
				}

				double scalar = 0;
				for (int m = 0; m < Dsize; m++)
					scalar += D[m] * Z[m];

				*(result + count) = scalar;
				lastResult = scalar;
				count++;		

				delete[] D;
				delete[] V;
				delete[] temp;
				delete[] P;
				delete[] Z;
				delete[] B;

				delete[] scalars;
				delete[] search;
			}
		}

	}
	return true;
}

bool Interpolation::idw(double * range, int order, double smooth, int * resol, double * result, std::vector<pointFinal> & outputData){

	double begin_row = *(range);
	double begin_col = *(range + 1);
	double begin_high = *(range + 2);
	double end_row = *(range + 3);
	double end_col = *(range + 4);
	double end_high = *(range + 5);

	int resol_x = *(resol);
	int resol_y = *(resol + 1);
	int resol_z = *(resol + 2);

	double cnt_x = (end_row - begin_row) / (double)resol_x;
	double cnt_y = (end_col - begin_col) / (double)resol_y;
	double cnt_z = (end_high - begin_high) / (double)resol_z;

	int count = 0;
	double lastResult = 0;
	//设置进度条范围
	bool stop = progress_cb(resol_x* resol_y, 0, "");

	for (int i = 0; i < resol_x; i++) {
		double i_f = cnt_x*i + begin_row;
		for (int j = 0; j < resol_y; j++) {
			auto start = clock();
			double j_f = cnt_y*j + begin_col;
			for (int k = 0; k < resol_z; k++) {
				double k_f = cnt_z*k + begin_high;


				std::vector<double> itemPoint = { i_f,j_f,k_f };
				std::vector<std::vector<double>> searchPoints;
				std::vector<double> disVec;
				if (m_searchType == 0)
					searchPoints = m_tree->all_points();
				if (m_searchType == 1)
					m_tree->k_closest_points(itemPoint, m_maxNum, m_radius, searchPoints, disVec);		
				if (m_searchType == 2)
					m_tree->k_closest_points(itemPoint, m_maxNum, m_radius_t, searchPoints, disVec);			
				
				if (searchPoints.size() < m_minNum)
				{
					*(result + count) = NAN;
					count++;
					outputData.push_back(pointFinal(i_f, j_f, k_f, NAN));
					continue;
				}

				double *D = new double[searchPoints.size()];
				double Dsum = 0;
				for (int i = 0; i < searchPoints.size(); ++i) {
					D[i] = sqrt((searchPoints[i][0] - i_f)*(searchPoints[i][0] - i_f) + (searchPoints[i][1] - j_f)*(searchPoints[i][1] - j_f) + (searchPoints[i][2] - k_f)*(searchPoints[i][2] - k_f) + smooth * smooth);
					D[i] = 1 / pow(D[i], order);
					Dsum += D[i];
				}
				for (int i = 0; i < searchPoints.size(); ++i) {
					D[i] /= Dsum;
				}

				double scalar = 0;
				for (int i = 0; i < searchPoints.size(); i++)
					scalar += D[i] * searchPoints[i][3];

				*(result + count) = scalar;
				outputData.push_back(pointFinal(i_f, j_f, k_f, scalar));
				count++;

				delete[] D;

			}
			auto end = clock();
			//std::cout << scalar << std::endl;
			//std::cout << ((double)(end - start)) / CLOCKS_PER_SEC << std::endl;
			stop= progress_cb(resol_x* resol_y, i*resol_x + j+1, "p"); // 性能为p
			if (stop)
				break;
		}
		if (stop)
				break;
	}
	return !stop;
}
