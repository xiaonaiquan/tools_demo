#pragma once
#include <time.h>
#include <unordered_map>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <list>
#include <unordered_map>

template < class KeyType, class ValueType, class Hash = std::hash<KeyType>, class Pred = std::equal_to<KeyType> >
class LruMap
{
public:
	struct data_type
	{
		KeyType             key;
		ValueType           value;
		time_t              expiration;
		data_type(const KeyType& x_key, const ValueType& x_value, time_t x_expiration)
			: key(x_key), value(x_value), expiration(x_expiration)
		{}
	};

private:
	typedef std::list<data_type>                                    list_type;
	typedef typename list_type::iterator                            list_iterator;
	typedef std::unordered_map<KeyType, list_iterator, Hash, Pred>  map_type;

	size_t                  _capacity;
	map_type                _index;
	list_type               _nodelist;

public:
	typedef typename list_type::iterator            iterator;
	typedef typename list_type::const_iterator      const_iterator;

	LruMap(size_t capacity = 1024) : _capacity(capacity), _index(), _nodelist()
	{
		_index.reserve(capacity);
	}

	~LruMap()
	{}

	void clear()
	{
		_index.clear();
		_nodelist.clear();
	}

	size_t  capacity() const { return _capacity; }
	bool    empty() const { return _index.empty(); }
	size_t  size() const { return _index.size(); }

	const_iterator begin() const { return _nodelist.begin(); }
	iterator       begin() { return _nodelist.begin(); }
	const_iterator end() const { return _nodelist.end(); }
	iterator       end() { return _nodelist.end(); }

	void capacity(size_t n)
	{
		_capacity = n;
		if (_capacity == 0)
		{
			_index.clear();
			_nodelist.clear();
		}
		else
		{
			_index.reserve(n);
		}
	}

	iterator get(const KeyType& key, bool erasing_expired = true)
	{
		auto map_it = _index.find(key);
		if (map_it == _index.end())
		{
			return end();
		}
		auto list_it = map_it->second;
		if (erasing_expired && time(NULL) > list_it->expiration)
		{
			erase(key);
			return end();
		}
		_nodelist.splice(_nodelist.end(), _nodelist, list_it);
		return list_it;
	}

	iterator put(const KeyType& key, const ValueType& value, time_t expiration_sec)
	{
		auto map_it = _index.find(key);
		if (map_it != _index.end())
		{
			auto list_it = map_it->second;
			list_it->value = value;
			list_it->expiration = expiration_sec + time(NULL);
			_nodelist.splice(_nodelist.end(), _nodelist, list_it);
			return list_it;
		}

		if (_index.size() >= _capacity)
		{
			vacate(16);
		}
		_nodelist.push_back(data_type(key, value, expiration_sec + time(NULL)));
		auto list_it = _nodelist.end();
		--list_it;
		_index.insert(std::make_pair(key, list_it));
		return list_it;
	}

	void erase(const KeyType& key)
	{
		auto map_it = _index.find(key);
		if (map_it == _index.end())
		{
			return;
		}
		auto list_it = map_it->second;
		_nodelist.erase(list_it);
		_index.erase(map_it);
	}
	void erase(iterator it)
	{
		_index.erase(it->key);
		_nodelist.erase(it);
	}

	void vacate(size_t n)
	{
		auto it = _nodelist.begin();
		for (size_t i = 0; it != _nodelist.end() && i < n; ++i)
		{
			_index.erase(it->key);
			it = _nodelist.erase(it);
		}
	}
};