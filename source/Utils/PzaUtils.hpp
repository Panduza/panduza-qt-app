#pragma once

#include <QString>
#include <QDebug>

class PzaUtils
{
    public:
        template<typename N>
        static bool IsInStdMap(const std::unordered_map<QString, N> &map, const QString &key)
        {
            return (map.count(key) != 0) ? true : false;
        };

        template<typename N>
        static bool IsInVector(const std::vector<N> &vec, const N &item) {
            return std::find(vec.begin(), vec.end(), item) != vec.end();
        }

        template<typename N>
        static int IndexInVector(const std::vector<N> &vec, const N &item)
        {
            auto it = find(vec.begin(), vec.end(), item);

            if (it != vec.end()) {
                int index = it - vec.begin();
                return index;
            }
            else
                return -1;
        }

        template<typename N>
        static void DeleteFromVector(std::vector<N> &vec, const N &item) {
            auto it = std::find(vec.begin(), vec.end(), item);
            if (it != vec.end()) {
                int i = it - vec.begin();
                vec.erase(vec.begin() + i);
            }
        }

    private:
        PzaUtils() = delete;
        ~PzaUtils() = delete;
};