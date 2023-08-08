#pragma once

#include "consts.hpp"
#include "context.hpp"
#include "map.hpp"
#include "pch.hpp"
#include "texture.hpp"

class Monster {
   public:
    enum class Direction { Right = 0, Down, Left, Up };

    Direction movingDir = Direction::Right;
    Direction intentionDir = Direction::Right;

    Image image;
    float speed = 5.0f;

    Monster(const Image& image, const Vector2& position)
        : image(image), position_(position) {}

    virtual ~Monster() = default;

    static MapCoordinate DirectionToCoordinate(Direction& direction);

    void Move(const Vector2& offset) {
        offset_ = offset;
        position_ += offset;
    }

    auto& GetPosition() const { return position_; }
    auto& GetVelocity() const { return offset_; }

    void Reset(const Vector2& position) {
        position_ = position;
        movingDir = Direction::Right;
        intentionDir = Direction::Right;
    }

    Rect GetRect() const { return Rect(position_, {TileSize, TileSize}); }

    MapCoordinate GetMapCorrdinate() const {
        Rect rect = GetRect();
        auto center = rect.Center();
        return {static_cast<int>(center.x / TileSize),
                static_cast<int>(center.y / TileSize)};
    }

    void Draw();
    virtual void Update();
    virtual void Debug() = 0;

   protected:
    Vector2 offset_;
    Vector2 position_;
    bool isTurnBack() const;
    void doUpdate();
};

class Pacman : public Monster {
   public:
    Pacman(const Image& image, const Vector2& position)
        : Monster(image, position) {}

    void Debug() override{};

   private:
};

class Ghost : public Monster {
   public:
    Ghost(const Image& image, const Vector2& position, std::string name,
          SDL_Color color)
        : Monster(image, position), name_(name), color_(color) {
        speed = 4;
    }
    std::vector<MapCoordinate> path;

    static void InitAiMap();
    void Update() override;
    void Debug() override;

    using AIType =
        std::function<Direction(Pacman&, Ghost&, std::vector<MapCoordinate>&)>;

   private:
    std::string name_;
    SDL_Color color_;
    inline static std::unordered_map<std::string, AIType> aiMap_ =
        std::unordered_map<std::string, AIType>();
    static AIType aiPinky_;
    static AIType aiBlinky_;
};

Monster::Direction GetDirectionFromPath(const std::vector<MapCoordinate>& path);
