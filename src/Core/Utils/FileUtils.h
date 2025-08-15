#pragma once

#include <string>

namespace Pekan
{
namespace FileUtils
{

	// Reads a text file's contents into a string
	std::string readTextFileToString(const char* filepath);

	// Writes a string into a text file.
	// If the file doesn't exist, it will be created.
	// If the file exists, it will be overwritten.
	void writeStringToTextFile(const char* filepath, const char* content);

	// Reads an image file.
	// Returns raw pixel data, where each byte is a component of a pixel.
	// 
	// Fills in output parameters with
	// - width of image, in pixels
	// - height of image, in pixels
	// - number of color channels of the image
	// 
	// If image fails to load, a null pointer will be returned.
	const unsigned char* readImageFile(const char* filepath, int& width, int& height, int& numChannels);

} // namespace FileUtils
} // namespace Pekan
