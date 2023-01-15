#ifndef JOB
#define JOB
#include <queue>
#include <vector>

class Job {
 private:
  static int count;
  Job* mother;
  int id;
  double sFraction;
  double arrivalTime;
  double finishTime;
  double jobtime;
  int finished;
  void divide();

 public:
  double startTime;
  double jobTime;
  void update();
  bool isChild();
  bool isComplete();
  static std::queue<Job*> jobQ;
  struct Compare {
    bool operator()(const Job* l, const Job* r) {
      return l->finished < r->finished;
    }
  };
  static std::priority_queue<Job*, std::vector<Job*>, Job::Compare> dangling;
  Job(double sFraction, double jobTime, Job* mother = nullptr);
  ~Job();
  static bool jobAvailable();
  static void killDangling();
  static void clearJobs();
};

#endif