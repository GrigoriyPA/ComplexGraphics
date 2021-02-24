class Additional_lines : public Drawable {
public:
	Additional_lines(Vector2f pos1, Coordinate_system* cs) {
		this->cs = cs;

		this->pos1 = pos1;
		this->pos2 = pos1;
		po1 = ((Vect2(pos1) - Vect2(cs->get_global_center())) / cs->get_count()).get_Vector2f();
		po2 = po1;

		mline.setPrimitiveType(Lines);
		mline.append(Vertex(Vector2f(0, 0), colour_line));
		mline.append(Vertex(Vector2f(0, 0), colour_line));

		tip1.setRadius(radius);
		tip1.setOrigin(Vector2f(radius, radius));
		tip1.setFillColor(colour_circles);

		tip2.setRadius(radius);
		tip2.setOrigin(Vector2f(radius, radius));
		tip2.setFillColor(colour_circles);

		this->set_global_pos();
	}


	void set_pos2(Vector2f pos2) {
		this->pos2 = pos2;
		po2 = ((Vect2(pos2) - Vect2(cs->get_global_center())) / cs->get_count()).get_Vector2f();
		build = true;
		this->set_global_pos();
	}

	inline void set_tmp_pos(Vector2f pos2) {
		this->pos2 = pos2;
		po2 = ((Vect2(pos2) - Vect2(cs->get_global_center())) / cs->get_count()).get_Vector2f();
		this->set_global_pos();
	}

	void set_global_pos() {
		gmline = mline;
		Vect2 p1(po1), p2(po2), ce(cs->get_global_center());
		pos1 = (p1 * cs->get_count() + ce).get_Vector2f();
		pos2 = (p2 * cs->get_count() + ce).get_Vector2f();

		gmline[0].position = pos1;
		gmline[1].position = pos2;

		pair < Vector2f, Vector2f > p = cs->get_intersection(pos1, pos2);

		Vector2f rp1 = p.first, rp2 = p.second;
		t1_vis = cs->cheñk_aff(pos1.x, pos1.y);
		if (t1_vis) {
			rp1 = pos1;
			if (p.first.x != INF)
				rp2 = p.first;
		}
		t2_vis = cs->cheñk_aff(pos2.x, pos2.y);
		if (t2_vis) {
			rp2 = pos2;
			if (p.first.x != INF)
				rp1 = p.first;
		}

		tip1.setPosition(pos1);
		tip2.setPosition(pos2);
		mline[0].position = rp1;
		mline[1].position = rp2;
		l = Line2(pos1, pos2);
	}


	inline bool get_build() {
		return build;
	}

	inline double get_move() {
		return move;
	}

	inline VertexArray get_mline() {
		return gmline;
	}


	void check_target(double x, double y, bool flag) {
		if (flag) {
			mouse = Vector2f(x, y);
			Vect2 p1(pos1), p2(pos2), p(mouse);
			if ((p1 - p).length() <= radius)
				move = 1;
			else if ((p2 - p).length() <= radius)
				move = 2;
			else if (this->cheñk_belonging(x, y))
				move = 3;
		}
		else
			move = 0;
	}

	void check_move(double x, double y) {
		if (move) {
			double dx = (x - mouse.x) / cs->get_count(), dy = (y - mouse.y) / cs->get_count();
			mouse = Vector2f(x, y);
			if (move == 1 || move == 3) {
				po1.x += dx;
				po1.y += dy;
			}
			if (move == 2 || move == 3) {
				po2.x += dx;
				po2.y += dy;
			}
			this->set_global_pos();
		}
	}

	bool cheñk_belonging(double x, double y) {
		Vect2 p(x, y);
		Line2 pl(p, p + l.N);
		return (l.intersect(pl) - p).length() <= min_dist && (Vect2(pos1) - p) * (Vect2(pos2) - p) <= 0;
	}


	void draw(RenderTarget& target, RenderStates states) const {
		target.draw(mline, states);
		if (t1_vis)
			target.draw(tip1, states);
		if (t2_vis)
			target.draw(tip2, states);
	}

private:
	Coordinate_system* cs;
	double move = 0, min_dist = 5;
	bool build = false, t1_vis = true, t2_vis = true;
	Vector2f pos1, pos2, po1, po2, mouse;
	Line2 l;
	VertexArray mline, gmline;
	CircleShape tip1, tip2;

	double radius = 6;
	Color colour_line = Color(215, 75, 9), colour_circles = Color(230, 230, 230);
};


string to_str(double x) {
	string res = to_string(x);
	for (int i = res.length() - 1; i >= 0; i--) {
		if (res[i] != '0') {
			if (res[i] == ',')
				res.pop_back();
			break;
		}
		res.pop_back();
	}
	return res;
}


string to_str(Color c) {
	return to_string(int(c.r)) + ", " + to_string(int(c.g)) + ", " + to_string(int(c.b));
}


bool get_color(string s, Color& res) {
	vector < string > tmp(3);
	int i = 0;
	for (int ind = 0; ind < 3; ind++) {
		for (; i < s.length() && '0' <= s[i] && s[i] <= '9'; i++) {
			tmp[ind] += s[i];
		}
		if (ind < 2) {
			if (i == s.length() || s[i] != ',')
				return false;
			i++;
			for (; i < s.length() && s[i] == ' '; i++) {}
		}
	}
	if (i < s.length() || !tmp[0].length() || !tmp[1].length() || !tmp[2].length())
		return false;
	int a = atoi(tmp[0].c_str()), b = atoi(tmp[1].c_str()), c = atoi(tmp[2].c_str());
	res = Color(a, b, c);
	return a <= 255 && b <= 255 && c <= 255;
}


bool check_positive_number(string s) {
	bool flag = s.length() && s[0] != ',', pt = false;
	for (char el : s) {
		flag = flag && (el == ',' && !pt || '0' <= el && el <= '9');
		pt = pt || el == ',';
	}
	return flag;
}


class Settings : public Interface_feature {
public:
	double pv, lv;
	vector < Color > clsv;
	Button converting_button, show_right_button, coloring_button;

	Settings(Vector2f position, Vector2f size, Font& font, Color font_colour, vector < double > init) {
		pos = position;
		this->size = size;
		this->font_colour = font_colour;
		this->font = font;
		height = size.y / (cnt_vertical + 1);
		width = size.x / cnt_horizontal;
		colorw = width * cl_width;

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		window.setPosition(pos);
		window.setSize(size);
		window.setFillColor(fill_colour);
		window.setOutlineThickness(outline_width);
		window.setOutlineColor(outline_color);

		ln.setPrimitiveType(Lines);
		ln.append(Vertex(Vector2f(pos.x + indent, pos.y + height), outline_color));
		ln.append(Vertex(Vector2f(pos.x + size.x - indent, pos.y + height), outline_color));
		ln.append(Vertex(Vector2f(pos.x + width, pos.y + indent), outline_color));
		ln.append(Vertex(Vector2f(pos.x + width, pos.y + size.y - indent), outline_color));
		for (int i = 0; i < 7; i++) {
			ln.append(Vertex(Vector2f(pos.x + indent, pos.y + (i + 2) * height), outline_color));
			if (i == 2 || i == 4)
				ln.append(Vertex(Vector2f(pos.x + width - 2 * indent - colorw, pos.y + (i + 2) * height), outline_color));
			else
				ln.append(Vertex(Vector2f(pos.x + width - indent, pos.y + (i + 2) * height), outline_color));
		}

		at.resize(2);
		vector < string > s = { "General", "Personal" };
		for (int i = 0; i < 2; i++) {
			at[i].setString(s[i]);
			at[i].setFont(font);
			at[i].setCharacterSize(height - 2 * indent);
			at[i].setFillColor(font_colour);
		}
		at[0].setPosition(Vector2f(pos.x + width / 2 - at[0].getGlobalBounds().width / 2, pos.y + indent));
		at[1].setPosition(Vector2f(pos.x + 2 * width - at[1].getGlobalBounds().width / 2, pos.y + indent));

		precision.setString("Precision =  ");
		precision.setFont(font);
		precision.setCharacterSize(height - 2 * indent);
		precision.setFillColor(font_colour);
		precision.setPosition(Vector2f(pos.x + indent, pos.y + indent + height));
		pv = init[0];
		precision_input = Text_input(Vector2f(pos.x + indent + precision.getGlobalBounds().width, pos.y + indent - margine + height),
			Vector2f(width - 2 * indent - precision.getGlobalBounds().width, height - 2 * indent + 2 * margine),
			font, margine, to_str(pv));

		line_frequency.setString("Line frequency =  ");
		line_frequency.setFont(font);
		line_frequency.setCharacterSize(height - 2 * indent);
		line_frequency.setFillColor(font_colour);
		line_frequency.setPosition(Vector2f(pos.x + indent, pos.y + indent + 2 * height));
		lv = init[1];
		line_frequency_input = Text_input(Vector2f(pos.x + indent + line_frequency.getGlobalBounds().width, pos.y + indent - margine + 2 * height),
			Vector2f(width - 2 * indent - line_frequency.getGlobalBounds().width, height - 2 * indent + 2 * margine),
			font, margine, to_str(lv));

		colours.resize(4);
		clsv.resize(4);
		colours_input.resize(4);
		colours_view.setPrimitiveType(Quads);
		s = { "Left", "Right", "Up", "Down" };
		for (int i = 0, ind = 2; i < 4; i++, ind += 3) {
			colours[i].setString(s[i] + " color =  ");
			colours[i].setFont(font);
			colours[i].setCharacterSize(height - 2 * indent);
			colours[i].setFillColor(font_colour);
			colours[i].setPosition(Vector2f(pos.x + indent, pos.y + indent + (i + 3) * height));
			clsv[i] = Color(init[ind], init[ind + 1], init[ind + 2]);
			colours_input[i] = Text_input(Vector2f(pos.x + indent + colours[i].getGlobalBounds().width, pos.y + indent - margine + (i + 3) * height),
				Vector2f(width - 3 * indent - colours[i].getGlobalBounds().width - colorw, height - 2 * indent + 2 * margine),
				font, margine, to_str(clsv[i]));
			if (i % 2) {
				colours_view.append(Vertex(Vector2f(pos.x + width - indent, pos.y - indent + margine + (i + 4) * height), clsv[i]));
				colours_view.append(Vertex(Vector2f(pos.x + width - indent - colorw, pos.y - indent + margine + (i + 4) * height), clsv[i]));
			}
			else {
				colours_view.append(Vertex(Vector2f(pos.x + width - indent - colorw, pos.y + indent - margine + (i + 3) * height), clsv[i]));
				colours_view.append(Vertex(Vector2f(pos.x + width - indent, pos.y + indent - margine + (i + 3) * height), clsv[i]));
			}
		}

		converting.setString("Converting grid lines  ");
		converting.setFont(font);
		converting.setCharacterSize(height - 2 * indent);
		converting.setFillColor(font_colour);
		converting.setPosition(Vector2f(pos.x + indent, pos.y + indent + 7 * height));
		converting_button = Button(Vector2f(pos.x + width - height / 2, pos.y + 7.5 * height),
			Vector2f(height - 2 * indent, height - 2 * indent), 
			VertexArray(), check_mark(font_colour), init[14]);

		coloring.setString("Coloring area of definition  ");
		coloring.setFont(font);
		coloring.setCharacterSize(height - 2 * indent);
		coloring.setFillColor(font_colour);
		coloring.setPosition(Vector2f(pos.x + indent, pos.y + indent + 8 * height));
		coloring_button = Button(Vector2f(pos.x + width - height / 2, pos.y + 8.5 * height),
			Vector2f(height - 2 * indent, height - 2 * indent),
			VertexArray(), check_mark(font_colour), init[15]);

		show_right.setString("Show right window grid lines  ");
		show_right.setFont(font);
		show_right.setCharacterSize(height - 2 * indent);
		show_right.setFillColor(font_colour);
		show_right.setPosition(Vector2f(pos.x + indent + width, pos.y + indent + height));
		show_right_button = Button(Vector2f(pos.x + width + height / 2 + show_right.getGlobalBounds().width, pos.y + 1.5 * height),
			Vector2f(height - 2 * indent, height - 2 * indent),
			VertexArray(), check_mark(font_colour), init[16]);
	}

	Settings() {
		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;
	}


	void check_move(double x, double y) {
		converting_button.check_target(x, y);
		show_right_button.check_target(x, y);
		coloring_button.check_target(x, y);
	}

	void check_target(double x, double y, bool flag) {
		precision_input.check_target(x, y);
		line_frequency_input.check_target(x, y);
		for (int i = 0; i < 4; i++)
			colours_input[i].check_target(x, y);
		converting_button.check_activation(flag);
		show_right_button.check_activation(flag);
		coloring_button.check_activation(flag);
	}

	void check_input(int code, char type, bool shift = false, bool control = false) {
		precision_input.check_input(code, type, shift, control);
		if (check_positive_number(precision_input.get_string())) {
			precision_input.set_border_colour(outline_color);
			pv = atof(precision_input.get_string().c_str());
		}
		else
			precision_input.set_border_colour(inc_colour);

		line_frequency_input.check_input(code, type, shift, control);
		if (check_positive_number(line_frequency_input.get_string())) {
			line_frequency_input.set_border_colour(outline_color);
			lv = atof(line_frequency_input.get_string().c_str());
		}
		else
			line_frequency_input.set_border_colour(inc_colour);

		for (int i = 0; i < 4; i++) {
			colours_input[i].check_input(code, type, shift, control);
			Color res;
			if (get_color(colours_input[i].get_string(), res)) {
				colours_input[i].set_border_colour(outline_color);
				clsv[i] = res;
				colours_view[2 * i].color = clsv[i];
				colours_view[2 * i + 1].color = clsv[i];
			}
			else
				colours_input[i].set_border_colour(inc_colour);
		}
	}

	void check_counter() {
		precision_input.check_counter();
		line_frequency_input.check_counter();
		for (int i = 0; i < 4; i++)
			colours_input[i].check_counter();
	}


	void draw(RenderTarget& target, RenderStates states) const {
		target.draw(window, states);
		target.draw(ln, states);
		for (int i = 0; i < 2; i++)
			target.draw(at[i], states);

		target.draw(precision, states);
		target.draw(precision_input, states);

		target.draw(line_frequency, states);
		target.draw(line_frequency_input, states);

		for (int i = 0; i < 4; i++) {
			target.draw(colours[i], states);
			target.draw(colours_input[i], states);
		}
		target.draw(colours_view, states);

		target.draw(converting, states);
		target.draw(converting_button, states);

		target.draw(coloring, states);
		target.draw(coloring_button, states);

		target.draw(show_right, states);
		target.draw(show_right_button, states);
	}

	void reset() {
		precision_input.set_string(to_str(pv));
		precision_input.set_border_colour(outline_color);

		line_frequency_input.set_string(to_str(lv));
		line_frequency_input.set_border_colour(outline_color);

		for (int i = 0; i < 4; i++) {
			colours_input[i].set_string(to_str(clsv[i]));
			colours_input[i].set_border_colour(outline_color);
		}

		converting_button.check_target(0, 0);
		show_right_button.check_target(0, 0);
		coloring_button.check_target(0, 0);
	}

private:
	RectangleShape window;
	Text precision, line_frequency, converting, show_right, coloring;
	vector < Text > colours, at;
	Text_input precision_input, line_frequency_input;
	vector < Text_input > colours_input;
	double height, width, colorw;
	Color font_colour;
	VertexArray ln, colours_view;

	double indent = 15, margine = 4, cnt_vertical = 9, cnt_horizontal = 3, cl_width = 0.07;
	Color inc_colour = Color(215, 0, 0);
};


Color get_color(Vect2 v) {
	/*double H = v.Pol_angle() / (2 * PI);
	double L = (1 - pow(2, -v.length()));
	double S = 1, Q;*/
	v.y *= -1;
	double H = v.Pol_angle() / (2 * PI);
	double L = 1 - 1 / (1 + 0.47 * log(v.length() + 1));
	double S = 1;

	double Q;
	if (L < 0.5)
		Q = L * (1 + S);
	else
		Q = L + S - L * S;
	double P = 2 * L - Q, A = double(1) / 3;
	Vect3 res;
	for (int i = 0; i < 3; i++) {
		double T = H + A;
		if (T < 0)
			T++;
		else if (T > 1)
			T--;
		if (T < double(1) / 6)
			res[i] = P + (Q - P) * 6 * T;
		else if (T < double(1) / 2)
			res[i] = Q;
		else if (T < double(2) / 3)
			res[i] = P + (Q - P) * (double(2) / 3 - T) * 6;
		else
			res[i] = P;
		A -= double(1) / 3;
	}
	res *= 255;
	return res.get_Color();
}


vector < Vector2f > interpolate(double i0, double d0, double i1, double d1, double step = 1) {
	vector < Vector2f > values;
	double a = ((d1 - d0) / (i1 - i0)) * step, d = d0;
	for (double i = i0; i < i1; i += step) {
		values.push_back(Vector2f(i, d));
		d += a;
	}
	values.push_back(Vector2f(i1, d1));
	return values;
}


class Complex_graphic : public Interface_feature {
public:
	Complex_graphic(Vector2f pos, Vector2f size, Text_input* text, Font& font) {
		this->pos = pos;
		this->size = size;
		this->font = font;
		this->text = text;

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		window.setPosition(pos);
		window.setSize(size);
		window.setFillColor(fill_colour);
		window.setOutlineThickness(outline_width);
		window.setOutlineColor(outline_color);

		double iy = (size.y - 2 * bsize) / 3;
		plus.set_position(Vector2f(pos.x + bindent + bsize / 2, pos.y + indent + bsize / 2));
		plus.set_size(Vector2f(bsize, bsize));
		plus.set_icon(fplus());
		plus.set_icon_activation(fcross());

		minus.set_position(Vector2f(pos.x + bindent + bsize / 2, pos.y + 1.5 * bsize + 2 * indent));
		minus.set_size(Vector2f(bsize, bsize));
		minus.set_icon(fminus());
		minus.set_icon_activation(fcross());

		settings.set_position(Vector2f(pos.x + bindent + bsize / 2, pos.y + size.y - indent - bsize / 2));
		settings.set_size(Vector2f(bsize, bsize));
		settings.set_icon(fpoints());
		settings.set_icon_activation(check_mark());

		initial.set_font(font);
		initial.set_pos(Vector2f(pos.x + 2 * bindent + bsize, pos.y + indent));
		initial.set_size(Vector2f(size.x / 2 - 1.5 * indent - bindent - bsize / 2, size.y - 2 * indent));
		initial.set_colours(leftC, rightC, upC, downC);

		final.set_font(font);
		final.set_pos(Vector2f(pos.x + size.x / 2 + 0.5 * indent + bindent + bsize / 2, pos.y + indent));
		final.set_size(Vector2f(size.x / 2 - 1.5 * indent - bindent - bsize / 2, size.y - 2 * indent));
		final.set_draw_lines(false);

		wset = Settings(Vector2f(pos.x + 2 * bindent + bsize, pos.y + indent),
			Vector2f(size.x - indent - 2 * bindent - bsize, size.y - 2 * indent),
			font, Color::White, 
			{ step, initial.get_desired(), 
			double(leftC.r), double(leftC.g), double(leftC.b),
			double(rightC.r), double(rightC.g), double(rightC.b),
			double(upC.r), double(upC.g), double(upC.b),
			double(downC.r), double(downC.g), double(downC.b),
			double(conv), double(paint), double(final.get_draw_lines()) });

		this->set_lines_c();
	}


	void check_target(double x, double y, bool flag, Mouse::Button bt) {
		bool active = settings.activation;
		if (active)
			wset.check_target(x, y, flag);

		if (bt == Mouse::Right && !settings.activation) {
			initial.check_target(x, y, flag);
			final.check_target(x, y, flag);
		}
		if (bt == Mouse::Left) {
			plus.check_activation(flag);
			minus.check_activation(flag);
			settings.check_activation(flag);
			if (active && !settings.activation) {
				this->check_settings();
				wset.reset();
			}
			this->check_button_activation();

			if (initial.cheñk_aff(x, y)) {
				if (plus.activation && !flag) {
					if (alines.size() == 0 || alines[alines.size() - 1].get_build()) {
						alines.push_back(Additional_lines(Vector2f(x, y), &initial));
						precalc.resize(precalc.size() + 1);
					}
					else
						alines[alines.size() - 1].set_pos2(Vector2f(x, y));
				}
				else if (minus.activation && !flag) {
					vector < Additional_lines > tmp;
					vector < vector < Vector2f > > ptmp;
					for (int i = 0; i < alines.size(); i++)
						if (!alines[i].cheñk_belonging(x, y)) {
							tmp.push_back(alines[i]);
							ptmp.push_back(precalc[i]);
						}
					bool flag = false;
					if (tmp.size() < alines.size())
						flag = true;
					alines = tmp;
					precalc = ptmp;
					if (flag)
						this->set_lines_a();
				}
			}
			else if (alines.size() > 0 && !alines[alines.size() - 1].get_build()) {
				alines.pop_back();
				precalc.pop_back();
				this->set_lines_a();
			}

			if (!plus.activation && !minus.activation && !settings.activation)
				for (int i = 0; i < alines.size(); i++)
					alines[i].check_target(x, y, flag);
		}
	}

	void check_input(int code, char type, bool shift = false, bool control = false) {
		if (settings.activation)
			wset.check_input(code, type, shift, control);
		bool update = text->get_string() != exp.get_exp();
		if (update)
			correct = exp.set_exp(text->get_string());
		if (correct && update) {
			this->set_lines_c();
			this->set_lines_a();
			this->set_paint();
			text->set_border_colour(c_colour);
		}
		else if (!correct)
			text->set_border_colour(inc_colour);
	}

	void check_move(double x, double y) {
		if (settings.activation)
			wset.check_move(x, y);

		if (initial.get_move()) {
			initial.check_move(x, y);
			final.set_center(initial.get_center());
			for (int i = 0; i < alines.size(); i++)
				alines[i].set_global_pos();
			this->set_lines_c();
			this->set_lines_a();
			this->set_paint();
		}
		else if (final.get_move()) {
			final.check_move(x, y);
			initial.set_center(final.get_center());
			for (int i = 0; i < alines.size(); i++)
				alines[i].set_global_pos();
			this->set_lines_c();
			this->set_lines_a();
			this->set_paint();
		}
		else if (!settings.activation){
			bool flag = false;
			Vector2f ic = initial.get_global_center(), fc = final.get_global_center();
			for (int i = 0; i < alines.size(); i++) {
				alines[i].check_move(x, y);
				if (alines[i].get_move()) {
					this->update_precalc(i, ic, fc, initial.get_count());
					flag = true;
				}
			}
			if (flag)
				this->set_lines_a();
		}
		if (alines.size() > 0 && !alines[alines.size() - 1].get_build()) {
			alines[alines.size() - 1].set_tmp_pos(Vector2f(x, y));
			this->update_precalc(alines.size() - 1, initial.get_global_center(), final.get_global_center(), initial.get_count());
			this->set_lines_a();
		}
		plus.check_target(x, y);
		minus.check_target(x, y);
		settings.check_target(x, y);
	}

	void check_scrol(double x, double y, double movement) {
		if (initial.cheñk_aff(x, y) && !settings.activation) {
			initial.check_scrol(x, y, movement);
			final.set_center(initial.get_center());
			final.set_count(initial.get_count());
			Vector2f ic = initial.get_global_center(), fc = final.get_global_center();
			for (int i = 0; i < alines.size(); i++) {
				alines[i].set_global_pos();
				this->update_precalc(i, ic, fc, initial.get_count());
			}
			this->set_lines_c();
			this->set_lines_a();
			this->set_paint();
		}
		else if (final.cheñk_aff(x, y) && !settings.activation){
			final.check_scrol(x, y, movement);
			initial.set_center(final.get_center());
			initial.set_count(final.get_count());
			Vector2f ic = initial.get_global_center(), fc = final.get_global_center();
			for (int i = 0; i < alines.size(); i++) {
				alines[i].set_global_pos();
				this->update_precalc(i, ic, fc, initial.get_count());
			}
			this->set_lines_c();
			this->set_lines_a();
			this->set_paint();
		}
	}

	void check_counter() {
		if (settings.activation)
			wset.check_counter();
	}


	void draw(RenderTarget& target, RenderStates states) const {
		target.draw(window, states);
		target.draw(plus, states);
		target.draw(minus, states);
		target.draw(settings, states);
		if (settings.activation) {
			target.draw(wset);
		}
		else {
			target.draw(initial, states);
			target.draw(final, states);
			if (conv)
				for (VertexArray l : clines_coord)
					target.draw(l, states);
			for (VertexArray l : clines_alines)
				target.draw(l, states);
			for (Additional_lines l : alines)
				target.draw(l, states);
		}
	}

private:
	bool correct = true, conv = true, paint = false;
	Vector2f size, pos;
	Coordinate_system initial, final;
	Settings wset;
	Button plus, minus, settings;
	Text_input* text;
	Complex_expression exp;
	vector < Additional_lines > alines;
	vector < VertexArray > clines_coord, clines_alines;
	vector < vector < Vector2f > > precalc;
	RectangleShape window;

	double step = 5, indent = 10, bsize = 40, bindent = 15;
	Color c_colour = Color(215, 75, 9), inc_colour = Color(215, 0, 0), 
		leftC = Color(255, 0, 0), rightC = Color(0, 0, 255), 
		upC = Color(0, 255, 0), downC = Color(0, 255, 255);

	void set_lines_c() {
		if (!conv)
			return;
		VertexArray initial_lines = initial.get_lines();
		clines_coord.clear();
		Vector2f ic = initial.get_global_center(), fc = final.get_global_center();
		double count = initial.get_count();
		for (int i = 0; i < initial_lines.getVertexCount(); i += 2) {
			if (initial_lines[i].position.y == initial_lines[i + 1].position.y)
				clines_coord.push_back(convert_horizontal(initial_lines[i], initial_lines[i + 1], ic, fc, count));
			else
				clines_coord.push_back(convert_vertical(initial_lines[i], initial_lines[i + 1], ic, fc, count));
		}
	}

	void set_lines_a() {
		clines_alines.clear();
		Vector2f ic = initial.get_global_center(), fc = final.get_global_center();
		double count = initial.get_count();
		for (int i = 0; i < alines.size(); i++)
			clines_alines.push_back(convert(i, alines[i].get_mline()[0].color, ic, fc, count));
	}

	void set_paint() {
		if (!paint) {
			initial.additional_layer = VertexArray();
			final.additional_layer = VertexArray();
			return;
		}
		VertexArray paint_left(Quads), paint_right(Quads);
		Vect2 ic = initial.get_global_center(), p0 = initial.get_pos(), sz = initial.get_size();
		double count = initial.get_count();
		for (Vect2 p = p0; p.y < p0.y + sz.y; p += Vect2(0, step)) {
			for (p.x = p0.x; p.x < p0.x + sz.x; p += Vect2(step, 0)) {
				Vect2 pr(min(p.x + step, p0.x + sz.x), p.y);
				Vect2 pd(p.x, min(p.y + step, p0.y + sz.y));
				Vect2 prd(min(p.x + step, p0.x + sz.x), min(p.y + step, p0.y + sz.y));
				paint_left.append(Vertex(p.get_Vector2f(), get_color(exp.calc(Complex((p - ic) / count)).get_vet2())));
				paint_left.append(Vertex(pr.get_Vector2f(), get_color(exp.calc(Complex((pr - ic) / count)).get_vet2())));
				paint_left.append(Vertex(prd.get_Vector2f(), get_color(exp.calc(Complex((prd - ic) / count)).get_vet2())));
				paint_left.append(Vertex(pd.get_Vector2f(), get_color(exp.calc(Complex((pd - ic) / count)).get_vet2())));
			}
		}
		Vect2 fc = final.get_global_center();
		p0 = final.get_pos();
		sz = final.get_size();
		count = final.get_count();
		for (Vect2 p = p0; p.y < p0.y + sz.y; p += Vect2(0, step)) {
			for (p.x = p0.x; p.x < p0.x + sz.x; p += Vect2(step, 0)) {
				Vect2 pr(min(p.x + step, p0.x + sz.x), p.y);
				Vect2 pd(p.x, min(p.y + step, p0.y + sz.y));
				Vect2 prd(min(p.x + step, p0.x + sz.x), min(p.y + step, p0.y + sz.y));
				paint_right.append(Vertex(p.get_Vector2f(), get_color((p - fc) / count)));
				paint_right.append(Vertex(pr.get_Vector2f(), get_color((pr - fc) / count)));
				paint_right.append(Vertex(prd.get_Vector2f(), get_color((prd - fc) / count)));
				paint_right.append(Vertex(pd.get_Vector2f(), get_color((pd - fc) / count)));
			}
		}
		initial.additional_layer = paint_left;
		final.additional_layer = paint_right;
	}

	void check_button_activation() {
		if (settings.activation) {
			plus.set_inactive();
			minus.set_inactive();
		}
		else {
			if (plus.activation)
				minus.set_inactive();
			else
				minus.set_active();

			if (minus.activation)
				plus.set_inactive();
			else
				plus.set_active();
		}
	}

	void check_settings() {
		step = wset.pv;
		leftC = wset.clsv[0];
		rightC = wset.clsv[1];
		upC = wset.clsv[2];
		downC = wset.clsv[3];
		conv = wset.converting_button.activation;
		paint = wset.coloring_button.activation;

		initial.set_desired(wset.lv);
		initial.set_colours(leftC, rightC, upC, downC);

		final.set_desired(wset.lv);
		final.set_draw_lines(wset.show_right_button.activation);

		Vector2f ic = initial.get_global_center(), fc = final.get_global_center();
		for (int i = 0; i < alines.size(); i++)
			this->update_precalc(i, ic, fc, initial.get_count());
		this->set_lines_c();
		this->set_lines_a();
		this->set_paint();
	}

	void update_precalc(int i, Vector2f ic, Vector2f fc, double count) {
		Vector2f p0 = alines[i].get_mline()[0].position, p1 = alines[i].get_mline()[1].position;

		p0.x = (p0.x - ic.x) / count;
		p0.y = (ic.y - p0.y) / count;
		p1.x = (p1.x - ic.x) / count;
		p1.y = (ic.y - p1.y) / count;

		vector < Vector2f > xy;
		if (abs(p1.x - p0.x) > abs(p1.y - p0.y)) {
			if (p0.x > p1.x)
				swap(p0, p1);
			xy = interpolate(p0.x, p0.y, p1.x, p1.y, step / count);
		}
		else {
			if (p0.y > p1.y)
				swap(p0, p1);
			xy = interpolate(p0.y, p0.x, p1.y, p1.x, step / count);
			for (int i = 0; i < xy.size(); i++)
				swap(xy[i].x, xy[i].y);
		}
		precalc[i] = xy;
	}

	VertexArray convert_horizontal(Vertex p1, Vertex p2, Vector2f ic, Vector2f fc, double count) {
		double x1 = (p1.position.x - ic.x) / count, x2 = (p2.position.x - ic.x) / count, y = (ic.y - p1.position.y) / count;
		if (x1 > x2)
			swap(x1, x2);

		VertexArray result(Lines);
		Vector2f last_point;
		bool last_flag, flag, work = true;
		for (double x = x1; work; x += step / count) {
			if (x >= x2) {
				work = false;
				x = x2;
			}
			Complex c(x, y);

			c = exp.calc(c); // function(c);

			Vector2f np(c.re, c.im);
			np.x = np.x * count + fc.x;
			np.y = fc.y - np.y * count;

			flag = final.cheñk_aff(np.x, np.y);
			if (x != x1) {
				if (last_flag && flag) {
					result.append(Vertex(np, p1.color));
					result.append(Vertex(np, p1.color));
				}
				else if (!last_flag && flag) {
					result.append(Vertex(final.get_intersection(np, last_point).first, p1.color));
					result.append(Vertex(np, p1.color));
					result.append(Vertex(np, p1.color));
				}
				else if (last_flag && !flag)
					result.append(Vertex(final.get_intersection(np, last_point).first, p1.color));
				else {
					pair < Vector2f, Vector2f > P = final.get_intersection(np, last_point);
					if (P.second.x != INF) {
						result.append(Vertex(P.first, p1.color));
						result.append(Vertex(P.second, p1.color));
					}
				}
			}
			else if (flag)
				result.append(Vertex(np, p1.color));

			last_flag = flag;
			last_point = np;
		}
		return result;
	}

	VertexArray convert_vertical(Vertex p1, Vertex p2, Vector2f ic, Vector2f fc, double count) {
		double y1 = (ic.y - p1.position.y) / count, y2 = (ic.y - p2.position.y) / count, x = (p1.position.x - ic.x) / count;
		if (y1 > y2)
			swap(y1, y2);

		VertexArray result(Lines);
		Vector2f last_point;
		bool last_flag, flag, work = true;
		for (double y = y1; work; y += step / count) {
			if (y >= y2) {
				work = false;
				y = y2;
			}
			Complex c(x, y);

			c = exp.calc(c); // function(c);

			Vector2f np(c.re, c.im);
			np.x = np.x * count + fc.x;
			np.y = fc.y - np.y * count;

			flag = final.cheñk_aff(np.x, np.y);
			if (y != y1) {
				if (last_flag && flag) {
					result.append(Vertex(np, p1.color));
					result.append(Vertex(np, p1.color));
				}
				else if (!last_flag && flag) {
					result.append(Vertex(final.get_intersection(np, last_point).first, p1.color));
					result.append(Vertex(np, p1.color));
					result.append(Vertex(np, p1.color));
				}
				else if (last_flag && !flag)
					result.append(Vertex(final.get_intersection(np, last_point).first, p1.color));
				else {
					pair < Vector2f, Vector2f > P = final.get_intersection(np, last_point);
					if (P.second.x != INF) {
						result.append(Vertex(P.first, p1.color));
						result.append(Vertex(P.second, p1.color));
					}
				}
			}
			else if (flag)
				result.append(Vertex(np, p1.color));

			last_flag = flag;
			last_point = np;
		}
		return result;
	}

	VertexArray convert(int ind, Color col, Vector2f ic, Vector2f fc, double count) {
		VertexArray result(Lines);
		Vector2f last_point;
		bool last_flag, flag;
		for (int i = 0; i < precalc[ind].size(); i++) {
			Complex c(precalc[ind][i].x, precalc[ind][i].y);

			c = exp.calc(c); // function(c);

			Vector2f np(c.re, c.im);
			np.x = np.x * count + fc.x;
			np.y = fc.y - np.y * count;

			flag = final.cheñk_aff(np.x, np.y);
			if (i) {
				if (last_flag && flag) {
					result.append(Vertex(np, col));
					result.append(Vertex(np, col));
				}
				else if (!last_flag && flag) {
					result.append(Vertex(final.get_intersection(np, last_point).first, col));
					result.append(Vertex(np, col));
					result.append(Vertex(np, col));
				}
				else if (last_flag && !flag)
					result.append(Vertex(final.get_intersection(np, last_point).first, col));
				else {
					pair < Vector2f, Vector2f > p = final.get_intersection(np, last_point);
					if (p.second.x != INF) {
						result.append(Vertex(p.first, col));
						result.append(Vertex(p.second, col));
					}
				}
			}
			else if (flag)
				result.append(Vertex(np, col));

			last_flag = flag;
			last_point = np;
		}
		return result;
	}
};