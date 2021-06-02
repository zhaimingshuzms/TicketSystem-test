#ifndef TICKETS_FILEMANAGER_H
#define TICKETS_FILEMANAGER_H

#include <fstream>
using namespace std;

template<class T>
class FileManager {
private:
    fstream file;
public:
    explicit FileManager(const string& file_name) {
        //file with file_name may not exist
        file.open(file_name,ios::out|ios::in|ios::binary);
        if(!file) file.open(file_name,ios::out|ios::binary);
        file.close();
        file.open(file_name,ios::in|ios::out|ios::binary);
    }

    ~FileManager() {file.close();}

    T read(const long &offset) {
        T tmp;
        file.seekg(offset,ios::beg);
        file.read(reinterpret_cast<char *>(&tmp),sizeof(T));
        return tmp;
    }

    void save(T val,const long &offset=-1) {
        if (offset==-1) file.seekp(0,ios::end);
        else file.seekp(offset,ios::beg);
        file.write(reinterpret_cast<char*>(&val),sizeof(T));
    }

    long End() {
        file.seekg(0,ios::end);
        return file.tellg();
    }
};

#endif //TICKETS_FILEMANAGER_H
