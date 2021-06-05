//
// Created by Administrator on 2021/6/5.
//

#ifndef ORDER_PY_FILEARRAY_HPP
#define ORDER_PY_FILEARRAY_HPP
#include "../FileManager.h"
typedef unsigned int UINT;
template<class T,int LEN>
class filearray{
    FileManager<T,LEN> *file;
public:
    filearray(const std::string &filename){
        file=new FileManager<T,LEN>(filename);
    }
    ~filearray() {
        delete file;
    }
    T operator [](const UINT &pos){
        return *(file->read(pos*sizeof(T)));
    }
    void update(const UINT &pos,const T &val){
        *(file->read(pos*sizeof(T)))=val;
        file->cache->modify(pos*sizeof(T));
    }
    void insert(const UINT &pos,const T &val){
        file->save(val);
        file->cache->modify(pos*sizeof(T));
    }
};
#endif //ORDER_PY_FILEARRAY_HPP
