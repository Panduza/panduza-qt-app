#pragma once

#include <QString>
#include <QDebug>
#include <any>

namespace PzaUtils {

template<typename N>
static bool IsInStdMap(const std::map<QString, N> &map, const QString &key)
{
    return (map.count(key) != 0) ? true : false;
}

template<typename N>
static bool IsInStdMap(const std::unordered_map<QString, N> &map, const QString &key)
{
    return (map.count(key) != 0) ? true : false;
}

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

template<typename N, typename T>
static int IndexInStdMap(const std::map<N, T> &map, const T &item)
{
    auto it = find(map.begin(), map.end(), item);
    if (it != map.end()) {
        int index = it - map.begin();
        return index;
    }
    else
        return -1;
}

template<typename N, typename T>
static int ValueIndexInStdMap(const std::map<N, T> &map, const T &item)
{
    auto it = find(map.begin(), map.end(), item);
    if (it != map.end()) {
        int index = it - map.begin();
        return index;
    }
    else
        return -1;
}

template<typename N, typename T>
static int ValueIndexInStdMap(const std::unordered_map<N, T> &map, const T &item)
{
    auto it = find(map.begin(), map.end(), item);
    if (it != map.end()) {
        int index = it - map.begin();
        return index;
    }
    else
        return -1;
}

template<typename N, typename T>
static int KeyIndexInStdMap(const std::map<N, T> &map, const N &key)
{
    return std::distance(map.begin(), map.find(key));
}

template<typename N, typename T>
static int KeyIndexInStdMap(const std::unordered_map<N, T> &map, const N &key)
{
    return std::distance(map.begin(), map.find(key));
}

template<typename N, typename T>
static void ReplaceKeyInStdMap(std::map<N, T> &map, const N &key, const N &newKey)
{
    if (map.size() == 0 || map.count(key) == 0)
        return ;
        
    auto node = map.extract(key);
    node.key() = newKey;
    map.insert(std::move(node));
}

template<typename N>
static void DeleteFromVector(std::vector<N> &vec, const N &item) {
    auto it = std::find(vec.begin(), vec.end(), item);
    if (it != vec.end())
        vec.erase(it);
}

template<typename N>
static void ForEachDeleteInVector(std::vector<N> &vec, const std::function<void(const N &elem)> &f) {
    for (auto it = vec.begin(); it != vec.end();) {
        f(*it);
        it = vec.erase(it);
    }
}

template<typename N>
static void ForEachDeletedInVector(std::vector<N> &vec, const std::function<void(const N &elem)> &f) {
    int len = vec.size();
    for (int i = 0; i < len; i++) {
        f(vec[0]);
    }
}


template<typename N>
static QString AllocateName(const std::map<QString, N> &list, const QString &defaultName)
{
    QString name = defaultName;
    int i = 1;

    for (;;) {
        if (PzaUtils::IsInStdMap(list, name) == false) {
            break ;
        }
        name = defaultName + " " + QString::number(i++);
    }
    return name;
};

template<typename N>
static QString AllocateName(const std::vector<N> &list, const QString &defaultName, const std::function<const QString &(const N &entry)> &f)
{
    QString name = defaultName;
    int index = 1;

    for (size_t i = 0; i < list.size();) {
        const QString &tmp = f(list[i]);
        if (tmp == name) {
            name = defaultName + " " + QString::number(index++);
            i = 0;
        }
        else
            i++;
    }
    return name;
};

};