#ifndef GRAPHICSGENERATOR_HPP
#define GRAPHICSGENERATOR_HPP

// -------------------------------------- //
class GraphicsGenerator
{
public:
	GraphicsGenerator();
	~GraphicsGenerator();

	bool StartDisplay();
	bool UpdatePerspective(int x, int y);
	bool EndDisplay();

};





#endif

