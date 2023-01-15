#ifndef EVENT
#define EVENT

class Job;
class Core;
class Event {
 public:
  double triggerTime;
  Event(double time);
  Event *next;
  virtual ~Event();
  virtual void handler() = 0;
};

class Arrival : public Event {
 public:
  Arrival(double time);
  ~Arrival();
  void handler();
};

class Departure : public Event {
 private:
  Job *j;
  Core *c;

 public:
  Departure(Job *j, Core *c);
  ~Departure();
  void handler();
};

class Termination : public Event {
 public:
  static int incompleteJobs;
  static int jobsCompleted;
  static double lastJobCompletedAt;
  static double totalIdleTime;
  Termination();
  ~Termination();
  void handler();
};

#endif