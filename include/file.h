#ifndef FAT16_FILE_H
#define FAT16_FILE_H

struct File {
    uint8_t status;
    char file_name[9];
    char extension[3];
    uint16_t size;
    uint16_t creation_time_hms;
    uint16_t creation_time_date;
    uint16_t modified_time_hms;
    uint16_t modified_time_date;
    uint8_t attr;
    uint16_t cluster_number;
};

#endif //FAT16_FILE_H
