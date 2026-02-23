#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <SFML/Graphics.hpp>

// Added 'NONE' so the entity can stand still
enum class Direction : uint8_t {
    NONE = 0, 
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4,
};

class Player_base : public sf::Drawable, public sf::Transformable {
protected:
    // Moved these to 'protected' so derived classes (like a HumanPlayer or AIEnemy) can use them
    sf::Vector2i position;    // Grid coordinates (e.g., x=2, y=3)
    uint8_t m_tilesize;       // Needed to calculate actual screen pixels
    sf::RectangleShape shape; // The visual representation of the player

    // Required by sf::Drawable to actually render the entity
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    Player_base(uint8_t tilesize);
    
    // CRITICAL: Virtual destructor prevents memory leaks in polymorphic base classes
    virtual ~Player_base() = default; 

    // Made this pure virtual (= 0). Every derived class MUST write its own movement logic
    virtual Direction intent_to_move() = 0; 

    sf::Vector2i get_position() const;
    void set_position(sf::Vector2i new_position);
};

#endif