// SWE-450 Milestone 2 (Doodle Jump Game)
// Dima Bondar
// 02/09/2025

#ifndef LCD_H
#define LCD_H

#include <stdbool.h>

// Screen size
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

// Colors
#define LCD_WHITE 0x00
#define LCD_BLACK 0xFF

// Setup and cleanup
bool lcd_initialize();
void lcd_cleanup();

// Basic display
void lcd_clear();
void lcd_refresh();
void lcd_backlight(bool on);

// Text display
void lcd_write_text(const char* text, int x, int y);
void lcd_write_centered(const char* text, int y);

// Shapes
void lcd_draw_rectangle(int x1, int y1, int x2, int y2);
void lcd_draw_circle(int x, int y, int radius);

#endif