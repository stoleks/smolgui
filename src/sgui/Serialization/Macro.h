#pragma once

/**
 * Macro to ease serialization and deserialization
 */
#define ToJson(object, member)   { #member, object.member }
#define FromJson(object, member) j.at (#member).get_to (object.member)
