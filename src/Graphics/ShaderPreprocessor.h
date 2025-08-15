#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

namespace Pekan
{
namespace Graphics
{

    // A class used for preprocessing .pkshad* files into .glsl files
    //
    // * .pkshad is a custom Pekan format for shaders
    class ShaderPreprocessor
    {
    public:

        // Preprocesses a .pkshad file containing a Pekan shader, using a given list of substitutions.
        // Generates a corresponding .glsl file with the resulting shader
        // in the same path, same name, with .glsl extension.
        static void preprocess(const std::string& pkshadFilepath, const std::unordered_map<std::string, std::string>& substitutions);
    };

} // namespace Graphics
} // namespace Pekan