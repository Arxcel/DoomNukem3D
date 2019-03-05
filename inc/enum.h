#ifndef ENUM_H
# define ENUM_H

typedef enum    e_MovementDirection
{
                Nothing = 0,
                Forward,
                Backward,
                Right,
                Left,
}               MoveDir;

enum
{
    MaxQueue = 32
};  // maximum number of pending portal renders


#endif