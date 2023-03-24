// Copyright (c) Meta Platforms, Inc. All Rights Reserved
// Copyright (c) Facebook, Inc. and its affiliates.
// This source code is licensed under the MIT license found at https://github.com/facebookresearch/habitat-sim/blob/master/LICENSE

// based on https://github.com/facebookresearch/habitat-sim/tree/master/src/esp/io

#pragma once

/** @file
 * @brief See JsonAllTypes.h. Don't include this header directly in user code.
 */

#include "JsonBuiltinTypes.h"

#include <map>
#include <iostream>
#include <vector>

namespace habitat_io {

inline JsonGenericValue toJsonValue(const std::string& str,
                                    JsonAllocator& allocator) {
  JsonGenericValue strObj;
  strObj.SetString(str.c_str(), allocator);
  return strObj;
}

/**
 * @brief Populate passed @p val with value. Returns whether successfully
 * populated, or not. Logs an error if inappropriate type.
 *
 * @param obj json value to parse
 * @param val destination value to be populated
 * @return whether successful or not
 */
inline bool fromJsonValue(const JsonGenericValue& obj, std::string& val) {
  if (obj.IsString()) {
    val = obj.GetString();
    return true;
  }
  std::cerr << "Invalid string value";
  return false;
}

// For std::vector, we use rapidjson::kArrayType. For an empty vector, we
// omit the member altogether rather than add an empty array.

template <typename T>
void addMember(JsonGenericValue& value,
               rapidjson::GenericStringRef<char> name,
               const std::vector<T>& vec,
               JsonAllocator& allocator) {
  if (!vec.empty()) {
    addMember(value, name, toJsonArrayHelper(vec.data(), vec.size(), allocator),
              allocator);
  }
}

template <typename T>
bool readMember(const JsonGenericValue& value,
                const char* tag,
                std::vector<T>& vec) {
  assert(vec.empty());
  JsonGenericValue::ConstMemberIterator itr = value.FindMember(tag);
  if (itr != value.MemberEnd()) {
    const JsonGenericValue& arr = itr->value;
    if (!arr.IsArray()) {
      std::cerr << "JSON tag " << tag << " is not an array";
      return false;
    }
    vec.reserve(arr.Size());
    for (size_t i = 0; i < arr.Size(); i++) {
      const auto& itemObj = arr[i];
      T item;
      if (!fromJsonValue(itemObj, item)) {
        vec.clear();  // return an empty container on failure
        std::cerr << "Failed to parse array element " << i << " in JSON tag "
                   << tag;
        return false;
      }
      vec.emplace_back(std::move(item));
    }
  }
  // if the tag isn't found, the container is left empty and we return success
  return true;
}

/**
 * @brief Specialization to handle reading a JSON object into an
 * std::map<std::string, std::string>.  Check passed json doc for existence of
 * passed @p tag and verify it is an object. If present, populate passed @p val
 * with key-value pairs in cell. Returns whether tag is found and successfully
 * populated, or not. Logs an error if tag is found but is inappropriately
 * configured
 *
 * @param d json document to parse
 * @param tag string tag to look for in json doc
 * @param val destination std::map to be populated
 * @return whether successful or not
 */

template <>
inline bool readMember(const JsonGenericValue& d,
                       const char* tag,
                       std::map<std::string, std::string>& val) {
  if (d.HasMember(tag)) {
    if (d[tag].IsObject()) {
      const auto& jCell = d[tag];
      for (rapidjson::Value::ConstMemberIterator it = jCell.MemberBegin();
           it != jCell.MemberEnd(); ++it) {
        const std::string key = it->name.GetString();
        if (it->value.IsString()) {
          val.emplace(key, it->value.GetString());
        } else {
          std::cerr << "Invalid string value specified in JSON config " << tag
                     << " at " << key << ". Skipping.";
        }
      }  // for each value
      return true;
    } else {  // if member is object
      std::cerr << "Invalid JSON Object value specified in JSON config at "
                 << tag << "; Unable to populate std::map.";
    }
  }  // if has tag
  return false;
}  //  readMember<std::map<std::string, std::string>>

/**
 * @brief Specialization to handle reading a JSON object into an
 * std::map<std::string, float>.  Check passed json doc for existence of
 * passed @p tag and verify it is an object. If present, populate passed @p val
 * with key-value pairs in cell. Returns whether tag is found and successfully
 * populated, or not. Logs an error if tag is found but is inappropriately
 * configured
 *
 * @param d json document to parse
 * @param tag string tag to look for in json doc
 * @param val destination std::map to be populated
 * @return whether successful or not
 */

template <>
inline bool readMember(const JsonGenericValue& d,
                       const char* tag,
                       std::map<std::string, float>& val) {
  if (d.HasMember(tag)) {
    if (d[tag].IsObject()) {
      const auto& jCell = d[tag];
      for (rapidjson::Value::ConstMemberIterator it = jCell.MemberBegin();
           it != jCell.MemberEnd(); ++it) {
        const std::string key = it->name.GetString();
        if (it->value.IsFloat()) {
          val.emplace(key, it->value.GetFloat());
        } else {
          std::cerr << "Invalid float value specified in JSON map " << tag
                     << " at " << key << ". Skipping.";
        }
      }  // for each value
      return true;
    } else {  // if member is object
      std::cerr << "Invalid JSON Object value specified in JSON config at "
                 << tag << "; Unable to populate std::map.";
    }
  }  // if has tag
  return false;
}  //  readMember<std::map<std::string, float>>

}