VertexArray fcross(Color c = Color::White) {
	VertexArray res(Quads);

	res.append(Vertex(Vector2f(-0.3, -0.4), c));
	res.append(Vertex(Vector2f(-0.4, -0.3), c));
	res.append(Vertex(Vector2f(0.3, 0.4), c));
	res.append(Vertex(Vector2f(0.4, 0.3), c));

	res.append(Vertex(Vector2f(0.3, -0.4), c));
	res.append(Vertex(Vector2f(0.4, -0.3), c));
	res.append(Vertex(Vector2f(-0.3, 0.4), c));
	res.append(Vertex(Vector2f(-0.4, 0.3), c));

	return res;
}

VertexArray fplus(Color c = Color::White) {
	VertexArray res(Quads);

	res.append(Vertex(Vector2f(-0.4, -0.1), c));
	res.append(Vertex(Vector2f(-0.4, 0.1), c));
	res.append(Vertex(Vector2f(0.4, 0.1), c));
	res.append(Vertex(Vector2f(0.4, -0.1), c));

	res.append(Vertex(Vector2f(-0.1, -0.4), c));
	res.append(Vertex(Vector2f(-0.1, 0.4), c));
	res.append(Vertex(Vector2f(0.1, 0.4), c));
	res.append(Vertex(Vector2f(0.1, -0.4), c));

	return res;
}

VertexArray fminus(Color c = Color::White) {
	VertexArray res(Quads);

	res.append(Vertex(Vector2f(-0.4, -0.1), c));
	res.append(Vertex(Vector2f(-0.4, 0.1), c));
	res.append(Vertex(Vector2f(0.4, 0.1), c));
	res.append(Vertex(Vector2f(0.4, -0.1), c));

	return res;
}

VertexArray fpoints(Color c = Color::White) {
	VertexArray res(Quads);
	double a = 0.1, d = 0.07;

	res.append(Vertex(Vector2f(0, a), c));
	res.append(Vertex(Vector2f(a, 0), c));
	res.append(Vertex(Vector2f(0, -a), c));
	res.append(Vertex(Vector2f(-a, 0), c));

	res.append(Vertex(Vector2f(2 * a + d, a), c));
	res.append(Vertex(Vector2f(3 * a + d, 0), c));
	res.append(Vertex(Vector2f(2 * a + d, -a), c));
	res.append(Vertex(Vector2f(a + d, 0), c));

	res.append(Vertex(Vector2f(-2 * a - d, a), c));
	res.append(Vertex(Vector2f(-a - d, 0), c));
	res.append(Vertex(Vector2f(-2 * a - d, -a), c));
	res.append(Vertex(Vector2f(-3 * a - d, 0), c));

	return res;
}

VertexArray check_mark(Color c = Color::White) {
	VertexArray res(Quads);
	double a = 0.25, b = 0.3 / sqrt(2), x = 0.5 / sqrt(2), d = 0.08;
	double l = (x - b) / 2;

	res.append(Vertex(Vector2f(-l, d), c));
	res.append(Vertex(Vector2f(-l, a + d), c));
	res.append(Vertex(Vector2f(-b - a / 2 - l, -b + a / 2 + d), c));
	res.append(Vertex(Vector2f(-b - l, -b + d), c));

	res.append(Vertex(Vector2f(-l, d), c));
	res.append(Vertex(Vector2f(-l, a + d), c));
	res.append(Vertex(Vector2f(x + a / 2 - l, -x + a / 2 + d), c));
	res.append(Vertex(Vector2f(x - l, -x + d), c));

	return res;
}


class Button : public Interface_feature {
public:
	bool activation = false;

	Button(Vector2f position, Vector2f size, VertexArray icon, VertexArray icon_activation, bool activation = false) {
		pos = position;
		this->size = size;
		this->icon = icon;
		this->icon_activation = icon_activation;
		this->activation = activation;

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		button.setPointCount(8);
		button.setFillColor(fill_colour);
		button.setOutlineThickness(outline_width);
		button.setOutlineColor(outline_color);
		this->set_button();
		this->set_icon();

		button_target.setPrimitiveType(TriangleFan);
		button_target.resize(10);
	}

	Button() {
		pos = Vector2f(0, 0);
		size = Vector2f(0, 0);

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		button.setPointCount(8);
		button.setFillColor(fill_colour);
		button.setOutlineThickness(outline_width);
		button.setOutlineColor(outline_color);
		this->set_button();
		this->set_icon();

		button_target.setPrimitiveType(TriangleFan);
		button_target.resize(10);
	}


	void set_position(Vector2f position) {
		pos = position;
		this->set_button();
		this->set_icon();
	}

	void set_size(Vector2f size) {
		this->size = size;
		this->set_button();
		this->set_icon();
	}

	inline void set_icon(VertexArray icon) {
		this->icon = icon;
		this->set_icon();
	}

	inline void set_icon_activation(VertexArray icon_activation) {
		this->icon_activation = icon_activation;
		this->set_icon();
	}

	void set_active() {
		mcolour = 1;
		active = true;
		this->set_icon();
		this->set_button();
		if (target)
			this->set_button_target();
	}

	void set_inactive() {
		mcolour = mina;
		active = false;
		this->set_icon();
		this->set_button();
		if (target)
			this->set_button_target();
	}


	void check_target(double x, double y) {
		if (active && x >= pos.x - size.x / 2 && x <= pos.x + size.x / 2 && y >= pos.y - size.y / 2 && y <= pos.y + size.y / 2) {
			mouse_position.x = x;
			mouse_position.y = y;
			this->set_button_target();
			target = true;
		}
		else
			target = false;
	}

	void check_activation(bool flag) {
		if (active) {
			if (scaled) {
				size.x /= scale;
				size.y /= scale;
				scaled = false;
				this->set_icon();
				this->set_button();
				if (target)
					this->set_button_target();
			}
			if (!flag && target)
				if (activation)
					activation = false;
				else
					activation = true;
			else if (flag && target) {
				size.x *= scale;
				size.y *= scale;
				scaled = true;
				this->set_icon();
				this->set_button();
				this->set_button_target();
			}
		}
	}


	void draw(RenderTarget& target, RenderStates states) const {
		target.draw(button, states);
		if (this->target)
			target.draw(button_target, states);
		if (activation)
			target.draw(icon_activationAP, states);
		else
			target.draw(iconAP, states);
	}

private:
	bool target = false, scaled = false, active = true;
	double mcolour = 1;
	Vector2f mouse_position;
	VertexArray button_target, icon, icon_activation, iconAP, icon_activationAP;
	ConvexShape button;

	double ratio = 0.1, scale = 1.03, mina = 0.6;
	Color interpolate_colour = Color(255, 0, 0, 150);

	void set_icon() {
		double w = size.x, h = size.y, r = ratio;
		iconAP = icon;
		for (size_t i = 0; i < iconAP.getVertexCount(); i++) {
			iconAP[i].position = Vector2f(w * iconAP[i].position.x + pos.x, h * iconAP[i].position.y + pos.y);
			iconAP[i].color = (Vect3(iconAP[i].color) * mcolour).get_Color();
		}
		icon_activationAP = icon_activation;
		for (size_t i = 0; i < icon_activationAP.getVertexCount(); i++) {
			icon_activationAP[i].position = Vector2f(w * icon_activationAP[i].position.x + pos.x, h * icon_activationAP[i].position.y + pos.y);
			icon_activationAP[i].color = (Vect3(icon_activationAP[i].color) * mcolour).get_Color();
		}
	}

	void set_button() {
		double w = size.x, h = size.y, r = ratio;
		button.setPoint(0, Vector2f(pos.x + (r - 0.5) * w, pos.y - 0.5 * h));
		button.setPoint(1, Vector2f(pos.x + (0.5 - r) * w, pos.y - 0.5 * h));
		button.setPoint(2, Vector2f(pos.x + 0.5 * w, pos.y + (r - 0.5) * h));
		button.setPoint(3, Vector2f(pos.x + 0.5 * w, pos.y + (0.5 - r) * h));
		button.setPoint(4, Vector2f(pos.x + (0.5 - r) * w, pos.y + 0.5 * h));
		button.setPoint(5, Vector2f(pos.x + (r - 0.5) * w, pos.y + 0.5 * h));
		button.setPoint(6, Vector2f(pos.x - 0.5 * w, pos.y + (0.5 - r) * h));
		button.setPoint(7, Vector2f(pos.x - 0.5 * w, pos.y + (r - 0.5) * h));
		button.setFillColor((Vect3(fill_colour) * mcolour).get_Color());
		button.setOutlineColor((Vect3(outline_color) * mcolour).get_Color());
	}

	void set_button_target() {
		double w = size.x, h = size.y, r = ratio;
		button_target[0].position = Vector2f(mouse_position.x, mouse_position.y);
		button_target[0].color = interpolate_colour;
		button_target[1].position = Vector2f(pos.x + (r - 0.5) * w, pos.y - 0.5 * h);
		button_target[2].position = Vector2f(pos.x + (0.5 - r) * w, pos.y - 0.5 * h);
		button_target[3].position = Vector2f(pos.x + 0.5 * w, pos.y + (r - 0.5) * h);
		button_target[4].position = Vector2f(pos.x + 0.5 * w, pos.y + (0.5 - r) * h);
		button_target[5].position = Vector2f(pos.x + (0.5 - r) * w, pos.y + 0.5 * h);
		button_target[6].position = Vector2f(pos.x + (r - 0.5) * w, pos.y + 0.5 * h);
		button_target[7].position = Vector2f(pos.x - 0.5 * w, pos.y + (0.5 - r) * h);
		button_target[8].position = Vector2f(pos.x - 0.5 * w, pos.y + (r - 0.5) * h);
		button_target[9].position = Vector2f(pos.x + (r - 0.5) * w, pos.y - 0.5 * h);
		for (int i = 1; i < 10; i++)
			button_target[i].color = fill_colour;
	}
};