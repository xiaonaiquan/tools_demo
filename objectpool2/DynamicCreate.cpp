#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <unordered_map>
#include <functional>
#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace CQ
{

	class Actor
	{
	public:
		Actor() { std::cout << "Actor construct" << std::endl; }
		virtual ~Actor() {};
		virtual void Say() { std::cout << "Actor" << std::endl; }
	};

	template<typename ...Targs>
	class ActorFactory
	{
	public:
		//typedef Actor* (*ActorCreateFunction)();
		//std::function< Actor*(Targs...args) > pp;

		static ActorFactory* Instance() {
			std::cout << "static ActorFactory* Instance()" << std::endl;
			if (!m_pActorFactory) { m_pActorFactory = new ActorFactory(); }
			return m_pActorFactory;
		}

		virtual ~ActorFactory() {};

		//Lambda: static std::string ReadTypeName(const char * name)

		//bool Regist(const std::string& strTypeName, ActorCreateFunction pFunc)
		//bool Regist(const std::string& strTypeName, std::function<Actor*()> pFunc)
		bool Regist(const std::string& strTypeName, std::function<Actor* (Targs&&... args)> pFunc) {
			std::cout << "bool ActorFactory::Regist(const std::string& strTypeName, std::function<Actor*(Targs... args)> pFunc)" << std::endl;
			if (!pFunc) { return false; }
			bool bReg = m_mapCreateFunction.insert(std::make_pair(strTypeName, pFunc)).second;
			std::cout << "m_mapCreateFunction.size() =" << m_mapCreateFunction.size() << std::endl;
			return bReg;
		}

		Actor* Create(const std::string& strTypeName, Targs&&... args) {
			std::cout << "Actor* ActorFactory::Create(const std::string& strTypeName, Targs... args)" << std::endl;
			auto iter = m_mapCreateFunction.find(strTypeName);
			if (iter == m_mapCreateFunction.end()) { return nullptr; }
			else {
				//return(iter->second());
				return iter->second(std::forward<Targs>(args)...);
			}
		}

	private:
		ActorFactory() { std::cout << "ActorFactory construct" << std::endl; };
		static ActorFactory<Targs...>* m_pActorFactory;
		std::unordered_map<std::string, std::function<Actor* (Targs&&...)> > m_mapCreateFunction;
	};

	template<typename ...Targs>
	ActorFactory<Targs...>* ActorFactory<Targs...>::m_pActorFactory = nullptr;

	template<typename T, typename ...Targs>
	class DynamicCreator
	{
	public:
		struct Register
		{
			Register() {
				std::cout << "DynamicCreator.Register construct" << std::endl;
				char* szDemangleName = nullptr;
				std::string strTypeName;

#ifdef _MSC_VER
				szDemangleName = const_cast<char*>(typeid(T).name());
				if (szDemangleName) strTypeName = szDemangleName + 6; // 6 is strlen("class ")
#elif defined(__GNUC__)
				szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
				if (szDemangleName) {
					strTypeName = szDemangleName;
					free(szDemangleName);
				}
#else
#error unexpected c complier (msc/gcc), Need to implement this method for demangle
#endif
				ActorFactory<Targs...>::Instance()->Regist(strTypeName, CreateObject);
			}
			inline void do_nothing()const { };
		};
		DynamicCreator() {
			std::cout << "DynamicCreator construct" << std::endl;
			m_oRegister.do_nothing();
		}
		virtual ~DynamicCreator() { m_oRegister.do_nothing(); };

		static T* CreateObject(Targs&&... args) {
			std::cout << "static Actor* DynamicCreator::CreateObject(Targs... args)" << std::endl;
			return new T(std::forward<Targs>(args)...);
		}

		virtual void Say() { std::cout << "DynamicCreator say" << std::endl; }
		static Register m_oRegister;
	};

	template<typename T, typename ...Targs>
	typename DynamicCreator<T, Targs...>::Register DynamicCreator<T, Targs...>::m_oRegister;

	class Cmd : public Actor, public DynamicCreator<Cmd>
	{
	public:
		Cmd() { std::cout << "Create Cmd " << std::endl; }
		virtual void Say() { std::cout << "I am Cmd" << std::endl; }
	};

	class Step : public Actor, DynamicCreator<Step, std::string, int>
	{
	public:
		Step(const std::string& strType, int iSeq) { std::cout << "Create Step: " << strType << " " << iSeq << std::endl; }
		virtual void Say() { std::cout << "I am Step" << std::endl; }
	};

	class Test : public Actor, DynamicCreator<Test>, DynamicCreator<Test, std::string, int>
	{
	public:
		Test() { std::cout << "Create Test! " << std::endl; }
		Test(const std::string& strType, int iSeq) { std::cout << "Create Test: " << strType << " " << iSeq << std::endl; }
		virtual void Say() { std::cout << "I am Test" << std::endl; }
	};

	class ObjMaker
	{
	public:
		template<typename ...Targs>
		static Actor* CreateActor(const std::string& strTypeName, Targs&&... args) {
			return ActorFactory<Targs...>::Instance()->Create(strTypeName, std::forward<Targs>(args)...);
		}
	};
}

int main()
{
	std::cout << "----------------------------------------------------------------------" << std::endl;
	CQ::Actor* p1 = CQ::ObjMaker::CreateActor(std::string("CQ::Cmd"));
	if (p1) p1->Say();

	std::cout << "----------------------------------------------------------------------" << std::endl;
	CQ::Actor* p2 = CQ::ObjMaker::CreateActor(std::string("CQ::Step"), std::string("Good job"), 1002);
	if (p2) p2->Say();

	std::cout << "----------------------------------------------------------------------" << std::endl;
	auto p3 = CQ::ObjMaker::CreateActor(std::string("CQ::Test"));
	if (p3) p3->Say();

	std::cout << "----------------------------------------------------------------------" << std::endl;
	auto p4 = CQ::ObjMaker::CreateActor(std::string("CQ::Test"), std::string("dqsjqian"), 1002);
	if (p4) p4->Say();

	return 0;
}
