#include "scheduler.h"

#include <fstream>
#include <iostream>

#include "config.h"
#include "cpu.h"
#include "event.h"
#include "job.h"

double Scheduler::time;
Event *Scheduler::eventList;
void Scheduler::init() {
  std::cout << "initiating scheduler..." << std::endl;
  Scheduler::time = 0;
  Cpu::init();
}
double Scheduler::now() { return Scheduler::time; }
void Scheduler::update(double time) { Scheduler::time = time; }
void Scheduler::schedule(Event *e) {
  if (!Scheduler::eventList) {
    Scheduler::eventList = e;
    return;
  }
  Event *p = Scheduler::eventList;
  if (e->triggerTime < p->triggerTime) {
    e->next = p;
    Scheduler::eventList = e;
    return;
  }
  Event *q = p->next;
  while (q && q->triggerTime <= e->triggerTime) {
    p = p->next;
    q = q->next;
  }
  p->next = e;
  e->next = q;
}

void Scheduler::clearEvents() {
  while (eventList) {
    Event *t = Scheduler::eventList;
    Scheduler::eventList = Scheduler::eventList->next;
    delete (t);
  }
}
void Scheduler::run() {
  Scheduler::schedule(new Arrival(0));
  std::ofstream log("log/" + std::to_string(CORES) + "/" +
                        std::to_string(INTER_ARRIVAL_MEAN).substr(0, 4) +
                        ".trace.json",
                    std::ios_base::app);
  log << "[" << std::endl;
  while (Scheduler::eventList) {
    Scheduler::update(Scheduler::eventList->triggerTime);
    Event *t = Scheduler::eventList;
    Scheduler::eventList = Scheduler::eventList->next;
    t->handler();
    Job::killDangling();
    delete (t);
  }
  log << "]" << std::endl;
  log.close();
}
