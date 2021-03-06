#include <stdint.h>
#include <iostream>
#include <string.h>
#include "../src/LogFS.h"
#include "test.h"
#include "file.h"

using namespace std;

char defaultName[] = "file.txt";
char secondFileName[] = "file2.txt";
char longName[] = "1234567890123456789001234567890.txt";
char defaultLogs[] = "some logs";
char secondLogs[] = "another logs line";

// --- TESTS ---

bool createFile() {
  LogFSRAMTest fs;

  return fs.createFile(defaultName).getStatus() == LOGFS_OK;
}

bool createLongNameFile() {
  LogFSRAMTest fs;

  return fs.createFile(longName).getStatus() == LOGFS_ERR_LONG_FILE_NAME;
}

bool createTwoFilesWithSameName() {
  LogFSRAMTest fs;
  fs.createFile(defaultName);

  return fs.createFile(defaultName).getStatus() == LOGFS_ERR_FILE_ALREADY_EXIST;
}

bool openFileNotExist() {
  LogFSRAMTest fs;

  return fs.openFile(defaultName).getStatus() == LOGFS_ERR_FILE_NOT_FOUND;
}

bool createAndOpenFile() {
  LogFSRAMTest fs;

  fs.createFile(defaultName);
  return fs.openFile(defaultName).getStatus() == LOGFS_OK;
}

bool createAndDeleteFile() {
  LogFSRAMTest fs;

  fs.createFile(defaultName);
  return fs.deleteFile(defaultName) == LOGFS_OK;
}

bool deleteFileNotExist() {
  LogFSRAMTest fs;

  return fs.deleteFile(defaultName) == LOGFS_ERR_FILE_NOT_FOUND;
}

bool deleteLongNameFile() {
  LogFSRAMTest fs;

  return fs.deleteFile(longName) == LOGFS_ERR_LONG_FILE_NAME;
}

bool openDeletedFile() {
  LogFSRAMTest fs;

  fs.createFile(defaultName);
  fs.deleteFile(defaultName);

  return fs.openFile(defaultName).getStatus() == LOGFS_ERR_FILE_NOT_FOUND;
}

bool fileNotExist() {
  LogFSRAMTest fs;

  return !fs.exist(defaultName);
}

bool fileExist() {
  LogFSRAMTest fs;

  fs.createFile(defaultName);
  return fs.exist(defaultName);
}

bool writeFile() {
  LogFSRAMTest fs;
  LogFSFile file = fs.createFile(defaultName);

  return file.write((uint8_t*)defaultLogs, strlen(defaultLogs)) == LOGFS_OK;
}

bool fileNotOpened() {
  LogFSRAMTest fs;

  LogFSFile file = fs.createFile(longName);

  return file.write((uint8_t*)defaultLogs, strlen(defaultLogs)) == LOGFS_ERR_FILE_NOT_OPENED;
}

bool readFile() {
  LogFSRAMTest fs;

  uint8_t logsLength = strlen(defaultLogs) + 1;
  LogFSFile file = fs.createFile(defaultName);

  file.write(defaultLogs, logsLength);
  char buffer[logsLength];
  file.read(buffer, logsLength);

  return strcmp(buffer, defaultLogs) == 0;
}

bool writeWithTwoOpen() {
  LogFSRAMTest fs;

  char logs[] = "12345678\n";
  fs.createFile(defaultName);
  uint8_t chunkSize = 3;
  for (uint8_t i = 0; i < strlen(logs); i += chunkSize) {
    LogFSFile file = fs.openFile(defaultName);
    file.write((uint8_t*)(logs + i), chunkSize);
  }
  char buffer[strlen(logs)];

  LogFSFile file = fs.openFile(defaultName);
  file.read((uint8_t*)buffer, strlen(logs));

  return strcmp(buffer, logs);
}

bool readTwoFiles() {
  LogFSRAMTest fs;

  uint8_t logsLength1 = strlen(defaultLogs) + 1;
  uint8_t logsLength2 = strlen(secondLogs) + 1;
  LogFSFile file1 = fs.createFile(defaultName);
  LogFSFile file2 = fs.createFile(secondFileName);
  file1.write((uint8_t*)defaultLogs, logsLength1);
  file2.write((uint8_t*)secondLogs, logsLength2);

  char buffer1[logsLength1];
  char buffer2[logsLength2];
  file1.read((uint8_t*)buffer1, logsLength1);
  file2.read((uint8_t*)buffer2, logsLength2);

  return strcmp(buffer1, defaultLogs) == 0 && strcmp(buffer2, secondLogs) == 0;
}

bool writeAndReadMoreThanPageSize() {
  LogFSRAMTest fs;

  uint16_t logSize = PAGE_SIZE * 2 + 24;
  char log[logSize];
  for (uint16_t i = 0; i < logSize; i++) {
    log[i] = i + 48;
  }
  log[logSize - 1] = '\0';

  LogFSFile file = fs.createFile(defaultName);
  file.write((uint8_t*)log, logSize);

  char buffer[logSize];
  file.read((uint8_t*)buffer, logSize);

  return strcmp(buffer, log) == 0;
}

bool writeAndReadMoreThanSectorSize() {
  LogFSRAMTest fs;

  uint16_t logSize = SECTOR_SIZE * 2 + 24;
  char log[logSize];
  for (uint16_t i = 0; i < logSize; i++) {
    log[i] = i + 48;
  }
  log[logSize - 1] = '\0';

  LogFSFile file = fs.createFile(defaultName);
  file.write((uint8_t*)log, logSize);

  char buffer[logSize];
  file.read((uint8_t*)buffer, logSize);

  return strcmp(buffer, log) == 0;
}

bool writeByChunks() {
  LogFSRAMTest fs;

  char logs[] = "12345678\n";
  LogFSFile file = fs.createFile(defaultName);
  uint8_t chunkSize = 3;
  for (uint8_t i = 0; i < strlen(logs); i += chunkSize) {
    file.write((uint8_t*)(logs + i), chunkSize);
  }
  char buffer[strlen(logs)];
  file.read((uint8_t*)buffer, strlen(logs));

  return strcmp(buffer, logs);
}

bool readByChunks() {
  LogFSRAMTest fs;

  char logs[] = "12345678\n";
  LogFSFile file = fs.createFile(defaultName);
  uint8_t chunkSize = 3;
  file.write((uint8_t*)logs, strlen(logs));
  char buffer[strlen(logs)];

  for (uint8_t i = 0; i < strlen(logs); i += chunkSize) {
    file.read((uint8_t*)(buffer + i), chunkSize);
  }

  return strcmp(buffer, logs);
}

bool getTotalSize() {
  LogFSRAMTest fs;
  return fs.getTotalSize() > 0;
}

bool getUsedSize() {
  LogFSRAMTest fs;
  fs.createFile(defaultName);
  fs.createFile(secondFileName);

  return fs.getUsedSize() == (fs.getHeader()->sectorSize - sizeof(fs.getHeader()->sectorsAmount)) * 2;
}

bool getAvailableSize() {
  LogFSRAMTest fs;
  fs.createFile(defaultName);

  return fs.getAvailableSize() == fs.getTotalSize() - fs.getUsedSize();
}

bool releaseSectors() {
  LogFSRAMTest fs;

  uint32_t sizeBefore = fs.getAvailableSize();
  fs.createFile(defaultName);
  uint32_t sizeInTheMiddle = fs.getAvailableSize();
  fs.deleteFile(defaultName);
  uint32_t sizeAfter = fs.getAvailableSize();

  return sizeBefore == sizeAfter && sizeInTheMiddle != sizeAfter;
}

bool readEmptyFiles() {
  LogFSRAMTest fs;

  char buffer[16];

  return fs.readFiles().next(buffer) == LOGFS_ERR_FILE_NOT_FOUND;
}

bool readCreatedFiles() {
  LogFSRAMTest fs;

  fs.createFile(defaultName);
  fs.createFile(secondFileName);

  char buffer1[16];
  char buffer2[16];

  LogFSDirectory dir = fs.readFiles();
  dir.next(buffer1);
  dir.next(buffer2);
  return strcmp(buffer1, defaultName) == 0 && strcmp(buffer2, secondFileName) == 0;
}

bool checkWriteSize() {

  LogFSRAMTest fs;
  LogFSFile file = fs.createFile(defaultName);
  uint8_t logLength = strlen(defaultLogs) + 1;
  file.write((uint8_t*)defaultLogs, logLength);
  file.write((uint8_t*)defaultLogs, logLength);

  return file.size() == logLength * 2;
}

bool overflowSectorsMap() {
  LogFSRAMTest fs;

  uint16_t filesCount = MAX_MEMORY_SIZE / SECTOR_SIZE;
  while (filesCount--) {
    if (fs.createFile(defaultName).getStatus() != LOGFS_OK) return false;
    if (fs.deleteFile(defaultName) != LOGFS_OK) return false;
  }
  LogFSFile file = fs.createFile(defaultName);
  file.write(defaultLogs, strlen(defaultLogs) + 1);

  char buffer[strlen(defaultLogs) + 1];
  file.read(buffer, file.size());

  return strcmp(buffer, defaultLogs) == 0;
}

bool fillAllTheStorage() {
  LogFSRAMTest fs;

  LogFSFile file = fs.createFile(defaultName);
  uint32_t counter = 0;

  while (file.write(defaultLogs, strlen(defaultLogs)) == LOGFS_OK) {
    counter++;
    if (counter % 10 == 0) {
//      cout << "log: " << counter << endl;
    }
  }

  fs.deleteFile(defaultName);;

  return fs.getUsedSize() == 0;
}

void testFile() {
  cout << "File:" << endl;

  test("create file", createFile());
  test("create long name file", createLongNameFile());
  test("create two files with same name", createTwoFilesWithSameName());
  test("open file not exist", openFileNotExist());
  test("create and open", createAndOpenFile());
  test("create and delete file", createAndDeleteFile());
  test("delete file not exist", deleteFileNotExist());
  test("delete long name file", deleteLongNameFile());
  test("open deleted file", openDeletedFile());
  test("file not exist", fileNotExist());
  test("file exist", fileExist());
  test("write file", writeFile());
  test("file not opened", fileNotOpened());
  test("write and read file", readFile());
  test("write with two open", writeWithTwoOpen());
  test("write and read two files", readTwoFiles());
  test("write and read more than page size", writeAndReadMoreThanPageSize());
  test("write and read more than sector size", writeAndReadMoreThanSectorSize());
  test("write by chunks", writeByChunks());
  test("read by chunks", readByChunks());
  test("get total size", getTotalSize());
  test("get used size", getUsedSize());
  test("get available size", getAvailableSize());
  test("release pages", releaseSectors());
  test("read empty files list", readEmptyFiles());
  test("read created files list", readCreatedFiles());
  test("check write size", checkWriteSize());

  test("overflow sectors map", overflowSectorsMap());
  test("fill all the storage", fillAllTheStorage());

  // page release after write with full storage
}
