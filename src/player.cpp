#include "player.h"
#include <iostream>

Player_base::Player_base(uint8_t tilesize, int x, int y) : m_tilesize(tilesize), position(0, 0) {
    shape.setSize(sf::Vector2f(tilesize, tilesize));
    shape.setFillColor(sf::Color::White); 
    sf::Transformable::setPosition(position.x * m_tilesize, position.y * m_tilesize);
}

sf::Vector2i Player_base::get_position() const {
    return position;
}

void Player_base::set_position(sf::Vector2i new_position) {
    position = new_position;
    sf::Transformable::setPosition(position.x * m_tilesize, position.y * m_tilesize);
}

void Player_base::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(shape, states);
}

Direction Player_base::intent_to_move() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        return Direction::UP;
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        return Direction::RIGHT;
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        return Direction::DOWN;
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        return Direction::LEFT;
    }else{
        return Direction::NONE;
    }
}
void Player_base::feed_back(sf::Vector2i cur_position,uint8_t top, uint8_t right, uint8_t down, uint8_t left){
    std::cout<<"X:"<<cur_position.x<<" Y:"<<cur_position.y<<std::endl;
    return;
}