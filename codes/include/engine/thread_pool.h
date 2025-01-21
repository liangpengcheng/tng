#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include "core/types.h"

namespace tng {

	class ThreadPool
	{
	public:
		ThreadPool() : quit_(false), thread_count_(0) {}
		~ThreadPool() { Destroy(); }

		void Create(int thread_count = 0);
		void Destroy();
		void Quit() { quit_ = true; }

		void AddTask(std::function<void()> t);
		bool HasTask();
		void WaitForDone();

	private:
		void Run();

		std::vector<std::thread> threads_;
		std::mutex task_lock_;
		std::condition_variable task_cond_;
		std::vector<std::function<void()>> task_;
		std::mutex done_lock_;
		std::condition_variable done_cond_;
		bool quit_;
		int thread_count_;
	};

}