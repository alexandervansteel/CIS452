/*
* Reference for pthreads from Dulimarta
* Must be compiled with -lpthread flag
*/
#include <thread>
#include <future>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>

#include <signal.h>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::setprecision;

/* prototype of the worker thread */
void loadFile (string);

std::random_device r;
std::uniform_int_distribution<short> file_access_time(7,10);
std::uniform_real_distribution<float> cache_hit_prob(0.0, 1.0);
int requested, completed, tot_access_time;

// mutex for shared operations by the worker thread
std::mutex c_mutex;

// condition variable used by the worker thread to notify
// the main thread when a file access completed.
std::condition_variable fs_done;

std::mutex fs_mutex;
std::unique_lock<std::mutex> fs_lock (fs_mutex);

void handler (int s) {
  cout << "File requests " << requested << " ("
  << completed << ") completed" << endl;

  while (requested > completed) {
    cout << "The main thread is waiting for " <<
    (requested - completed) << " in-progress file operations" << endl;
    fs_done.wait(fs_lock); // block on the condition var
  }
  cout << "Average access time is " << std::fixed << setprecision(4) <<
  ((float) tot_access_time / completed) << " seconds"
  << endl;
  exit (0);
}

int main() {
  requested = completed = tot_access_time = 0;
  signal (SIGINT, handler);
  while (true) { /* SIGINT will "break" this loop */
  string filename;
  cout << "Enter filename: ";
  cout.flush();
  cin >> filename;
  std::thread worker(loadFile, filename);
  worker.detach();
  requested++;
}
return 0;
}

/* The filename will be passed by value from the main thread
* which means that the worker thread will get a COPY
*/
void loadFile (std::string name) {
  auto is_hit = cache_hit_prob (r);
  int access_time;

  if (is_hit <= 0.8) {
    std::cout << "** Cache hit " << name << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    access_time = 1;
  } else {
    std::cout << "File " << name << " has to be read from disk" << endl;
    access_time = file_access_time(r);
    std::this_thread::sleep_for(std::chrono::seconds(access_time));
    cout << "Worker thread finished loading file " << name << " after "
    << access_time << " seconds" << endl;
  }

  /* protect shared update using mutex */
  c_mutex.lock();
  completed++;
  tot_access_time += access_time;
  c_mutex.unlock();

  fs_done.notify_one(); // tell the main thread that I’m done
}
