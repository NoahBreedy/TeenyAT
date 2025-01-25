; Author: Noah Breedy
; Flappy bird using the left linear fader 
; when the fader makes a jump greater than 
; or equal to 25% it will move the player up 
; or down the appropriate amount      

.const LCD_CLEAR_SCREEN 0xA001
.const LCD_MOVE_UP 0xA004
.const LCD_MOVE_DOWN 0xA005
.const FADER_LEFT 0xA020

.const PLAYER_POS 0x0100