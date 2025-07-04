#include "Bullet3.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"
Engine::Point p1;
PlayScene *Bullet3::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Bullet3::OnExplode(Enemy *enemy) {
}
Bullet3::Bullet3(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent) : Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent) {
    Velocity = forwardDirection.Normalize() * speed;
    Rotation = rotation;
    CollisionRadius = 10;
    Engine::Point p1 = Position;
}
void Bullet3::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();
    // Can be improved by Spatial Hash, Quad Tree, ...
    // However simply loop through all enemies is enough for this program.
    for (auto &it : scene->EnemyGroup->GetObjects()) {
        Enemy *enemy = dynamic_cast<Enemy *>(it);
        
        if (!enemy->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            OnExplode(enemy);
            enemy->Hit(damage);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }

    }
    while(!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Position - Size / 2 - Engine::Point(0.0001, 0.0001), PlayScene::GetClientSize())){
        float distance = std::sqrt((p1.x - Position.x)*(p1.x - Position.x) + (p1.y - Position.y)*(p1.y - Position.y));
        if (distance >= 1){
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            break;
        }
    }
    // Check if out of boundary.
        /*if (!Engine::Collider::IsRectOverlap(Position - Size / 2 - Engine::Point(0.0001, 0.0001), Position + Size / 2  + Engine::Point(0.0001, 0.0001), Position- Size / 2 , Position + Size / 2))
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);*/
/*     if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Position - Size / 2 - Engine::Point(0.0001, 0.0001), PlayScene::GetClientSize()))
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
*/
}
