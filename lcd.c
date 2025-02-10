// SWE-450 Milestone 2 (Doodle Jump Game)
// Dima Bondar
// 02/09/2025

#include "lcd.h"
#include "LCD_Lib.h"
#include "lcd_graphic.h"
#include "font.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <fcntl.h>
#include <sys/mman.h>

// Memory settings
#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0x04000000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

// Global vars
static void* virtual_base = NULL;
static int fd = -1;
static LCD_CANVAS LcdCanvas;

bool lcd_initialize() {
    // Open memory
    fd = open("/dev/mem", (O_RDWR | O_SYNC));
    if (fd == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return false;
    }

    // Map memory
    virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), 
                       MAP_SHARED, fd, HW_REGS_BASE);
    if (virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return false;
    }

    // Setup canvas
    LcdCanvas.Width = LCD_WIDTH;
    LcdCanvas.Height = LCD_HEIGHT;
    LcdCanvas.BitPerPixel = 1;
    LcdCanvas.FrameSize = LcdCanvas.Width * LcdCanvas.Height / 8;
    LcdCanvas.pFrame = malloc(LcdCanvas.FrameSize);

    if (LcdCanvas.pFrame == NULL) {
        printf("Failed to allocate LCD frame buffer\n");
        munmap(virtual_base, HW_REGS_SPAN);
        close(fd);
        return false;
    }

    // Setup hardware
    LCDHW_Init(virtual_base);
    LCDHW_BackLight(true);
    LCD_Init();

    // Clear screen
    lcd_clear();
    
    return true;
}

void lcd_cleanup() {
    // Free memory
    if (LcdCanvas.pFrame) {
        free(LcdCanvas.pFrame);
        LcdCanvas.pFrame = NULL;
    }
    
    if (virtual_base) {
        if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
            printf("ERROR: munmap() failed...\n");
        }
        virtual_base = NULL;
    }
    
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

// Clear screen
void lcd_clear() {
    DRAW_Clear(&LcdCanvas, LCD_WHITE);
    DRAW_Refresh(&LcdCanvas);
}

// Update display
void lcd_refresh() {
    DRAW_Refresh(&LcdCanvas);
}

// Toggle backlight
void lcd_backlight(bool on) {
    LCDHW_BackLight(on);
}

// Draw text at position
void lcd_write_text(const char* text, int x, int y) {
    DRAW_PrintString(&LcdCanvas, x, y, (char*)text, LCD_BLACK, &font_16x16);
    lcd_refresh();
}

// Draw centered text
void lcd_write_centered(const char* text, int y) {
    if (!text) return;
    
    // Center the text
    int text_width = strlen(text) * 8;
    int x = (LCD_WIDTH - text_width) / 2;
    if (x < 0) x = 0;
    
    lcd_write_text(text, x, y);
}

// Draw rectangle
void lcd_draw_rectangle(int x1, int y1, int x2, int y2) {
    DRAW_Rect(&LcdCanvas, x1, y1, x2, y2, LCD_BLACK);
    lcd_refresh();
}

// Draw circle
void lcd_draw_circle(int x, int y, int radius) {
    DRAW_Circle(&LcdCanvas, x, y, radius, LCD_BLACK);
    lcd_refresh();
}