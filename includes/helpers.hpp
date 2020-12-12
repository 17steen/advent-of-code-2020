#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <string_view>

template <typename T>
constexpr auto type_name() noexcept
{
    std::string_view name = "Error: unsupported compiler", prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
    prefix = "auto type_name() [T = ";
    suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto type_name() [with T = ";
    suffix = "]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "auto __cdecl type_name<";
    suffix = ">(void) noexcept";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}

class line_iterator
{
private:
    std::istream &ref;
    std::string str;
    std::size_t len;
    bool end;

public:
    line_iterator(std::istream &s) noexcept : ref(s)
    {
    }

    constexpr const std::string &operator*() const noexcept
    {
        return str;
    }
    line_iterator &operator++()
    {
        end = ref.eof();
        std::getline(ref, str);
        return *this;
    }
    constexpr line_iterator &operator+=(int n)
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
    constexpr bool operator!=(line_iterator &) const noexcept
    {
        return !end;
    }
};

class into_lines
{
private:
    std::istream &stream;

public:
    into_lines(std::istream &s) : stream(s) {}
    line_iterator begin()
    {
        //++ required in order not to populate std::string
        return ++line_iterator(stream);
    }
    //this serves no purpose other than fitting the norm
    line_iterator end()
    {
        return line_iterator(stream);
    }
};

class iter_file
{
private:
    std::ifstream str;

public:
    template <typename... Args>
    iter_file(Args &&... args) noexcept : str(std::forward<Args>(args)...)
    {
        if (!str.is_open())
            std::runtime_error("file did not open");
    }
    line_iterator begin()
    {
        return ++line_iterator(str);
    }
    line_iterator end()
    {
        return line_iterator(str);
    }
};

auto open(const std::string &path)
    -> std::string
{
    std::ifstream t(path, std::ios::in);
    if (!t.is_open())
    {
        throw std::runtime_error("file did not open");
    }
    std::stringstream str;
    str << t.rdbuf();
    return str.str();
}

auto open_lines(const std::string &path)
    -> std::vector<std::string>
{
    std::ifstream t(path, std::ios::in);
    std::vector<std::string> vec;
    std::string str;
    while (!t.eof())
    {
        std::getline(t, str);
        vec.emplace_back(std::move(str));
    }
    return vec;
}

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
        sum += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }
    return sum / amount;
}