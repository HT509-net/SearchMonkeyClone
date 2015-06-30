#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton : private T
{
public:
	static T* Instance();

private:
	Singleton() {}
	virtual ~Singleton() {}
};

template <typename T>
T* Singleton<T>::Instance()
{
	static Singleton<T> obj;

	return &obj;
}

#endif
