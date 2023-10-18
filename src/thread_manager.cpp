#include "thread_manager.hpp"

ThreadManager::ThreadManager() {
	stop_ = true;

	auto worker_count = std::thread::hardware_concurrency();
	for (int i = 0; i != worker_count; i++) {
		auto worker = [this]() {
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
		};

		workers_.push_back(std::thread(std::move(worker)));
	}
}

ThreadManager::~ThreadManager() {
	condition_.notify_all();
	for (auto& w : workers_) {
		if (w.joinable()) 
			w.join();
	}
}

void ThreadManager::setStop(bool stop) {
	stop_ = stop;
}
