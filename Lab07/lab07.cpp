#include <iostream>
#include <Windows.h>
#include <stdio.h>

int main(){

  // freq high-res counter
  __int64 freq = 0;
  if (QueryPerformanceFrequency((LARGE_INTEGER *)&freq) != 0) {
    std::cout << "Freq of counter - clock ticks per sec: " << freq << std::endl;
  } else {
    std::cout << "Error getting freq " << std::endl;
  }

  // period of high-res counter (ms)
  float period = (1 / (float) freq) * 1000.0;
  printf("Period of counter - millisec per clock tick: %f ms \n", period);

  // duration of 1 mil iter empty loop
  __int64 before = 0;
  if (QueryPerformanceCounter((LARGE_INTEGER *)&before) == 0) {
    std::cout << "Error getting counter " << std::endl;
  }
  for (int i = 0; i <= 1000000; i++) {
    ;
  }
  __int64 after = 0;
  if (QueryPerformanceCounter((LARGE_INTEGER *)&after) == 0) {
    std::cout << "Error getting counter " << std::endl;
  }

  float loop_dur = ((float)(after - before) / (float)freq) * 1000.0;
  printf("Duration of 1 mil iter empty loop: %f ms\n", loop_dur);

  // overhead of counter itself
  before = 0;
  if (QueryPerformanceCounter((LARGE_INTEGER *)&before) == 0) {
    std::cout << "Error getting counter " << std::endl;
  }
  __int64 overhead = 0;
  if (QueryPerformanceCounter((LARGE_INTEGER *)&overhead) == 0) {
    std::cout << "Error getting counter " << std::endl;
  }
  after = 0;
  if (QueryPerformanceCounter((LARGE_INTEGER *)&after) == 0) {
    std::cout << "Error getting counter " << std::endl;
  }

  float overhead_dur = ((float)(after - before) / (float)freq) * 1000.0;
  printf("Counter overhead: %f ms\n", overhead_dur);

  return 0;
}
