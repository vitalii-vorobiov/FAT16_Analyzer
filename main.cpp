#include <iostream>
#include <fstream>
#include <stdio.h>
#include "include/bootsector.h"
#include "include/file.h"
#define DIR_SIZE 32

int read_data(std::ifstream &data, void *out, int offset, size_t size) {
    data.seekg(offset, std::ios::beg);
    data.read((char *) out, size);
    return 0;
}

void print_date_time(uint16_t time, uint16_t date) {
    std::cout
        << std::setfill('0') << std::setw(2) << ((time >> 11) & 0x1F) << ":" // Hours
        << std::setfill('0') << std::setw(2) << ((time >> 5) & 0x3F) << ":"  // Minutes
        << std::setfill('0') << std::setw(2) << (time & 0x1F) * 2 << " ";    // Seconds

    std::cout
        << std::setfill('0') << std::setw(2) << (date & 0x1F) << "/"        // Day
        << std::setfill('0') << std::setw(2) << ((date >> 5) & 0x0F) << "/" // Month
        << ((date >> 9) & 0x7F) + 1980 << " | ";                                  // Year
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "You didnt specify image" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream img(argv[1], std::ios::binary | std::ios::in);
    Bootsector bootsector{};

    read_data(img, &bootsector.bytes_per_sector, 11, sizeof(uint16_t));
    read_data(img, &bootsector.sectors_per_cluster, 13, sizeof(uint8_t));
    read_data(img, &bootsector.number_of_reserved_sectors_filesystem, 14, sizeof(uint16_t));
    read_data(img, &bootsector.number_of_FATs, 16, sizeof(uint8_t));
    read_data(img, &bootsector.number_of_files_root_dir, 17, sizeof(uint16_t));
    read_data(img, &bootsector.size_of_FAT_in_sectors, 22, sizeof(uint16_t));
    read_data(img, &bootsector.signature_value, 510, sizeof(uint16_t));

    std::cout << "BOOTSECTOR: " << std::endl;

    std::cout << "Bytes per Sector:           " << bootsector.bytes_per_sector << std::endl;
    std::cout << "Sectors in a cluster:       " << bootsector.sectors_per_cluster << std::endl;
    std::cout << "FAT copies:                 " << bootsector.number_of_FATs << std::endl;
    std::cout << "FAT copy size in sectors:   " << bootsector.size_of_FAT_in_sectors << std::endl;
    std::cout << "FAT copy size in bytes:     " << bootsector.size_of_FAT_in_sectors * bootsector.bytes_per_sector << std::endl;
    std::cout << "Number of Root dir:         " << bootsector.number_of_files_root_dir << std::endl;
    std::cout << "Root dir size in bytes:     " << bootsector.number_of_files_root_dir * DIR_SIZE << std::endl;
    std::cout << "Number of reserved sectors: " << bootsector.number_of_reserved_sectors_filesystem << std::endl;
    std::cout << "Signature:                  " << ((bootsector.signature_value == 0xAA55) ? "Correct" : "Incorrect") << std::endl;
    std::cout << std::endl;

    std::cout << "FILES: " << std::endl;

    size_t offset = bootsector.bytes_per_sector + bootsector.number_of_FATs * bootsector.size_of_FAT_in_sectors * bootsector.bytes_per_sector;

    File file{};
    for (int i = 0; i < bootsector.number_of_files_root_dir; ++i) {
        read_data(img, &file.status, offset, sizeof(uint8_t));
        read_data(img, &file.file_name, offset, sizeof(char) * 8);
        read_data(img, &file.extension, offset + 8, sizeof(char) * 3);
        read_data(img, &file.size, offset + 28, sizeof(uint16_t));

        read_data(img, &file.creation_time_hms, offset + 14, sizeof(uint16_t));
        read_data(img, &file.creation_time_date, offset + 16, sizeof(uint16_t));
        read_data(img, &file.modified_time_hms, offset + 22, sizeof(uint16_t));
        read_data(img, &file.modified_time_date, offset + 24, sizeof(uint16_t));

        read_data(img, &file.attr, offset + 11, sizeof(uint8_t));
        read_data(img, &file.cluster_number, offset + 26, sizeof(uint16_t));

        if (file.status != 0x00) {
            std::cout << file.file_name;
            if (file.size != 0) {
                std::cout << "." << file.extension << " | " << std::setfill(' ') << std::setw(5) << file.size << " | ";
            } else {
                std::cout << "    " << " | " << std::setfill(' ') << std::setw(5) << "DIR" << " | ";
            }

            print_date_time(file.creation_time_hms, file.creation_time_date);
            print_date_time(file.modified_time_hms, file.modified_time_date);

            if (file.attr & 0x01) { std:: cout << "Read-Only" << " | ";}
            if (file.attr & 0x02) { std:: cout << "Hidden File" << " | ";}
            if (file.attr & 0x04) { std:: cout << "System File" << " | ";}
            if (file.attr & 0x08) { std:: cout << "Volume Label" << " | ";}
            if (file.attr & 0x0f) { std:: cout << "Long File Name" << " | ";}
            if (file.attr & 0x10) { std:: cout << "Directory" << " | ";}
            if (file.attr & 0x20) { std:: cout << "Archive" << " | ";}

            std::cout << file.cluster_number << " ";
            std::cout << std::endl;
        }

        offset += DIR_SIZE;
    }
    return 0;
}