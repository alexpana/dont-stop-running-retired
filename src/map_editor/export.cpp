#include "export.h"

#include <fstream>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;
using namespace rapidjson;

namespace dsr {

    template<typename T, typename V>
    static void appendMember(Writer<T> &w, std::string memberName, V value);

    template<typename T, typename E>
    static void append(Writer<T> &w, E &e);

    template<typename T>
    static void append(Writer<T> &w, bool elem) {
        w.Bool(elem);
    }

    template<typename T>
    static void append(Writer<T> &w, F64 elem) {
        w.Double(elem);
    }

    template<typename T>
    static void append(Writer<T> &w, F32 elem) {
        w.Double(elem);
    }

    template<typename T>
    static void append(Writer<T> &w, U32 elem) {
        w.Int(elem);
    }

    template<typename T>
    static void append(Writer<T> &w, std::string elem) {
        w.String(elem.c_str());
    }

    template<typename T>
    static void append(Writer<T> &w, glm::vec2 vec2) {
        w.StartArray();
        w.Double(vec2.x);
        w.Double(vec2.y);
        w.EndArray();
    }

    template<typename T>
    static void append(Writer<T> &w, GameObject &gameObject) {
        w.StartObject();
        appendMember(w, "name", gameObject.name);
        w.EndObject();
    }

    template<typename T>
    static void append(Writer<T> &w, LevelMap::Entity &entity) {
        w.StartObject();
        appendMember(w, "type", entity.gameObject.name);
        appendMember(w, "position", entity.position);
        appendMember(w, "scale", entity.scale);
        appendMember(w, "rotation", entity.rotation);
        w.EndObject();
    }

    template<typename T, typename E>
    static void append(Writer<T> &w, std::vector<E> vector) {
        w.StartArray();
        for (auto &it : vector) {
            append(w, it);
        }
        w.EndArray();
    }

    template<typename T, typename V>
    static void appendMember(Writer<T> &w, std::string memberName, V value) {
        w.String(memberName.c_str());
        append(w, value);
    }

    void exportLevelMap(const std::string &filename, const LevelMap &levelMap) {

        StringBuffer s;
        Writer<StringBuffer> writer(s);

        writer.StartObject();
        appendMember(writer, "player_start", levelMap.playerStartPosition);
        appendMember(writer, "entities", levelMap.entities);

        // TODO: export shapes
        // TODO: export background
        // TODO: export coins
        // TODO: export particle systems

        writer.EndObject();

        ofstream fout(filename);
        fout << s.GetString() << endl;
        fout.close();
    }
}