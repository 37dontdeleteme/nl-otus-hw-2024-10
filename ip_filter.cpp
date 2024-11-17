#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <array>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}
using ip_int_vector = std::vector<std::array<int, 4>>;
using ip_int_tuple = std::tuple<int, int, int, int>;

template<typename... Args>
ip_int_vector filter(const ip_int_vector &ip_pool, Args... args) {
  static_assert(sizeof...(args) >= 1 && sizeof...(args) <= 4);
  ip_int_vector filtered_pool;
  for(const auto &ip : ip_pool) {
    int i = 0;
    bool is_push = false;
    for(const auto & filter_arg : {args...}) {
      if(ip.at(i) == filter_arg)
        is_push = true;
      else {
        is_push = false;
        break;
      }
      i++;
    }
    if(is_push)
      filtered_pool.emplace_back(ip);
  }
  return filtered_pool;
}

ip_int_vector filter_any(const ip_int_vector &ip_pool, int value) {
  ip_int_vector filtered_ip_pool;
  for(const auto &ip : ip_pool)
    for(const auto &octet : ip)
      if(octet == value) {
        filtered_ip_pool.emplace_back(ip);
        break;
      }
  return filtered_ip_pool;
}

void reverse_lexicographically_sort(ip_int_vector& source_ip) {
  std::vector<ip_int_tuple> int_ip;
  for(const auto& pool : source_ip)
    int_ip.push_back(std::make_tuple(pool.at(0), pool.at(1), pool.at(2), pool.at(3)));
  std::sort(int_ip.begin(), int_ip.end(), 
  [](const ip_int_tuple& a, const ip_int_tuple& b) -> bool{
    return a > b;
  });
  source_ip.clear();
  for(const auto& pool: int_ip)
    source_ip.push_back({std::get<0>(pool),
                         std::get<1>(pool),
                         std::get<2>(pool),
                         std::get<3>(pool)
                        });
}

void print_ip_pool(const ip_int_vector &ip_pool) {
  for(ip_int_vector::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            for(std::array<int, 4>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
                if (ip_part != ip->cbegin())
                {
                    std::cout << ".";

                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }
}

int main()
{
    try
    {
        ip_int_vector ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            int i = 0;
            std::array<int, 4> int_ip;
            for(const auto &octet : split(v.at(0), '.')) {
              int_ip.at(i) = std::stoi(octet);
              i++;
            }
            ip_pool.push_back(int_ip);
        }

        reverse_lexicographically_sort(ip_pool);
        // TODO reverse lexicographically sort

        print_ip_pool(ip_pool);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
        decltype(ip_pool) filtered_pool1 = filter(ip_pool, 1);
        print_ip_pool(filtered_pool1);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        decltype(auto) filtered_pool2 = filter(ip_pool, 46, 70);
        print_ip_pool(filtered_pool2);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        auto filtered_pool3 = filter_any(ip_pool, 46);
        print_ip_pool(filtered_pool3);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
