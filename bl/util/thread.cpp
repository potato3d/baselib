#include <bl/util/thread.h>

namespace bl
{
	thread_control::thread_control()
		: _need_loop(false),
		  _in_loop(false),
		  _quit(false)
	{
	}

	thread_control::~thread_control()
	{
		quit();
	}

	void thread_control::quit()
	{
		_quit = true;
		wake();
	}

	void thread_control::wake()
	{
		mutex_lock l(_mutex);
		_need_loop = true;
		_condition.notify_all();
	}

	bool thread_control::loop()
	{
		unique_mutex_lock l(_mutex);
		_in_loop = false;
		if(!_need_loop)
		{
			_condition.wait(l);
		}
		_need_loop = false;
		_in_loop = true;
		return !_quit;
	}

	bool thread_control::in_loop() const
	{
		return _in_loop;
	}
} // namespace bl
