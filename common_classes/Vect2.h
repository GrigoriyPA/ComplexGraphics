class Vect2 {
public:
	double x, y;

	Vect2(double x, double y) {
		this->x = x;
		this->y = y;
	}

	Vect2(Vector2f V) {
		x = V.x;
		y = V.y;
	}

	Vect2() {
		x = 0;
		y = 0;
	}


	inline Vector2f get_Vector2f() {
		return Vector2f(x, y);
	}


	bool operator ==(Vect2 other) {
		return x == other.x && y == other.y;
	}

	bool operator !=(Vect2 other) {
		return x != other.x || y != other.y;
	}


	inline void operator +=(Vect2 other) {
		x += other.x;
		y += other.y;
	}

	inline void operator -=(Vect2 other) {
		x -= other.x;
		y -= other.y;
	}

	inline void operator *=(double other) {
		x *= other;
		y *= other;
	}

	void operator /=(double other) {
		if (other != 0) {
			x /= other;
			y /= other;
		}
		else {
			x = INF;
			y = INF;
		}
	}


	inline Vect2 operator -() {
		return Vect2(-x, -y);
	}


	inline Vect2 operator +(Vect2 other) {
		return Vect2(x + other.x, y + other.y);
	}

	inline Vect2 operator -(Vect2 other) {
		return Vect2(x - other.x, y - other.y);
	}

	inline Vect2 operator *(double other) {
		return Vect2(x * other, y * other);
	}

	inline double operator *(Vect2 other) {
		return x * other.x + y * other.y;
	}

	inline double operator ^(Vect2 other) {
		return x * other.y - y * other.x;
	}

	Vect2 operator /(double other) {
		if (other != 0)
			return Vect2(x / other, y / other);
		return Vect2(INF, INF);
	}


	inline double length() {
		return sqrt(*this * *this);
	}

	inline Vect2 normalization() {
		return *this / (*this).length();
	}


	inline double Cos_angle(Vect2 V) {
		return (*this * V) / ((*this).length() * V.length());
	}

	inline double Sin_angle(Vect2 V) {
		return (*this ^ V) / ((*this).length() * V.length());
	}

	double Pol_angle() {
		double angle = atan2(y, x);
		if (angle < 0)
			angle += 2 * PI;
		return angle;
	}

	inline Vect2 Reflect_vect(Vect2 N) {
		return N * (N * *this) * 2 - *this;
	}

	Vect2 Refract_vect(Vect2 N, double n_obj, double n_env) {
		double cos_alf = (-*this).Cos_angle(N);
		Vect2 a = N * (N * *this);
		Vect2 x = *this - a;
		if (cos_alf >= 0) {
			Vect2 y = x * (n_env / n_obj);
			Vect2 b = a * (sqrt(*this * *this - y * y) / a.length());
			return b + y;
		}
		Vect2 y = x * (n_obj / n_env);
		if ((*this).length() >= y.length()) {
			Vect2 b = a * (sqrt(*this * *this - y * y) / a.length());
			return b + y;
		}
		return Vect2();
	}

	inline void Set_max(Vect2 V) {
		x = max(x, V.x);
		y = max(y, V.y);
	}

	inline void Set_min(Vect2 V) {
		x = min(x, V.x);
		y = min(y, V.y);
	}
};