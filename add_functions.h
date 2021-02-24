#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <queue>
using namespace sf;
using namespace std;


const double INF = 1000000000, EPS = 0.00001;
const double PI = 3.1415926535, E = 2.718281828459, FI = 2 / (sqrt(5) + 1);


bool eqt(double x, double y, double err = EPS) {
	return abs(x - y) <= err;
}


short sign(double x) {
	if (eqt(x, 0))
		return 0;
	if (x > 0)
		return 1;
	return -1;
}
