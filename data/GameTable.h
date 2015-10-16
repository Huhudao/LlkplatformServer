#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <map>
#include <assert.h>
#include <boost/shared_ptr.hpp>

#include "../tools/Mutex.h"
#include "../tools/Condition.h"

class GameTable{
public:
	enum TableType{
		TwoPlayers,
		ThreePlayers
	};
private:
	typedef shared_ptr<GameTable> TablePtr;
	static map<int, TablePtr> tables;
	int tableId;
	
};
#endif
