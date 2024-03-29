#ifndef __color_hpp__
#define __color_hpp__


#include <string>


namespace common
{

extern bool EnableColorLog;


#define RESET		0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		    1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7
#define	UNDEFINED	777


extern std::string color(int color = UNDEFINED);

}

#endif
