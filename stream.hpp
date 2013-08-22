#include <functional>
// #include <memory>

template <class T>
class Stream
{
private:
	typedef Stream<T>* (*fTail)();
	T* headValue_;
	std::function<Stream<T>*()> tailPromise_;

public:
	Stream(T* headValue = nullptr, std::function<Stream<T>*()> tailPromise = nullptr)
	{
		headValue_ = headValue;
		if (tailPromise != nullptr) {
			tailPromise_ = tailPromise;
		} else {
			tailPromise_ = [] () -> Stream<T>* {
				return new Stream<T>();
			};
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
		return new Stream<T>(head(), [this, number] () -> Stream<T>* {
			return this->tail()->take(number - 1);
		});
	}
	static Stream<int>* makeOnes() {
		int* i = new int(1);
		return new Stream<int>(i, [] () -> Stream<int>* {
			return Stream<int>::makeOnes();
		});
	}
	static Stream<int>* range(int low, int high) {
		int* i = new int(low);
		return new Stream<int>(i, [low, high] () -> Stream<int>* {
			return Stream<int>::range(low + 1, high);
		});
	}
};
