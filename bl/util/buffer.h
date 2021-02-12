#pragma once
#include <bl/util/array.h>

namespace bl
{
	template<typename t_value, typename t_size = int>
	class buffer
	{
		//static_assert(std::is_trivially_copyable<t_value>::value, "buffer can only contain trivially copyable classes");
	public:
		typedef t_value value_type;
		typedef t_size size_type;

		buffer() = default;
		explicit buffer(t_size initial_capacity);
		buffer(std::initializer_list<t_value> list);
		buffer(t_value* src, t_size src_size);
		buffer(t_size new_size, const t_value& default_value);

		buffer(const buffer&) = delete;
		buffer& operator=(const buffer&) = delete;

		buffer(buffer&& other);
		buffer& operator=(buffer&& other);

		void reset(t_size new_capacity);
		void reset(t_size new_size, const t_value& default_value);
		void reset(t_value* src, t_size src_size);

		void clear();

		void add(const t_value& value);
		void add(t_value&& value);

		t_value& operator[](t_size index);
		const t_value& operator[](t_size index) const;

		t_value& front();
		const t_value& front() const;
		t_value& back();
		const t_value& back() const;

		t_value* begin();
		const t_value* begin() const;
		t_value* end();
		const t_value* end() const;

		t_value* ptr();
		const t_value* ptr() const;

		bool empty_capacity() const;
		t_size capacity() const;
		t_size capacity_bytes() const;

		bool empty_size() const;
		t_size size() const;
		t_size size_bytes() const;

		bool contains(const t_value& value) const;
		t_size index_of(const t_value& value) const;

	private:
		array<t_value, t_size> _memory;
		t_size _size;
	};

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	template<typename t_value, typename t_size>
	buffer<t_value, t_size>::buffer(t_size initial_capacity)
		: _memory(initial_capacity), _size(0)
	{
	}

	template<typename t_value, typename t_size>
	buffer<t_value, t_size>::buffer(std::initializer_list<t_value> list)
		: _memory(list), _size(list.size())
	{
	}

	template<typename t_value, typename t_size>
	buffer<t_value, t_size>::buffer(t_value* src, t_size src_size)
		: _memory(src, src_size), _size(src_size)
	{
	}

	template<typename t_value, typename t_size>
	buffer<t_value, t_size>::buffer(t_size new_size, const t_value& default_value)
		: _memory(new_size, default_value), _size(new_size)
	{
	}

	template<typename t_value, typename t_size>
	buffer<t_value, t_size>::buffer(buffer<t_value, t_size>&& other)
		: _memory(other._memory), _size(other._size)
	{
		other._size = 0;
	}

	template<typename t_value, typename t_size>
	buffer<t_value, t_size>& buffer<t_value, t_size>::operator=(buffer<t_value, t_size>&& other)
	{
		_memory = other._memory;
		_size = other._size;
		other._size = 0;
		return *this;
	}

	template<typename t_value, typename t_size>
	void buffer<t_value, t_size>::reset(t_size new_capacity)
	{
		_memory.reset(new_capacity);
		_size = 0;
	}

	template<typename t_value, typename t_size>
	void buffer<t_value, t_size>::reset(t_size new_size, const t_value& default_value)
	{
		_memory.reset(new_size, default_value);
		_size = new_size;
	}

	template<typename t_value, typename t_size>
	void buffer<t_value, t_size>::reset(t_value* src, t_size src_size)
	{
		_memory.reset(src, src_size);
		_size = src_size;
	}

	template<typename t_value, typename t_size>
	void buffer<t_value, t_size>::clear()
	{
		_size = 0;
	}

	template<typename t_value, typename t_size>
	void buffer<t_value, t_size>::add(const t_value& value)
	{
		_memory[_size++] = value;
	}

	template<typename t_value, typename t_size>
	void buffer<t_value, t_size>::add(t_value&& value)
	{
		_memory[_size++] = std::move(value);
	}

	template<typename t_value, typename t_size>
	t_value& buffer<t_value, t_size>::operator[](t_size index)
	{
		return _memory[index];
	}

	template<typename t_value, typename t_size>
	const t_value& buffer<t_value, t_size>::operator[](t_size index) const
	{
		return _memory[index];
	}

	template<typename t_value, typename t_size>
	t_value& buffer<t_value, t_size>::front()
	{
		return _memory.front();
	}

	template<typename t_value, typename t_size>
	const t_value& buffer<t_value, t_size>::front() const
	{
		return _memory.front();
	}

	template<typename t_value, typename t_size>
	t_value& buffer<t_value, t_size>::back()
	{
		return _memory.back();
	}

	template<typename t_value, typename t_size>
	const t_value& buffer<t_value, t_size>::back() const
	{
		return _memory.back();
	}

	template<typename t_value, typename t_size>
	t_value* buffer<t_value, t_size>::begin()
	{
		return _memory.begin();
	}

	template<typename t_value, typename t_size>
	const t_value* buffer<t_value, t_size>::begin() const
	{
		return _memory.begin();
	}

	template<typename t_value, typename t_size>
	t_value* buffer<t_value, t_size>::end()
	{
		return _memory.begin() + _size;
	}

	template<typename t_value, typename t_size>
	const t_value* buffer<t_value, t_size>::end() const
	{
		return _memory.begin() + _size;
	}

	template<typename t_value, typename t_size>
	t_value* buffer<t_value, t_size>::ptr()
	{
		return _memory.ptr();
	}

	template<typename t_value, typename t_size>
	const t_value* buffer<t_value, t_size>::ptr() const
	{
		return _memory.ptr();
	}

	template<typename t_value, typename t_size>
	bool buffer<t_value, t_size>::empty_capacity() const
	{
		return _memory.empty();
	}

	template<typename t_value, typename t_size>
	t_size buffer<t_value, t_size>::capacity() const
	{
		return _memory.size();
	}

	template<typename t_value, typename t_size>
	t_size buffer<t_value, t_size>::capacity_bytes() const
	{
		return _memory.size_bytes();
	}

	template<typename t_value, typename t_size>
	bool buffer<t_value, t_size>::empty_size() const
	{
		return _size == 0;
	}

	template<typename t_value, typename t_size>
	t_size buffer<t_value, t_size>::size() const
	{
		return _size;
	}

	template<typename t_value, typename t_size>
	t_size buffer<t_value, t_size>::size_bytes() const
	{
		return _size * sizeof(t_value);
	}

	template<typename t_value, typename t_size>
	bool buffer<t_value, t_size>::contains(const t_value& value) const
	{
		return _memory.contains(value);
	}

	template<typename t_value, typename t_size>
	t_size buffer<t_value, t_size>::index_of(const t_value& value) const
	{
		return _memory.index_of(value);
	}
} // namespace bl
