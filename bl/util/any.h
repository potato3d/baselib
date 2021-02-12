#pragma once

namespace bl
{
	class any
	{
	public:
		inline any();

		template<typename t_value>
		inline any(const t_value& value);

		inline any(const any& other);

		inline bool operator==(const any& other) const;

		template<typename t_value>
		inline bool contains() const;

		template<typename t_value>
		inline const t_value& get() const;

	private:
		class iholder
		{
		public:
			virtual ~iholder() {}
			virtual iholder* clone() const = 0;
			virtual bool equals(iholder* other) const = 0;
		};

		template<typename t_value>
		class holder : public iholder
		{
		public:
			inline holder(const t_value& value);
			inline virtual iholder* clone() const override;
			inline virtual bool equals(iholder* other) const override;
			inline const t_value& get() const;

		private:
			t_value _value;
		};

		iholder* _holder;
	};

	any::any()
		: _holder(nullptr)
	{
	}

	template<typename t_value>
	any::any(const t_value& value)
		: _holder(new holder<t_value>(value))
	{
	}

	any::any(const any& other)
		: _holder(other._holder? other._holder->clone() : nullptr)
	{
	}

	bool any::operator==(const any& other) const
	{
		return _holder->equals(other._holder);
	}

	template<typename t_value>
	bool any::contains() const
	{
		return dynamic_cast<holder<t_value>*>(_holder) != nullptr;
	}

	template<typename t_value>
	const t_value& any::get() const
	{
		return dynamic_cast<holder<t_value>*>(_holder)->get();
	}

	template<typename t_value>
	any::holder<t_value>::holder(const t_value& value)
		: _value(value)
	{
	}

	template<typename t_value>
	any::iholder* any::holder<t_value>::clone() const
	{
		return new holder<t_value>(_value);
	}

	template<typename t_value>
	bool any::holder<t_value>::equals(iholder* other) const
	{
		auto other_holder = dynamic_cast<holder<t_value>*>(other);
		if(other_holder == nullptr)
		{
			return false;
		}
		return _value == other_holder->_value;
	}

	template<typename t_value>
	const t_value& any::holder<t_value>::get() const
	{
		return _value;
	}
} // namespace bl
