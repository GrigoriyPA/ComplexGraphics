class Text_input : public Interface_feature {
public:
	Text_input(Vector2f position, Vector2f size, Font& font, double margin_top = 6, string memory = "") {
		pos = position;
		this->size = size;
		this->memory = memory;
		this->font = font;
		this->margin_top = margin_top;
		this->add_in_history();

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		cursor.setFillColor(outline_color);
		cursor.setPosition(Vector2f(position.x + margin_left, position.y + margin_top));
		cursor.setSize(Vector2f(cursor_width, size.y - margin_top));

		entry_field.setPosition(position);
		entry_field.setSize(size);
		entry_field.setFillColor(fill_colour);
		entry_field.setOutlineThickness(outline_width);
		entry_field.setOutlineColor(outline_color);

		text.setString(memory);
		text.setFont(font);
		text.setCharacterSize(size.y - margin_top * 2);
		text.setFillColor(font_colour);
		text.setPosition(Vector2f(position.x + margin_left, position.y + margin_top));
	}
	
	Text_input() {
		this->add_in_history();

		fill_colour = Color::Black;
		outline_color = Color(215, 75, 9);
		outline_width = 2;

		cursor.setFillColor(outline_color);
		cursor.setPosition(Vector2f(pos.x + margin_left, pos.y + margin_top));
		cursor.setSize(Vector2f(cursor_width, size.y - margin_top));

		entry_field.setPosition(pos);
		entry_field.setSize(size);
		entry_field.setFillColor(fill_colour);
		entry_field.setOutlineThickness(outline_width);
		entry_field.setOutlineColor(outline_color);

		text.setString(memory);
		text.setCharacterSize(size.y - margin_top * 2);
		text.setFillColor(font_colour);
		text.setPosition(Vector2f(pos.x + margin_left, pos.y + margin_top));
	}


	inline void set_border_colour(Color col) {
		entry_field.setOutlineColor(col);
	}

	void set_string(string s) {
		memory = s;
		history.clear();
		text_left = "";
		text_left_vis = "";
		this->set_text();
	}


	inline string get_string() {
		return memory;
	}

	inline double get_character_size() {
		return text.getCharacterSize();
	}


	void check_target(double x, double y) {
		if (x >= pos.x && x <= pos.x + size.x && y >= pos.y && y <= pos.y + size.y) {
			target = true;
			x -= pos.x + margin_left;
			if (x <= 0)
				text_left = "";
			else if (x >= text.getGlobalBounds().width)
				text_left = memory;
			else {
				Text search("", *text.getFont(), text.getCharacterSize());
				string search_build;
				double last = 0;
				text_left = "";

				for (char sign : memory) {
					search_build += sign;
					search.setString(search_build);
					double actual = search.getGlobalBounds().width;
					if (x >= actual) {
						last = actual;
						text_left += sign;
					}
					else {
						if (x > (last + actual) / 2)
							text_left += sign;
						break;
					}
				}
			}
			this->set_text();
		}
		else
			target = false;
		cursor_visibility = true;
		counter = 0;
	}

	void check_input(int code, char type, bool shift = false, bool control = false) {
		if (target) {
			if (type == 't' && code >= 32) {
				char sign = code;
				text_left += sign;
				memory.insert(text_left.length() - 1, 1, sign);
				this->set_text();
				this->add_in_history();
			}
			else if (type == 's') {
				switch (code) {
					case 25: {
						if (control && shift && ind < history.size() - 1) {
							ind++;
							text_left = history[ind].second;
							memory = history[ind].first;
							this->set_text();
							cursor_visibility = true;
							counter = 0;
						}
						else if (control && !shift && ind > 0) {
							ind--;
							text_left = history[ind].second;
							memory = history[ind].first;
							this->set_text();
							cursor_visibility = true;
							counter = 0;
						}
						break;
					}

					case 58: {
						target = false;
						cursor_visibility = true;
						counter = 0;
						break;
					}

					case 59: {
						if (text_left.size() > 0) {
							text_left.pop_back();
							memory.erase(text_left.length(), 1);
							this->set_text();
							this->add_in_history();
							cursor_visibility = true;
							counter = 0;
						}
						break;
					}

					case 66: {
						if (text_left.size() < memory.size()) {
							memory.erase(text_left.length(), 1);
							this->set_text();
							this->add_in_history();
							cursor_visibility = true;
							counter = 0;
						}
						break;
					}

					case 63: {
						text_left = memory;
						this->set_text();
						cursor_visibility = true;
						counter = 0;
						break;
					}

					case 64: {
						text_left = "";
						this->set_text();
						cursor_visibility = true;
						counter = 0;
						break;
					}

					case 71: {
						if (text_left.size() > 0) {
							text_left.pop_back();
							this->set_text();
							cursor_visibility = true;
							counter = 0;
						}
						break;
					}

					case 72: {
						if (text_left.size() < memory.size()) {
							text_left += memory[text_left.size()];
							this->set_text();
							cursor_visibility = true;
							counter = 0;
						}
						break;
					}

					default: {
						break;
					}
				}
			}
		}
	}

	void check_counter() {
		if (target) {
			counter++;
			if (counter == max_counter) {
				counter = 0;
				if (cursor_visibility)
					cursor_visibility = false;
				else
					cursor_visibility = true;
			}
		}
	}


	void draw(RenderTarget& target, RenderStates states) const {
		target.draw(entry_field, states);
		target.draw(text, states);
		if (this->target && cursor_visibility)
			target.draw(cursor, states);
	}

private:
	int counter = 0, ind = 0, vis = 0;
	bool target = false, cursor_visibility = true;
	string memory, text_left = "", text_left_vis = "";
	vector < pair < string, string > > history;
	RectangleShape entry_field, cursor;
	Text text;

	int max_counter = 20, histori_size = 50;
	double cursor_width = 1, margin_top = 6, margin_left = 3;
	Color font_colour = Color::White;

	void set_text() {
		text.setString(memory);
		vis = min(vis, int(text_left.length()));
		if (text_left.length() > 0 && text_left.length() == memory.length() && vis == text_left.length())
			vis--;
		string s;
		text_left_vis = "";
		bool flag = true;
		for (int i = vis; i < memory.length(); i++) {
			s += memory[i];
			text.setString(s);
			if (text.getGlobalBounds().width > size.x - 2 * margin_left) {
				s.pop_back();
				text.setString(s);
				flag = s.length() + vis >= text_left.length();
				break;
			}
			if (i < text_left.length())
				text_left_vis += memory[i];
		}
		if (!flag) {
			text_left_vis = "";
			vis = text_left.length() - 1;
			for (; vis >= 0; vis--) {
				text_left_vis.insert(0, 1, memory[vis]);
				text.setString(text_left_vis);
				if (text.getGlobalBounds().width > size.x - 2 * margin_left) {
					text_left_vis.erase(text_left_vis.begin());
					vis++;
					text.setString(text_left_vis);
					break;
				}
			}
		}
		Text check_width1("S" + text_left_vis, *text.getFont(), text.getCharacterSize());
		Text check_width2("S", *text.getFont(), text.getCharacterSize());
		double cursor_position = check_width1.getGlobalBounds().width + margin_left - check_width2.getGlobalBounds().width;
		this->cursor.setPosition(Vector2f(pos.x + cursor_position, pos.y + margin_top / 2));
	}

	void add_in_history() {
		while (ind < int(history.size()) - 1)
			history.pop_back();
		history.push_back(make_pair(memory, text_left));
		if (history.size() == 2 * histori_size) {
			for (int i = 0; i < histori_size; i++)
				history[i] = history[histori_size + i];
			history.resize(histori_size);
		}
		ind = history.size() - 1;
	}
};