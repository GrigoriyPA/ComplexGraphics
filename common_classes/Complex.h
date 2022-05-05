class Complex {
public:
	double re, im, lim_exp = 0.00001;

	Complex(double real = 0, double imaginary = 0) {
		re = real;
		im = imaginary;
	}

	Complex(Vect2 init) {
		re = init.x;
		im = init.y;
	}


	void operator+=(Complex other) {
		im += other.im;
		re += other.re;
	}

	void operator*=(Complex other) {
		double re_tmp = re * other.re - im * other.im;
		im = re* other.im + im * other.re;
		re = re_tmp;
	}


	Complex operator+(Complex other) {
		return Complex(re + other.re, im + other.im);
	}

	Complex operator+(double other) {
		return Complex(re + other, im);
	}

	Complex operator-() {
		return Complex(-re, -im);
	}

	Complex operator-(Complex other) {
		return Complex(re - other.re, im - other.im);
	}

	Complex operator-(double other) {
		return Complex(re - other, im);
	}

	Complex operator*(Complex other) {
		return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
	}

	Complex operator*(double other) {
		return Complex(re * other, im * other);
	}

	Complex operator/(Complex other) {
		if (other.get_modul() != 0)
			return Complex((re * other.re + im * other.im) / (other.get_modul() * other.get_modul()), (other.re * im - re * other.im) / (other.get_modul() * other.get_modul()));
		return Complex(INF, INF);
	}

	Complex operator/(double other) {
		if (other != 0)
			return Complex(re / other, im / other);
		return Complex(INF, INF);
	}

	Complex operator^(Complex other) {
		if (eqt(im, 0)) {
			other *= log(re);
			Complex res(1, 0), u = other;
			double n = 1, f = 1;
			for (; (u / f).get_modul() >= lim_exp; u *= other, n++, f *= n) {
				res += u / f;
			}
			return res;
		}
		return Complex(INF, INF);
	}

	double get_modul() {
		return sqrt(re * re + im * im);
	}

	double get_angle() {
		double modul = this->get_modul(), angle = 0;
		if (modul > 0) {
			angle = acos(re / modul);
			if (im < 0)
				angle = 2 * PI - angle;
		}
		return angle;
	}

	Vect2 get_vet2() {
		return Vect2(re, im);
	}
};