#pragma once

template <typename T>
class IObserver {
public:
	virtual ~IObserver() {}
	virtual void Renewal(const T& value) = 0;
};
