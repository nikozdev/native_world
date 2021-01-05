#ifndef NW_ALGORITHM_H
#define NW_ALGORITHM_H

namespace NW
{
	//
}

#define XY_TO_X(x, y, w) (y * w + x)

#define FIND_BY_FUNC(Container, Type, val, func) std::find_if(Container.begin(), Container.end(),	\
	[=](Type Obj)->bool {return Obj func() == val; });
#define FIND_BY_NAME(Container, Type, val, func) std::find_if(Container.begin(), Container.end(),	\
	[=](Type Obj)->bool {return (strcmp(&(Obj func())[0], val) == 0); });

#endif // NW_ALGORITHM_H