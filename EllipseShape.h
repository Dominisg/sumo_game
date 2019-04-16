#ifndef SUMO_PR_ELLIPSE_H
#define SUMO_PR_ELLIPSE_H

#include <SFML/Graphics.hpp>


class EllipseShape : public sf::Shape
{
public:

	EllipseShape(const sf::Vector2f& radius);
	EllipseShape();
	void setRadius(const sf::Vector2f& radius);
	const sf::Vector2f& getRadius() const;
	virtual unsigned int getPointCount() const;	
	virtual sf::Vector2f getPoint(unsigned int index) const;
	

private:

	sf::Vector2f m_radius;
};


#endif //SUMO_PR_ELLIPSE_H