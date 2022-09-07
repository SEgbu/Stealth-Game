#include <physics.hpp>

b2World* Physics::world;

Physics::Physics(float downwardsYForce){
    gravity = b2Vec2(0.0f, downwardsYForce); // setting the gravity to the downardsForce
    world = new b2World(gravity); // initialising the world with the gravity
}

Physics::~Physics(){
    delete world; // deallocate world pointer
    world = nullptr; // nullify the world pointer
}

// creating a physic body which doesn't move
b2Body* Physics::createStaticBody(glm::vec2 position, glm::vec2 size){
    // define the static body behaviour
    b2BodyDef staticBodyDef;
    staticBodyDef.position.Set(position.x, position.y);

    // create the physics body using this behaviour
    b2Body* staticBody = world->CreateBody(&staticBodyDef);

    // set the size of the shape of this physics body
    b2PolygonShape staticBodyShape;
    staticBodyShape.SetAsBox(size.x / 2, size.y / 2);

    // add the shape to the physics body
    staticBody->CreateFixture(&staticBodyShape, 0.0f);

    return staticBody;
}

// creating a physic body which does move
b2Body* Physics::createDynamicBody(glm::vec2 position, glm::vec2 size, float density, float friction){ 
    // defining what this physics body is
    b2BodyDef dynamicBodyDef;
    dynamicBodyDef.type = b2_dynamicBody;
    dynamicBodyDef.position.Set(position.x, position.y);

    // creating this physics body with the body definition
    b2Body* dynamicBody = world->CreateBody(&dynamicBodyDef);

    // setting the shape of this physics body
    b2PolygonShape dynamicBodyShape;
    dynamicBodyShape.SetAsBox(size.x / 2, size.y / 2);

    /* setting the shape, density and friction 
    and adding this information to the fixture (physics body 
    behaviour) definition */
    b2FixtureDef dynamicBodyFixtureDef;
    dynamicBodyFixtureDef.shape = &dynamicBodyShape;
    dynamicBodyFixtureDef.density = 1.0f;
    dynamicBodyFixtureDef.friction = 0.0f;

    // adding the fixture information to this physics body 
    dynamicBody->CreateFixture(&dynamicBodyFixtureDef);

    return dynamicBody;
}

