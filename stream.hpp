// See: https://github.com/dionyziz/stream.js/blob/master/stream.js

#include <stdexcept>
#include <functional>
#include <memory>

template <class T>
class Stream
{
private:
	std::shared_ptr<T> headValue_;
	std::function<std::shared_ptr<Stream<T>>()> tailPromise_;

public:
	Stream(std::shared_ptr<T> headValue = nullptr, std::function<std::shared_ptr<Stream<T>>()> tailPromise = nullptr)
	{
		headValue_ = headValue;
		if (tailPromise != nullptr) {
			tailPromise_ = tailPromise;
		} else {
			tailPromise_ = [] () -> std::shared_ptr<Stream<T>> {
				return std::shared_ptr<Stream<T>>(new Stream<T>());
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
	std::shared_ptr<T> head()
	{
		return headValue_;
	}
	auto tail() -> decltype(tailPromise_())
	{
		if (empty())
			throw std::logic_error("Cannot get tail of empty stream.");
		return tailPromise_(); // TODO: remember the result, set function to nullptr
	}
	// TODO: length, add, append, zip, map
	std::shared_ptr<Stream<T>> filter(std::function<bool(T)> f)
	{
		if (empty())
			return std::shared_ptr<Stream<T>>(new Stream<T>());
		auto headValue = head();
		if (f(*headValue))
			return std::shared_ptr<Stream<T>>(new Stream<T>(headValue, [this,f] {
				return this->tail()->filter(f);
			}));
		return this->tail()->filter(f);	
	}
	std::shared_ptr<Stream<T>> take(int number)
	{
		if (empty() || number == 0)
			return std::shared_ptr<Stream<T>>(new Stream<T>());
		return std::shared_ptr<Stream<T>>(new Stream<T>(head(), [this, number] () -> std::shared_ptr<Stream<T>> {
			return this->tail()->take(number - 1);
		}));
	}
	template <class U>
	std::shared_ptr<Stream<U>> map(std::function<U(T)> f) {
		if (empty())
			return std::shared_ptr<Stream<U>>(new Stream<U>());
		return std::shared_ptr<Stream<U>>(new Stream<U>(std::shared_ptr<U>(new U(f(*head()))), [this, f] () -> std::shared_ptr<Stream<U>> {
			return this->tail()->map(f);
		}));
	}
	void walk(std::function<void(T)> f) {
		if (avail()) {
			f(*head());
			auto s = tail();
			while(s->avail()) {
				f(*s->head());
				s = s->tail();
			}
		}
	}
	static std::shared_ptr<Stream<int>> makeOnes() {
		T* i = new T(1);
		return std::shared_ptr<Stream<int>>(new Stream<int>(i, [] () -> std::shared_ptr<Stream<int>> {
			return Stream<int>::makeOnes();
		}));
	}
	static std::shared_ptr<Stream<int>> range(int low, int high) {
		return std::shared_ptr<Stream<int>>(new Stream<int>(std::shared_ptr<T>(new T(low)), [low, high] () -> std::shared_ptr<Stream<int>> {
			return Stream<int>::range(low + 1, high);
		}));
	}
};
