//
// Created by Administrator on 2021/6/4.
//

#ifndef CODE_HASHMAP_H
#define CODE_HASHMAP_H

#include <functional>

template<class Key, class Data, class Hash=std::hash<Key>>
class HashMap {
private:
    class list {
    private:
        struct node {
            node *nxt;
            Key key;
            Data data;

            node(node *_nxt, const Key &_key, const Data &_data) : nxt(_nxt), key(_key), data(_data) {}
        } *head = nullptr;

        int size = 0;

        void delete_head() {
            node *tmp = head;
            head = head->nxt;
            delete tmp;
        }

    public:
        list() = default;

        ~list() { while (head) delete_head(); }

        void insert(const Key &_key, const Data &_data) { head = new node(head, _key, _data), size++; }

        void erase(const Key &_key) {
            if (!size) return;
            size--;
            if (head->key == _key) {
                delete_head();
                return;
            }
            node *pre = nullptr, *cur = head;
            while (cur) {
                if (cur->key == _key) break;
                pre = cur, cur = cur->nxt;
            }
            pre->nxt = cur->nxt;
            delete cur;
        }

        node *find(const Key &_key) {
            if (!size) return nullptr;
            node *cur = head;
            while (cur) {
                if (cur->key == _key) return cur;
                cur = cur->nxt;
            }
            return nullptr;
        }
    } *hash_value;

    int siz = 0;
    const int dividend;
    Hash h;

    int get_hash_value(const Key &_key) { return h(_key) % dividend; }

public:
    explicit HashMap(int _dividend = 997) : dividend(_dividend) { hash_value = new list[dividend]; }

    ~HashMap() { delete[] hash_value; }

    void insert(const Key &_key, const Data &_data) {
        hash_value[get_hash_value(_key)].insert(_key, _data);
        siz++;
    }

    void erase(const Key &_key) {
        hash_value[get_hash_value(_key)].erase(_key);
        siz--;
    }

    Data &operator[](const Key &_key) { return hash_value[get_hash_value(_key)].find(_key)->data; }

    int count(const Key &_key) {
        if (hash_value[get_hash_value(_key)].find(_key)) return 1;
        return 0;
    }
    auto find(const Key &_key){
        return hash_value[get_hash_value(_key)].find(_key);
    }

    int size() { return siz; }

    void insert(std::pair<Key,Data> p) {insert(p.first,p.second);}
};

#endif //CODE_HASHMAP_H
