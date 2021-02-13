#ifndef HW5_FILEIO_H
#define HW5_FILEIO_H


#include <string>
#include "Cell.h"
#include "AbstractHex.h"

namespace FatihKaanHex {
    class FileIO {
    private:

        std::string getDataFromLine(const std::string &line);

        void loadData(std::istream &inStream);

    public:

        static std::string stackToStr(Stack stack);

        int loadIntData(std::istream &inStream);

        void loadData(std::string &data, std::istream &inStream);

        void loadData(player &data, std::istream &inStream);

        void loadData(bool &data, std::istream &inStream);

        Stack loadStackData(int size, std::istream &inStream);

        memoryStatus loadMemStatusData(std::istream &inStream);

        void saveData(const std::string &identifier, const int &data, std::ostream &outStream);

        void saveData(const std::string &identifier, const char &data, std::ostream &outStream);

        void saveData(const std::string &identifier, const std::string &data, std::ostream &outStream);

        void saveData(const std::string &identifier, const bool &data, std::ostream &outStream);
    };

}
#endif //HW5_FILEIO_H
