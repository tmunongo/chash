#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

// similar to generics
// we want to be generic over both key and value types
// make K and V parameters of HashMap
template <
    typename  K,
    typename  V,
    typename Hash = std::hash<K>
>
class HashMap {
private:
    // Node structure for our linked list
    struct Node {
        K key;
        V value;
        // we use a unique pointer which ensures that 
        // dynamically allocated memory is released
        // from the free store (heap)
        // a more explicit mechanism for ownership transfer in C++
        std::unique_ptr<Node> next;

    Node(const K& k, const V& v)
        : key(k), value(v), next(nullptr) {}
    };

    std::vector<std::unique_ptr<Node>> buckets;
    size_t size_;
    float max_load_factor_;
    Hash hasher;

public:
    // sensible defaults for the initial capacity
    // and max_load_factor
    explicit HashMap(size_t initial_capacity = 16, float max_load_factor = 0.75)
        : buckets(initial_capacity)
        , size_(0)
        , max_load_factor_(max_load_factor)
        , hasher() {}

    /**
        Inserts a key and value into the hashmap.
        Will grow the hashmap if the load factor
        is about max_load_factor

        @param: key: the key
        @param: value: the value
    */
    void insert(const K& key, const V& value);
    /**
        Removes a value with a given key from the hashmap

        @param key: the key to be removed
        @return Confirmation if removed as a boolean
     */
    bool remove(const K& key);
    /**
        resizes the hashmap to the new given capacity

        @param: new_capacity
    */
    void resize(size_t new_capacity);
    /**
        Gets a value given a key

        @param key: the key to be retrieved
        @return a pointer to a value (or nullptr if not found)
    */
    V* get(const K& key);
    const V* get(const K& key) const;
    /** 
        returns the size of the hashmap

        @return size_t: the size of the hashmap
    */
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
};

// Templates must have both declarations and implementations in the header file because templates are resolved during compilation.
// Do not create a separate .cpp file for templated class definitions.
template<typename K, typename V, typename Hash>
void HashMap<K, V, Hash>::insert(const K& key, const V& value) {
    // resize?
    // we want to keep our load factor below the max
    if (float(size_ + 1) / buckets.size() > max_load_factor_) {
        resize(buckets.size() * 2);
    }

    size_t index = hasher(key) % buckets.size();

    // does the key already exist?
    auto current = buckets[index].get();
    while (current) {
        // if same key, update value
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next.get();
    }

    // insert new node at the beginning of the list
    auto new_node = std::make_unique<Node>(key, value);
    new_node->next = std::move(buckets[index]);
    buckets[index] = std::move(new_node);
    size_++;
}

template<typename K, typename V, typename Hash>
V* HashMap<K, V, Hash>::get(const K& key) {
    size_t index = hasher(key) % buckets.size();

    auto current = buckets[index].get();
    while (current) {
        if (current->key == key) {
            return &current->value;
        }

        current = current->next.get();
    }

    return nullptr;
}

template<typename K, typename V, typename Hash>
bool HashMap<K, V, Hash>::remove(const K& key) {
    size_t index = hasher(key) % buckets.size();

    // value not found
    if (!buckets[index]) {
        return false;
    }

    if (buckets[index]->key == key) {
        buckets[index] = std::move(buckets[index]->next);
        size_--;
        return true;
    }

    auto current = buckets[index].get();
    while (current->next){
        if (current->next->key == key) {
            current->next = std::move(current->next->next);
            size_--;
            return true;
        }
        current = current->next.get();
    }

    // couldn't remove, for some reason
    return false;
}

template<typename K, typename V, typename Hash>
void HashMap<K, V, Hash>::resize(size_t new_capacity) {
    std::vector<std::unique_ptr<Node>> new_buckets(new_capacity);

    for (auto& bucket : buckets) {
        while (bucket) {
            size_t new_index = hasher(bucket->key) % new_capacity;

            auto next = std::move(bucket->next);
            bucket->next = std::move(bucket->next);
            new_buckets[new_index] = std::move(bucket);
            bucket = std::move(next);
        }
    }

    buckets = std::move(new_buckets);
}