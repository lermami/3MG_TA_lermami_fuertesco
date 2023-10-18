#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <tuple>


class ThreadManager {
public:

	ThreadManager();
	~ThreadManager();

	void setStop(bool stop);

	template<typename T>
	std::future<T> addToChain(const std::function<T()>& f, std::string key) {
		std::shared_ptr<std::packaged_task<T()>> task = std::make_shared<std::packaged_task<T()>>(std::move(f));
		std::future<T> future = task->get_future();

		//seguir
		for (auto& t : not_done_jobs_) {
			if (get<0>(t) == key) {

			}
		}

		tuple tl = make_tuple(key, [task]() {(*task)(); })
		not_done_jobs_.push(tl);

		return future;
	}

	template<typename T> 
	std::future<T> add(const std::function<T()>& f) {
		std::shared_ptr<std::packaged_task<T()>> task = std::make_shared<std::packaged_task<T()>>(std::move(f));
		std::future<T> future = task->get_future();
		{
			std::lock_guard<std::mutex> lock(queue_mutex_);
			jobs_.push([task]() {(*task)(); });
		}
		condition_.notify_one();

		return future;
	}

	template<typename T> 
	bool checkFuture(const std::future<T>& f) {
		std::future_status status;

		switch (status = f.wait_for(1s); status) {
			case std::future_status::deferred: return false;
			case std::future_status::timeout: return false;
			case std::future_status::ready: return true;
		}
	}

	template<typename T> 
	void waitFuture(const std::future<T>& f) {
		std::future_status status;

		do {
			status = f.wait_for(1s);
		} while (status != std::future_status::ready);
	}

private:
	std::vector<std::thread> workers_;
	std::queue<std::function<void()>> jobs_;
	std::queue < std::tuple < std::string,std::function<void() >>> not_done_jobs_;
	std::mutex queue_mutex_;
	std::condition_variable condition_;
	bool stop_;
};