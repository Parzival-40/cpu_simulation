#include "event.h"

#include <fstream>
#include <iostream>

#include "config.h"
#include "cpu.h"
#include "job.h"
#include "random.h"
#include "scheduler.h"

// std::queue<Core *> Cpu::availableCores;

int Termination::jobsCompleted = 0;
int Termination::incompleteJobs = 0;
// double Termination::totalIdleTime = 0;

Event::Event(double time)
    : triggerTime(Scheduler::now() + time), next(nullptr) {}

Event::~Event() {}

Arrival::Arrival(double time) : Event(time) {}
Departure::Departure(Job *j, Core *c) : Event(j->jobTime), j(j), c(c) {}

void Arrival::handler() {
  double sf = Random::gamma(5, 1, 0, 1);
  double jt = Random::gamma(5, 1, 0, 10);
  new Job(sf, jt);
  Scheduler::schedule(new Arrival(Random::expo(INTER_ARRIVAL_MEAN)));
}
void Departure::handler() {
  if (j->isChild()) {
    j->update();
    delete (j);
  } else {
    if (j->isComplete()) {
      delete (j);
    } else {
      Job::dangling.push(j);
    }
  }
  if (!Job::jobAvailable()) {
    c->lastBusyAt = Scheduler::now();
    c->setStatus(idle);
    Cpu::availableCores.push(c);
  } else {
    c->execute(Job::jobQ.front());
    Job::jobQ.pop();
  }
}

Arrival::~Arrival() {}

Departure::~Departure() {}

Termination::Termination() : Event(0) {}
void Termination::handler() {
  Job::clearJobs();
  Scheduler::clearEvents();
  std::ofstream log("log/" + std::to_string(CORES) + "_cores_" +
                        std::to_string(INTER_ARRIVAL_MEAN).substr(0, 4) +
                        "_inter_arrival_mean.json",
                    std::ios_base::app);
  log << "{\"cores\":[" << std::endl;
  Cpu::removeCores();
  log << "]}," << std::endl;
  log << "{\"analysis\":{" << std::endl;
  Cpu::removeCpu();
  log << "\"total_idle_time\":" << Termination::totalIdleTime
      << ",\"incomplete_jobs\":" << Termination::incompleteJobs
      << ",\"last_job_completed_at\":" << Termination::lastJobCompletedAt
      << ",\"utilization\":\""
      << (Termination::lastJobCompletedAt -
          Termination::totalIdleTime / CORES) /
             Termination::lastJobCompletedAt * 100
      << "%\"";

  log << "}}" << std::endl;
  log.close();
}

Termination::~Termination() {}