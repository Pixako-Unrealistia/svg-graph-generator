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

    /**
     * @brief Constructs an empty OrderedMap.
     */
    OrderedMap() {}

    /**
     * @brief Copy constructor for OrderedMap.
     *
     * @param other The OrderedMap to copy from.
     */
    OrderedMap(const OrderedMap<KeyType, ValueType> &other)
        : items_(other.items_), map_(other.map_)
    {
    }

    /**
     * @brief Move constructor for an OrderedMap.
     *
     * @param other The OrderedMap to move from.
     */
    OrderedMap(OrderedMap<KeyType, ValueType> &&other)
        : items_(std::move(other.items_)), map_(std::move(other.map_))
    {
    }

    /**
     * @brief Constructs an OrderedMap from an initializer list of key-value pairs.
     *
     * @param init_list The initializer list of key-value pairs.
     */
    OrderedMap(std::initializer_list<std::pair<KeyType, ValueType>> init_list)
    {
        for (const auto &item : init_list)
        {
            insert(item);
        }
    }

    /**
     * @brief Assigns the contents of an OrderedMap to another.
     *
     * @tparam KeyType The type of the keys of the maps.
     * @tparam ValueType The type of the values of the maps.
     * @param other The other OrderedMap to assign to.
     *
     * @return OrderedMap<KeyType, ValueType>& A reference to this OrderedMap after the assignment.
     */
    OrderedMap<KeyType, ValueType> &operator=(OrderedMap<KeyType, ValueType> other)
    {
        swap(*this, other);
        return *this;
    }

    /**
     * @brief Returns an iterator pointing to the first element in the OrderedMap.
     * @return An iterator pointing to the first element in the OrderedMap.
     */
    iterator begin() { return items_.begin(); }

    /**
     * @brief Returns an iterator pointing to the past-the-end element in the OrderedMap.
     * @return An iterator pointing to the past-the-end element in the OrderedMap.
     */
    iterator end() { return items_.end(); }

    /**
     * @brief Returns a const_iterator pointing to the first element in the OrderedMap.
     * @return A const_iterator pointing to the first element in the OrderedMap.
     */
    const_iterator begin() const { return items_.begin(); }

    /**
     * @brief Returns a const_iterator pointing to the past-the-end element in the OrderedMap.
     * @return A const_iterator pointing to the past-the-end element in the OrderedMap.
     */
    const_iterator end() const { return items_.end(); }

    /**
     * @brief Inserts an element into the OrderedMap.
     * @param item The element to insert in std::pair<KeyType, ValueType> format.
     */
    void insert(const std::pair<KeyType, ValueType> &item)
    {
        if (map_.find(item.first) == map_.end())
        {
            items_.push_back(item);
            map_[item.first] = --items_.end();
        }
    }

    /**
     * @brief Returns a reference to the value associated with the given key. If the key does not exist in the map,
     * it is inserted with a default-constructed value.
     *
     * @param key The key to access the value.
     * @return ValueType& A reference to the value associated with the key.
     */
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

    /**
     * @brief Finds an element in the OrderedMap.
     *
     * @param key The key of the element to find.
     * @return An iterator pointing to the element if it is found, otherwise an iterator pointing to the past-the-end element.
     */
    iterator find(const KeyType &key)
    {
        auto it = map_.find(key);
        if (it == map_.end())
            return end();
        return it->second;
    }

    /**
     * @brief Finds an element in the OrderedMap.
     *
     * @param key The key of the element to find.
     * @return A const_iterator pointing to the element if it is found, otherwise a const_iterator pointing to the past-the-end element.
     */
    const_iterator find(const KeyType &key) const
    {
        auto it = map_.find(key);
        if (it == map_.end())
            return end();
        return it->second;
    }

    /**
     * @brief Erases the element at the specified position from the OrderedMap.
     *
     * @tparam KeyType The type of the keys.
     * @tparam ValueType The type of the values.
     *
     * @param it An iterator pointing to the element to be erased.
     */
    void erase(iterator it)
    {
        map_.erase(it->first);
        items_.erase(it);
    }

    /**
     * @brief Erases the element with the specified key from the OrderedMap.
     *
     * @tparam KeyType The type of the keys.
     *
     * @param key The key of the element to be erased.
     */
    void erase(const KeyType &key)
    {
        auto it = map_.find(key);
        if (it != map_.end())
        {
            items_.erase(it->second);
            map_.erase(it);
        }
    }

    /**
     * @brief Checks if the OrderedMap is empty.
     *
     * @return Returns true if the OrderedMap is empty, false otherwise.
     */
    bool empty() const { return items_.empty(); }

    /**
     * @brief Returns the number of elements in the OrderedMap.
     *
     * @return The number of elements in the OrderedMap.
     */
    size_t size() const { return items_.size(); }

    /**
     * @brief Removes all elements from the OrderedMap.
     */
    void clear()
    {
        items_.clear();
        map_.clear();
    }

    /**
     * @brief Swaps the contents of two OrderedMap instances.
     *
     * @tparam KeyType The type of the keys in the maps.
     * @tparam ValueType The type of the values in the maps.
     * @param first The first OrderedMap.
     * @param second The second OrderedMap.
     */
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
