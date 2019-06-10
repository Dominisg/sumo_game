//
// Created by domins on 10.06.19.
//

#include <cmath>
#include "BattleArea.h"
#include "Server.h"
BattleArea::BattleArea() {
    contour.setRadius({ 315,165 });
    contour.setOrigin({ contour.getRadius().x, contour.getRadius().y });
    contour.setPosition(680, 245);
}

bool BattleArea::isInside(Player_State *s) {
    EllipseShape scon = s->contour;
    scon.setRotation(s->angle);
    sf::Vector2f pos;
    sf::Transform tf = scon.getTransform();

    for (int i =0; i<scon.getPointCount();i++){
        pos = scon.getPoint(i);
        pos = tf.transformPoint(pos);

        if(pow((pos.x-680),2)/pow(315,2) + pow((pos.y-245),2)/pow(165,2) < 1)
            return true;
    }
    return false;
}