#pragma once

#include "Observer.h"

template <typename T>
class ISubject {
public:
	virtual ~ISubject() {}
	virtual void AddSubscriber(IObserver<T>* observer) = 0;
	virtual void RemoveSubscriber(IObserver<T>* observer) = 0;
};
