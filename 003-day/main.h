#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

auto read_input(const std::string &path)
    -> std::vector<std::string>;

template <typename F, typename... Args>
double benchmark(F func, Args &&... args)
{
    int amount = 1000;
    double sum = 0;
    for (int i = 0; i < amount; ++i)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto t2 = std::chrono::high_resolution_clock::now();
        sum += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    }
    return sum / amount;
}

class into_lines
{
private:
    std::istream &stream;
    class iterator
    {
    private:
        std::istream &ref;
        std::string str;
        std::size_t len;
        bool end;

    public:
        iterator(std::istream &s) noexcept : ref(s)
        {
        }

        constexpr const std::string &operator*() const noexcept
        {
            return str;
        }
        iterator &operator++()
        {
            std::getline(ref, str);
            end = ref.eof();
            return *this;
        }
        constexpr iterator &operator+=(int n)
        {
            if (n < 0)
                [[unlikely]]
                {
                    throw std::out_of_range("cannot be negative");
                }

            while (n--)
                ++*this;
            return *this;
        }
        constexpr bool operator!=(iterator &) const noexcept
        {
            return !end;
        }
        friend class into_lines;
    };

public:
    into_lines(std::istream &s) : stream(s) {}
    iterator begin()
    {
        //++ required in order not to populate std::string
        return ++iterator(stream);
    }
    //this serves no purpose other than fitting the norm
    iterator end()
    {
        return iterator(stream);
    }
};
