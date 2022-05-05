class Vect3 {
public:
	double x, y, z;

	Vect3(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vect3(Color V) {
		x = V.r;
		y = V.g;
		z = V.b;
	}

	Vect3() {
		x = 0;
		y = 0;
		z = 0;
	}


	inline Color get_Color() {
		return Color(x, y, z);
	}


	double& operator[](int i) {
		if (i == 0)
			return x;
		if (i == 1)
			return y;
		if (i == 2)
			return z;
	}


	bool operator ==(Vect3 other) {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator !=(Vect3 other) {
		return x != other.x || y != other.y || z != other.z;
	}


	void operator +=(Vect3 other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void operator -=(Vect3 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	void operator *=(double other) {
		x *= other;
		y *= other;
		z *= other;
	}

	void operator /=(double other) {
		if (other != 0) {
			x /= other;
			y /= other;
			z /= other;
		}
		else {
			x = INF;
			y = INF;
			z = INF;
		}
	}


	inline Vect3 operator -() {
		return Vect3(-x, -y, -z);
	}


	inline Vect3 operator +(Vect3 other) {
		return Vect3(x + other.x, y + other.y, z + other.z);
	}

	inline Vect3 operator -(Vect3 other) {
		return Vect3(x - other.x, y - other.y, z - other.z);
	}

	inline Vect3 operator *(double other) {
		return Vect3(x * other, y * other, z * other);
	}

	inline double operator *(Vect3 other) {
		return x * other.x + y * other.y + z * other.z;
	}

	inline Vect3 operator ^(Vect3 other) {
		return Vect3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	Vect3 operator /(double other) {
		if (other != 0)
			return Vect3(x / other, y / other, z / other);
		return Vect3(INF, INF, INF);
	}


	inline double length() {
		return sqrt(*this * *this);
	}

	inline Vect3 normalization() {
		return *this / (*this).length();
	}


	inline double Cos_angle(Vect3 V) {
		return (*this * V) / ((*this).length() * V.length());
	}

	inline double Sin_angle(Vect3 V) {
		return ((*this ^ V).length()) / ((*this).length() * V.length());
	}

	inline Vect3 Reflect_vect(Vect3 N) {
		return N * (N * *this) * 2 - *this;
	}

	Vect3 Refract_vect(Vect3 N, double n_obj, double n_env) {
		double cos_alf = (-*this).Cos_angle(N);
		Vect3 a = N * (N * *this);
		Vect3 x = *this - a;
		if (cos_alf >= 0) {
			Vect3 y = x * (n_env / n_obj);
			Vect3 b = a * (sqrt(*this * *this - y * y) / a.length());
			return b + y;
		}
		Vect3 y = x * (n_obj / n_env);
		if ((*this).length() >= y.length()) {
			Vect3 b = a * (sqrt(*this * *this - y * y) / a.length());
			return b + y;
		}
		return Vect3();
	}

	void Set_max(Vect3 V) {
		x = max(x, V.x);
		y = max(y, V.y);
		z = max(z, V.z);
	}

	void Set_min(Vect3 V) {
		x = min(x, V.x);
		y = min(y, V.y);
		z = min(z, V.z);
	}
};