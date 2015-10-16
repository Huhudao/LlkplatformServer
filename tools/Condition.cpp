#include <time.h>
#include <sys/time.h>
#include "Condition.h"

bool Condition::waitForSeconds(int seconds){
	struct timespec theTime;
	clock_gettime(CLOCK_MONOTONIC, &theTime);
	theTime.tv_sec += seconds;
	return 0 == pthread_cond_timedwait(&condition, mutex.getMutex(), &theTime);
}
