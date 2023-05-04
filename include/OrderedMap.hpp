#pragma once

#include <map>
#include <list>
#include <cstddef>

template <typename KeyType, typename ValueType>
class OrderedMap
{
public:
    typedef typename std::list<std::pair<KeyType, ValueType>>::iterator iterator;
    typedef typename std::list<std::pair<KeyType, ValueType>>::const_iterator const_iterator;

    OrderedMap() {}

    OrderedMap(const OrderedMap<KeyType, ValueType> &other)
        : items_(other.items_), map_(other.map_)
    {
    }

    OrderedMap(OrderedMap<KeyType, ValueType> &&other)
        : items_(std::move(other.items_)), map_(std::move(other.map_))
    {
    }

    OrderedMap(std::initializer_list<std::pair<KeyType, ValueType>> init_list)
    {
        for (const auto &item : init_list)
        {
            insert(item);
        }
    }

    OrderedMap<KeyType, ValueType> &operator=(OrderedMap<KeyType, ValueType> other)
    {
        swap(*this, other);
        return *this;
    }

    iterator begin() { return items_.begin(); }
    iterator end() { return items_.end(); }
    const_iterator begin() const { return items_.begin(); }
    const_iterator end() const { return items_.end(); }

    void insert(const std::pair<KeyType, ValueType> &item)
    {
        if (map_.find(item.first) == map_.end())
        {
            items_.push_back(item);
            map_[item.first] = --items_.end();
        }
    }

    ValueType &operator[](const KeyType &key)
    {
        auto it = map_.find(key);
        if (it == map_.end())
        {
            insert(std::make_pair(key, ValueType()));
            it = map_.find(key);
        }
        return it->second->second;
    }

    iterator find(const KeyType &key)
    {
        auto it = map_.find(key);
        if (it == map_.end())
            return end();
        return it->second;
    }

    const_iterator find(const KeyType &key) const
    {
        auto it = map_.find(key);
        if (it == map_.end())
            return end();
        return it->second;
    }

    void erase(iterator it)
    {
        map_.erase(it->first);
        items_.erase(it);
    }

    void erase(const KeyType &key)
    {
        auto it = map_.find(key);
        if (it != map_.end())
        {
            items_.erase(it->second);
            map_.erase(it);
        }
    }

    bool empty() const { return items_.empty(); }
    size_t size() const { return items_.size(); }
    void clear()
    {
        items_.clear();
        map_.clear();
    }

    friend void swap(OrderedMap<KeyType, ValueType> &first,
                     OrderedMap<KeyType, ValueType> &second)
    {
        using std::swap;
        swap(first.items_, second.items_);
        swap(first.map_, second.map_);
    }

private:
    std::list<std::pair<KeyType, ValueType>> items_;
    std::map<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> map_;
};
