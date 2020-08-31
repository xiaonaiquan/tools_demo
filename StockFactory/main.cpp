/*
C++ 中线程安全的对象回调
https://blog.csdn.net/loveyumomo/article/details/23244201
*/

#include <iostream>
#include <functional>
#include <mutex>
#include <map>
#include <assert.h> 

class noncopyable
{
protected:
	noncopyable() {}
	~noncopyable() {}
private:  // emphasize the following members are private
	noncopyable(const noncopyable&);
	const noncopyable& operator=(const noncopyable&);
};

class Stock
{
public:
	Stock(const std::string& key) 
	{
		key_ = key;
	}
	~Stock() 
	{
		std::cout << "stock xigou" << std::endl;
	}

	std::string key()
	{
		return key_;
	}

private:
	std::string key_;
};

class StockFactory : public std::enable_shared_from_this<StockFactory>,noncopyable
{
public:
	std::shared_ptr<Stock> get(const std::string& key)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		std::shared_ptr<Stock> pStock;
		std::weak_ptr<Stock>& wkStock = stocks_[key];
		pStock = wkStock.lock();
		if (!pStock) 
		{
			pStock.reset(new Stock(key),
				std::bind(&StockFactory::weakDeleteCallback,
					std::weak_ptr<StockFactory>(shared_from_this()),
					std::placeholders::_1));
			// 上面必须强制把 shared_from_this() 转型为 weak_ptr，才不会延长生命期  
			wkStock = pStock;
		}
		return pStock;
	}

	~StockFactory()
	{
		std::cout << "StockFactory xigou" << std::endl;
	}

private:
	static void weakDeleteCallback(std::weak_ptr<StockFactory> wkFactory,
		Stock* stock)
	{
		std::shared_ptr<StockFactory> factory(wkFactory.lock());
		if (factory) 
		{  // 如果 factory 还在，那就清理 stocks_  
			factory->removeStock(stock);
		}
		delete stock;  // sorry, I lied  
	}

	void removeStock(Stock* stock)
	{
		if (stock) 
		{
			std::lock_guard<std::mutex> locker(m_mutex);
			stocks_.erase(stock->key());
		}
	}

private:
	std::map<std::string, std::weak_ptr<Stock> > stocks_;
	mutable std::mutex m_mutex;
};

void test1()
{
	std::shared_ptr<StockFactory> factory(new StockFactory);
	{
		std::shared_ptr<Stock> stock = factory->get("NYSE:IBM");
		std::shared_ptr<Stock> stock2 = factory->get("NYSE:IBM");
		assert(stock == stock2);
		// stock destructs here  
	}
	// factory destructs here  
}

void test2()
{
	std::shared_ptr<Stock> stock;
	{
		std::shared_ptr<StockFactory> factory(new StockFactory);
		stock = factory->get("NYSE:IBM");
		std::shared_ptr<Stock> stock2 = factory->get("NYSE:IBM");
		assert(stock == stock2);
		// factory destructs here  
	}
	// stock destructs here  
}

int main()
{
	test2();
	system("PAUSE");
	return 0;
}