/*
Copyright 2020 gtips

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define RGBLIGHT_LAYERS
#define TAPPING_TERM 200
#define ONESHOT_TAP_TOGGLE 5
#define ONESHOT_TIMEOUT 5000
#define RGBLIGHT_DEFAULT_MODE   RGBLIGHT_MODE_STATIC_LIGHT
#define RGBLIGHT_DEFAULT_VAL    50
#define RGBLIGHT_LIMIT_VAL  255
#define RGBLIGHT_DEFAULT_SAT    180

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
