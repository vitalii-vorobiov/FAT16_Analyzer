#ifndef FAT16_BOOTSECTOR_H
#define FAT16_BOOTSECTOR_H

struct Bootsector {
    uint16_t bytes_per_sector;
    uint16_t sectors_per_cluster;
    uint16_t number_of_FATs;
    uint16_t size_of_FAT_in_sectors;
    uint16_t number_of_files_root_dir;
    uint16_t number_of_reserved_sectors_filesystem;
    uint16_t signature_value;
};

#endif //FAT16_BOOTSECTOR_H
