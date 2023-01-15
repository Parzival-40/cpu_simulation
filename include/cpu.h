#ifndef CPU
#define CPU
#include <queue>
#include <vector>

enum state { idle, busy };

class Job;
class Core {
 private:
  enum state status;

 public:
  Core();
  ~Core();
  double idleTime;
  double lastBusyAt;
  void execute(Job* j);
  void setStatus(state s);
};

class Cpu {
 private:
  static Cpu* cpu;
  Cpu(int core_count);
  ~Cpu();

 public:
  static std::vector<Core*> allCores;
  static std::queue<Core*> availableCores;
  static double totalIdleTime;
  static bool coreAvailable();
  static void init();
  static Cpu* instance();
  static void removeCores();
  static void removeCpu();
};

#endif