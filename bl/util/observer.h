#pragma once
#include <bl/util/array.h>

namespace bl
{
	template<typename t_observer>
	class observer_base;

	template<typename t_observer>
	class subject_base
	{
		friend class observer_base<t_observer>;

	public:
		virtual ~subject_base();

		virtual void add_observer(t_observer* observer);
		virtual void remove_observer(t_observer* observer);

	protected:
		template<typename t_observer_method, typename ...t_args>
		void notify(t_observer_method method, t_args&& ...args);

	private:
		array<t_observer*> _observers;
	};

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	template<typename t_observer>
	class observer_base
	{
		friend class subject_base<t_observer>;

	public:
		virtual ~observer_base();

	private:
		void _add_subject(subject_base<t_observer>* subject);
		void _remove_subject(subject_base<t_observer>* subject);

		array<subject_base<t_observer>*> _subjects;
	};

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	template<typename t_observer>
	subject_base<t_observer>::~subject_base()
	{
		for(auto observer : _observers)
		{
			reinterpret_cast<observer_base<t_observer>*>(observer)->_remove_subject(this);
		}
	}

	template<typename t_observer>
	void subject_base<t_observer>::add_observer(t_observer* observer)
	{
		reinterpret_cast<observer_base<t_observer>*>(observer)->_add_subject(this);
		_observers.add(observer);
	}

	template<typename t_observer>
	void subject_base<t_observer>::remove_observer(t_observer* observer)
	{
		reinterpret_cast<observer_base<t_observer>*>(observer)->_remove_subject(this);
		_observers.remove_all(observer);
	}

	template<typename t_observer>
	template<typename t_observer_method, typename ...t_args>
	void subject_base<t_observer>::notify(t_observer_method method, t_args&& ...args)
	{
		typename decltype(_observers)::size_type i = 0;
		while(i < _observers.size())
		{
			// Save reference to current observer in container.
			auto observer = _observers[i];

			(observer->*method)(std::forward<t_args>(args)...);

			// If observer at current index did not change, we need to increment to the next observer.
			// Else, the current observer was removed and now index already points to the next one (do not increment).
			if(i < _observers.size() && _observers[i] == observer)
			{
				++i;
			}
		}
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	template<typename t_observer>
	observer_base<t_observer>::~observer_base()
	{
		for(auto subject : _subjects)
		{
			subject->remove_observer(static_cast<t_observer*>(this));
		}
	}

	template<typename t_observer>
	void observer_base<t_observer>::_add_subject(subject_base<t_observer>* subject)
	{
		_subjects.add(subject);
	}

	template<typename t_observer>
	void observer_base<t_observer>::_remove_subject(subject_base<t_observer>* subject)
	{
		_subjects.remove_all(subject);
	}
} // namespace bl
