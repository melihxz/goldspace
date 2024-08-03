#ifndef KEYBOARD_H
#define KEYBOARD_H

#define USB_KEYBOARD_BASE 0xFCBA0000
#define USB_KEYBOARD_DATA (USB_KEYBOARD_BASE + 0x00)
#define USB_KEYBOARD_STATUS (USB_KEYBOARD_BASE + 0x04)
#define USB_KEYBOARD_STATUS_READY 0x01

void init_keyboard(void);
char read_keyboard(void);

#endif // KEYBOARD_H
