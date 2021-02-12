#pragma once
#include <unordered_set>
#include <unordered_map>

namespace bl
{
	template<typename t_value,
			 typename t_hash = std::hash<t_value>,
			 typename t_pred = std::equal_to<t_value>,
			 typename t_alloc = std::allocator<t_value>>
	using hash_set = std::unordered_set<t_value, t_hash, t_pred, t_alloc>;

	template<typename t_value,
			 typename t_hash = std::hash<t_value>,
			 typename t_pred = std::equal_to<t_value>,
			 typename t_alloc = std::allocator<t_value>>
	using hash_multiset = std::unordered_multiset<t_value, t_hash, t_pred, t_alloc>;

	template<typename t_key,
			 typename t_value,
			 typename t_hash = std::hash<t_key>,
			 typename t_pred = std::equal_to<t_key>,
			 typename t_alloc = std::allocator<std::pair<const t_key, t_value>>>
	using hash_map = std::unordered_map<t_key, t_value, t_hash, t_pred, t_alloc>;

	template<typename t_key,
			 typename t_value,
			 typename t_hash = std::hash<t_key>,
			 typename t_pred = std::equal_to<t_key>,
			 typename t_alloc = std::allocator<std::pair<const t_key, t_value>>>
	using hash_multimap = std::unordered_multimap<t_key, t_value, t_hash, t_pred, t_alloc>;
} // namespace bl
