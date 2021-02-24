class Complex_expression {
public:
	Complex_expression (string s = "") {
		p[')'] = 0;
		signs.insert('('); p['('] = -1;
		signs.insert('+'); p['+'] = 1;
		signs.insert('-'); p['-'] = 1;
		signs.insert('*'); p['*'] = 2;
		signs.insert('/'); p['/'] = 2;
		signs.insert('^'); p['^'] = 3;
		this->set_exp(s);
	}


	bool set_exp(string s) {
		if (s == "") {
			exp.clear();
			exp.push_back(make_pair('(', Complex()));
			exp.push_back(make_pair('n', Complex()));
			exp.push_back(make_pair(')', Complex()));
			return true;
		}

		this->s = s;
		s = '(' + s + ')';
		int cnt = 0;
		bool flag_num = false, flag_point = false;
		string num;
		vector < pair < char, Complex > > new_exp;
		for (char c : s) {
			if ('0' <= c && c <= '9') {
				flag_num = true;
				num += c;
				continue;
			}
			else if (c == '.') {
				if (!flag_num || flag_point)
					return false;
				flag_point = true;
				num += ',';
				continue;
			}
			else if (flag_num) {
				if (num.back() == ',')
					return false;
				flag_num = false;
				flag_point = false;
				new_exp.push_back(make_pair('n', Complex(atof(num.c_str()), 0)));
				num = "";
			}

			switch (c)
			{
			case 'c': {
				if (!signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				break;
			}

			case 'i': {
				if (!signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair('n', Complex(0, 1)));
				break;
			}

			case '+': {
				if (new_exp.back().first != '(' && signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				break;
			}

			case '-': {
				if (new_exp.back().first != '(' && signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				break;
			}

			case '*': {
				if (signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				break;
			}

			case '/': {
				if (signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				break;
			}

			case '^': {
				if (signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				break;
			}

			case '(': {
				if (new_exp.size() > 0 && !signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				cnt++;
				break;
			}

			case ')': {
				cnt--;
				if (cnt < 0 || new_exp.back().first != '(' && signs.count(new_exp.back().first))
					return false;
				new_exp.push_back(make_pair(c, Complex()));
				break;
			}

			default:
				return false;
				break;
			}
		}
		if (cnt)
			return false;
		exp = new_exp;
		return true;
	}


	inline string get_exp() {
		return s;
	}


	Complex operation_un(Complex a, char o) {
		switch (o)
		{
		case '+': {
			return a;
			break;
		}

		case '-': {
			return -a;
			break;
		}

		default:
			break;
		}
	}

	Complex operation_bin(Complex a, Complex b, char o) {
		switch (o)
		{
		case '+': {
			return a + b;
			break;
		}

		case '-': {
			return a - b;
			break;
		}

		case '*': {
			return a * b;
			break;
		}

		case '/': {
			return a / b;
			break;
		}

		case '^': {
			return a ^ b;
			break;
		}

		default:
			break;
		}
	}

	Complex calc(Complex c) {
		vector < pair < char, Complex > > st;
		for (pair < char, Complex > el : exp) {
			if (el.first == 'c')
				st.push_back(make_pair('c', c));
			else if (el.first == 'n')
				st.push_back(make_pair('c', el.second));
			else if (el.first == '(')
				st.push_back(make_pair('(', Complex()));
			else if (st.back().first == '(')
				st.push_back(make_pair(el.first, Complex()));
			else {
				Complex res = st.back().second;
				st.pop_back();
				while (st.size() > 1 && p[st.back().first] >= p[el.first]) {
					if (st[st.size() - 2].first == 'c') {
						res = operation_bin(st[st.size() - 2].second, res, st.back().first);
						st.pop_back();
					}
					else
						res = operation_un(res, st.back().first);
					st.pop_back();
				}
				if (el.first == ')') {
					st.pop_back();
					st.push_back(make_pair('c', res));
				}
				else {
					st.push_back(make_pair('c', res));
					st.push_back(make_pair(el.first, Complex()));
				}
			}
		}
		if (st.size() == 0)
			return Complex();
		else if (st[0].first == 'c')
			return st[0].second;
		else 
			return operation_un(st[1].second, st[0].first);
	}

private:
	string s;
	vector < pair < char, Complex > > exp;
	map < char, int > p;
	set < char > signs;
	int max_p = 2;
};