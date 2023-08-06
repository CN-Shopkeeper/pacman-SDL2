#include "map.hpp"

#include "context.hpp"
#include "game_context.hpp"

Map::Map(std::string_view desc, const Size& size) {
    tiles_.reset(
        new Matrix<Tile>(static_cast<int>(size.w), static_cast<int>(size.h)));
    for (int y = 0; y < size.h; y++) {
        for (int x = 0; x < size.x; x++) {
            char c = desc[x + y * size.w];
            Tile::Type type;
            switch (c) {
                case 'W':
                    type = Tile::Type::Wall;
                    break;
                case 'B':
                    type = Tile::Type::Bean;
                    break;
                case 'P':
                    type = Tile::Type::PowerBean;
                    break;
                case ' ':
                    type = Tile::Type::Empty;
                    break;
                case 'G':
                    type = Tile::Type::GhostDoor;
                    break;
                default:
                    break;
            }
            tiles_->Get(x, y).type = type;
        }
    }
}

void Map::Draw() {
    auto& ctx = Context::GetInstance();
    auto& renderer = ctx.GetRenderer();
    auto tilesheet = ctx.GetTextureManager().FindTilesheet(TilesheetName);
    for (int y = 0; y < tiles_->Height(); y++) {
        for (int x = 0; x < tiles_->Width(); x++) {
            auto tileType = tiles_->Get(x, y).type;
            renderer.DrawImage(tilesheet->Get(static_cast<int>(tileType), 0),
                               {x * tilesheet->GetTileSize().w * Scale,
                                y * tilesheet->GetTileSize().h * Scale},
                               {Scale, Scale});
        }
    }
}

std::string Map::GenerateMap() {
    auto tetris = Tetris::GenerateTetris<tetrisHeight, tetrisWidth>();
    const size_t width3 = tetrisWidth * 3;
    const size_t height3 = tetrisHeight * 3;
    std::array<int, height3 * width3> terris3{0};
    std::array<int, height3 * width3> halfPath{0};
    std::array<int, MapWidth * MapHeight> finalPath{0};

    for (int y = 0; y < tetrisHeight; y++) {
        for (int x = 0; x < tetrisWidth; x++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    terris3[(x * 3 + i) + (y * 3 + j) * tetrisWidth * 3] =
                        tetris[x + y * tetrisWidth];
                }
            }
        }
    }
    // 右边或者上面或右上不一样
    for (int i = 1; i < width3 - 1; i++) {
        for (int j = 1; j < height3 - 1; j++) {
            if (terris3[i + j * (width3)] != terris3[i + (j - 1) * (width3)] ||
                terris3[i + j * (width3)] !=
                    terris3[(i + 1) + (j) * (width3)] ||
                terris3[i + j * (width3)] !=
                    terris3[(i + 1) + (j - 1) * (width3)]) {
                halfPath[i + j * (width3)] = 1;
            }
        }
    }
    // 设置鬼门
    halfPath[1 + (3 * 3 + 1) * width3] = 2;
    // 鬼门内置空
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            halfPath[1 + i + (3 * 3 + 2 + j) * width3] = -1;
        }
    }
    for (int y = 0; y < height3; y++) {
        halfPath[0 + y * width3] = halfPath[1 + y * width3];
        halfPath[width3 - 1 + y * width3] = 1;
    }
    for (int x = 0; x < width3; x++) {
        halfPath[x] = 1;
        halfPath[x + (height3 - 1) * width3] = 1;
    }
    for (int y = 0; y < height3; y++) {
        for (int x = 1; x < width3; x++) {
            finalPath[width3 + x - 1 + (y + 1) * width3 * 2] =
                finalPath[width3 - x + (y + 1) * width3 * 2] =
                    halfPath[x + y * width3];
        }
    }

    std::string mapStr = std::string(MapWidth * MapHeight, ' ');
    for (int y = 0; y < MapHeight; y++) {
        for (int x = 0; x < MapWidth; x++) {
            switch (finalPath[x + y * MapWidth]) {
                case 0:
                    mapStr[x + y * MapWidth] = 'W';
                    break;
                case 1:
                    mapStr[x + y * MapWidth] = 'B';
                    break;
                case 2:
                    mapStr[x + y * MapWidth] = 'G';
                    break;
                case -1:
                    mapStr[x + y * MapWidth] = ' ';
                    break;
                default:
                    break;
            }
        }
    }
    return mapStr;
}

template <const size_t height, const size_t width>
std::array<int, height * width> Tetris::GenerateTetris() {
    std::array<int, height * width> tetris{0};
    int flag = 1;
    // 鬼门
    tetris[0 + 3 * width] = tetris[1 + 3 * width] = tetris[0 + 4 * width] =
        tetris[1 + 4 * width] = -1;
    // 长生点
    tetris[0 + 7 * width] = -1;
    auto cnt = getTreisCount();
    bfs<height, width>(tetris, 0, 6, cnt, flag++);
    tetris[0 + 7 * width] = 0;
    // 遍历所有的空余地块
    for (int x = 0; x < width; x++) {
        for (int y = 4; y >= 0; y--) {
            if (tetris[x + y * width] == 0) {
                cnt = getTreisCount();
                bfs<height, width>(tetris, x, y, cnt, flag++);
            }
        }
        for (int y = 5; y < height; y++) {
            if (tetris[x + y * width] == 0) {
                cnt = getTreisCount();
                bfs<height, width>(tetris, x, y, cnt, flag++);
            }
        }
    }

    // for (int y = 0; y < height; y++) {
    //     for (int x = 0; x < width; x++) {
    //         std::cout << tetris[x + y * width] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return tetris;
}

int Tetris::getTreisCount() {
    srand((unsigned)time(NULL));
    int rand = std::rand() % 100;
    // 1个的概率: 5%
    // 2个的概率: 10%
    // 3个的概率: 20%
    // 4个的概率: 35%
    // 5个的概率: 30%
    if (rand < 5) {
        return 1;
    } else if (rand < 15) {
        return 2;
    } else if (rand < 35) {
        return 3;
    } else if (rand < 70) {
        return 4;
    } else {
        return 5;
    }
}

template <const size_t height, const size_t width>
void Tetris::bfs(std::array<int, height * width>& terris, int x, int y, int cnt,
                 int num) {
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::queue<Vector2> queue;
    queue.push({static_cast<float>(x), static_cast<float>(y)});
    while (cnt >= 0 && !queue.empty()) {
        auto now = queue.front();
        int x = now.x;
        int y = now.y;
        queue.pop();
        terris[now.x + now.y * width] = num;
        cnt--;
        std::vector<Vector2> toBeAdded;
        if (x + 1 < width && terris[x + 1 + y * width] == 0) {
            toBeAdded.push_back(
                {static_cast<float>(x + 1), static_cast<float>(y)});
        }
        if (y - 1 >= 0 && terris[x + (y - 1) * width] == 0) {
            toBeAdded.push_back(
                {static_cast<float>(x), static_cast<float>(y - 1)});
        }
        if (y + 1 < height && terris[x + (y + 1) * width] == 0) {
            toBeAdded.push_back(
                {static_cast<float>(x), static_cast<float>(y + 1)});
        }
        std::shuffle(std::begin(toBeAdded), std::end(toBeAdded), rng);
        for (auto v : toBeAdded) {
            queue.push(v);
        }
    }
}
