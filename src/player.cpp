#include "player.h"

Player_base::Player_base(uint8_t tilesize) : m_tilesize(tilesize), position(0, 0) {
    shape.setSize(sf::Vector2f(tilesize, tilesize));
    shape.setFillColor(sf::Color::Blue); 
    setPosition(position.x * m_tilesize, position.y * m_tilesize);
}

sf::Vector2i Player_base::get_position() const {
    return position;
}

void Player_base::set_position(sf::Vector2i new_position) {
    position = new_position; // Update logical grid position
    
    // Update the actual visual rendering position on the screen
    setPosition(position.x * m_tilesize, position.y * m_tilesize);
}

void Player_base::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Apply the Transformable properties (position, rotation, scale)
    states.transform *= getTransform();
    
    // Draw our shape to the screen
    target.draw(shape, states);
}