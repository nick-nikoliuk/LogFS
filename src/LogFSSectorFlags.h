#ifndef LOGFS_TABLE_FILE_H
#define LOGFS_TABLE_FILE_H

#include <string.h>
#include <stdint.h>
#include "config.h"

#define LOGFS_EMPTY_SECTOR 1
#define LOGFS_ACTIVE_SECTOR 2
#define LOGFS_FILE_START_SECTOR 4

struct LogFSSectorFlags {
  uint8_t flags;

  bool isEmpty() {
    return flags & LOGFS_EMPTY_SECTOR;
  }

  bool isDeleted() {
    return !(flags & LOGFS_ACTIVE_SECTOR);
  }

  bool isFileStart() {
    return flags & LOGFS_FILE_START_SECTOR;
  }

  void addFlags(uint8_t newFlags) {
    flags |= newFlags;
  }

  void removeFlags(uint8_t newFlags) {
    flags &= ~newFlags;
  }
};

#endif
