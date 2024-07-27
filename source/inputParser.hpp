#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/json.hpp>

namespace jaffar
{

  struct input_t
  {
    uint8_t position = 0;
    bool fire = false;
  };


class InputParser
{

public:

  InputParser(const nlohmann::json &config)
  {
  }

  inline input_t parseInputString(const std::string& inputString) const
  {
    // Converting input into a stream for parsing
    std::istringstream ss(inputString);

    // Start separator
    if (ss.get() != '|') reportBadInputString(inputString);
    if (ss.get() != '.') reportBadInputString(inputString);
    if (ss.get() != '.') reportBadInputString(inputString);
    if (ss.get() != '|') reportBadInputString(inputString);

    // Storage for the input
    input_t input;

    // Parsing console inputs
    parseArkanoidInput(input, ss, inputString);

    // End separator
    if (ss.get() != '|') reportBadInputString(inputString);

    // If its not the end of the stream, then extra values remain and its invalid
    ss.get();
    if (ss.eof() == false) reportBadInputString(inputString);

    // Returning valid / input pair
    return input;
  }

  private:

  static inline void reportBadInputString(const std::string& inputString)
  {
    JAFFAR_THROW_LOGIC("Could not decode input string: '%s'\n", inputString.c_str());
  }

  static inline bool parseArkanoidInput(input_t& input, std::istringstream& ss, const std::string& inputString)
  {
    char c = 0;
    ss.get(); // Empty
    ss.get(); // Empty

    c = ss.get(); // Hundreds
    if (c != ' ') input.position += 100 * ( (uint8_t)c - 48 );

    c = ss.get(); // Tenths
    if (c != ' ') input.position += 10 * ( (uint8_t)c - 48 );

    c = ss.get(); // Units
    if (c != ' ') input.position += (uint8_t)c - 48;

    ss.get(); // Comma

    // Fire
    c = ss.get();
    if (c != '.' && c != 'F') reportBadInputString(inputString);
    if (c == 'F') input.fire = true;
    return true;
  }

}; // class InputParser

} // namespace jaffar