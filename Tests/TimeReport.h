#ifndef HEAP_TIMEREPORT_H
#define HEAP_TIMEREPORT_H


#include <fstream>

inline void reportTime(const char *name, int res) {
    // deal with CLion bug with relative paths
    std::ofstream fout("C:\\Users\\M\\alg\\labs\\heap\\time_results.txt", std::ios::app);
    fout << name << ": " << res << " ms" << std::endl;
    fout.close();
}



#endif //HEAP_TIMEREPORT_H
