/**
 * @file ThreadManager.hpp
 * @brief Header file for the ThreadManager class.
 *
 * @defgroup ThreadManager Thread Manager class
 * @brief This file defines the ThreadManager class, which provides functionality for managing a pool of threads.
 */
#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
using namespace std::chrono_literals;

/**
 * @class ThreadManager
 * @brief This class manages a pool of threads that can be used to execute tasks concurrently.
 *
 * The ThreadManager class provides functionality to add tasks to a queue,
 *  spawn worker threads, and manage their execution. It allows adding functions
 *  as tasks, checking the status of futures, and waiting for them to complete.
 *
 * @ingroup ThreadManager
 */

class ThreadManager {
public:
	/**
	 * Constructor for the ThreadManager class.
	 *
	 * Creates a ThreadManager object and start all the threads of the device.
	 */
	ThreadManager();

	/**
	* Destructor for the ThreadManager class.
	*
	* Waits for all worker threads to finish their tasks and releases resources.
	*/
	~ThreadManager();

	/**
	* Signals the worker threads to stop processing new tasks.
	*
	* This method sets an internal flag to stop processing new tasks, allowing
	*  existing tasks to finish before exiting.
	*
	* @param stop Flag indicating whether to stop processing new tasks (true) or resume (false).
	*/
	void setStop(bool stop);

	/**
	 * Template method to add a task to the queue and return a future object.
	 *
	 * This template method allows adding a function object to the task queue.
	 *  It creates a std::packaged_task, associates it with the function, and returns a future
	 *  object that represents the eventual result of the task.
	 *
	 * @tparam T The return type of the function object (task).
	 * @param f The function object to be executed as a task.
	 * @return A std::future object representing the eventual result of the task.
	 */
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

	/**
	 * Template method to check the status of a future object.
	 *
	 * This template method checks the status of a std::future object
	 *  using std::future_status. It returns true if the future is ready,
	 *  false otherwise (deferred or timed out).
	 *
	 * @tparam T The type of the value stored in the future.
	 * @param f The future object to check.
	 * @return True if the future is ready, false otherwise.
	 */
	template<typename T> 
	bool checkFuture(const std::future<T>& f) {
		std::future_status status;

		switch (status = f.wait_for(1s); status) {
			case std::future_status::deferred: return false;
			case std::future_status::timeout: return false;
			case std::future_status::ready: return true;
		}
	}

	/**
	 * Template method to wait for a future object to complete.
	 *
	 * This template method repeatedly calls std::future_status::wait_for
	 *  on the provided future object until it reaches a ready state.
	 *
	 * @tparam T The type of the value stored in the future.
	 * @param f The future object to wait for.
	 */
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
	std::vector < std::tuple < std::string,std::function<void() >>> not_done_jobs_;
	std::mutex queue_mutex_;
	std::condition_variable condition_;
	bool stop_;
};