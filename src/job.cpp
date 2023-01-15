#include "job.h"

#include <fstream>
#include <iostream>
#include <queue>

#include "config.h"
#include "cpu.h"
#include "event.h"
#include "scheduler.h"

int Job::count = 0;
std::queue<Job *> Job::jobQ;
std::queue<Core *> Cpu::availableCores;
double Termination::lastJobCompletedAt;
std::priority_queue<Job *, std::vector<Job *>, Job::Compare> Job::dangling;
Job::Job(double sFraction, double jobTime, Job *mother)
    : id(++count),
      finished(0),
      sFraction(sFraction),
      jobtime(jobTime),
      jobTime(jobTime),
      mother(mother) {
  arrivalTime = Scheduler::now();
  if (sFraction != 1) {
    divide();
  }
}

void Job::divide() {
  double pTime = (1 - sFraction) * jobTime / (CORES - 1);
  jobTime *= sFraction;
  sFraction = 1;
  if (Cpu::coreAvailable()) {
    Core *c = Cpu::availableCores.front();
    Cpu::availableCores.pop();
    c->execute(this);
  } else {
    Job::jobQ.push(this);
  }
  for (int i = 1; i < CORES; i++) {
    Job *j = new Job(1, pTime, this);
    if (Cpu::coreAvailable()) {
      Core *c = Cpu::availableCores.front();
      Cpu::availableCores.pop();
      c->execute(j);
    } else {
      Job::jobQ.push(j);
    }
  }
}

bool Job::jobAvailable() { return !Job::jobQ.empty(); }

Job::~Job() {
  finishTime = Scheduler::now();
  if (!isChild()) {
    if (isComplete())
      Termination::jobsCompleted++;
    else
      Termination::incompleteJobs++;
    if (Termination::jobsCompleted == JOBS) {
      Scheduler::schedule(new Termination());
      Termination::lastJobCompletedAt = Scheduler::now();
    }
    std::ofstream log("log/" + std::to_string(CORES) + "/" +
                          std::to_string(INTER_ARRIVAL_MEAN).substr(0, 4) +
                          ".trace.json",
                      std::ios_base::app);
    log << "{\"job_id\":" << id << ",\"arrival_time\":" << arrivalTime
        << ",\"delay_time\":" << startTime - arrivalTime
        << ",\"start_time\":" << startTime << ",\"finish_time\":" << finishTime
        << ",\"job_time\":" << jobtime
        << ",\"is_complete\":" << (isComplete() ? "true" : "false")
        << ",\"completion_time\":" << finishTime - startTime
        << ",\"sojourn_time\":" << finishTime - arrivalTime << "},"
        << std::endl;
    log.close();
  }
}

void Job::update() { mother->finished++; }
bool Job::isChild() { return mother != nullptr; }
bool Job::isComplete() { return finished == CORES - 1; }
void Job::killDangling() {
  while (!Job::dangling.empty() && Job::dangling.top()->isComplete()) {
    Job *temp = Job::dangling.top();
    Job::dangling.pop();
    delete (temp);
  }
}

void Job::clearJobs() {
  while (!Job::jobQ.empty()) {
    Job *t = jobQ.front();
    Job::jobQ.pop();
    delete (t);
  }
}