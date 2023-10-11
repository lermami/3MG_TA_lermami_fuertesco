#include "thread_manager.hpp"

ThreadManager::ThreadManager() {

}

void ThreadManager::add(std::function<void()> task) {
	jobs_.push(std::move(task));
}

void ThreadManager::worker() {
	while (true) {
		std::function<void()> job;
		{
			std::unique_lock<std::mutex> lock(queue_mutex_);
			condition_.wait(lock, [this] { return !jobs_.empty() || stop_; });
			if (stop_ && jobs_.empty()) return;
			job = std::move(jobs_.front());
			jobs_.pop();
		}
		job();
	}
}