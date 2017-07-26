// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef SkTraceEventCommon_DEFINED
#define SkTraceEventCommon_DEFINED

// Trace events are for tracking application performance and resource usage.
// Macros are provided to track:
//    Duration of scoped regions
//    Instantaneous events
//    Counters
//
// The first two arguments to all TRACE macros are the category and name. Both are strings, and
// must have application lifetime (statics or literals). The same applies to arg_names, and string
// argument values. However, you can force a copy of a string argument value with TRACE_STR_COPY:
//     TRACE_EVENT1("category", "name", "arg1", "literal string is only referenced");
//     TRACE_EVENT1("category", "name", "arg1", TRACE_STR_COPY("string will be copied"));
//
//
// Categories are used to group events, and
// can be enabled or disabled by the tracing framework. The trace system will automatically add the
// process id, thread id, and microsecond timestamp to all events.
//
//
// The TRACE_EVENT[0-2] macros trace the duration of entire scopes:
//   void doSomethingCostly() {
//     TRACE_EVENT0("MY_SUBSYSTEM", "doSomethingCostly");
//     ...
//   }
//
// Additional parameters can be associated with an event:
//   void doSomethingCostly2(int howMuch) {
//     TRACE_EVENT1("MY_SUBSYSTEM", "doSomethingCostly", "howMuch", howMuch);
//     ...
//   }
//
//
// Trace event also supports counters, which is a way to track a quantity as it varies over time.
// Counters are created with the following macro:
//   TRACE_COUNTER1("MY_SUBSYSTEM", "myCounter", g_myCounterValue);
//
// Counters are process-specific. The macro itself can be issued from any thread, however.
//
// Sometimes, you want to track two counters at once. You can do this with two counter macros:
//   TRACE_COUNTER1("MY_SUBSYSTEM", "myCounter0", g_myCounterValue[0]);
//   TRACE_COUNTER1("MY_SUBSYSTEM", "myCounter1", g_myCounterValue[1]);
// Or you can do it with a combined macro:
//   TRACE_COUNTER2("MY_SUBSYSTEM", "myCounter",
//                  "bytesPinned", g_myCounterValue[0],
//                  "bytesAllocated", g_myCounterValue[1]);
// The tracing UI will show these counters in a single graph, as a summed area chart.

#if defined(TRACE_EVENT0)
#error "Another copy of this file has already been included."
#endif

// Records a pair of begin and end events called "name" for the current scope, with 0, 1 or 2
// associated arguments. If the category is not enabled, then this does nothing.
#define TRACE_EVENT0(category_group, name) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED(category_group, name)

#define TRACE_EVENT1(category_group, name, arg1_name, arg1_val) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED(category_group, name, arg1_name, arg1_val)

#define TRACE_EVENT2(category_group, name, arg1_name, arg1_val, arg2_name, arg2_val) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED(category_group, name, arg1_name, arg1_val, arg2_name, arg2_val)

// Records a single event called "name" immediately, with 0, 1 or 2 associated arguments. If the
// category is not enabled, then this does nothing.
#define TRACE_EVENT_INSTANT0(category_group, name, scope)                   \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category_group, name, \
                           TRACE_EVENT_FLAG_NONE | scope)

#define TRACE_EVENT_INSTANT1(category_group, name, scope, arg1_name, arg1_val) \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category_group, name,    \
                           TRACE_EVENT_FLAG_NONE | scope, arg1_name, arg1_val)

#define TRACE_EVENT_INSTANT2(category_group, name, scope, arg1_name, arg1_val, \
                             arg2_name, arg2_val)                              \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category_group, name,    \
                           TRACE_EVENT_FLAG_NONE | scope, arg1_name, arg1_val, \
                           arg2_name, arg2_val)

// Records the value of a counter called "name" immediately. Value
// must be representable as a 32 bit integer.
#define TRACE_COUNTER1(category_group, name, value)                         \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, category_group, name, \
                           TRACE_EVENT_FLAG_NONE, "value",                  \
                           static_cast<int>(value))

// Records the values of a multi-parted counter called "name" immediately.
// The UI will treat value1 and value2 as parts of a whole, displaying their
// values as a stacked-bar chart.
#define TRACE_COUNTER2(category_group, name, value1_name, value1_val,       \
                       value2_name, value2_val)                             \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, category_group, name, \
                           TRACE_EVENT_FLAG_NONE, value1_name,              \
                           static_cast<int>(value1_val), value2_name,       \
                           static_cast<int>(value2_val))

// Macro to efficiently determine if a given category group is enabled.
#define TRACE_EVENT_CATEGORY_GROUP_ENABLED(category_group, ret)             \
  do {                                                                      \
    INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category_group);                 \
    if (INTERNAL_TRACE_EVENT_CATEGORY_GROUP_ENABLED_FOR_RECORDING_MODE()) { \
      *ret = true;                                                          \
    } else {                                                                \
      *ret = false;                                                         \
    }                                                                       \
  } while (0)


// Phase indicates the nature of an event entry. E.g. part of a begin/end pair.
#define TRACE_EVENT_PHASE_BEGIN ('B')
#define TRACE_EVENT_PHASE_END ('E')
#define TRACE_EVENT_PHASE_COMPLETE ('X')
#define TRACE_EVENT_PHASE_INSTANT ('I')
#define TRACE_EVENT_PHASE_ASYNC_BEGIN ('S')
#define TRACE_EVENT_PHASE_ASYNC_END ('F')
#define TRACE_EVENT_PHASE_COUNTER ('C')

// Flags for changing the behavior of TRACE_EVENT_API_ADD_TRACE_EVENT.
#define TRACE_EVENT_FLAG_NONE (static_cast<unsigned int>(0))
#define TRACE_EVENT_FLAG_COPY (static_cast<unsigned int>(1 << 0))
#define TRACE_EVENT_FLAG_HAS_ID (static_cast<unsigned int>(1 << 1))
#define TRACE_EVENT_FLAG_MANGLE_ID (static_cast<unsigned int>(1 << 2))
#define TRACE_EVENT_FLAG_SCOPE_OFFSET (static_cast<unsigned int>(1 << 3))
#define TRACE_EVENT_FLAG_SCOPE_EXTRA (static_cast<unsigned int>(1 << 4))
#define TRACE_EVENT_FLAG_EXPLICIT_TIMESTAMP (static_cast<unsigned int>(1 << 5))
#define TRACE_EVENT_FLAG_ASYNC_TTS (static_cast<unsigned int>(1 << 6))
#define TRACE_EVENT_FLAG_BIND_TO_ENCLOSING (static_cast<unsigned int>(1 << 7))
#define TRACE_EVENT_FLAG_FLOW_IN (static_cast<unsigned int>(1 << 8))
#define TRACE_EVENT_FLAG_FLOW_OUT (static_cast<unsigned int>(1 << 9))
#define TRACE_EVENT_FLAG_HAS_CONTEXT_ID (static_cast<unsigned int>(1 << 10))

#define TRACE_EVENT_FLAG_SCOPE_MASK                          \
  (static_cast<unsigned int>(TRACE_EVENT_FLAG_SCOPE_OFFSET | \
                             TRACE_EVENT_FLAG_SCOPE_EXTRA))

// Type values for identifying types in the TraceValue union.
#define TRACE_VALUE_TYPE_BOOL (static_cast<unsigned char>(1))
#define TRACE_VALUE_TYPE_UINT (static_cast<unsigned char>(2))
#define TRACE_VALUE_TYPE_INT (static_cast<unsigned char>(3))
#define TRACE_VALUE_TYPE_DOUBLE (static_cast<unsigned char>(4))
#define TRACE_VALUE_TYPE_POINTER (static_cast<unsigned char>(5))
#define TRACE_VALUE_TYPE_STRING (static_cast<unsigned char>(6))
#define TRACE_VALUE_TYPE_COPY_STRING (static_cast<unsigned char>(7))
#define TRACE_VALUE_TYPE_CONVERTABLE (static_cast<unsigned char>(8))

// Enum reflecting the scope of an INSTANT event. Must fit within TRACE_EVENT_FLAG_SCOPE_MASK.
#define TRACE_EVENT_SCOPE_GLOBAL (static_cast<unsigned char>(0 << 3))
#define TRACE_EVENT_SCOPE_PROCESS (static_cast<unsigned char>(1 << 3))
#define TRACE_EVENT_SCOPE_THREAD (static_cast<unsigned char>(2 << 3))

#define TRACE_EVENT_SCOPE_NAME_GLOBAL ('g')
#define TRACE_EVENT_SCOPE_NAME_PROCESS ('p')
#define TRACE_EVENT_SCOPE_NAME_THREAD ('t')

#endif  // SkTraceEventCommon_DEFINED
