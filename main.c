/*
     .k8GOGGNqkSFS5XkqXPSkSkkqXXFS5kSkSS15U22F2515U2uujuu1U1u2U1U2uUuFS.
   :0qE     JS5uuJuuFFX51jU2SSk12jU2SSXF5uuu15SFS5k12ujj21S5kFS5S12jJYu11
  5XS:        1UYYLu.   vUUX    U22r     SUF         SUF           ;YYLuU5
 1F5i  NNSkS7  2uLJui   51u     S5.      .PX         .XX           LJvLLu1.
 kUk  0iLk5FFu vuYY2:   5F    Xkk7        78    E0    i0    GEXPXk2uLLvLLuk
X25, 8O   2kX0  5YJUi   M    555    PkXk   i    q1FU   7    ONNkP12YLvLvLYS
S25  8888  888  5uY5         FuS    PS50   .    FuUU   7          uJvLvLLJ2i
kUF             SJjU.      P02UF    P25k   .    Su2Y   v          2LLvLvLL17
S21  XJj88  0u  1uY2.        X2k           .    k11E   v    7;ii:JuJvLvLvJ2:
2257 jqv   Pqq  1LJur         PP.          7    EX:    q    OqqXP51JYvLvYYS.
 X2F  kXkXSXk  kJYLU:   O     ,Z    0PXZ   i    ii    q0    i:::,,.jLLvLLuF'
 ik1k  ;qkPj  .uJvYu:   UN      :   XU2F   :         S5S           iJLLvjUF8
  :PSq       72uLLLui   uSi    .;   2uY1   r.       72j1           LYYLYJSU88
    XqE2   rP12juJuu1FX55U5FqXXSXkXF1juUkkPSXSPXPXPF1Jju5FkFSFXFSF5uujUu5j28V
      .uGOZESS5S5SFkkPkPkXkPXqXPXqXXFkSkkPXPXPkqSkSS1521252121U2u2u12Suv7

*
* Arduino Micro (Leonardo) XInput Pad Emulator firmware
*
* Copyright (c) 2017
* Bruno Freitas - bruno@brunofreitas.com
* Jon Wilson    - degenatrons@gmail.com
* Kevin Mackett - kevin@sharpfork.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "XInputPad.h"
#include "util.h"

void setup_pins(void);

int main(void) {
	uint8_t pad_up, pad_down, pad_left, pad_right, pad_y, pad_b, pad_x, pad_a, pad_black,
	pad_white, pad_start, pad_select, pad_l3, pad_r3, pad_l, pad_r, pad_left_analog_x,
	pad_left_analog_y, pad_right_analog_x, pad_right_analog_y;

	uint8_t shift;

	// Set clock @ 16Mhz
	CPU_PRESCALE(0);

	// Setup pins
	setup_pins();

	// Init XBOX pad emulation
	xbox_init(true);

	// Pins polling and gamepad status updates
	for (;;) {
		xbox_reset_watchdog();

		shift = !bit_check(PIND, 7);

		pad_up = !bit_check(PINC, 2);
		pad_down = !bit_check(PIND,0);
		pad_left = !bit_check(PIND, 1);
		pad_right = !bit_check(PIND, 2);
		pad_y = !bit_check(PIND, 3);
		pad_b = !bit_check(PIND, 4);
		pad_x = !bit_check(PIND, 5);
		pad_a = !bit_check(PIND, 6);
		pad_black =  !bit_check(PINB, 0);
		pad_white =  !bit_check(PINB, 1);
		pad_start =  !bit_check(PINC, 4);
		pad_select =  !bit_check(PINC, 5);
		pad_l3 =  !bit_check(PINC, 6);
		pad_r3 =  !bit_check(PINC, 7);
		pad_l = !bit_check(PINB, 7);
		pad_r = !bit_check(PINB, 6);

		pad_left_analog_x = pad_left_analog_y = pad_right_analog_x = pad_right_analog_y = 0x7F;

		if(!bit_check(PINB, 5)) {
			pad_right_analog_x = 0x00;
		} else if(!bit_check(PINB, 4)) {
			pad_right_analog_x = 0xFF;
		}

		if(!bit_check(PINB, 3)) {
			pad_right_analog_y = 0x00;
		} else if(!bit_check(PINB, 2)) {
			pad_right_analog_y = 0xFF;
		}

		if(shift) {
			if(pad_left) {
				pad_left_analog_x = 0x00;
			} else if(pad_right) {
				pad_left_analog_x = 0xFF;
			}

			if(pad_up) {
				pad_left_analog_y = 0x00;
			} else if(pad_down) {
				pad_left_analog_y = 0xFF;
			}

			pad_left = pad_right = pad_up = pad_down = 0;
		}

		pad_up    ? bit_set(gamepad_state.digital_buttons_1, XBOX_DPAD_UP)    : bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_UP);
		pad_down  ? bit_set(gamepad_state.digital_buttons_1, XBOX_DPAD_DOWN)  : bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_DOWN);
		pad_left  ? bit_set(gamepad_state.digital_buttons_1, XBOX_DPAD_LEFT)  : bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_LEFT);
		pad_right ? bit_set(gamepad_state.digital_buttons_1, XBOX_DPAD_RIGHT) : bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_RIGHT);

		pad_start  ? bit_set(gamepad_state.digital_buttons_1, XBOX_START)       : bit_clear(gamepad_state.digital_buttons_1, XBOX_START);
		pad_select ? bit_set(gamepad_state.digital_buttons_1, XBOX_BACK)        : bit_clear(gamepad_state.digital_buttons_1, XBOX_BACK);
		pad_l3     ? bit_set(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK)  : bit_clear(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK);
		pad_r3     ? bit_set(gamepad_state.digital_buttons_1, XBOX_RIGHT_STICK) : bit_clear(gamepad_state.digital_buttons_1, XBOX_RIGHT_STICK);

		pad_a ? bit_set(gamepad_state.digital_buttons_2, XBOX_A)    : bit_clear(gamepad_state.digital_buttons_2, XBOX_A);
		pad_b ? bit_set(gamepad_state.digital_buttons_2, XBOX_B)  : bit_clear(gamepad_state.digital_buttons_2, XBOX_B);
		pad_x ? bit_set(gamepad_state.digital_buttons_2, XBOX_X)  : bit_clear(gamepad_state.digital_buttons_2, XBOX_X);
		pad_y ? bit_set(gamepad_state.digital_buttons_2, XBOX_Y) : bit_clear(gamepad_state.digital_buttons_2, XBOX_Y);

		pad_black ? bit_set(gamepad_state.digital_buttons_2, XBOX_LB)    : bit_clear(gamepad_state.digital_buttons_2, XBOX_LB);
		pad_white ? bit_set(gamepad_state.digital_buttons_2, XBOX_RB)    : bit_clear(gamepad_state.digital_buttons_2, XBOX_RB);

		if(pad_start && pad_select) {
			bit_set(gamepad_state.digital_buttons_2, XBOX_HOME);
		} else {
			bit_clear(gamepad_state.digital_buttons_2, XBOX_HOME);
		}

		gamepad_state.l_x = pad_left_analog_x * 257 + -32768;
		gamepad_state.l_y = pad_left_analog_y * -257 + 32767;
		gamepad_state.r_x = pad_right_analog_x * 257 + -32768;
		gamepad_state.r_y = pad_right_analog_y * -257 + 32767;

		gamepad_state.lt = pad_l * 0xFF;
		gamepad_state.rt = pad_r * 0xFF;

		xbox_send_pad_state();
	}
}

void setup_pins(void) {

	// Setup pins
	bit_clear(DDRC, 1 << 2);
	bit_set(PORTC, 1 << 2);
	
	bit_clear(DDRD, 1 << 0);
	bit_set(PORTD, 1 << 0);
	
	bit_clear(DDRD, 1 << 1);
	bit_set(PORTD, 1 << 1);
	
	bit_clear(DDRD, 1 << 2);
	bit_set(PORTD, 1 << 2);
	
	bit_clear(DDRD, 1 << 3);
	bit_set(PORTD, 1 << 3);
	
	bit_clear(DDRD, 1 << 4);
	bit_set(PORTD, 1 << 4);
	
	bit_clear(DDRD, 1 << 5);
	bit_set(PORTD, 1 << 5);
	
	bit_clear(DDRD, 1 << 6);
	bit_set(PORTD, 1 << 6);
	
	bit_clear(DDRD, 1 << 7);
	bit_set(PORTD, 1 << 7);
	
	bit_clear(DDRB, 1 << 0);
	bit_set(PORTB, 1 << 0);
	
	bit_clear(DDRB, 1 << 1);
	bit_set(PORTB, 1 << 1);
	
	bit_clear(DDRC, 1 << 4);
	bit_set(PORTC, 1 << 4);
	
	bit_clear(DDRC, 1 << 5);
	bit_set(PORTC, 1 << 5);
	
	bit_clear(DDRC, 1 << 6);
	bit_set(PORTC, 1 << 6);
	
	bit_clear(DDRC, 1 << 7);
	bit_set(PORTC, 1 << 7);
	
	bit_clear(DDRB, 1 << 7);
	bit_set(PORTB, 1 << 7);
	
	bit_clear(DDRB, 1 << 6);
	bit_set(PORTB, 1 << 6);
	
	bit_clear(DDRB, 1 << 5);
	bit_set(PORTB, 1 << 5);
	
	bit_clear(DDRB, 1 << 4);
	bit_set(PORTB, 1 << 4);
	
	bit_clear(DDRB, 1 << 3);
	bit_set(PORTB, 1 << 3);
	
	bit_clear(DDRB, 1 << 2);
	bit_set(PORTB, 1 << 2);
}
