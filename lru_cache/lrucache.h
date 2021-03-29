#pragma once
#include <unordered_map>
#include <list>
#include <optional>
template<class Key,class Value>
class lru_cache
{
public:
	using key_type = Key;
	using value_type = Value ;
	using list_type = std::list<key_type>;
	using map_type = std::unordered_map<Key, std::pair<value_type, typename list_type::iterator>>;

	explicit lru_cache(size_t capacity) :m_capacity(capacity)
	{

	}
	~lru_cache() = default;

	size_t size() const
	{
		return m_map.size();
	}

	size_t capacity() const
	{
		return m_capacity;
	}

	bool empty() const
	{
		return m_map.empty();
	}

	bool contains(const key_type& key)
	{
		return m_map.find(key) != m_map.end();
	}

	void insert(const key_type& key, const value_type& value)
	{
		typename map_type::iterator i = m_map.find(key);
		if (i == m_map.end()) 
		{
			
			if (size() >= m_capacity) 
			{
				evict();
			}

			m_list.push_front(key);
			m_map[key] = std::make_pair(value, m_list.begin());
		}
	}

	std::optional<value_type> get(const key_type& key)
	{
		
		typename map_type::iterator i = m_map.find(key);
		if (i == m_map.end()) {
			
			return std::nullopt;
		}

		typename list_type::iterator j = i->second.second;
		if (j != m_list.begin()) 
		{
			m_list.erase(j);
			m_list.push_front(key);

			
			j = m_list.begin();
			const value_type& value = i->second.first;
			m_map[key] = std::make_pair(value, j);

			return value;
		}
		else 
		{
			return i->second.first;
		}
	}

	void clear()
	{
		m_map.clear();
		m_list.clear();
	}

private:
	void evict()
	{
		// evict item from the end of most recently used list
		typename list_type::iterator i = --m_list.end();
		m_map.erase(*i);
		m_list.erase(i);
	}

private:
	map_type m_map;
	list_type m_list;
	size_t m_capacity;
};