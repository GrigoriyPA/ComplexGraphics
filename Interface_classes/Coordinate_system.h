class Coordinate_system : public Interface_feature {
public:
	VertexArray additional_layer;

	Coordinate_system(Vector2f pos, Vector2f size, Font& font) {
		this->pos = pos;
		this->size = size;
		this->font = font;

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		window.setPosition(pos);
		window.setSize(size);
		window.setFillColor(fill_colour);
		window.setOutlineThickness(outline_width);
		window.setOutlineColor(outline_color);

		clines.setPrimitiveType(Lines);
		arrows.setPrimitiveType(Lines);
		TH.setFont(font);
		TH.setOutlineColor(coordinates_colour);
		TH.setCharacterSize(text_size);
		TV.setFont(font);
		TV.setOutlineColor(coordinates_colour);
		TV.setCharacterSize(text_size);

		this->set_count();
		this->set_lines();
	}

	Coordinate_system() {
		pos = Vector2f(0, 0);
		size = Vector2f(0, 0);

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		window.setPosition(pos);
		window.setSize(size);
		window.setFillColor(fill_colour);
		window.setOutlineThickness(outline_width);
		window.setOutlineColor(outline_color);

		clines.setPrimitiveType(Lines);
		arrows.setPrimitiveType(Lines);
		TH.setCharacterSize(text_size);
		TH.setOutlineColor(coordinates_colour);
		TV.setCharacterSize(text_size);
		TV.setOutlineColor(coordinates_colour);

		this->set_count();
	}


	void set_pos(Vector2f pos) {
		this->pos = pos;
		window.setPosition(pos);
		this->set_lines();
	}

	void set_size(Vector2f size) {
		this->size = size;
		window.setSize(size);
		this->set_lines();
	}

	void set_count(double count) {
		this->count = count;
		this->set_count();
		this->set_lines();
	}

	inline void set_center(Vector2f center) {
		this->center = center;
		this->set_lines();
	}

	inline void set_draw_lines(bool draw_lines) {
		this->draw_lines = draw_lines;
		this->set_lines();
	}

	inline void set_font(Font& font) {
		this->font = font;
		TH.setFont(font);
		TV.setFont(font);
		this->set_lines();
	}

	void set_colours(Color leftC, Color rightC, Color upC, Color downC) {
		this->leftC = leftC;
		this->rightC = rightC;
		this->upC = upC;
		this->downC = downC;
		this->set_lines();
	}

	void set_desired(double desired) {
		this->desired = desired;
		while (this->set_count()) {}
		this->set_lines();
	}


	inline Vector2f get_center() {
		return center;
	}

	inline Vector2f get_global_center() {
		return Vector2f(center.x * size.x + pos.x, center.y * size.y + pos.y);
	}

	inline Vector2f get_pos() {
		return pos;
	}

	inline Vector2f get_size() {
		return size;
	}

	inline double get_count() {
		return count;
	}

	inline double get_desired() {
		return desired;
	}

	inline bool get_draw_lines() {
		return draw_lines;
	}

	inline bool get_move() {
		return move;
	}

	inline VertexArray get_lines() {
		return clines;
	}

	pair < Vector2f, Vector2f > get_intersection(Vector2f c1, Vector2f c2) {
		vector < Vect2 > p = { pos, Vect2(pos.x + size.x, pos.y), Vect2(pos.x + size.x, pos.y + size.y), Vect2(pos.x, pos.y + size.y) };
		pair < Vector2f, Vector2f > res(Vector2f(INF, INF), Vector2f(INF, INF));
		Line2 l(c1, c2);
		Vect2 v1(c1), v2(c2);
		int cnt = int(!cheñk_aff(c1.x, c1.y)) + int(!cheñk_aff(c2.x, c2.y));
		for (int i = 0; i < 4 && cnt; i++) {
			Vect2 p1 = p[i], p2 = p[(i + 1) % 4], pa(INF, INF);
			if (p1.x == p2.x && l.N.y != 0) {
				Vect2 pi(p1.x, (-l.d - l.N.x * p1.x) / l.N.y);
				if (pi.y >= min(p1.y, p2.y) && pi.y <= max(p1.y, p2.y) && (v1 - pi) * (v2 - pi) < EPS)
					pa = pi;
			}
			else if (p1.y == p2.y && l.N.x != 0) {
				Vect2 pi((-l.d - l.N.y * p1.y) / l.N.x, p1.y);
				if (pi.x >= min(p1.x, p2.x) && pi.x <= max(p1.x, p2.x) && (v1 - pi) * (v2 - pi) < EPS)
					pa = pi;
			}
			if (pa.x != INF) {
				cnt--;
				if (res.first.x == INF)
					res.first = pa.get_Vector2f();
				else
					res.second = pa.get_Vector2f();
			}
		}
		return res;
	}


	void check_target(double x, double y, bool flag) {
		if (flag && this->cheñk_aff(x, y)) {
			move = true;
			mouse = Vector2f(x, y);
		}
		else
			move = false;
	}

	void check_move(double x, double y) {
		if (move) {
			double dx = x - mouse.x, dy = y - mouse.y;
			mouse = Vector2f(x, y);
			center = Vector2f(center.x + dx / size.x, center.y + dy / size.y);
			this->set_lines();
		}
	}

	void check_scrol(double x, double y, double movement) {
		if (this->cheñk_aff(x, y)) {
			x -= pos.x;
			y -= pos.y;
			if (movement > 0) {
				center = Vector2f((x + (center.x * size.x - x) * movement * sensitivity) / size.x, (y + (center.y * size.y - y) * movement * sensitivity) / size.y);
				count *= movement * sensitivity;
			}
			else {
				center = Vector2f((x + (x - center.x * size.x) / (movement * sensitivity)) / size.x, (y + (y - center.y * size.y) / (movement * sensitivity)) / size.y);
				count /= -movement * sensitivity;
			}
			this->set_count();
			this->set_lines();
		}
	}

	inline bool cheñk_aff(double x, double y) {
		return x >= pos.x && x <= pos.x + size.x && y >= pos.y && y <= pos.y + size.y;
	}


	void draw(RenderTarget& target, RenderStates states) const {
		target.draw(window, states);
		target.draw(additional_layer, states);
		target.draw(clines, states);
		target.draw(arrows, states);
		for (Text text : coordinates)
			target.draw(text, states);
		if (TH_vis)
			target.draw(TH, states);
		if (TV_vis)
			target.draw(TV, states);
	}

private:
	bool move = false, draw_lines = true, TH_vis, TV_vis;
	Vector2f mouse;
	vector < Text > coordinates;
	Text TH, TV;
	VertexArray clines, arrows;
	RectangleShape window;
	
	int text_size = 20;
	double count = 70, desired = 70, current = 1, mult = 1, pow = 0, sensitivity = 1.1, arrow_indent = 6, arrow_pr = 12, indent = 6;
	Vector2f center = Vector2f(0.5, 0.5);
	Vect3 leftC = Vect3(255, 255, 255), rightC = Vect3(255, 255, 255), upC = Vect3(255, 255, 255), downC = Vect3(255, 255, 255);
	string h = "re", v = "im";
	Color coordinates_colour = Color::White, lines_colour = Color::White;

	bool set_count() {
		if (current * mult * count > desired) {
			double nwc = 5, nwp = mult;
			if (current == 5)
				nwc = 2;
			else if (current == 2)
				nwc = 1;
			else
				nwp /= 10;
			if (current * mult * count - desired >= abs(nwc * nwp * count - desired)) {
				current = nwc;
				if (nwp < mult)
					pow--;
				mult = nwp;
				return true;
			}
		}
		else if (current * mult * count < desired) {
			double nwc = 1, nwp = mult;
			if (current == 5)
				nwp *= 10;
			else if (current == 2)
				nwc = 5;
			else
				nwc = 2;
			if (desired - current * mult * count >= abs(nwc * nwp * count - desired)) {
				current = nwc;
				if (nwp > mult)
					pow++;
				mult = nwp;
				return true;
			}
		}
		return false;
	}

	void set_lines() {
		clines.clear();
		arrows.clear();
		coordinates.clear();
		Vector2f ce(center.x * size.x, center.y * size.y);
		double step = current * mult * count, x = fmod(ce.x, step);

		double H_indent = indent;
		if (pow == 0) {
			TH.setString(h);
			TV.setString(v);
			H_indent *= 2;
		}
		else if (pow == 1) {
			TH.setString(h + ", 10");
			TV.setString(v + ", 10");
		}
		else {
			TH.setString(h + ", 10^" + to_string(int(pow)));
			TV.setString(v + ", 10^" + to_string(int(pow)));
		}
		TH.setPosition(Vector2f(pos.x + size.x - TH.getGlobalBounds().width - indent - 1, min(max(ce.y + indent / 2, double(0)), size.y - TH.getGlobalBounds().height - H_indent - 1) + pos.y));
		TV.setPosition(Vector2f(min(max(ce.x - TV.getGlobalBounds().width - indent - 1, double(0)), double(size.x - TV.getGlobalBounds().width - indent - 1)) + pos.x, indent / 2 + pos.y));
		TH_vis = size.x - TH.getGlobalBounds().width - indent - 1 >= ce.x;
		TV_vis = indent / 2 + TV.getGlobalBounds().height <= ce.y;

		if (x < 0)
			x += step;
		double counterx = (x - ce.x) / count;
		long long cx = (x - ce.x) / (count * mult);
		if (abs(counterx / mult - cx) > abs(counterx / mult - cx + 1))
			cx--;
		while (x < size.x) {
			double k = (counterx * count + ce.x) / size.x;
			Color cl = (leftC * (1 - k) + rightC * k).get_Color();
			if (eqt(x, ce.x)) {
				arrows.append(Vertex(Vector2f(pos.x + x + 1, pos.y), cl));
				arrows.append(Vertex(Vector2f(pos.x + x + 1, pos.y + size.y), cl));
				arrows.append(Vertex(Vector2f(pos.x + x - 1, pos.y), cl));
				arrows.append(Vertex(Vector2f(pos.x + x - 1, pos.y + size.y), cl));
				clines.append(Vertex(Vector2f(pos.x + x, pos.y), cl));
				clines.append(Vertex(Vector2f(pos.x + x, pos.y + size.y), cl));
			}
			else if (draw_lines) {
				clines.append(Vertex(Vector2f(pos.x + x, pos.y), cl));
				clines.append(Vertex(Vector2f(pos.x + x, pos.y + size.y), cl));
			}

			if (x <= size.x - TH.getGlobalBounds().width - indent - 1) {
				Text text;
				text.setFont(font);
				text.setOutlineColor(coordinates_colour);
				text.setCharacterSize(text_size);
				text.setString(to_string(cx));
				text.setPosition(Vector2f(max(x - text.getGlobalBounds().width - indent - 1, double(0)) + pos.x, min(max(ce.y + indent / 2, double(0)), size.y - text.getGlobalBounds().height - indent - 1) + pos.y));
				coordinates.push_back(text);
			}

			x += step;
			cx += current;
			counterx += current * mult;
		}
		double y = fmod(ce.y, step);
		if (y < 0)
			y += step;
		double countery = (ce.y - y) / count;
		long long cy = (ce.y - y) / (count * mult);
		if (abs(countery / mult - cy) > abs(countery / mult - cy - 1))
			cy++;
		while (y < size.y) {
			double k = (ce.y - countery * count) / size.y;
			Color cl = (upC * (1 - k) + downC * k).get_Color();
			if (eqt(y, ce.y)) {
				arrows.append(Vertex(Vector2f(pos.x, pos.y + y + 1), cl));
				arrows.append(Vertex(Vector2f(pos.x + size.x, pos.y + y + 1), cl));
				arrows.append(Vertex(Vector2f(pos.x, pos.y + y - 1), cl));
				arrows.append(Vertex(Vector2f(pos.x + size.x, pos.y + y - 1), cl));
				clines.append(Vertex(Vector2f(pos.x, pos.y + y), cl));
				clines.append(Vertex(Vector2f(pos.x + size.x, pos.y + y), cl));
			}
			else if (draw_lines) {
				clines.append(Vertex(Vector2f(pos.x, pos.y + y), cl));
				clines.append(Vertex(Vector2f(pos.x + size.x, pos.y + y), cl));
			}

			if (y >= TV.getGlobalBounds().height + indent / 2) {
				Text text;
				text.setOutlineColor(coordinates_colour);
				text.setFont(font);
				text.setCharacterSize(text_size);
				text.setString(to_string(cy));
				text.setPosition(Vector2f(min(max(ce.x - text.getGlobalBounds().width - indent - 1, double(0)), double(size.x - text.getGlobalBounds().width - indent - 1)) + pos.x, min(y + indent / 2, size.y - text.getGlobalBounds().height - indent - 1) + pos.y));
				coordinates.push_back(text);
			}

			y += step;
			cy -= current;
			countery -= current * mult;
		}
		if (ce.x > 0 && ce.x < size.x) {
			arrows.append(Vertex(Vector2f(pos.x + ce.x, pos.y), lines_colour));
			arrows.append(Vertex(Vector2f(pos.x + ce.x - arrow_indent, pos.y + arrow_pr), lines_colour));
			arrows.append(Vertex(Vector2f(pos.x + ce.x, pos.y), lines_colour));
			arrows.append(Vertex(Vector2f(pos.x + ce.x + arrow_indent, pos.y + arrow_pr), lines_colour));
		}
		if (ce.y > 0 && ce.y < size.y) {
			arrows.append(Vertex(Vector2f(pos.x + size.x, pos.y + ce.y), lines_colour));
			arrows.append(Vertex(Vector2f(pos.x + size.x - arrow_pr, pos.y + ce.y - arrow_indent), lines_colour));
			arrows.append(Vertex(Vector2f(pos.x + size.x, pos.y + ce.y), lines_colour));
			arrows.append(Vertex(Vector2f(pos.x + size.x - arrow_pr, pos.y + ce.y + arrow_indent), lines_colour));
		}
	}
};