#include "thread_manager.hpp"

ThreadManager::ThreadManager() {
	stop_ = false;

	auto worker_count = std::thread::hardware_concurrency();
	for (int i = 0; i != worker_count; i++) {
		auto worker = [this]() {
			while (true) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(queue_mutex_);
					condition_.wait(lock, [this] { return !jobs_.empty() || stop_; });
					if (stop_ && jobs_.empty()) return;
					task = std::move(jobs_.front());
					jobs_.pop();
				}
				task();
			}
		};
		workers_.push_back(std::thread{ std::move(worker) });
	}
}

void ThreadManager::add(std::function<void()> task) {
	std::lock_guard<std::mutex> lock{ queue_mutex_ };
	jobs_.push(std::move(task));
}

ThreadManager::~ThreadManager() {
	for (auto& t : workers_) {
		if (t.joinable()) {
			t.join();
		}
	}
}