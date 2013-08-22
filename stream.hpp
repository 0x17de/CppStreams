// See: https://github.com/dionyziz/stream.js/blob/master/stream.js

#include <functional>

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
	bool avail()
	{
		return headValue_ != nullptr;
	}
	T* head()
	{
		return headValue_;
	}
	Stream<T>* tail()
	{
		if (empty())
			throw "Cannot get tail of empty stream.";
		return tailPromise_(); // TODO: remember the result, set function to nullptr
	}
	// TODO: length, add, append, zip, map
	Stream<T>* filter(std::function<bool(T)> f)
	{
		if (empty())
			return new Stream<T>();
		auto headValue = head();
		if (f(*headValue))
			return new Stream<T>(headValue, [this,f] {
				return this->tail()->filter(f);
			});
		return this->tail()->filter(f);	
	}
	Stream<T>* take(int number)
	{
		if (empty() || number == 0)
			return new Stream<T>();
		return new Stream<T>(head(), [this, number] () -> Stream<T>* {
			return this->tail()->take(number - 1);
		});
	}
	template <class U>
	Stream<U>* map(std::function<U(T)> f) {
		if (empty())
			return new Stream<U>();
		float* i = new float(f(*head()));
		return new Stream<U>(i, [this, f] () -> Stream<U>* {
			return this->tail()->map(f);
		});
	}
	void walk(std::function<void(T)> f) {
		Stream<T>* s = this;
		while(s->avail()) {
			f(*s->head());
			s = s->tail();
		}
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
