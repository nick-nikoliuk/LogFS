#ifndef LOGFS_STORAGEIO_H
#define LOGFS_STORAGEIO_H

#include <stdint.h>

struct LogFSStorageIO {
private:
  uint32_t _capacity;
  uint16_t _pageSize;
  uint16_t _sectorSize;

public:
  LogFSStorageIO(uint32_t capacity, uint16_t sectorSize, uint16_t pageSize) {
    _capacity = capacity;
    _sectorSize = sectorSize;
    _pageSize = pageSize;
  }

  void writeBytes(uint32_t sectorIndex, uint16_t pageIndex, uint16_t offset, void* data, uint16_t length);
  void readBytes(uint32_t sectorIndex, uint16_t pageIndex, uint16_t offset, void* data, uint16_t length);

  virtual void resetChip() = 0;
  virtual void resetSector(uint32_t sectorIndex) = 0;
  virtual void writePage(uint32_t sectorIndex, uint16_t pageIndex, void* pageData) = 0;
  virtual void readPage(uint32_t sectorIndex, uint16_t pageIndex, void* pageData) = 0;

  uint32_t getCapacity() { return _capacity; }
  uint16_t getSectorSize() { return _sectorSize; }
  uint16_t getPageSize() { return _pageSize; }
};

#endif
