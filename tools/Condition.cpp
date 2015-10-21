#include <time.h>
#include <sys/time.h>

#include "Condition.h"
#include "Log.h"

bool Condition::waitForSeconds(int seconds){
	struct timespec theTime;
	struct timeval now;
	gettimeofday(&now, NULL);
	theTime.tv_sec = now.tv_sec + seconds;
	theTime.tv_nsec = now.tv_usec * 1000;
	return 0 == pthread_cond_timedwait(&condition, mutex.getMutex(), &theTime);
}
