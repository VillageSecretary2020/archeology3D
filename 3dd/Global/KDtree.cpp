#include "KDtree.h"

inline double distance_squared(const vector<double>& a, const vector<double>& b) {
	double d = 0;
	for (int i = 0; i<DIM; i++)
		d += (a[i] - b[i])*(a[i] - b[i]);
	return d;
}

inline bool isOnRange(const vector<double>& a, const vector<double>& b, const vector<double>& r) {
	double d = 0;
	for (int i = 0; i<DIM; i++)
		d += (a[i] - b[i])*(a[i] - b[i]) / r[i] / r[i];
	if (d <= 1)
		return true;
	return false;
}


KDTree::KDTree(const vector<Point>& points) {

	this->npoints = points.size();
	this->ndim = DIM;
	this->points = points;
	nodesPtrs.reserve(npoints);

	vector<char> sidehelper(npoints, 'x');

	vector< MinHeap<double> > heaps(ndim, npoints);
	for (int dIdx = 0; dIdx<ndim; dIdx++)
		for (int pIdx = 0; pIdx<npoints; pIdx++)
			heaps[dIdx].push(points[pIdx][dIdx], pIdx);
	vector< vector<int> > sorter(ndim, vector<int>(npoints, 0));
	for (int dIdx = 0; dIdx<ndim; dIdx++)
		heaps[dIdx].heapsort(sorter[dIdx]);

	build_recursively(sorter, sidehelper, 0);
}

KDTree::~KDTree() {
	for (unsigned int i = 0; i < nodesPtrs.size(); i++)
		delete nodesPtrs[i];
}

int KDTree::build_recursively(vector< vector<int> >& sorter, vector<char>& sidehelper, int dim) {

	int numel = sorter[dim].size();

	if (numel == 1) {
		Node *node = new Node();
		int nodeIdx = nodesPtrs.size();
		nodesPtrs.push_back(node);
		node->LIdx = -1;
		node->RIdx = -1;

		node->pIdx = sorter[dim][0];
		node->key = 0;
		return nodeIdx;
	}

	int iMedian = floor((numel - 1) / 2.0);
	int pidxMedian = sorter[dim][iMedian];
	int nL = iMedian + 1;
	int nR = numel - nL;

	for (int i = 0; i<sorter[dim].size(); i++) {
		int pidx = sorter[dim][i];
		sidehelper[pidx] = (i <= iMedian) ? 'l' : 'r';
	}

	vector< vector<int> > Lsorter(ndim, vector<int>(nL, -1));
	vector< vector<int> > Rsorter(ndim, vector<int>(nR, -1));

	for (int idim = 0; idim<ndims(); idim++) {
		int iL = 0, iR = 0;
		for (int i = 0; i<sorter[idim].size(); i++) {
			int pidx = sorter[idim][i];
			if (sidehelper[pidx] == 'l')
				Lsorter[idim][iL++] = pidx;
			if (sidehelper[pidx] == 'r')
				Rsorter[idim][iR++] = pidx;
		}
	}

	Node* node = new Node();
	int nodeIdx = nodesPtrs.size();
	nodesPtrs.push_back(node);
	node->pIdx = -1; 
	node->key = points[pidxMedian][dim];
	node->LIdx = build_recursively(Lsorter, sidehelper, (dim + 1) % ndim);
	node->RIdx = build_recursively(Rsorter, sidehelper, (dim + 1) % ndim);
	return nodeIdx;
}

void KDTree::k_closest_points(const Point& Xq, int k, double radis, vector<vector<double>>& xs, vector<double>& distances) {

	Bmin = vector<double>(ndim, -DBL_MAX);
	Bmax = vector<double>(ndim, +DBL_MAX);
	this->k = k;
	this->terminate_search = false;

	knn_search(Xq, radis);

	int N = pq.size();
	for (int i = 0; i < N; i++) {
		pair<double, int> topel = pq.top();
		pq.pop();
		if (i >= N - k) {
			xs.push_back(this->points[topel.second]);
			distances.push_back(sqrt(topel.first));
		}
	}

	std::reverse(xs.begin(), xs.end());
	std::reverse(distances.begin(), distances.end());
}

void KDTree::k_closest_points(const Point & Xq, int k, vector<double>& radis, vector<vector<double>>& xs, vector<double>& distances)
{
	Bmin = vector<double>(ndim, -DBL_MAX);
	Bmax = vector<double>(ndim, +DBL_MAX);
	this->k = k;
	this->terminate_search = false;

	knn_search(Xq, radis);

	int N = pq.size();
	for (int i = 0; i < N; i++) {
		pair<double, int> topel = pq.top();
		pq.pop();
		if (i >= N - k) {
			xs.push_back(this->points[topel.second]);
			distances.push_back(sqrt(topel.first));
		}
	}

	std::reverse(xs.begin(), xs.end());
	std::reverse(distances.begin(), distances.end());
}

// KNN算法的全称是K Nearest Neighbors，意思是K个最近的邻居
void KDTree::knn_search(const Point& Xq, double radis, int nodeIdx/*=0*/, int dim/*=0*/) {

	Node* node = nodesPtrs[nodeIdx];
	double temp;

	if (node->isLeaf()) {
		double distance = distance_squared(Xq, points[node->pIdx]);
		if (distance > radis * radis)
			return;
		if (pq.size() == k && pq.top().first>distance) {
			pq.pop();
			pq.push(distance, node->pIdx);
		}
		else if (pq.size()<k)
			pq.push(distance, node->pIdx);

		return;
	}

	if (Xq[dim] <= node->key) {
		temp = Bmax[dim];
		Bmax[dim] = node->key;
		knn_search(Xq, radis, node->LIdx, (dim + 1) % ndim);
		Bmax[dim] = temp;
	}
	else {
		temp = Bmin[dim]; 
		Bmin[dim] = node->key;
		knn_search(Xq, radis, node->RIdx, (dim + 1) % ndim);
		Bmin[dim] = temp;
	}

	if (Xq[dim] <= node->key) {
		temp = Bmin[dim]; 
		Bmin[dim] = node->key;
		if (bounds_overlap_ball(Xq) && (Xq[dim] - node->key)*(Xq[dim] - node->key) < radis * radis)
			knn_search(Xq, radis, node->RIdx, (dim + 1) % ndim);
		Bmin[dim] = temp;
	}
	else {
		temp = Bmax[dim]; 
		Bmax[dim] = node->key;
		if (bounds_overlap_ball(Xq) && (Xq[dim] - node->key)*(Xq[dim] - node->key) < radis * radis)
			knn_search(Xq, radis, node->LIdx, (dim + 1) % ndim);
		Bmax[dim] = temp;
	}
}

void KDTree::knn_search(const Point & Xq, std::vector<double> &radis, int nodeIdx, int dim)
{
	Node* node = nodesPtrs[nodeIdx];
	double temp;

	if (node->isLeaf()) {	
		if (!isOnRange(Xq, points[node->pIdx], radis))
			return;
		double distance = distance_squared(Xq, points[node->pIdx]);
		if (pq.size() == k && pq.top().first>distance) {
			pq.pop();
			pq.push(distance, node->pIdx);
		}
		else if (pq.size()<k)
			pq.push(distance, node->pIdx);

		return;		
	}

	if (Xq[dim] <= node->key) {
		temp = Bmax[dim]; 
		Bmax[dim] = node->key;
		knn_search(Xq, radis, node->LIdx, (dim + 1) % ndim);
		Bmax[dim] = temp;
	}
	else {
		temp = Bmin[dim];
		Bmin[dim] = node->key;
		knn_search(Xq, radis, node->RIdx, (dim + 1) % ndim);
		Bmin[dim] = temp;
	}

	if (Xq[dim] <= node->key) {
		temp = Bmin[dim];
		Bmin[dim] = node->key;
		if (bounds_overlap_ball(Xq) && (Xq[dim] - node->key)*(Xq[dim] - node->key) < radis[dim] * radis[dim])
			knn_search(Xq, radis, node->RIdx, (dim + 1) % ndim);
		Bmin[dim] = temp;
	}
	else {
		temp = Bmax[dim]; 
		Bmax[dim] = node->key;
		if (bounds_overlap_ball(Xq) && (Xq[dim] - node->key)*(Xq[dim] - node->key) < radis[dim] * radis[dim])
			knn_search(Xq, radis, node->LIdx, (dim + 1) % ndim);
		Bmax[dim] = temp;
	}
}

void KDTree::closest_point(const Point &p, vector<double>& x, double& dist) {

	vector<vector<double>> idxs;
	vector<double> dsts;
	k_closest_points(p, 1, 999999999, idxs, dsts);
	x = idxs[0];
	dist = dsts[0];
	return;
}

double KDTree::bounds_overlap_ball(const Point& Xq) {

	if (pq.size()<k)
		return true;

	double sum = 0;
	double best_dist_sq = pq.top().first;
	for (int d = 0; d < ndim; d++) {

		if (Xq[d] < Bmin[d]) {
			sum += (Xq[d] - Bmin[d])*(Xq[d] - Bmin[d]);
			if (sum > best_dist_sq)
				return false;
		}
		else if (Xq[d] > Bmax[d]) {
			sum += (Xq[d] - Bmax[d])*(Xq[d] - Bmax[d]);
			if (sum > best_dist_sq)
				return false;
		}
	}

	return true;
}
