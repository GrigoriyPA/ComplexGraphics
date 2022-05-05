#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <set>
#include <map>

#include "add_functions.h"

#include "common_classes/Vect2.h"
#include "common_classes/Vect3.h"
#include "common_classes/Line2.h"
#include "common_classes/Complex.h"

#include "special_classes/Complex_expression.h"

#include "Interface_classes/Interface_feature.h"
#include "Interface_classes/Button.h"
#include "Interface_classes/Text_input.h"
#include "Interface_classes/Coordinate_system.h"
#include "Interface_classes/Complex_graphic.h"


const int Cw = 1240, Ch = 670, FPS = 30, max_cnt = 2;
Color background(0, 0, 0);
Font arial;


int main() {
	setlocale(LC_ALL, "RU");
	RenderWindow window(VideoMode(Cw, Ch), "Complex graphics");
	window.setFramerateLimit(FPS);

	arial.loadFromFile("fonts/arial.ttf");

	Text_input text(Vector2f(20, 20), Vector2f(400, 40), arial);
	Complex_graphic graphic(Vector2f(20, 70), Vector2f(1200, 580), &text, arial);

	int cnt = 0;
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event))
			switch (event.type) {
				case Event::Closed: {
					window.close();
					break;
				}

				case Event::MouseMoved: {
					cnt++;
					if (cnt == max_cnt) {
						graphic.check_move(event.mouseMove.x, event.mouseMove.y);
						cnt = 0;
					}
					break;
				}

				case Event::MouseButtonPressed: {
					graphic.check_target(event.mouseButton.x, event.mouseButton.y, true, event.mouseButton.button);
					break;
				}

				case Event::MouseButtonReleased: {
					text.check_target(event.mouseButton.x, event.mouseButton.y);
					graphic.check_target(event.mouseButton.x, event.mouseButton.y, false, event.mouseButton.button);
					break;
				}

				case Event::MouseWheelScrolled: {
					if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
						graphic.check_scrol(event.mouseWheelScroll.x, event.mouseWheelScroll.y, event.mouseWheelScroll.delta);
					}
					break;
				}

				case Event::TextEntered: {
					text.check_input(event.text.unicode, 't');
					graphic.check_input(event.text.unicode, 't');
					//cout << event.text.unicode << "\t" << char(event.text.unicode) << "\t";
					break;
				}

				case::Event::KeyPressed: {
					text.check_input(event.key.code, 's', event.key.shift, event.key.control);
					graphic.check_input(event.key.code, 's', event.key.shift, event.key.control);
					//cout << event.key.code << "\n";
					break;
				}

				default:
					break;
			}

		window.clear(background);
		window.draw(graphic);
		window.draw(text);
		window.display();

		text.check_counter();
		graphic.check_counter();
	}

	return 0;
}
