#pragma once

typedef enum {
  STARTUP,
  FSI_INIT,
  SD_MOUNT,
  MOUNT_ERROR,
  LIST_DIRS,
  NO_GRNLTR_DIR,
  DIR_ERROR,
  NO_WAVS,
  READING_WAV,
  MISSING_WAV,
  GRNLTR_INIT,
  OK
  } status_t;
