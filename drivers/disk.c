// SPDX-License-Identifier: GPL-2.0-only
/*
 * drivers/disk.c
 * 
 * ATA PIO driver. Not really functional.
 *
 * Copyright (C) 2024 Goldside543
 *
 */

#include "../kernel/io.h"
#include "disk.h"

void ata_pio_select_drive(uint8_t drive) {
    outb(ATA_REG_DRIVE_SELECT, 0xA0 | (drive << 4));
}

void ata_pio_wait_ready() {
    uint8_t status;
    // Read 15 times and pay attention to the last
    for (int i = 0; i < 15; i++) {
        status = inb(ATA_REG_STATUS);
    }
    
    // Ensure the drive is not busy and is ready
    while (status & ATA_STATUS_BUSY) {
        status = inb(ATA_REG_STATUS);
    }
}

void ata_pio_init() {
    outb(0x3F6, 0);    

    // Select the primary master drive
    ata_pio_select_drive(0);

    // Send IDENTIFY command to the drive
    ata_pio_wait_ready();
    outb(ATA_REG_COMMAND, ATA_COMMAND_IDENTIFY);

    // Wait for the drive to be ready
    ata_pio_wait_ready();

}

void ata_pio_read(uint32_t lba, void *buffer, size_t size) {
    uint8_t *buf = (uint8_t *)buffer;
    ata_pio_wait_ready();
    ata_pio_select_drive(0);

    // Setup LBA address
    outb(ATA_REG_SECTOR_COUNT, 1);
    outb(ATA_REG_LBA_LOW, (lba & 0xFF));
    outb(ATA_REG_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_REG_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_REG_COMMAND, ATA_COMMAND_READ_SECTORS);

    ata_pio_wait_ready();
    while (size--) {
        if (inb(ATA_REG_STATUS) & ATA_STATUS_DRQ) {
            *buf++ = inb(ATA_REG_DATA);
        }
    }
}

void ata_pio_write(uint32_t lba, const void *buffer, size_t size) {
    const uint8_t *buf = (const uint8_t *)buffer;
    ata_pio_wait_ready();
    ata_pio_select_drive(0); // Select primary master drive

    // Setup LBA address
    outb(ATA_REG_SECTOR_COUNT, 1);
    outb(ATA_REG_LBA_LOW, (lba & 0xFF));
    outb(ATA_REG_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_REG_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_REG_COMMAND, ATA_COMMAND_WRITE_SECTORS);

    ata_pio_wait_ready();
    while (size--) {
        if (inb(ATA_REG_STATUS) & ATA_STATUS_DRQ) {
            outb(ATA_REG_DATA, *buf++);
        }
    }
}
