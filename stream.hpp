#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED


#include <functional>

template <class T>
class Stream
{
public:
    Stream()
    :
        empty(true)
    {
    }
    Stream(T data, std::function<Stream<T>()> tail)
    :
        empty(false),
        data(data),
        tail(tail)
    {
    }
    static Stream<T> range(T from)
    {
        return Stream<T>(from, [=] { return Stream<T>::range(from + 1); });
    }
    Stream<T> take(int count)
    {
        if (empty || count == 0)
            return Stream();
        --count;

        Stream a = *this;
        return Stream<T>(data, [=] { return a.tail().take(count); });
    }
    template <class U>
    Stream<U> map(std::function<U(T)> func)
    {
        if (empty)
            return Stream<U>();

        Stream a = *this;
        return Stream<U>(func(data), [=] { return a.tail().map<U>(func); });
    }
    Stream<T> filter(std::function<bool(T)> func)
    {
        if (empty)
            return Stream();

        Stream a = *this;
        if (func(data))
            return Stream<T>(data, [=] { return a.tail().filter(func); });
        return a.tail().filter(func);
    }
    void walk(std::function<void(T)> func)
    {
        func(data);
        auto t = tail();
        if (!t.empty)
            tail().walk(func);
    }
private:
    bool empty;
    T data;
    std::function<Stream<T>()> tail;
};


#endif // STREAM_H_INCLUDED

