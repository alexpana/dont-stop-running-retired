#pragma once

#include <cstdio>
#include <string>


#pragma push_macro("Bool")
#undef Bool

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#pragma pop_macro("Bool")

namespace dsr {

    class JSONDocument {
    public:
        JSONDocument(const std::string filename) {

            fileHandle = fopen(filename.c_str(), "rb");

            readBuffer = new char[READ_BUFFER_SIZE];

            rapidjson::FileReadStream is(fileHandle, readBuffer, READ_BUFFER_SIZE);

            document.ParseStream(is);
        }

        ~JSONDocument() {
            delete[] readBuffer;

            fclose(fileHandle);
        }

        rapidjson::Document &getDocument() {
            return document;
        }

    private:
        rapidjson::Document document;
        char *readBuffer;
        FILE *fileHandle;

        static constexpr unsigned int READ_BUFFER_SIZE = 65536;
    };
}