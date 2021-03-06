#ifndef LOGFS_CONFIG_H
#define LOGFS_CONFIG_H

#define LogFS_NAME "LOGFS"
#define LogFS_VERSION 1

// FS GLOBAL CONFIG LOGFS v1
#define LogGS_FILE_NAME_LENGTH 32

enum {
  LOGFS_OK = 0,
  LOGFS_ERR_NOT_FORMATTED,
  LOGFS_ERR_DIFFERENT_VERSION,
  LOGFS_ERR_LOW_SPACE_HEADER,
  LOGFS_ERR_LOW_SPACE_SECTORS_MAP,
  LOGFS_ERR_LOW_SPACE_SECTORS,

  LOGFS_ERR_LONG_FILE_NAME,
  LOGFS_ERR_FILE_NOT_FOUND,
  LOGFS_ERR_FILE_NOT_OPENED,
  LOGFS_FILE_CLOSED,
  LOGFS_ERR_NOT_INITIALIZED,
  LOGFS_ERR_FILE_ALREADY_EXIST
};

#endif
