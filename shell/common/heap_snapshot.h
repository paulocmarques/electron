// Copyright (c) 2018 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_COMMON_HEAP_SNAPSHOT_H_
#define SHELL_COMMON_HEAP_SNAPSHOT_H_

namespace base {
class File;
}

namespace v8 {
class Isolate;
}

namespace electron {

bool TakeHeapSnapshot(v8::Isolate* isolate, base::File* file);

}  // namespace electron

#endif  // SHELL_COMMON_HEAP_SNAPSHOT_H_
