#include "bag.h"

Bag::Bag(dge::Vector2 _position, dge::Vector2 _size, int _category, Level* _level)
    : position(_position), size(_size), category(_category), level(_level)
{
    lastPosition = dge::Vector2I(position.x,position.y);
    nextPosition = dge::Vector2I(position.x,position.y);
    velocity = dge::Vector2(0,0);
};

void Bag::updatePosition(float timestep)
{
    dge::Vector2I currentLoc(dge::round(position.x), dge::round(position.y));
    MapObject* currentLocObj = level->map[currentLoc.y][currentLoc.x];
    nextPosition = currentLoc;
    if ((currentLocObj != nullptr) && (currentLocObj->type == MapObjectType::CONVEYER))
    {
        dge::Vector2I conveyerMotion = ((Conveyer*)currentLocObj)->getSpeed();
        dge::Vector2I newNextPosition = currentLoc+conveyerMotion;
        MapObject* nextPositionObj = level->map[newNextPosition.y][newNextPosition.x];
        if((nextPositionObj == nullptr) || (nextPositionObj->type != MapObjectType::WALL))
        {
            nextPosition = newNextPosition;
        }
    }

    // NOTE: We move in each direction independently so that we don't end up moving very slowly
    //       in the perpendicular direction after turning a corner onto a long straight.
    //       This means that we need to manually constrain the movement in each direction in order
    //       to maintain a constant movement speed
    float maxMoveDistance = timestep;
    float xOffset;
    float yOffset;
    xOffset = nextPosition.x - position.x;
    yOffset = nextPosition.y - position.y;
    if(abs(xOffset*yOffset) > 0.01f)
    {
        maxMoveDistance *= 1.0f/ROOTTWO;
    }
    position.x = dge::slide(position.x, nextPosition.x, maxMoveDistance);
    position.y = dge::slide(position.y, nextPosition.y, maxMoveDistance);

    xOffset = position.x - nextPosition.x;
    yOffset = position.y - nextPosition.y;
    float offsetDist = xOffset*xOffset + yOffset*yOffset;
    if(offsetDist < 0.1f*0.1f)
    {
        lastPosition = nextPosition;
    }
}
