#include <time.h>
#include <sys/time.h>
#include "Condition.h"

bool Condition::waitForSeconds(int seconds){
	struct timespec theTime;
	clock_gettime(CLOCK_MONOTONIC, &theTime);
	theTime.tv_sec += second;
	return 0 == pthread_cond_timewait(&condition, mutex.getMutex(), &theTime);
}
