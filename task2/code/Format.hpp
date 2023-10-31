#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstddef>

class FormatString {
public:
    template<typename... Args>
    static std::string format(const std::string& formatStr, Args... args) {
        std::vector<std::string> arguments = { argToString(args)... };
        return insertArgsAndColors(formatStr, arguments);
    }

private:
    template<typename T>
    static std::string argToString(T value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    static std::string insertArgsAndColors(const std::string& formatStr, const std::vector<std::string>& args) {
        std::string result = formatStr;
        size_t argIndex = 0;

        static const std::unordered_map<std::string, std::string> colors = {
                {"{red}", "\033[31m"},
                {"{green}", "\033[32m"},
                {"{yellow}", "\033[33m"},
                {"{blue}", "\033[34m"},
                {"{reset}", "\033[39m"}
        };

        for (size_t i = 0; i < result.size(); ++i) {
            bool replaced = false;

            // Check for colors
            for (const auto& [key, value] : colors) {
                if (result.compare(i, key.size(), key) == 0) {
                    result.replace(i, key.size(), value);
                    i += value.size() - 1;
                    replaced = true;
                    break;
                }
            }

            // Check for arguments
            if (!replaced && i + 1 < result.size() && result[i] == '{' && result[i + 1] == '}' && argIndex < args.size()) {
                result.replace(i, 2, args[argIndex]);
                i += args[argIndex].size() - 1;
                ++argIndex;
            }
        }

        return result;
    }
};
