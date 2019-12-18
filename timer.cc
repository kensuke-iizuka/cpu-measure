#include <iostream>
#include <chrono>
#include <thread>

int main()
{
  // std::chrono::system_clock::timepoint start, end;
  auto start = std::chrono::system_clock::now();
  // Execution
  // std::this_thread::sleep_for(std::chrono::seconds(19));
  auto end = std::chrono::system_clock::now();
  double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
  std::cout << "Elapsed time: " << elapsed << "[ms]" << std::endl;
  return 0;
}
