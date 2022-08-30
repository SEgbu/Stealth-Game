#include <physics.hpp>

b2World* world;

Physics::Physics(float downwardsYForce){
    gravity = b2Vec2(0.0f, -downwardsYForce);
}

void Physics::init(){
    world = new b2World(gravity);
}

b2Body* Physics::createStaticBody(glm::vec2 position, glm::vec2 size){
    b2BodyDef staticBodyDef;
    staticBodyDef.type = b2_staticBody;
    staticBodyDef.position.Set(position.x, position.y);

    b2Body* staticBody = world->CreateBody(&staticBodyDef);

    b2PolygonShape staticBodyShape;
    staticBodyShape.SetAsBox(size.x / 2, size.y / 2);

    staticBody->CreateFixture(&staticBodyShape, 0.0f);

    return staticBody;
}

b2Body* Physics::createDynamicBody(glm::vec2 position, glm::vec2 size, float density, float friction){
    b2BodyDef dynamicBodyDef;
    dynamicBodyDef.type = b2_dynamicBody;
    dynamicBodyDef.position.Set(position.x, position.y);

    b2Body* dynamicBody = world->CreateBody(&dynamicBodyDef);

    b2PolygonShape dynamicBodyShape;
    dynamicBodyShape.SetAsBox(size.x / 2, size.y / 2);

    b2FixtureDef dynamicBodyFixtureDef;
    dynamicBodyFixtureDef.shape = &dynamicBodyShape;
    dynamicBodyFixtureDef.density = 1.0f;
    dynamicBodyFixtureDef.friction = 0.3f;

    dynamicBody->CreateFixture(&dynamicBodyFixtureDef);
}

