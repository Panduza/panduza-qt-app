#ifndef PZAUTILS_HPP
#define PZAUTILS_HPP

#include <QString>

class PzaUtils
{
    public:
        template<typename N>
        static bool isInHash(const std::unordered_map<QString, N> &map, const QString &key)
        {
            return (map.count(key) != 0) ? true : false;
        };

    private:
        PzaUtils() = delete;
        ~PzaUtils() = delete;
};

#endif