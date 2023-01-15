#ifndef SCHEDULER
#define SCHEDULER

class Event;
class Scheduler {
 public:
  static double time;
  static Event *eventList;
  static void init();
  static double now();
  static void update(double time);
  static void schedule(Event *e);
  static void run();
  static void clearEvents();
};

#endif