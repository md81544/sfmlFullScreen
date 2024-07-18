#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>

int main()
{
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    if (modes.empty()) {
        std::cout << "Empty set of modes returned\n";
        return 1;
    } else {
        for (std::size_t i = 0; i < modes.size(); ++i) {
            sf::VideoMode mode = modes[i];
            std::cout << "Mode #" << i << ": " << mode.width << "x" << mode.height << " - "
                      << mode.bitsPerPixel << " bpp" << std::endl;
        }
    }

    // So the resolutions reported are:
    // Mode #0: 3300x2100 - 32 bpp
    // Mode #1: 2880x1800 - 32 bpp
    // Mode #2: 2560x1600 - 32 bpp
    // Mode #3: 2304x1440 - 32 bpp
    // Mode #4: 2048x1536 - 32 bpp
    // Mode #5: 1680x1048 - 32 bpp
    // Mode #6: 1600x1200 - 32 bpp
    // Mode #7: 1280x960 - 32 bpp

    // If we use the highest resolution in the list, we get back a reported window size
    // of 1920 x 1200. This seems more sane now, because 1920 is what I've got my "resolution"
    // set to in MacOS's settings. So the best approach seems to be to just choose the
    // highest mode and then determine what the resolution actually is once it's created.
    // BTW - the highest resolutions in the list above do not correspond to what MacOS says
    // is available... but this approach seems to work OK.
    sf::RenderWindow window(
        sf::VideoMode(modes[0].width, modes[0].height), "Test", sf::Style::Fullscreen);
    window.setFramerateLimit(24);

    sf::Font font;
    if (!font.loadFromFile("/Users/mcd/src/sfmlFullScreen/open_sans.ttf")) {
        std::cout << "Couldn't load font file\n";
        return 2;
    }
    sf::Text txt;
    txt.setFont(font);
    txt.setFillColor({ 0, 200, 0 });
    txt.setCharacterSize(100);
    std::string txtString = "Resolution: " + std::to_string(window.getSize().x) + " x "
        + std::to_string(window.getSize().y) + "\n\n"
        + "Press Esc to quit";
    txt.setString(txtString);
    txt.setPosition({ 100.f, 100.f });

    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Yellow);
    rect.setOutlineThickness(4.f);
    rect.setSize({ 1900.f, 1180.f });
    rect.setFillColor(sf::Color::Transparent);
    rect.setPosition({ 10.f, 10.f });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        window.clear({ 0, 0, 0 });
        window.draw(rect);
        window.draw(txt);
        window.display();
    }
}
