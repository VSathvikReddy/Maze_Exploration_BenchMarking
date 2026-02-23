#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <SFML/Graphics.hpp>

// Added 'NONE' so the entity can stand still
enum class Direction : uint8_t { 
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
    NONE = 4
};

class Player_base : public sf::Drawable, public sf::Transformable{
private:
    void set_position(sf::Vector2i new_position);
    friend class Game;

    sf::Vector2i position;
    uint8_t m_tilesize;
    sf::RectangleShape shape;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:
    std::string name = "Default";
public:
    Player_base(uint8_t tilesize);
    virtual ~Player_base() = default; 

    virtual Direction intent_to_move();

    sf::Vector2i get_position() const;
};

#endif