class Line2 {
public:
	Vect2 N, P1, P2;
	double d;

	Line2(Vect2 P1, Vect2 P2) {
		this->P1 = P1;
		this->P2 = P2;
		N = Vect2((P2 - P1).y, -(P2 - P1).x);
		d = -(P1 * N);
	}

	Line2(double A, double B, double C) {
		N = Vect2(A, B);
		d = C / N.length();
		N = N.normalization();

		if (N.x == 0) {
			P1 = Vect2(1, -d / N.y);
			P2 = Vect2(-1, -d / N.y);
		}
		else {
			P1 = Vect2((-d - N.y) / N.x, 1);
			P2 = Vect2((-d + N.y) / N.x, -1);
		}
	}

	Line2() {
		d = 0;
	}


	inline bool check_aff(Vect2 P) {
		return eqt(N * P + d, 0);
	}


	inline Vect2 intersect(Line2 L) {
		Vect2 v1(L.d, d), v2(L.N.y, N.y), v3(L.N.x, N.x);
		return Vect2(v1 ^ v2, v3 ^ v1) / (v2 ^ v3);
	}
};


Vect2 cuts_intersection(Vect2 a, Vect2 b, Vect2 c, Vect2 d) {
	Line2 AB(a, b), CD(c, d);
	Vect2 P = AB.intersect(CD);
	if (P.x == INF || (P - a) * (P - b) < EPS && (P - c) * (P - d) < EPS)
		return P;
	return Vect2(INF, INF);
}