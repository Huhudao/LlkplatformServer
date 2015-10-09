#ifndef TOOLS_H
#define TOOLS_H

#include <boost/shared_ptr.hpp>

template <typename T>
void movePtr(boost::shared_ptr<T> &sp1, boost::shared_ptr<T> &sp2){
	sp1 = sp2;
	sp2.reset();
}

#endif
