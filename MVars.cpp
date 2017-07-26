#include "MVars.hpp"
#include "XorStr.hpp"

const char *opt_Key[] = { "Off", "L Mouse", "R Mouse", "", "M Mouse", "Mouse 4", "Mouse 5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "", "", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
const char *opt_Key2[] = { "Off", "L Mouse", "R Mouse", "", "M Mouse", "Mouse 4", "Mouse 5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "", "", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
const char *opt_EspType[] = { "Off", "Box", "Corner" };
const char *opt_AimSpot[] = { "Head", "Neck", "Body", "Pelvis" };
const char *opt_AAxType[] = { "Off", "Static Up", "Static Down", "Up", "Down", "Fake Up", "Fake Down", "Fake Zero", "Dance", "Front" };
const char *opt_AAyType[] = { "Off", "Spinbot", "Jitter", "Side", "Backwards", "Forwards", "Left", "Right", "Static" };
const char *opt_TargetType[] = { "Fov", "Distance", "Threat" };
int realAimSpot[] = { 8, 7, 6, 0 };
int realHitboxSpot[] = { 0, 1, 2, 3};
int realFakeLagChoke[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
int realFakeLagSend[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };