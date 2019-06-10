//
// Created by domins on 10.06.19.
//

#ifndef SUMO_PR_BATTLEAREA_H
#define SUMO_PR_BATTLEAREA_H


#include "../utils/EllipseShape.h"

class Player_State;

class BattleArea {
    EllipseShape contour;

public:
    BattleArea();
    bool isInside(Player_State * s);
};


#endif //SUMO_PR_BATTLEAREA_H
