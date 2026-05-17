/**
 * MS51_16K.h — SDCC-oriented SFR map for MS51FB9AE / MS51XB9AE / MS51XB9BE.
 *
 * Curated from Nuvoton TRM Rev 1.03 (not the Keil-only vendor BSP).
 * Additional board-specific aliases live in src/bsp/ms51/ms51_registers.h.
 */
#ifndef MS51_16K_H
#define MS51_16K_H

#include <stdint.h>

/* SDCC SFR syntax */
#ifndef __SDCC
#define __SDCC
#endif

__sfr __at(0x80) P0;
__sfr __at(0x90) P1;
__sfr __at(0xA0) P2;
__sfr __at(0xB0) P3;

__sfr __at(0x87) P0M1;
__sfr __at(0x86) P0M2;
__sfr __at(0x97) P1M1;
__sfr __at(0x96) P1M2;
__sfr __at(0xA7) P2M1;
__sfr __at(0xA6) P2M2;
__sfr __at(0xB7) P3M1;
__sfr __at(0xB6) P3M2;

__sfr __at(0xA8) IE;
__sfr __at(0xB8) IP;
__sfr __at(0xD8) EIE;
__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;
__sfr __at(0x8A) TL0;
__sfr __at(0x8C) TH0;

__sfr __at(0xE8) ADCCON0;
__sfr __at(0xE1) ADCCON1;
__sfr __at(0xE2) ADCCON2;
__sfr __at(0xC2) ADCRL;
__sfr __at(0xC3) ADCRH;
__sfr __at(0xE3) ADCDLY;

/* ADCCON0 bits */
#define ADCF    0x80
#define ADCS    0x40
#define ADCHS_MASK 0x0F

/* ADCCON1 bits */
#define ADCEN   0x01

#endif /* MS51_16K_H */
