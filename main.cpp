#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <filesystem>
#include <iostream>
#include <vector>

int main(int, char* argv[])
{
    // This is just for testing an SFML bug in SFML on Apple silicon Macs
    // See my report at https://github.com/SFML/SFML/issues/2300

    // Get the location of the executable (this will follow symlinks)
    std::filesystem::path executable(argv[0]);
    std::filesystem::path cwd = executable.remove_filename();

    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    if (modes.empty()) {
        std::cout << "Empty set of modes returned\n";
    } else {
        for (std::size_t i = 0; i < modes.size(); ++i) {
            sf::VideoMode mode = modes[i];
            std::cout << "Mode #" << i << ": " << mode.width << "x" << mode.height << " - "
                      << mode.bitsPerPixel << " bpp" << std::endl;
        }
    }

    // So the resolutions reported on a older Intel macbook are:
    // Mode #0: 3300x2100 - 32 bpp
    // Mode #1: 2880x1800 - 32 bpp
    // Mode #2: 2560x1600 - 32 bpp
    // Mode #3: 2304x1440 - 32 bpp
    // Mode #4: 2048x1536 - 32 bpp
    // Mode #5: 1680x1048 - 32 bpp
    // Mode #6: 1600x1200 - 32 bpp
    // Mode #7: 1280x960 - 32 bpp

    // However on an Apple silicon mac, the modes vector is empty.
    // Setting NSHighResolutionCapable to true or false in the Info.plist appears to
    // have no effect either way.
    sf::RenderWindow window;
    if (!modes.empty()) {
        window.create(
            sf::VideoMode(modes[0].width, modes[0].height), "Test", sf::Style::Fullscreen);
    } else {
        // So we fall back to windowed. Any attempt at fullscreen will cause a segfault
        // owing to the bug outlined in my report at https://github.com/SFML/SFML/issues/2300
        window.create(sf::VideoMode(1280, 800), "Test");
    }
    window.setFramerateLimit(24);

    sf::Font font;
    std::filesystem::path fontFile = cwd / "open_sans.ttf";
    if (!font.loadFromFile(fontFile.c_str())) {
        std::cout << "Couldn't load font file " << fontFile.c_str() << std::endl;
        return 2;
    }
    sf::Text txt;
    txt.setFont(font);
    txt.setFillColor({ 0, 200, 0 });
    txt.setCharacterSize(100);
    std::string txtString = "Resolution: " + std::to_string(window.getSize().x) + " x "
        + std::to_string(window.getSize().y) + "\n\n" + "Press Esc to quit";
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
