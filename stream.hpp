#include <functional>
#include <memory>

template <class T>
class Stream
{
private:
	T* headValue_;
	std::function<Stream<T>*()> tailPromise_;

public:
	Stream(T* headValue = nullptr, std::function<Stream<T>*()> tailPromise = nullptr)
	{
		headValue_ = headValue;
		if (tailPromise != nullptr) {
			tailPromise_ = tailPromise;
		} else {
			tailPromise_ = []{ return new Stream<T>(); };
		}
	}
	bool empty()
	{
		return headValue_ == nullptr;
	}
	T* head()
	{
		return headValue_;
	}
	Stream<T>* tail()
	{
		if (empty())
			throw "Cannot get tail of empty stream.";
		return tailPromise_();
	}
	// length, add, append, zip, map
	Stream<T>* filter(std::function<bool(T)> f)
	{
		if (empty())
			return new Stream<T>();
		auto headValue = head();
		if (f(headValue))
			return new Stream<T>(headValue, [&tailPromise_,&f] {
				return tailPromise_.filter(f);
			});
		return tailPromise_.filter(f);	
	}
	Stream<T>* take(int number)
	{
		if (empty() || number == 0)
			return new Stream<T>();
		return new Stream<T>(head(), [this, number] {
			return this->tail()->take(number - 1);
		});
	}
};

Stream<int>* Stream_makeOnes() {
	int i = 1;
	return new Stream<int>(&i, [Stream] (int) {
		return Stream_makeOnes();
	});
}
