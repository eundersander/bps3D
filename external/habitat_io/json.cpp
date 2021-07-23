// Copyright (c) Facebook, Inc. and its affiliates.
// This source code is licensed under the MIT license found at https://github.com/facebookresearch/habitat-sim/blob/master/LICENSE

// based on https://github.com/facebookresearch/habitat-sim/tree/master/src/esp/io

#include "json.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>

namespace habitat_io {

bool writeJsonToFile(const JsonDocument& document,
                     const std::string& filepath,
                     bool usePrettyWriter,
                     int maxDecimalPlaces) {
  assert(!filepath.empty());
  std::string outFilePath = filepath;
  // if (!Cr::Utility::String::endsWith(outFilePath, ".json")) {
  //   outFilePath += ".json";
  // }

  auto* f = fopen(outFilePath.c_str(), "w");
  if (!f) {
    return false;
  }

  char writeBuffer[65536];
  rapidjson::FileWriteStream os(f, writeBuffer, sizeof(writeBuffer));

  bool writeSuccess = false;
  if (usePrettyWriter) {
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
    if (maxDecimalPlaces != -1) {
      writer.SetMaxDecimalPlaces(maxDecimalPlaces);
    }
    writeSuccess = document.Accept(writer);
  } else {
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    if (maxDecimalPlaces != -1) {
      writer.SetMaxDecimalPlaces(maxDecimalPlaces);
    }
    writeSuccess = document.Accept(writer);
  }
  fclose(f);

  return writeSuccess;
}

JsonDocument parseJsonFile(const std::string& file) {
  FILE* pFile = fopen(file.c_str(), "rb");
  char buffer[65536];
  rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
  JsonDocument d;
  d.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
  fclose(pFile);

  if (d.HasParseError()) {
    std::cerr << "Parse error reading " << file << " Error code "
               << d.GetParseError() << " at " << d.GetErrorOffset();
    throw std::runtime_error("JSON parse error");
  }
  return d;
}

JsonDocument parseJsonString(const std::string& jsonString) {
  JsonDocument d;
  d.Parse(jsonString.c_str());

  if (d.HasParseError()) {
    std::cerr << "Parse error parsing json string. Error code "
               << d.GetParseError() << " at " << d.GetErrorOffset();
    throw std::runtime_error("JSON parse error");
  }
  return d;
}

std::string jsonToString(const JsonDocument& d) {
  rapidjson::StringBuffer buffer{};
  rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
  d.Accept(writer);
  return buffer.GetString();
}

}