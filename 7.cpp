#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <string>
#include <map>
#include <functional>

float evaluateExpression(const std::string& expr) {
    // فقط یک ارزیابی ساده برای مثال
    // در نسخه‌های پیشرفته‌تر می‌تونیم از parser استفاده کنیم
    try {
        return std::stof(expr);
    } catch (...) {
        return 0.0f;
    }
}

float functionValue(const std::string& func, float x) {
    if (func == "sin") return std::sin(x);
    if (func == "cos") return std::cos(x);
    if (func == "tan") return std::tan(x);
    if (func == "log") return std::log(x);
    if (func == "exp") return std::exp(x);
    return x;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Enigeering Calculator");
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text inputText("", font, 24);
    inputText.setPosition(20, 20);
    inputText.setFillColor(sf::Color::White);

    std::string input = "";
    std::string currentFunction = "sin";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!input.empty()) input.pop_back();
                } else if (event.text.unicode == '\r') {
                    // Enter key: do nothing for now
                } else {
                    input += static_cast<char>(event.text.unicode);
                }
            }
        }

        inputText.setString("ورودی: " + input);

        window.clear(sf::Color::Black);
        window.draw(inputText);

        // رسم نمودار تابع
        sf::VertexArray graph(sf::LinesStrip, 800);
        for (int i = 0; i < 800; ++i) {
            float x = (i - 400) / 40.0f; // مقیاس محور x
            float y = functionValue(currentFunction, x);
            graph[i].position = sf::Vector2f(i, 300 - y * 40); // مقیاس محور y
            graph[i].color = sf::Color::Green;
        }
        window.draw(graph);

        window.display();
    }

    return 0;
}
