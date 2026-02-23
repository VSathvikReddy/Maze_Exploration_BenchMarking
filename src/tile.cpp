#include "../Include/tile.hpp"

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}

TileMap::TileMap() {
    m_vertices.setPrimitiveType(sf::Quads);
    set_tiles_in_map();
}
TileMap::~TileMap() {}

bool TileMap::loadFromFile(const std::string &address) {
    return m_tileset.loadFromFile(address);
}
bool TileMap::loadFromImage(const sf::Image &img) {
    return m_tileset.loadFromImage(img);
}

void TileMap::set_tiles_in_map() {
    m_vertices.resize(width * height * 4);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            sf::Vertex* quad = &m_vertices[(x + y * width) * 4];

            quad[0].position = { x * pixel, y * pixel };
            quad[1].position = { (x + 1) * pixel, y * pixel };
            quad[2].position = { (x + 1) * pixel, (y + 1) * pixel };
            quad[3].position = { x * pixel, (y + 1) * pixel };
        }
    }
}

bool TileMap::set_textures(const std::vector<int> &tiles) {
    if (tiles.size() != width * height) return false;

    int tilesPerCol = m_tileset.getSize().y / tileSize.y;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if(!set_tile(x, y, tiles,tilesPerCol)){ return false;};
        }
    }
    return true;
}

bool TileMap::set_tile(int x, int y,const std::vector<int> &tiles) {
    if(x<0 || x>=width || y<0 || y>=height){ return false;}
    int tilesPerCol = m_tileset.getSize().y / tileSize.y;
    return set_tile(x, y,tiles ,tilesPerCol);
}

bool TileMap::set_tile(int x, int y, const std::vector<int> &tiles ,int tilesPerCol){
    int tileNumber = tiles[x +y*width];
    if(tileNumber>=tilesPerCol){ return false;}
    float ty = tileNumber;

    int dir=0; float tx = 0.0f;
    if(tileNumber==1 || tileNumber ==0){check_neighbour(x,y,tiles,dir,tx);}

    sf::Vertex* quad = &m_vertices[(x + y * width) * 4];

    quad[(0+dir)%4].texCoords = { tx * tileSize.x, ty * tileSize.y };
    quad[(1+dir)%4].texCoords = { (tx + 1) * tileSize.x, ty * tileSize.y };
    quad[(2+dir)%4].texCoords = { (tx + 1) * tileSize.x, (ty + 1) * tileSize.y };
    quad[(3+dir)%4].texCoords = { tx * tileSize.x, (ty + 1) * tileSize.y };

    return true;
}

void TileMap::check_neighbour(int x, int y, const std::vector<int> &tiles, int &dir, float &tx){
    std::vector<int> dx = {1,0,-1,0};
    std::vector<int> dy = {0,1,0,-1};

    std::vector<int> nei_dir;nei_dir.reserve(4);
    for(int i=0;i<4;i++){
        int nx = x+dx[i],ny = y+dy[i];
        if(nx>=0 && nx<width && ny>=0 && ny<height){
            if(tiles[x + y*width] == tiles[nx +ny*width]){
                nei_dir.push_back(i);
            }
        }
    }
    if(nei_dir.size()==0){ tx=0;dir=0;}
    else if(nei_dir.size()==1){ tx=2;dir = nei_dir[0];}
    else if(nei_dir.size()==3){
        tx=4;
        for(int i=0;i<4;i++){
            if(std::find(nei_dir.begin(), nei_dir.end(), i) == nei_dir.end()){
                dir = i;
                break;
            }
        }
    }
    else if(nei_dir.size()==2){
        if((nei_dir[0]+nei_dir[1])%2 ==0){
            tx=1;dir=nei_dir[0]%2;
        }else {
            tx = 3;
            int a = nei_dir[0], b = nei_dir[1];
            if(b==(a+1)%4){
                dir=a+1;
            }else if(a==(b+1)%4){
                dir=b+1;
            }else{ 
                dir = a;// fallback
            }
        }
    }
}
bool TileMap::make_transparent(int x, int y){
    if(x<0 || x>=width || y<0 || y>=height){ return false;}
    sf::Vertex* quad = &m_vertices[(x + y * width) * 4];

    for(int i=0;i<4;i++){
        quad[i].color.a = 0;
    }
    return true;
}
