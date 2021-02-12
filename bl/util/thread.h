#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>

#ifdef __GNUG__
#define thread_local __thread
#elif defined(_MSC_VER)
#define thread_local __declspec(thread)
#endif

namespace bl
{
	using std::condition_variable;
	using std::mutex;
	using std::thread;
	typedef std::lock_guard<mutex> mutex_lock;
	typedef std::unique_lock<mutex> unique_mutex_lock;

	class thread_control
	{
	public:
		thread_control();
		~thread_control();
		void wake();
		bool loop();
		bool in_loop() const;
		void quit();

	private:
		bool _need_loop;
		bool _in_loop;
		bool _quit;
		mutex _mutex;
		condition_variable _condition;
	};
} // namespace bl
