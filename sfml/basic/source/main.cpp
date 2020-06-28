#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <switch.h>
#include <dirent.h>

extern int main()
{
    // Setup NXLink
    socketInitializeDefault();
    nxlinkStdio();

    Result rc = romfsInit();
    
    sf::Music music;
    if (!music.openFromFile("romfs:/sample.wav"))
        return -1;
    

    sf::Font font;
    if (!font.loadFromFile("romfs:/sans.ttf"))
    {
        printf("Font not loaded\r\n");
        return EXIT_FAILURE;
    }

    sf::VideoMode mode = sf::VideoMode::getDesktopMode();

    sf::Text message("Hello, world!", font);

    sf::RenderWindow window(mode, "Hello World SFML Window");
    sf::Clock deltaClock;

    printf("Loading texture\n");
    sf::Texture texture;
    if (!texture.loadFromFile("romfs:/example.png")) {
        printf("Texture failed to load\n");
        return EXIT_FAILURE;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::CircleShape circle(40.f);
    circle.setPosition(100., -100.);

    // set the shape color to green
    circle.setFillColor(sf::Color::Magenta);

    float speed = 0.02f;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            printf("Event: %u\n", e.type);
        }

        window.clear();
        
        window.draw(message);
        window.draw(sprite);
        sf::Time dtTime = deltaClock.restart();
        float dt = dtTime.asSeconds();

        sf::Vector2f spritePos = sprite.getPosition();
        sf::Vector2f msgPos = message.getPosition();

        if (sf::Touch::isDown(1))
        {
            music.play();
        } else if (sf::Touch::isDown(2))
        {
            music.pause();
        }

        if (sf::Touch::isDown(0))
        {
            auto touchPos = sf::Touch::getPosition(0);
            spritePos.x = (float) touchPos.x;
            spritePos.y = (float) touchPos.y;

        } else
        {
            spritePos.x += speed * dt * sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
            spritePos.y += speed * dt * -sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
            msgPos.x += speed * dt * sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
            msgPos.y += speed * dt * -sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V);
        }

        sprite.setPosition(spritePos);
        message.setPosition(msgPos);

        window.display();
    }

    socketExit();
    return EXIT_SUCCESS;
}