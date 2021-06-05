//
// Created by Administrator on 2021/6/4.
//

#ifndef TICKETS_FILEMANAGER_H
#define TICKETS_FILEMANAGER_H

#include <fstream>
#include "HashMap.h"

using namespace std;
template<class T,int LEN>
class LRUCache {
private:
    struct node {
        T value;
        long address;
        node *pre = nullptr, *nxt = nullptr;
        HashMap<long,node*,LEN-3> *the_map;
        bool is_modify = false;

        node(const T &_value, long _address, HashMap<long, node *,LEN-3> *_the_map) : value(_value), address(_address),
                                                                                the_map(_the_map) {
            the_map->insert(make_pair(_address, this));
        }
    } *head = nullptr, *tail = nullptr;

    HashMap<long ,node*,LEN-3> table;
    int size;
    fstream &file;

    void save_node(node *n) {
        file.seekp(n->address, ios::beg);
        file.write(reinterpret_cast<char *>(&(n->value)), sizeof(T));
        table.erase(n->address);
    }

    T read_node(long offset) {
        T tmp;
        file.seekg(offset, ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(T));
        return tmp;
    }

public:
    explicit LRUCache(fstream &_file, int _size = LEN) : size(_size), file(_file) {}

    ~LRUCache() {
        for (; head; head = tail) {
            tail = head->nxt;
            if (head->is_modify) save_node(head);
            delete head;
        }
    }

    T *load(const long &offset) {
        node *n;
        if (!table.count(offset)) {
            n = new node(read_node(offset), offset, &table);
            if (size == table.size()) {
                node *tmp = tail;
                tail = tail->pre;
                if (tail) tail->nxt = nullptr;
                else head = nullptr;
                save_node(tmp);
                delete tmp;
            }
            if (!head) head = tail = n;
            else {
                n->nxt = head;
                head->pre = n;
                head = n;
            }
            return &(n->value);
        }
        n = table[offset];
        if (n != head) {
            n->pre->nxt = n->nxt;
            if (n->nxt) n->nxt->pre = n->pre;
            else tail = n->pre;
            n->pre = nullptr;
            n->nxt = head;
            head->pre = n;
            head = n;
        }
        return &(n->value);
    }

    void modify(long offset) {table[offset]->is_modify=true;}
};

template<class T,int LEN>
class FileManager {
private:
    fstream file;
    LRUCache<T,LEN> *cache= nullptr;
public:
    explicit FileManager(const string &file_name) {
        file.open(file_name, ios::out | ios::in | ios::binary);
        if (!file) file.open(file_name, ios::out | ios::binary);
        file.close();
        file.open(file_name, ios::in | ios::out | ios::binary);
        if (cache) delete cache;
        cache=new LRUCache<T,LEN>(file);
    }

    ~FileManager() { delete cache;file.close(); }

    T *read(const long &offset) {
        return cache->load(offset);
    }

    void save(T& val, long offset = -1) {
        if (offset == -1 || offset==End()) {
            file.seekp(0, ios::end);
            offset=file.tellg();
            file.write(reinterpret_cast<char *>(&val), sizeof(T));
            cache->load(offset);
        }
        else cache->modify(offset);
    }

    long End() {
        file.seekg(0, ios::end);
        return file.tellg();
    }
};

#endif //TICKETS_FILEMANAGER_H