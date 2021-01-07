#include <iostream>
#include "objectmanager.h"
#include "objectpool.h"
#include "destroyable.h"

using namespace std;

int i = 0;

class Data : public Destroyable
{
public:
	Data()
	{
		cout << "gouzao print" << endl;
	}
	~Data()
	{
		cout << "xigou print" << endl;
		i++;
	}

	void setData(std::string data)
	{
		cout << data.c_str() << endl;
	}
};

typedef ObjectManager<Data> DataMangaer;

int main()
{
	{
		ObjectPoolAllocator<Data> dataAlloctor(1024, 4096, 1024);

		DataMangaer a(&dataAlloctor);

		Data * data = a.Create();
		if (data)
		{
			cout << "data not empty" << endl;
		}
		data->setData("aaaa");

		a.call(data);

		cout << "finish" << endl;

	}
	cout << i << endl;
	system("PAUSE");
	return 0;
}
