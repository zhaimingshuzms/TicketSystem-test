#ifndef TICKETS_BPLUSTREE_H
#define TICKETS_BPLUSTREE_H

#include <iostream>
#include <cstring>
#include <vector>
#include "FileManager.h"

using namespace std;

template<class Key, class Data, const int M = 200>
class BPlusTree {
private:
    struct node {
        long son_address[M + 5] = {0};
        Data data_value[M + 5];
        Key index[M + 5];
        long nxt = -1;
        int size = 0;

        node() = default;

        node(const node &o) : nxt(o.nxt), size(o.size) {
            for (int i = 0; i <= o.size; i++) {
                son_address[i] = o.son_address[i];
                index[i] = o.index[i];
                data_value[i] = o.data_value[i];
            }
        }

        node &operator=(const node &o) {
            if (this == &o) return *this;
            nxt = o.nxt;
            size = o.size;
            for (int i = 0; i <= o.size; i++) {
                son_address[i] = o.son_address[i];
                index[i] = o.index[i];
                data_value[i] = o.data_value[i];
            }
            return *this;
        }
    };

    long root = -1;
    int depth = 0, leaf_size = 0;
    FileManager<node> *file;

    //to insert a Key into a no_leaf node
    void assist_insert(const Key &key, int dep, node *the_node, int *the_sub, long *node_pos, long former_pos) {
        if (dep == -1) {//form a new root
            depth++;
            node n;
            n.size = 1;
            n.index[0] = key;
            n.son_address[0] = root;
            n.son_address[1] = former_pos;
            root = file->End();
            file->save(n);
            return;
        }
        node *n = &the_node[dep];
        int j = the_sub[dep];
        if (n->size < M - 1) {
            for (int i = n->size; i > j; i--) n->index[i] = n->index[i - 1], n->son_address[i + 1] = n->son_address[i];
            n->index[j] = key;
            n->son_address[j + 1] = former_pos;
            n->size++;
            file->save(*n, node_pos[dep]);
            return;
        }
        Key tmp_key[M + 2];
        long tmp_address[M + 2], new_pos = file->End();
        tmp_address[0] = n->son_address[0];
        for (int i = 0; i < j; i++) tmp_key[i] = n->index[i], tmp_address[i + 1] = n->son_address[i + 1];
        for (int i = j + 1; i < M; i++) tmp_key[i] = n->index[i - 1], tmp_address[i + 1] = n->son_address[i];
        tmp_key[j] = key, tmp_address[j + 1] = former_pos;
        //split again
        node n1;
        n->size = M / 2, n1.size = M - 1 - M / 2;
        for (int i = 0; i < n->size; i++) n->index[i] = tmp_key[i], n->son_address[i] = tmp_address[i];
        n->son_address[n->size] = tmp_address[n->size];
        for (int i = n->size + 1; i < M; i++)
            n1.index[i - 1 - n->size] = tmp_key[i], n1.son_address[i - 1 - n->size] = tmp_address[i];
        n1.son_address[n1.size] = tmp_address[M];
        file->save(n1);
        file->save(*n, node_pos[dep]);
        assist_insert(tmp_key[n->size], dep - 1, the_node, the_sub, node_pos, new_pos);
    }

    void assist_erase(int dep, node *the_node, int *the_sub, long *node_pos) {
        node *n = &the_node[dep];
        int j = the_sub[dep];
        n->size--;
        for (int i = j; i < n->size; i++) n->index[i] = n->index[i + 1], n->son_address[i + 1] = n->son_address[i + 2];
        if (!dep) {
            if (n->size) file->save(*n, node_pos[0]);
            else depth--, root = node_pos[1];
            return;
        }
        if (n->size >= (M - 1) / 2) {
            file->save(*n, node_pos[dep]);
            return;
        }
        if (the_sub[dep - 1] == the_node[dep - 1].size) {//get the former node
            the_sub[dep + 1] = the_sub[dep - 1];
            the_sub[dep - 1] = the_node[dep - 1].size - 1;
            node_pos[dep + 1] = node_pos[dep];
            node_pos[dep] = the_node[dep - 1].son_address[the_sub[dep - 1]];
            the_node[dep + 1] = the_node[dep];
            the_node[dep] = file->read(node_pos[dep]);
            n = &the_node[dep];
        } else {
            the_sub[dep + 1] = the_sub[dep - 1] + 1;
            node_pos[dep + 1] = the_node[dep - 1].son_address[the_sub[dep + 1]];
            the_node[dep + 1] = file->read(node_pos[dep + 1]);
        }
        node *n1 = &the_node[dep + 1];
        if (n->size + n1->size >= M - 1) {//do a fake split
            Key tmp_key[n->size + n1->size+3];
            long tmp_address[n->size + n1->size+3];
            for (int i = 0; i < n->size; i++) tmp_address[i] = n->son_address[i], tmp_key[i] = n->index[i];
            tmp_address[n->size] = n->son_address[n->size];
            tmp_key[n->size] = the_node[dep - 1].index[the_sub[dep - 1]];
            for (int i = n->size + 1; i < n->size + n1->size + 1; i++)
                tmp_address[i] = n1->son_address[i - 1 - n->size], tmp_key[i] = n1->index[i - 1 - n->size];
            tmp_address[n->size + n1->size + 1] = n1->son_address[n1->size];
            int tmp_size = n->size + n1->size + 1;
            n->size = tmp_size / 2, n1->size = tmp_size - 1 - n->size;
            for (int i = 0; i < n->size; i++) n->son_address[i] = tmp_address[i], n->index[i] = tmp_key[i];
            n->son_address[n->size] = tmp_address[n->size];
            for (int i = 0; i < n1->size; i++)
                n1->son_address[i] = tmp_address[i + 1 + n->size], n1->index[i] = tmp_key[i + 1 + n->size];
            n1->son_address[n1->size] = tmp_address[n->size + n1->size + 1];
            the_node[dep - 1].index[the_sub[dep - 1]] = tmp_key[n->size];
            file->save(*n, node_pos[dep]);
            file->save(*n1, node_pos[dep + 1]);
            file->save(the_node[dep - 1], node_pos[dep - 1]);
            return;

        }
        //merge again
        n->index[n->size] = the_node[dep - 1].index[the_sub[dep - 1]];
        for (int i = 0; i < n1->size; i++)
            n->son_address[i + 1 + n->size] = n1->son_address[i], n->index[i + 1 + n->size] = n1->index[i];
        n->size += n1->size + 1;
        n->son_address[n->size] = n1->son_address[n1->size];
        file->save(*n, node_pos[dep]);
        assist_erase(dep - 1, the_node, the_sub, node_pos);
    }

public:
    explicit BPlusTree(const string &file_name) {
        file = new FileManager<node>(file_name);
    }

    ~BPlusTree() { delete file; }

    bool insert(const Key &key, const Data &data) {
        if (!leaf_size) {//when the tree is empty
            root = file->End();
            node root_node;
            root_node.data_value[0] = data;
            root_node.index[0] = key;
            root_node.size = 1;
            file->save(root_node, root);
            leaf_size = 1;
            depth = 1;
            return true;
        }
        node passed_node[depth + 2];
        long passed_pos[depth + 2] = {0};
        int passed_sub[depth + 2];//store information of the path
        passed_pos[0] = root;
        int j;
        for (int i = 0; i < depth; i++) {
            passed_node[i] = file->read(passed_pos[i]);
            for (j = 0; j < passed_node[i].size; j++) if (passed_node[i].index[j] >= key) break;
            if (i == depth - 1) break;
            if (j == passed_node[i].size || passed_node[i].index[j] > key)
                passed_pos[i + 1] = passed_node[i].son_address[j];
            else passed_pos[i + 1] = passed_node[i].son_address[++j];
            passed_sub[i] = j;
        }
        if (j < passed_node[depth - 1].size && passed_node[depth - 1].index[j] == key) { return false; }
        node *n = &passed_node[depth - 1];
        if (n->size < M - 1) {
            for (int i = n->size; i > j; i--) n->index[i] = n->index[i - 1], n->data_value[i] = n->data_value[i - 1];
            n->index[j] = key;
            n->data_value[j] = data;
            n->size++;
            file->save(*n, passed_pos[depth - 1]);
            leaf_size++;
            return true;
        }
        //after inserting the node will be overload
        Key tmp_key[M + 2];
        Data tmp_data[M + 2];
        for (int i = 0; i < j; i++) tmp_key[i] = n->index[i], tmp_data[i] = n->data_value[i];
        for (int i = j + 1; i < M; i++) tmp_key[i] = n->index[i - 1], tmp_data[i] = n->data_value[i - 1];
        tmp_key[j] = key, tmp_data[j] = data;
        //split
        node n1;
        n1.nxt = n->nxt, n->nxt = file->End();
        n->size = M / 2, n1.size = M - M / 2;
        //****(this line can be deleted)
        for (int i = 0; i < n->size; i++) n->index[i] = tmp_key[i], n->data_value[i] = tmp_data[i];
        for (int i = n->size; i < M; i++) n1.index[i - n->size] = tmp_key[i], n1.data_value[i - n->size] = tmp_data[i];
        file->save(n1);
        file->save(*n, passed_pos[depth - 1]);
        assist_insert(n1.index[0], depth - 2, passed_node, passed_sub, passed_pos, n->nxt);
        leaf_size++;
        return true;
    }

    bool erase(const Key &key) {
        if (!leaf_size) return false;
        node passed_node[depth + 2];
        long passed_pos[depth + 2] = {0};
        int passed_sub[depth + 2];//store information of the path
        passed_pos[0] = root;
        int j;
        for (int i = 0; i < depth; i++) {
            passed_node[i] = file->read(passed_pos[i]);
            for (j = 0; j < passed_node[i].size; j++) if (passed_node[i].index[j] >= key) break;
            if (i == depth - 1) break;
            if (j == passed_node[i].size || passed_node[i].index[j] > key)
                passed_pos[i + 1] = passed_node[i].son_address[j];
            else passed_pos[i + 1] = passed_node[i].son_address[++j];
            passed_sub[i] = j;
        }
        if (j == passed_node[depth - 1].size || passed_node[depth - 1].index[j] > key) { return false; }
        node *n = &passed_node[depth - 1];
        if (!j) //the erasing key may exist in no_leaf node
            for (int i = depth - 2; i >= 0; i--)
                if (passed_sub[i] && passed_node[i].index[passed_sub[i] - 1] == n->index[0]) {
                    passed_node[i].index[passed_sub[i] - 1] = n->index[1];
                    file->save(passed_node[i], passed_pos[i]);
                    break;
                }
        n->size--, leaf_size--;
        for (int i = j; i < n->size; i++) n->index[i] = n->index[i + 1], n->data_value[i] = n->data_value[i + 1];
        if (depth == 1) {//only one node
            if (n->size) file->save(*n, root);
            else depth = 0, root = -1;
            return true;
        }
        if (n->size >= M / 2) {
            file->save(*n, passed_pos[depth - 1]);
            return true;
        }
        //use passed_node[depth] to present the bro node
        if (passed_sub[depth - 2] == passed_node[depth - 2].size) {//get the former node
            passed_sub[depth] = passed_sub[depth - 2];
            passed_sub[depth - 2] = passed_node[depth - 2].size - 1;
            passed_pos[depth] = passed_pos[depth - 1];
            passed_pos[depth - 1] = passed_node[depth - 2].son_address[passed_sub[depth - 2]];
            passed_node[depth] = passed_node[depth - 1];
            passed_node[depth - 1] = file->read(passed_pos[depth - 1]);
            n = &passed_node[depth - 1];
        } else {
            passed_sub[depth] = passed_sub[depth - 2] + 1;
            passed_pos[depth] = passed_node[depth - 2].son_address[passed_sub[depth]];
            passed_node[depth] = file->read(passed_pos[depth]);
        }
        node *n1 = &passed_node[depth];
        if (n->size + n1->size >= M) {//do a fake split
            Key tmp_key[n->size + n1->size+2];
            Data tmp_data[n->size + n1->size+2];
            for (int i = 0; i < n->size; i++) tmp_data[i] = n->data_value[i], tmp_key[i] = n->index[i];
            for (int i = n->size; i < n->size + n1->size; i++)
                tmp_data[i] = n1->data_value[i - n->size], tmp_key[i] = n1->index[i - n->size];
            const Key old = tmp_key[n->size];
            int tmp_size = n->size + n1->size;
            n->size = (tmp_size + 1) / 2, n1->size = tmp_size - n->size;
            for (int i = 0; i < n->size; i++) n->data_value[i] = tmp_data[i], n->index[i] = tmp_key[i];
            for (int i = 0; i < n1->size; i++)
                n1->data_value[i] = tmp_data[i + n->size], n1->index[i] = tmp_key[i + n->size];
            file->save(*n, passed_pos[depth - 1]);
            file->save(*n1, passed_pos[depth]);
            //renew the no_leaf node
            for (int i = depth - 2; i >= 0; i--)
                if (passed_node[i].index[passed_sub[i]] == old) {
                    passed_node[i].index[passed_sub[i]] = n1->index[0];
                    file->save(passed_node[i], passed_pos[i]);
                    break;
                }
            return true;
        }
        //merge
        for (int i = 0; i < n1->size; i++)
            n->data_value[i + n->size] = n1->data_value[i], n->index[i + n->size] = n1->index[i];
        n->size += n1->size;
        n->nxt = n1->nxt;
        file->save(*n, passed_pos[depth - 1]);
        assist_erase(depth - 2, passed_node, passed_sub, passed_pos);
        return true;
    }

    pair<Data, bool> _find(const Key &key) {
        Data data;
        if (!leaf_size) return make_pair(data, false);
        long pos = root;
        node *n;
        n = new node;
        for (int i = 0; i < depth; i++){
            *n = file->read(pos);
            int j;
            for (j = 0; j < n->size; j++) if (n->index[j] >= key) break;
            if (i == depth - 1) {
                if (j == n->size || n->index[j] > key) {
                    delete n;
                    return make_pair(data, false);
                }
                delete n;
                return make_pair(n->data_value[j], true);
            }
            if (j == n->size || n->index[j] > key) pos = n->son_address[j];
            else pos = n->son_address[j + 1];
        }
    }

    vector<pair<Key, Data>> range_find(const Key &key1, const Key &key2) {
        vector<pair<Key, Data>> vec;
        if (!leaf_size) return vec;
        long pos = root;
        node *n;
        int j;
        for (int i = 0; i < depth; i++) {
            n = new node(file->read(pos));
            for (j = 0; j < n->size; j++) if (n->index[j] >= key1) break;
            if (i == depth - 1) break;
            if (j == n->size || n->index[j] > key1) pos = n->son_address[j];
            else pos = n->son_address[j + 1];
        }
        if (j == n->size || n->index[j] > key1) {
            delete n;
            return vec;
        }
        while (n->index[j] != key2) {
            vec.push_back(make_pair(n->index[j], n->data_value[j]));
            if (j == n->size) j = 0, *n = file->read(n->nxt);
            else j++;
        }
        vec.push_back(make_pair(n->index[j], n->data_value[j]));
        delete n;
        return vec;
    }

    int size() const { return leaf_size; }

    bool empty() { return !leaf_size; }

    void update(const Key &key, const Data &newData) {
        Data data;
        if (!leaf_size) return;
        long pos = root;
        node *n;
        n = new node;
        for (int i = 0; i < depth; i++) {
            *n = file->read(pos);
            int j;
            for (j = 0; j < n->size; j++) if (n->index[j] >= key) break;
            if (i == depth - 1 && j != n->size && n->index[j] <= key) {
                n->data_value[j] = newData;
                file->save(*n, pos);
            }
            if (j == n->size || n->index[j] > key) pos = n->son_address[j];
            else pos = n->son_address[j + 1];
        }
        delete n;
    }

    //overload function
    bool insert(const pair<Key, Data> &_pair) { return insert(_pair.first, _pair.second); }

    bool erase(const pair<Key, Data> &_pair) { return erase(_pair.first); }

    bool erase(const Key &key, const Data &data) { return erase(key); }

    void update(const pair<Key, Data> &_pair, const Data &newData) { update(_pair.first, newData); }

    Data operator[](const Key &key) { return _find(key).first; }

    pair<Data, bool> find(const Key &key) {
        return _find(key);
    }
    bool count(const Key &key) {
        return _find(key).second;
    }
};

#endif //TICKETS_BPLUSTREE_H