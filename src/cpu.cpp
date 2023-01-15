#include "cpu.h"

#include <fstream>
#include <iostream>

#include "config.h"
#include "event.h"
#include "job.h"
#include "random.h"
#include "scheduler.h"

// std::queue<Core *> Cpu::availableCores;

std::vector<Core *> Cpu::allCores;
double Termination::totalIdleTime = 0;
Cpu *Cpu::cpu;
bool Cpu::coreAvailable() { return !Cpu::availableCores.empty(); }
void Cpu::init() {
  std::cout << "initiating cpu..." << std::endl;
  Cpu::cpu = new Cpu(CORES);
  std::cout << "added " << CORES << " cores to the cpu." << std::endl;
}
Cpu::Cpu(int core_count) {
  do {
    std::cout << "adding cores..." << core_count << " remaining.." << std::endl;
    Core *t = new Core();
    Cpu::availableCores.push(t);
    Cpu::allCores.push_back(t);
  } while (--core_count);
}

Core::Core() : status(idle), lastBusyAt(0) {}
Core::~Core() {
  std::ofstream log("log/" + std::to_string(CORES) + "/" +
                        std::to_string(INTER_ARRIVAL_MEAN).substr(0, 4) +
                        ".res.json",
                    std::ios_base::app);
  log << "{\"idle_time\":" << idleTime << "}," << std::endl;
  Termination::totalIdleTime += idleTime;
  log.close();
}

Cpu::~Cpu() {}

void Core::setStatus(state s) { status = s; }

void Core::execute(Job *j) {
  if (status == idle) idleTime += Scheduler::now() - lastBusyAt;
  setStatus(busy);
  j->startTime = Scheduler::now();
  Scheduler::schedule(new Departure(j, this));
}

Cpu *Cpu::instance() { return cpu; }

void Cpu::removeCores() {
  while (!Cpu::allCores.empty()) {
    Core *t = Cpu::allCores.back();
    Cpu::allCores.pop_back();
    delete (t);
  }
}
void Cpu::removeCpu() { delete (cpu); }