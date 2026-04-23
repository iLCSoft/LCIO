#include "EVENT/LCParameters.h"
#include "UTIL/BitField64.h"
#include "UTIL/CheckCollections.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

namespace {

void jsonEscape(std::ostream &os, const std::string &s) {
    os << '"';
    for (char c : s) {
        switch (c) {
        case '"':
            os << "\\\"";
            break;
        case '\\':
            os << "\\\\";
            break;
        case '\b':
            os << "\\b";
            break;
        case '\f':
            os << "\\f";
            break;
        case '\n':
            os << "\\n";
            break;
        case '\r':
            os << "\\r";
            break;
        case '\t':
            os << "\\t";
            break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                os << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                   << static_cast<int>(c) << std::dec << std::setfill(' ');
            } else {
                os << c;
            }
        }
    }
    os << '"';
}

template <typename T>
void emitArray(std::ostream &os, const std::vector<T> &v) {
    os << '[';
    if constexpr (std::is_floating_point_v<T>) {
        os << std::setprecision(std::numeric_limits<T>::max_digits10);
    }
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) {
            os << ", ";
        }
        if constexpr (std::is_same_v<T, std::string>) {
            jsonEscape(os, v[i]);
        } else {
            os << v[i];
        }
    }
    os << ']';
}

template <typename T>
bool emitGroup(std::ostream &os, const std::string &label,
               const EVENT::LCParameters &params, const EVENT::StringVec &keys,
               bool needsComma) {
    if (keys.empty()) {
        return needsComma;
    }
    if (needsComma) {
        os << ",\n";
    }
    os << "    \"" << label << "\": {";
    for (size_t i = 0; i < keys.size(); ++i) {
        os << (i ? ",\n      " : "\n      ");
        jsonEscape(os, keys[i]);
        os << ": ";
        emitArray(os, params.getVals<T>(keys[i]));
    }
    os << "\n    }";
    return true;
}

void usage() {
    std::cout
        << "usage: check_col_params --output <out.json> --params <p1,p2,...> "
           "<input-file1> [input-file2 ...]\n";
}

} // namespace

int main(int argc, char **argv) {
    std::string outputPath;
    std::vector<std::string> paramNames;
    std::vector<std::string> inputs;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if ((a == "--output" || a == "-o") && i + 1 < argc) {
            outputPath = argv[++i];
        } else if ((a == "--params" || a == "-p") && i + 1 < argc) {
            const std::string ps = argv[++i];
            std::for_each(ps.begin(), ps.end(),
                          UTIL::LCTokenizer(paramNames, ','));
        } else if (a == "-h" || a == "--help") {
            usage();
            return 0;
        } else {
            inputs.push_back(a);
        }
    }

    if (outputPath.empty() || paramNames.empty() || inputs.empty()) {
        usage();
        return 1;
    }

    UTIL::CheckCollections cc;
    cc.checkParameters(inputs, paramNames);
    const auto &collected = cc.getCollectedParameters();

    std::ofstream out(outputPath);
    if (!out) {
        std::cerr << "error: cannot open '" << outputPath << "' for writing\n";
        return 1;
    }

    out << "{";
    bool firstColl = true;
    for (const auto &entry : collected) {
#if defined(__clang__) && __clang_major__ < 13
        const auto &name = entry.first;
        const auto &vals = entry.second;
#else
        const auto &[name, vals] = entry;
#endif
        EVENT::StringVec intKeys, floatKeys, doubleKeys, stringKeys;
        vals.getIntKeys(intKeys);
        vals.getFloatKeys(floatKeys);
        vals.getDoubleKeys(doubleKeys);
        vals.getStringKeys(stringKeys);

        if (intKeys.empty() && floatKeys.empty() && doubleKeys.empty() &&
            stringKeys.empty()) {
            continue;
        }

        if (!firstColl) {
            out << ",";
        }
        firstColl = false;
        out << "\n  " << name << ": {";

        bool needsComma = false;

        needsComma =
            emitGroup<int>(out, "int-params", vals, intKeys, needsComma);
        needsComma =
            emitGroup<float>(out, "float-params", vals, floatKeys, needsComma);
        needsComma = emitGroup<double>(out, "double-params", vals, doubleKeys,
                                       needsComma);
        needsComma = emitGroup<std::string>(out, "string-params", vals,
                                            stringKeys, needsComma);

        if (needsComma) {
            out << "\n  ";
        }
        out << "}";
    }
    out << "\n}\n";

    return 0;
}
