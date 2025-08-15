#include "ShaderPreprocessor.h"

#include "PekanLogger.h"
#include "Utils/FileUtils.h"

namespace Pekan
{
namespace Graphics
{

    // Replaces a filepath's extension with given extension
    static std::string replaceFilepathExtension(const std::string& filepath, const std::string& newExtension)
    {
        const size_t dotPos = filepath.find_last_of('.');
        if (dotPos == std::string::npos)
        {
            PK_LOG_WARNING("Trying to replace a filepath's extension in ShaderPreprocessor, but given filepath doesn't have an extension.", "Pekan");
            return filepath + newExtension;
        }

        return filepath.substr(0, dotPos) + newExtension;
    }

    void ShaderPreprocessor::preprocess(const std::string& pkshadFilepath, const std::unordered_map<std::string, std::string>& substitutions)
    {
        if (pkshadFilepath.substr(pkshadFilepath.size() - 7) != ".pkshad")
        {
            PK_LOG_WARNING("Trying to preprocess a .pkshad file but given filepath doesn't have a .pkshad extension.", "Pekan");
        }

        // Read .pkshad file
        const std::string pkshadContent = FileUtils::readTextFileToString(pkshadFilepath.c_str());

        // If we have no substitutions,
        // just write .pkshad file's contents into a corresponding .glsl file
        if (substitutions.empty())
        {
            const std::string glslFilepath = replaceFilepathExtension(pkshadFilepath, ".glsl");
            FileUtils::writeStringToTextFile(glslFilepath.c_str(), pkshadContent.c_str());
            return;
        }

        std::string glslContent = pkshadContent;

        // Traverse .glsl content,
        // searching for placeholders and replacing them with their values.
        size_t searchPos = 0;
        while (searchPos < glslContent.size())
        {
            // Find start of next placeholder
            const size_t start = glslContent.find("{{", searchPos);
            if (start == std::string::npos)
            {
                break;
            }
            // Find end of placeholder
            const size_t end = glslContent.find("}}", start);
            if (end == std::string::npos)
            {
                PK_LOG_ERROR("Failed to preprocess a .pkshad file because of a wrongly formatted placeholder: " << pkshadFilepath, "Pekan");
                break;
            }
            // Extract placeholder from between "{{" and "}}"
            const std::string placeholder = glslContent.substr(start + 2, end - (start + 2));
            // Find placeholder in given substitutions map
            auto it = substitutions.find(placeholder);
            if (it != substitutions.end())
            {
                // Replace placeholder with its value
                glslContent.replace(start, end - start + 2, it->second);
                // Continue search right after the inserted value
                searchPos = start + it->second.length();
            }
            else
            {
                PK_LOG_WARNING
                (
                    "Missing substitution for placeholder {{" << placeholder << "}} while preprocessing a .pkshad file: " << pkshadFilepath,
                    "Pekan"
                );
                // We can continue after that, we'll just not make the substitution as we don't have it
                searchPos = end + 2;
            }
        }

        // Write processed content to a .glsl file corresponding to the given .pkshad file
        const std::string glslFilepath = replaceFilepathExtension(pkshadFilepath, ".glsl");
        FileUtils::writeStringToTextFile(glslFilepath.c_str(), glslContent.c_str());
    }

} // namespace Graphics
} // namespace Pekan