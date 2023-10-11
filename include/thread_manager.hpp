#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadManager {
public:

	ThreadManager();
	void add(std::function<void()> task);

	void worker();

private:
	std::vector<std::thread> workers_;
	std::queue<std::function<void()>> jobs_;
	std::mutex queue_mutex_;
	std::condition_variable condition_;
	bool stop_;
};