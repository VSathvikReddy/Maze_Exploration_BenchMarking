#include <SFML/Graphics.hpp>
#include "../Include/values.hpp"

std::string first = "textures/texture_2.png";
std::string second = "textures/goal.png";
std::string dest = "textures/texture_2.png";

sf::Image png_merger_horizontal(const sf::Image &first, const sf::Image &second){
    unsigned int width = first.getSize().x + second.getSize().x;
    unsigned int height = std::max(first.getSize().y, second.getSize().y);

    sf::Image result;
    result.create(width, height, sf::Color::Transparent);

    // Copy first image
    result.copy(first, 0, 0);
    // Copy second image right next to the first
    result.copy(second, first.getSize().x, 0);

    return result; // Caller may save
}

sf::Image png_merger_vertical(const sf::Image &first, const sf::Image &second) {
    unsigned int width  = std::max(first.getSize().x, second.getSize().x);
    unsigned int height = first.getSize().y + second.getSize().y;

    sf::Image result;
    result.create(width, height, sf::Color::Transparent);

    // Copy first image at the top
    result.copy(first, 0, 0);

    // Copy second image below the first
    result.copy(second, 0, first.getSize().y);

    return result;
}

sf::Image png_merger(const sf::Image &first, const sf::Image &second,bool horizontal=true){
    if(horizontal){ return png_merger_horizontal(first,second);}
    else{ return png_merger_vertical(first,second); }
}


int main(){
    sf::Image a;a.loadFromFile(first);
    sf::Image b;b.loadFromFile(second);
    sf::Image result = png_merger(a,b,false);
    result.saveToFile(dest);
}