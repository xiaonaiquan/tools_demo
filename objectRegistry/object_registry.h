#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <functional> 

template <typename T>
class ObjectRegistry
{
public:

	typedef std::function<T*()> ConstructFn;
	typedef std::unordered_map<std::string, ConstructFn> RegistryMap;
	template <typename ST, class = typename std::enable_if<std::is_base_of<T, ST>::value>::type>
	static void register_object(std::string&& name) {
		register_object(std::forward<std::string>(name), []() -> T* {
			return new ST();
		});
	}

	static void register_object(std::string&& name, ConstructFn&& fn)
	{
		if (name.empty()) {
			return;
		}
		if (object_register.find(name) != object_register.end()) {
			printf("register object error,name:%s is exists\n", name.c_str());
			return;
		}
		//printf("register object name:%s,current object:%zd\n",name.c_str(),object_register.size());
		object_register.emplace(std::forward<std::string>(name), std::move(fn));
	}
	static T *get_object(const std::string &name)
	{
		if (object_register.find(name) != object_register.end())
		{
			ConstructFn fn = object_register[name];
			return fn();
		}
		return NULL;
	}
public:
	static RegistryMap object_register;
};