#ifndef SUMO_PR_ELLIPSE_H
#define SUMO_PR_ELLIPSE_H

#include <SFML/Graphics.hpp>


class EllipseShape : public sf::Shape
{
public:

	EllipseShape(const sf::Vector2f& radius);
	EllipseShape() = default;
	void setRadius(const sf::Vector2f& radius);
	const sf::Vector2f& getRadius() const;
	std::size_t getPointCount() const;
	virtual sf::Vector2f getPoint(std::size_t index) const;
	

private:

	sf::Vector2f m_radius;
};


#endif //SUMO_PR_ELLIPSE_H