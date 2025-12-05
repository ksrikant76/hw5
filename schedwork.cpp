#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsUsed,
    size_t day,
    size_t slot
);

static bool workerAlreadyAssigned(
    const std::vector<Worker_T>& day,
    size_t slot,
    Worker_T w
)
{
    //iterate through slots till current slot to see if worker has been assigned
    for (size_t i = 0; i < slot; ++i) {
        if (day[i] == w) return true;
    }
    return false;
}

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();

    // Add your code below
    const size_t n = avail.size();  //n: number of days
    const size_t k = avail[0].size();  //k: number of workers

    //if dont need any workers or have no workers, return false
    //or if there aren't enough workers to fulfill dailyNeed, return false
    //or if there aren't enough total worker shifts to fulfill total needed shifts, return false
    if (dailyNeed == 0 || k == 0
        || dailyNeed > k
        || (k * maxShifts) < (n * dailyNeed) ) {
        return false;
    }

    //make sched size n x dailyNeed and fill it with INVALID_ID
    sched.assign(n, std::vector<Worker_T>(dailyNeed, INVALID_ID));

    //make shiftsUsed vector to track how many shifts workers have used so far
    std::vector<size_t> shiftsUsed(k, 0);

    //start backtracking
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, 0, 0);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsUsed,
    size_t day,
    size_t slot
)
{
    const size_t n = avail.size();
    const size_t k = avail[0].size();

    //base case: all days have been assigned
    if (day == n) {
        return true;
    }

    //go to next day if all slots filled for this day
    if (slot == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day + 1, 0);
    }

    //try all workers for this slot
    for (Worker_T w = 0; w < k; ++w) {
        //if worker not available,
        //or if worker has already worked max shifts,
        //or if worker already assigned for the day,
        //go to next worker
        if (!avail[day][w]
            || shiftsUsed[w] >= maxShifts
            || workerAlreadyAssigned(sched[day], slot, w)) continue;

        //set worker
        sched[day][slot] = w;
        shiftsUsed[w]++;

        //go to next slot
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day, slot + 1)) {
            return true;
        }

        //backtrack
        shiftsUsed[w]--;
        sched[day][slot] = INVALID_ID;
    }

    //no valid worker for the slot
    return false;
}

