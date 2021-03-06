#ifndef wm_crypto_h

#include <avr/pgmspace.h>
#include <string.h>


static volatile unsigned char wm_rand[10];
static volatile unsigned char wm_key[6];
static volatile unsigned char wm_ft[8];
static volatile unsigned char wm_sb[8];

const unsigned char ans_tbl[7][6] PROGMEM = {
	{0xA8,0x77,0xA6,0xE0,0xF7,0x43},
	{0x5A,0x35,0x85,0xE2,0x72,0x97},
	{0x8F,0xB7,0x1A,0x62,0x87,0x38},
	{ 0xD,0x67,0xC7,0xBE,0x4F,0x3E},
	{0x20,0x76,0x37,0x8F,0x68,0xB7},
	{0xA9,0x26,0x3F,0x2B,0x10,0xE3},
	{0x30,0x7E,0x90, 0xE,0x85, 0xA},
};

const unsigned char sboxes[10][256] PROGMEM = {
	{
		0x70,0x51,   3,0x86,0x40, 0xD,0x4F,0xEB,0x3E,0xCC,0xD1,0x87,0x35,0xBD,0xF5, 0xB,
		0x5E,0xD0,0xF8,0xF2,0xD5,0xE2,0x6C,0x31, 0xC,0xAD,0xFC,0x21,0xC3,0x78,0xC1,   6,
		0xC2,0x4C,0x55,0xE6,0x4A,0x34,0x48,0x11,0x1E,0xDA,0xE7,0x1A,0x84,0xA0,0x96,0xA7,
		0xE3,0x7F,0xAF,0x63,0x9C,0xFA,0x23,0x5B,0x79,0xC8,0x9E,0xBA,0xB2,0xC9,0x22,0x12,
		0x4B,0xB3,0xA1,0xB6,0x32,0x49,0xA2,0xE1,0x89,0x39,0x10,0x66,0xC5,   7,0x8F,0x54,
		0xEA,0x91,0xCA,0x3F,0xF9,0x19,0xF0,0xD7,0x46,0xBC,0x28,0x1B,0x61,0xE8,0x2F,0x6A,
		0xAE,0x9D,0xF6,0x4E,   9,0x14,0x77,0x4D,0xDB,0x1F,0x2E,0x7B,0x7C,0xF1,0x43,0xA3,
		   0,0xB8,0x13,0x8C,0x85,0xB9,0x29,0x75,0x88,0xFD,0xD2,0x56,0x1C,0x50,0x97,0x41,
		0xE5,0x3B,0x60,0xB5,0xC0,0x64,0xEE,0x98,0xD6,0x2D,0x25,0xA4,0xAA,0xCD,0x7D,0xA8,
		0x83,0xC6,0xAB,0xBE,0x44,0x99,0x26,0x3C,0xCE,0x9F,0xBF,0xD3,0xCB,0x76,0x7A,0x7E,
		0x82,   1,0x8A,0x9A,0x80,0x1D, 0xE,0xB0,0x5C,0xD4,0x38,0x62,0xF4,0x30,0xE0,0x8E,
		0x53,0xB7,   2,0x57,0xAC,0xA6,0x52, 0xA,0x6D,0x92,0x65,0x17,0x24,0x33,0x45,0x72,
		0x74,0xB1,0xB4,0xF7,0x5D,0xED,0x2C,0xFF,0x47,0x37,0x5A,0x90,0xBB,0xDF,0x2A,0x16,
		0x59,0x95,0xD9,0xC4,0x27,0x67,0x73,0xC7,0x68,0xFE,0xA5,0xDD,0x6B,0x5F,0x93,0xD8,
		0xEC,   5,0x3A,0x8D,0x6E,0xFB,0x3D,0xA9,0x69,0x36,0xF3,0x94,0xDE,0xEF,0x15,0x6F,
		0x8B,0x9B,   8, 0xF,0xDC,0x81,0x18,0x20,   4,0xE4,0x71,0xCF,0xE9,0x2B,0x42,0x58,
	},
	{
		   1,0xA0,0xA9,0x62,0xD6,0x3F,0x85,0xA7,0xB6,0xD4,0xFA,0x15,0x66,0x17,   9,0xBD,
		0x5D,0x14,0x34,0x26,0x59,0x72,0x91,0x54,   6,0x4F,0xF8,0xB0,0x5B,0x74,0x93,0x99,
		0x8C,0xF2,0x45,0xCD,0xEA,0x4E,0xAD,0x10,0x4A,0xE5,0xCA,0xEE,0xDF,0xC6,0x6F,0x9F,
		0x88,0x8E,   2,0xCC,   8,0xA8,0x77,0x94,0x6D,0x21,0xB1,0x28,0xE4,0x39,0x79,0x96,
		0x60,0x71,0x81,0x16,0x2E,0xE6,0x78,0xB9,0xC4,0x46,0x9A,0x42,0xAE,0xB7,0x7C,0x43,
		0xB3,0x22,0x1A,0x86,0xC2,0x32,0x3D,0x2D,0x9C,0xD2,0x29,0xE9,0x63,0x9B,0xD1,0x31,
		0x38,0x5E,0x1E,0x36,0x41,0xBB,   3,0x18,0x2B,0x3E,0xBF,0x68,0x61,0xFC,0x52,0xC0,
		0xDE,0xE0, 0xA,0x58,0x13,0x5A,   0,0xBE,0x1C,0x90, 0xE,0x53,0x12,0xFD,0xE2,0x6E,
		0xBA,0xCE,0x24,0x27,0x44,0x7F,0x87,0xA3,0xA1,0xD5,0x50,0x40,0xE3,0xF9,0x83,0xF7,
		0xC7,0xA2,0x35,0xC8,0xDB,0x19,0xAB,0x2F,0x11,0x25,0xED,0x33,0x9E,0x55,0xE1,0x48,
		0xAF,0x73,0x84,0xDA,0x2A,0xAA,0x51,0xEB,0x9D,0x95,0xB2,0xCB,0xE7,0x70,0x80,0xFE,
		0x4C,0x65,   4,0xEF,0xC5,0xF1,0xC3,0x3A,0xB4,0xF5,0x5F,0x23,0x89,0xDD,0x30,0xA5,
		0x8B,0xD3,0xF6,0xDC,0x4D,0x64,0xD7,0xF0,0x8F,0xEC,0x56,0x37,0x5C,0xA4, 0xD,   7,
		0x76,0x8A,0x2C, 0xB,0xB5,0xD8,0xC1,0x1F,0xE8,0x3B,0xF4,0x4B,0x1B,0x47,0x6C,0x49,
		0x67,0x7B,0x92,0xCF,0x75,0x7E,0x20,0xD9,0x7D,0x3C,0x97,0x7A,0xD0,   5,0x6B, 0xF,
		0x1D,0xFB,0x82,0x98,0x57,0x8D,0xF3,0x6A,0xBC,0xAC,0xC9,0xA6,0xFF,0xB8,0x69, 0xC,
	},
	{
		0x4C,0x4D,0x72,   7,0x5A,0x49,0x33,0x8D,0xA2,0xAB,0x46,0x3D,0x63, 0xD,0xA0,0x97,
		0xFF,0xF0,0xF5,0xFA,0xC0,0xE9,0xDB,0x62,0xE4,0xE1,0x74,0x43,0xDC,0x86,0x18,0x29,
		0x37,0xF4,   6,0xE2,0xED,0x6F,0x90,0x48,0x1E,0x2D,0x1D,0xEA,0x73,0x94,0x54,0xDF,
		0x25,0xF6,0x47,0x27,0xD9,0x11,0x77,0xC9,0x84,0x1C,0x5B,0x5C,0x51,0x81,0xA6,0x22,
		0x3E,0x24,0x96,0xC8,0x8A,0xEC,0x82,0x7C,   9,0xB8,0x45,0x4A,0x57,0xBB,0x2F,0x50,
		0x75,0x8E,0x61,0x70,0x8C,0x6C,0xAF,0xD0,0xFD,0xB4,0x1B,0xAE,0xDE,0xFE,0x3B,0xB5,
		0x36,0xBD,0x55,   1, 0xE,0x9C,0x41,0x56,0x5F,0xB3,0x26,   3,0x83,0xBA,0x13,0x4B,
		0xCA,0xC5, 0xA,0xF8,0x60,0xA5,0xB9,0xC7,0xC3,0x98,0x32,0xFB,0x12,0xF9,0xA7,0x92,
		0xAA,0x68,0xF3,0x78,0x7E,   5,0x20,0x21,   2,0xE8,0xBF,0xF2,0xB0,0x59,0x8F,0xD2,
		0xCB,0x87,0x65,0x15,0xF1,0x1A,0xB2,0x30,0xAD,0xEE,0x58,0xA3,0x8B,0x66,0x1F,0x2C,
		0xD7,0x5D,0x19,0x85,0xA8,0xE6,0xD3,0x6B,0xA1, 0xC,0x91,0x93,0x6A,0x5E, 0xB,0x79,
		0xE3,0xDD,   0,0x4F,0x3C,0x89,0x6E,0x71,0x69,0xA9,0xAC,0x40,0xE5,0x99,0x28,0xC6,
		0x31,0x4E,0x7A,0xCD,   8,0x9E,0x7D,0xEF,0x17,0xFC,0x88,0xD8,0xA4,0x6D,0x44,0x95,
		0xD1,0xB7,0xD4,0x9B,0xBE,0x2A,0x34,0x64,0x2B,0xCF,0x2E,0xEB,0x38,0xCE,0x23,0xE0,
		0x3A,0x3F,0xF7,0x7B,0x9F,0x10,0x53,0xBC,0x52,0x67,0x16,0xE7,0x80,0x76,   4,0xC4,
		0xB6,0xC1,0xC2,0x7F,0x9A,0xDA,0xD5,0x39,0x42,0x14,0x9D,0xB1, 0xF,0x35,0xD6,0xCC,
	},
	{
		0xB9,0xDA,0x38, 0xC,0xA2,0x9C,   9,0x1F,   6,0xB1,0xB6,0xFD,0x1A,0x69,0x23,0x30,
		0xC4,0xDE,   1,0xD1,0xF4,0x58,0x29,0x37,0x1C,0x7D,0xD5,0xBF,0xFF,0xBD,0xC8,0xC9,
		0xCF,0x65,0xBE,0x7B,0x78,0x97,0x98,0x67,   8,0xB3,0x26,0x57,0xF7,0xFA,0x40,0xAD,
		0x8E,0x75,0xA6,0x7C,0xDB,0x91,0x8B,0x51,0x99,0xD4,0x17,0x7A,0x90,0x8D,0xCE,0x63,
		0xCB,0x4E,0xA0,0xAB,0x18,0x3A,0x5B,0x50,0x7F,0x21,0x74,0xC1,0xBB,0xB8,0xB7,0xBA,
		 0xB,0x35,0x95,0x31,0x59,0x9A,0x4D,   4,   7,0x1E,0x5A,0x76,0x13,0xF3,0x71,0x83,
		0xD0,0x86,   3,0xA8,0x39,0x42,0xAA,0x28,0xE6,0xE4,0xD8,0x5D,0xD3,0xD0,0x6E,0x6F,
		0x96,0xFB,0x5E,0xBC,0x56,0xC2,0x5F,0x85,0x9B,0xE7,0xAF,0xD2,0x3B,0x84,0x6A,0xA7,
		0x53,0xC5,0x44,0x49,0xA5,0xF9,0x36,0x72,0x3D,0x2C,0xD9,0x1B,0xA1,0xF5,0x4F,0x93,
		0x9D,0x68,0x47,0x41,0x16,0xCA,0x2A,0x4C,0xA3,0x87,0xD6,0xE5,0x19,0x2E,0x77,0x15,
		0x6D,0x70,0xC0,0xDF,0xB2,   0,0x46,0xED,0xC6,0x6C,0x43,0x60,0x92,0x2D,0xA9,0x22,
		0x45,0x8F,0x34,0x55,0xAE,0xA4, 0xA,0x66,0x32,0xE0,0xDC,   2,0xAC,0xE8,0x20,0x8C,
		0x89,0x62,0x4A,0xFE,0xEE,0xC3,0xE3,0x3C,0xF1,0x79,   5,0xE9,0xF6,0x27,0x33,0xCC,
		0xF2,0x9E,0x11,0x81,0x7E,0x80,0x10,0x8A,0x82,0x9F,0x48, 0xD,0xD7,0xB4,0xFC,0x2F,
		0xB5,0xC7,0xDD,0x88,0x14,0x6B,0x2B,0x54,0xEA,0x1D,0x94,0x5C,0xB0,0xEF,0x12,0x24,
		0xCD,0xEB,0xE1,0xE2,0x64,0x73,0x3F, 0xE,0x52,0x61,0x25,0x3E,0xF8, 0xF,0x4B,0xEC,
	},
	{
		0xC0,   0,0x30,0xF6,   2,0x49,0x3D,0x10,0x6E,0x20,0xC9,0xA6,0x2F,0xFE,0x2C,0x2B,
		0x75,0x2E,0x45,0x26,0xAB,0x48,0xA9,0x80,0xFC,   4,0xCC,0xD3,0xB5,0xBA,0xA3,0x38,
		0x31,0x7D,   1,0xD9,0xA7,0x7B,0x96,0xB6,0x63,0x69,0x4E,0xF7,0xDE,0xE0,0x78,0xCA,
		0x50,0xAA,0x41,0x91,0x65,0x88,0xE4,0x21,0x85,0xDA,0x3A,0x27,0xBE,0x1C,0x3E,0x42,
		0x5E,0x17,0x52,0x7F,0x1F,0x89,0x24,0x6F,0x8F,0x5C,0x67,0x74, 0xE,0x12,0x87,0x8D,
		0xE9,0x34,0xED,0x73,0xC4,0xF8,0x61,0x5B,   5,0xDF,0x59,0x4C,0x97,0x79,0x83,0x18,
		0xA4,0x55,0x95,0xEB,0xBD,0x53,0xF5,0xF1,0x57,0x66,0x46,0x9F,0xB2,0x81,   9,0x51,
		0x86,0x22,0x16,0xDD,0x23,0x93,0x76,0x29,0xC2,0xD7,0x1D,0xD4,0xBF,0x36,0x3F,0xEA,
		0x4B,0x11,0x32,0xB9,0x62,0x54,0x60,0xD6,0x6D,0x43,0x9A, 0xD,0x92,0x9C,0xB0,0xEF,
		0x58,0x6C,0x9D,0x77,0x2D,0x70,0xFA,0xF3,0xB3, 0xB,0xE2,0x40,0x7E,0xF4,0x8A,0xE5,
		0x8C,0x3C,0x56,0x71,0xD1,0x64,0xE1,0x82, 0xA,0xCB,0x13,0x15,0x90,0xEC,   3,0x99,
		0xAF,0x14,0x5D, 0xF,0x33,0x4A,0x94,0xA5,0xA8,0x35,0x1B,0xE3,0x6A,0xC6,0x28,0xFF,
		0x4D,0xE7,0x25,0x84,0xAC,   8,0xAE,0xC5,0xA2,0x2A,0xB8,0x37, 0xC,0x7A,0xA0,0xC3,
		0xCE,0xAD,   6,0x1A,0x9E,0x8B,0xFB,0xD5,0xD0,0xC1,0x1E,0xD0,0xB4,0x9B,0xB1,0x44,
		0xF2,0x47,0xC7,0x68,0xCF,0x72,0xBB,0x4F,0x5A,0xF9,0xDC,0x6B,0xDB,0xD2,0xE8,0x7C,
		0xC8,0xEE,0x98,0xA1,0xE6,0xD8,0x39,   7,0x5F,0xFD,0x8E,0x19,0xB7,0x3B,0xBC,0xCD,
	},
	{
		0x7C,0xE3,0x81,0x73,0xB2,0x11,0xBF,0x6F,0x20,0x98,0xFE,0x75,0x96,0xEF,0x6C,0xDA,
		0x50,0xE1,   9,0x72,0x54,0x45,0xBA,0x34,0x80,0x5B,0xED,0x3E,0x53,0x2C,0x87,0xA4,
		0x57,0xF3,0x33,0x3F,0x3C,0xB7,0x67,0xB4,0xA3,0x25,0x60,0x4F,   7,0x6B,0x1B,0x47,
		0x15, 0xF,0xE4, 0xA,0xEA,0xD1,0x32,0x78,0x36,0x49,0x8D,0x4B,0xD2,0xBC,0xA5,0xDC,
		0x1D, 0xD,0x4D,0xCD,0x9A,0x82,0x5F,0xFC,0x94,0x65,0xBE,0xE2,0xF4,0xC9,0x1E,0x44,
		0xCB,0x9E, 0xC,0x64,0x71,0x26,0x63,0xB3,0x14,0xE8,0x40,0x70,0x8A, 0xE,0x19,0x42,
		0x6D,0xAC,0x88,0x10,0x5C,0xDF,0x41,0xA9,0xAD,0xE5,0xFB,0x74,0xCC,0xD5,   6,0x8E,
		0x59,0x86,0xCE,0x1F,0x3D,0x76,0xE0,0x8F,0xB9,0x77,0x27,0x7B,0xA6,0xD8,0x29,0xD3,
		0xEC,0xB8,0x13,0xF7,0xFA,0xC3,0x51,0x6A,0xDE,0x4A,0x5A,0xEB,0xC2,0x8B,0x23,0x48,
		0x92,0xCF,0x62,0xA8,0x99,0xF8,0xD0,0x2E,0x85,0x61,0x43,0xC8,0xBD,0xF0,   5,0x93,
		0xCA,0x4E,0xF1,0x7D,0x30,0xFD,0xC4,0x69,0x66,0x2F,   8,0xB1,0x52,0xF9,0x21,0xE6,
		0x7A,0x2B,0xDD,0x39,0x84,0xFF,0xC0,0x91,0xD6,0x37,0xD4,0x7F,0x2D,0x9B,0x5D,0xA1,
		0x3B,0x6E,0xB5,0xC5,0x46,   4,0xF5,0x90,0xEE,0x7E,0x83,0x1C,   3,0x56,0xB6,0xAA,
		   0,0x17,   1,0x35,0x55,0x79, 0xB,0x12,0xBB,0x1A,0x31,0xE7,   2,0x28,0x16,0xC1,
		0xF6,0xA2,0xDB,0x18,0x9C,0x89,0x68,0x38,0x97,0xAB,0xC7,0x2A,0xD7,0x3A,0xF2,0xC6,
		0x24,0x4C,0xB0,0x58,0xA0,0x22,0x5E,0x9D,0xD9,0xA7,0xE9,0xAE,0xAF,0x8C,0x95,0x9F,
	},
	{
		0x28,0xB7,0x20,0xD7,0xB0,0x30,0xC3,   9,0x19,0xC0,0x67,0xD6,   0,0x3C,0x7E,0xE7,
		0xE9,0xF4,   8,0x5A,0xF8,0xB8,0x2E,   5,0xA6,0x25,0x9E,0x5C,0xD8,0x15, 0xD,0xE1,
		0xF6,0x11,0x54,0x6B,0xCD,0x21,0x46,0x66,0x5E,0x84,0xAD,   6,0x38,0x29,0x44,0xC5,
		0xA2,0xCE,0xF1,0xAA,0xC1,0x40,0x71,0x86,0xB5,0xEF,0xFC,0x36,0xA8,0xCB, 0xA,0x48,
		0x27,0x45,0x64,0xA3,0xAF,0x8C,0xB2,0xC6,0x9F,   7,0x89,0xDC,0x17,0xD3,0x49,0x79,
		0xFB,0xFE,0x1D,0xD0,0xB9,0x88,0x43,0x52,0xBC,   1,0x78,0x2B,0x7D,0x94,0xC7, 0xE,
		0xDE,0xA5,0xD5,0x9B,0xCC,0xF7,0x61,0x7A,0xC2,0x74,0x81,0x39,   3,0xAB,0x96,0xA0,
		0x37,0xBD,0x2D,0x72,0x75,0x3F,0xC9,0xD4,0x8E,0x6F,0xF9,0x8D,0xED,0x62,0xDB,0x1C,
		0xDF,   4,0xAC,0x1B,0x6C,0x14,0x4B,0x63,0xD0,0xBF,0xB4,0x82,0xEC,0x7B,0x1A,0x59,
		0x92,0xD2,0x10,0x60,0xB6,0x3D,0x5F,0xE6,0x80,0x6E,0x70,0xC4,0xF2,0x35,0xD9,0x7C,
		0xEE,0xE5,0x41,0xA4,0x5B,0x50,0xDD,0xBB,0x4C,0xF3,0x1F,0x9D,0x5D,0x57,0x55,0x51,
		0x97,0xE3,0x58,0x42,0x4D,0x9C,0x73,0xBA,0xC8,0x77,0x31,0x69,0x26,0xAE,0xEA,0x8A,
		0xDA,0x22,0xB3,0x87,0x56,0xFA,0x93, 0xB,0x34,0x16,0x33,0xE8,0xE4,0x53,0xBE,0xA9,
		0xB1,0x3A,0x3E,0xF5,0x90,0x6A,0xCF,0x3B,0x12,0xFD,0x8F,0x9A,0xA7,0x47,0x91,0x99,
		0xEB, 0xF,0x24,0xFF,0x23,0x18,0x85,0x4E,0x7F, 0xC,0xE0,0xA1,0xD2,0xD1,0x2C,0x2A,
		0x4A,   2,0x4F,0x1E,0x95,0x68,0x8B,0x98,0x83,0x6D,0x76,0xCA,0x65,0x32,0x13,0x2F,
	},
	{
		0xC3,0x82,0x9A,0xA4,0xBA,0x81,0x60,0x37,0x34,0x35,0xFC,0x80,0xA8,0x51,0x65,0x67,
		0xED,0x30,0x5F,0x10,0xD3,0x4A,0x27,0x2F,0x13,0xB9,0x2A,0xD2,0xCC,0xE1,0xEF,0xAE,
		0xEB,0xBE,0xF4,0xBD,0xCF,0x43,0xB3,0xC5,0x88,0x84,0xB7,0xDD,0x39,0x40,0xCE,0x48,
		0x6D,0x9B,0x72,0x61,0x7E,0xE7,0xA1,0x4E,0x53,0x2E,0x77,0x3B,0xE2,0xC9,0x36,0x22,
		0x1B,0x6E,0x73,0xB1,   3,0xB2,0x4C,0x87,0xA9,0xD4,0x4D, 0xF,0xD8,0x15,0x6C,0xAA,
		0x18,0xF6,0x49,0x57,0x5D,0xFB,0x7A,0x14,0x94,0x63,0xA0,0x11,0xB0,0x9E,0xDE,   5,
		0x46,0xC8,0xEE,0x47,0xDB,0xDC,0x24,0x89,0x9C,0x91,0x97,0x29,0xE9,0x7B,0xC1,   7,
		0x1E,0xB8,0xFD,0xFE,0xAC,0xC6,0x62,0x98,0x4F,0xF1,0x79,0xE0,0xE8,0x6B,0x78,0x56,
		0xB6,0x8D,   4,0x50,0x86,0xCA,0x6F,0x20,0xE6,0xEA,0xE5,0x76,0x17,0x1C,0x74,0x7F,
		0xBC, 0xD,0x2C,0x85,0xF7,0x66,0x96,0xE4,0x8B,0x75,0x3F,0x4B,0xD9,0x38,0xAF,0x7C,
		0xDA, 0xB,0x83,0x2D,0x31,0x32,0xA2,0xF5,0x1D,0x59,0x41,0x45,0xBF,0x3C,0x1F,0xF8,
		0xF9,0x8A,0xD0,0x16,0x25,0x69,0x12,0x99,0x9D,0x21,0x95,0xAB,   1,0xA6,0xD7,0xB5,
		0xC0,0x7D,0xFF,0x58, 0xE,0x3A,0x92,0xD1,0x55,0xE3,   8,0x9F,0xD6,0x3E,0x52,0x8E,
		0xFA,0xA3,0xC7,   2,0xCD,0xDF,0x8F,0x64,0x19,0x8C,0xF3,0xA7, 0xC,0x5E, 0xA,0x6A,
		   9,0xF0,0x93,0x5B,0x42,0xC2,   6,0x23,0xEC,0x71,0xAD,0xB4,0xCB,0xBB,0x70,0x28,
		0xD5,0x1A,0x5C,0x33,0x68,0x5A,   0,0x44,0x90,0xA5,0xC4,0x26,0x3D,0x2B,0xF2,0x54,
	},
	{
		0x96,0xAD,0xDA,0x1F,0xED,0x33,0xE1,0x81,0x69,   8, 0xD, 0xA,0xDB,0x35,0x77,0x9A,
		0x64,0xD1,0xFC,0x78,0xAA,0x1B,0xD0,0x67,0xA0,0xDD,0xFA,0x6C,0x63,0x71,   5,0x84,
		0x17,0x6A,0x89,0x4F,0x66,0x7F,0xC6,0x50,0x55,0x92,0x6F,0xBD,0xE7,0xD2,0x40,0x72,
		0x8D,0xBB,0xEC,   6,0x42,0x8A,0xE4,0x88,0x9D,0x7E,0x7A,0x82,0x27,0x13,0x41,0x1A,
		0xAF,0xC8,0xA4,0x76,0xB4,0xC2,0xFE,0x6D,0x1C,0xD9,0x61,0x30,0xB3,0x7C,0xEA,0xF7,
		0x29, 0xF,0xF2,0x3B,0x51,0xC1,0xDE,0x5F,0xE5,0x2A,0x2F,0x99, 0xB,0x5D,0xA3,0x2B,
		0x4A,0xAB,0x95,0xA5,0xD3,0x58,0x56,0xEE,0x28,0x31,   0,0xCC,0x15,0x46,0xCA,0xE6,
		0x86,0x38,0x3C,0x65,0xF5,0xE3,0x9F,0xD6,0x5B,   9,0x49,0x83,0x70,0x2D,0x53,0xA9,
		0x7D,0xE2,0xC4,0xAC,0x8E,0x5E,0xB8,0x25,0xF4,0xB9,0x57,0xF3,0xF1,0x68,0x47,0xB2,
		0xA2,0x59,0x20,0xCE,0x34,0x79,0x5C,0x90, 0xE,0x1E,0xBE,0xD5,0x22,0x23,0xB1,0xC9,
		0x18,0x62,0x16,0x2E,0x91,0x3E,   7,0x8F,0xD8,0x3F,0x93,0x3D,0xD4,0x9B,0xDF,0x85,
		0x21,0xFB,0x11,0x74,0x97,0xC7,0xD7,0xDC,0x4C,0x19,0x45,0x98,0xE9,0x43,   2,0x4B,
		0xBC,0xC3,   4,0x9C,0x6B,0xF0,0x75,0x52,0xA7,0x26,0xF6,0xC5,0xBA,0xCF,0xB0,0xB7,
		0xAE,0x5A,0xA1,0xBF,   3,0x8B,0x80,0x12,0x6E, 0xC,0xEB,0xF9,0xC0,0x44,0x24,0xEF,
		0x10,0xF8,0xA8,0x8C,0xE8,0x7B,0xFF,0x9E,0x2C,0xCD,0x60,0x36,0x87,0xB5,0x94,0xA6,
		0x54,0x73,0x3A,0x14,0x4E,   1,0x1D,0xB6,0xFD,0x37,0x48,0x4D,0x39,0xCB,0xE0,0x32,
	}
};

unsigned char wm_ror8(unsigned char a, unsigned char b)
{
	// bit shift with roll-over
	return (a >> b) | ((a << (8 - b)) & 0xFF);
}

void wm_gentabs()
{
	unsigned char idx;

	// check all idx
	for(idx = 0; idx < 7; idx++)
	{
		// generate test key
		unsigned char ans[6];
		unsigned char tkey[6];
		unsigned char t0[10];

		for(unsigned char i = 0; i < 6; i++)
		{
			ans[i] = pgm_read_byte(&(ans_tbl[idx][i]));
		}
		for(unsigned char i = 0; i < 10; i++)
		{
			t0[i] = pgm_read_byte(&(sboxes[0][wm_rand[i]]));
		}

		tkey[0] = ((wm_ror8((ans[0] ^ t0[5]), (t0[2] % 8)) - t0[9]) ^ t0[4]);
		tkey[1] = ((wm_ror8((ans[1] ^ t0[1]), (t0[0] % 8)) - t0[5]) ^ t0[7]);
		tkey[2] = ((wm_ror8((ans[2] ^ t0[6]), (t0[8] % 8)) - t0[2]) ^ t0[0]);
		tkey[3] = ((wm_ror8((ans[3] ^ t0[4]), (t0[7] % 8)) - t0[3]) ^ t0[2]);
		tkey[4] = ((wm_ror8((ans[4] ^ t0[1]), (t0[6] % 8)) - t0[3]) ^ t0[4]);
		tkey[5] = ((wm_ror8((ans[5] ^ t0[7]), (t0[8] % 8)) - t0[5]) ^ t0[9]);

		// compare with actual key
		if(memcmp(tkey, (void *) wm_key, 6) == 0) break; // if match, then use this idx
	}

	// generate encryption from idx key and rand
	wm_ft[0] = pgm_read_byte(&(sboxes[idx + 1][wm_key[4]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[3]]));
	wm_ft[1] = pgm_read_byte(&(sboxes[idx + 1][wm_key[2]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[5]]));
	wm_ft[2] = pgm_read_byte(&(sboxes[idx + 1][wm_key[5]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[7]]));
	wm_ft[3] = pgm_read_byte(&(sboxes[idx + 1][wm_key[0]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[2]]));
	wm_ft[4] = pgm_read_byte(&(sboxes[idx + 1][wm_key[1]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[4]]));
	wm_ft[5] = pgm_read_byte(&(sboxes[idx + 1][wm_key[3]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[9]]));
	wm_ft[6] = pgm_read_byte(&(sboxes[idx + 1][wm_rand[0]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[6]]));
	wm_ft[7] = pgm_read_byte(&(sboxes[idx + 1][wm_rand[1]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[8]]));

	wm_sb[0] = pgm_read_byte(&(sboxes[idx + 1][wm_key[0]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[1]]));
	wm_sb[1] = pgm_read_byte(&(sboxes[idx + 1][wm_key[5]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[4]]));
	wm_sb[2] = pgm_read_byte(&(sboxes[idx + 1][wm_key[3]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[0]]));
	wm_sb[3] = pgm_read_byte(&(sboxes[idx + 1][wm_key[2]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[9]]));
	wm_sb[4] = pgm_read_byte(&(sboxes[idx + 1][wm_key[4]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[7]]));
	wm_sb[5] = pgm_read_byte(&(sboxes[idx + 1][wm_key[1]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[8]]));
	wm_sb[6] = pgm_read_byte(&(sboxes[idx + 1][wm_rand[3]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[5]]));
	wm_sb[7] = pgm_read_byte(&(sboxes[idx + 1][wm_rand[2]])) ^ pgm_read_byte(&(sboxes[idx + 2][wm_rand[6]]));
}


#define wm_crypto_h
#endif

/*
 * Copyright (c) 2011 Peter Brinkmann (peter.brinkmann@gmail.com)
 *
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * Bare-bones Nintendo Wiimote library for Arduino, tricking the Wiimote into
 * treating the Arduino as a Classic Controller.  Communication works both to and from
 * the Arduino.
 *
 * Using insights into the communication between Wiimote and Nunchuk found
 * at http://www.ccsinfo.com/forum/viewtopic.php?p=91094.  The wmgui tool that
 * comes with cwiid is good for experimenting and testing.
 *
 * Here's how to connect the Arduino to the Wiimote:
 *      Arduino    Wiimote
 *      GND        GND  (white cable)
 *      AREF       3.3V (red cable)
 *      A4         SDA  (green cable)
 *      A5         SCL  (yellow cable)
 *
 * It doesn't look like it's possible to power the Arduino from the Wiimote.
 *
 * Note that some online instructions for using a Wiimote extension
 * with Arduino call for changes to twi.h.
 * Fortunately, this modification is no longer
 * necessary.  Just use a current version of the Arduino development
 * environment.
 */

#ifndef __WIIMOTE_H__
#define __WIIMOTE_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #else
  #include "WProgram.h"
  #endif
#include <Wire.h>


// Identification sequence for Classic Controller
static byte idbuf[] = { 0x00, 0x00, 0xa4, 0x20, 0x01, 0x01 };

// Classic Controller calibration data
byte calbuf[] = { 0xFC, 0x04, 0x7E, 0xFC, 0x04, 0x7E, 0xFC, 0x04, 0x7E,
		0xFC, 0x04, 0x7E, 0x00, 0x00, 0x00, 0x00 }; // Checksum will be calculated later...

//byte calbuf[] = { 0xff, 0x00, 0x80, 0xff, 0x00, 0x80, 0xff, 0x00, 0x80, 0xff,
//		0x00, 0x80, 0x00, 0x00, 0x51, 0xa6 }; // Homebrews (libogc) don't seem to like this one...

static byte outbuf[6];
static byte *curbuf = outbuf;
static byte state = 0;
static byte crypt_setup_done = 0;

/*
 * This function pointer informs the Arduino that a byte has been
 * written to one of the registers corresponding to the Extension.
 * Specifically, if you assign the function foo to this pointer and
 * you write yy bytes starting at the register address 04a400xx of
 * the Wiimote, then this library will call foo(xx, yy).  The bytes
 * are written to the global array wiimote_registers.
 *
 * Note that count will always be less than eight.  If you write
 * eight or more bytes to the Wiimote, this callback will be
 * invoked several times.
 */
void (*wiimote_receive)(byte offset, byte count) = NULL;

/*
 * Callback for streaming bytes from the Arduino to the Wiimote.
 * If this function pointer is not null, then it will be called
 * right _after_ the current output buffer has been sent to the
 * Wiimote, and the implementing function has the choice of either
 * overwriting the given buffer and returning it or returning
 * another buffer that contains the next six bytes to be sent.
 *
 * The return value of this function becomes the new output buffer.
 * The idea is much like the one behind wiimote_swap_buffers.  You
 * probably want to use only one of them; either wiimote_stream
 * for streaming data, or wiimote_swap_buffers for more sporadic
 * updates.
 *
 * There is one exception to this rule: Since this function is
 * called after the current output buffer has been sent, the very
 * first buffer will be all zero unless you initialize it with
 * wiimote_set_byte or wiimote_swap_buffers.  (This approach may
 * seem awkward, but it minimizes delays in I2C communication, and
 * in most cases an all-zero initial buffer is not a problem.)
 */
byte *(*wiimote_stream)(byte *buffer) = NULL;

/*
 * Global array for storing and accessing register values written
 * by the Wiimote.  Those are the 256 registers corresponding to
 * the Extension, starting at 04a40000.
 */
byte wiimote_registers[0x100];

/*
 * Start Wiimote <-> Extension communication encryption,
 * if requested by the application.
 */

static void setup_encryption() {
	int i;

	for (i = 0x40; i <= 0x49; i++) {
		wm_rand[9 - (i - 0x40)] = wiimote_registers[i];
	}

	for (i = 0x4A; i <= 0x4F; i++) {
		wm_key[5 - (i - 0x4A)] = wiimote_registers[i];
	}

	wm_gentabs();

	crypt_setup_done = 1;
}

/*
 * Generic function for sending data via Wire.write().
 * Supports Wiimote encryption, if enabled.
 */
static void send_data(uint8_t* data, uint8_t size, uint8_t addr) {
	static uint8_t buffer[8];
	int i;

	if (wiimote_registers[0xF0] == 0xAA && crypt_setup_done) {
		for (i = 0; i < size; i++) {
			buffer[i] = (data[i] - wm_ft[(addr + i) % 8]) ^ wm_sb[(addr + i)
					% 8];
		}

		Wire.write(buffer, size);
	} else {
		Wire.write(data, size);
	}
}

static void receive_bytes(int count) {
	byte crypt_keys_received = 0;

	if (count == 1) {
		state = Wire.read();
	} else if (count > 1) {
		byte addr = Wire.read();
		byte curr = addr;

		for (int i = 1; i < count; i++) {
			byte d = Wire.read();

			// Wii is trying to disable encryption...
			if(addr == 0xF0 && d == 0x55) {
				crypt_setup_done = 0;
			}

			if (wiimote_registers[0xF0] == 0xAA && crypt_setup_done) {
				// Decrypt
				wiimote_registers[curr] = (d ^ wm_sb[curr % 8]) + wm_ft[curr
						% 8];
				curr++;
			} else {
				wiimote_registers[curr++] = d;
			}

			// Check if last crypt key setup byte was received...
			if (curr == 0x50) {
				crypt_keys_received = 1;
			}

		}
		if (wiimote_receive) {
			wiimote_receive(addr, (byte) count - 1);
		}
	}

	// Setup encryption
	if (crypt_keys_received) {
		setup_encryption();
	}

}

static void handle_request() {

	static byte last_state = 0xFF;
	static byte offset = 0;

	switch (state) {

	case 0x00:
		send_data(curbuf, 6, 0x00);

		if (wiimote_stream) {
			curbuf = wiimote_stream(curbuf);
		}

		break;

	case 0xFA:
		send_data(wiimote_registers + state, 6, state);

		break;

	default:
		if(last_state == state) {
			offset += 8;
		} else {
			last_state = state;
			offset = 0;
		}

		send_data(wiimote_registers + state + offset, 8, state + offset);
		break;
	}
}

/*
 * Sets the given value of the current buffer.
 *
 * index of the value to be set, 0 <= index < 6
 * value new value of the byte at the given index
 *
 * Make sure to read the documentation of wiimote_stream if you
 * intend to use both wiimote_stream and this function.
 */
void wiimote_set_byte(int index, byte value) {
	curbuf[index] = value;
}

/*
 * Makes the given buffer the current buffer and returns the
 * previous buffer.  Use this function if you want to change
 * several values at once.
 *
 * buffer must be a byte buffer of length at least 6.
 *
 * Returns the previous buffer, suitable for reuse.
 *
 * Make sure to read the documentation of wiimote_stream if you
 * intend to use both wiimote_stream and this function.
 */
byte *wiimote_swap_buffers(byte *buffer) {
	byte *tmp = curbuf;
	curbuf = buffer;
	return tmp;
}

/*
 * Takes joystick, and button values and encodes them
 * into a buffer.
 *
 * Classic Controller
 *
 * Buffer encoding details:
 * http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
 */
void wiimote_write_buffer(byte *buffer, int bdl, int bdr, int bdu, int bdd,
		int ba, int bb, int bx, int by, int blt, int brt, int bminus, int bplus,
		int bhome, byte lx, byte ly, byte rx, byte ry, int bzl, int bzr) {

	buffer[0] = ((rx & 0x18) << 3) | (lx & 0x3F);
	buffer[1] = ((rx & 0x06) << 5) | (ly & 0x3F);
	buffer[2] = ((rx & 0x01) << 7) | (ry & 0x1F);
	buffer[3] = 0x00;

	buffer[4] = ((bdr ? 1 : 0) << 7) | ((bdd ? 1 : 0) << 6) | ((blt ? 1 : 0)
			<< 5) | ((bminus ? 1 : 0) << 4) | ((bplus ? 1 : 0) << 2)
			| ((brt ? 1 : 0) << 1) | ((bhome ? 1 : 0) << 3);

	buffer[5] = ((bb ? 1 : 0) << 6) | ((by ? 1 : 0) << 5) | ((ba ? 1 : 0)
			<< 4) | ((bx ? 1 : 0) << 3) | ((bdl ? 1 : 0) << 1) | (bdu ? 1
			: 0) | ((bzl ? 1 : 0) << 7) | ((bzr ? 1 : 0) << 2);

	buffer[4] = ~buffer[4];
	buffer[5] = ~buffer[5];
}


/*
 * Initializes Wiimote connection. Call this function in your
 * setup function.
 */
void wiimote_init() {
	byte calchecksum = 0;

	memset(wiimote_registers, 0xFF, 0x100);

	// Set extension id on registers
	for (int i = 0xFA; i <= 0xFF; i++) {
		wiimote_registers[i] = idbuf[i - 0xFA];
	}

	// Fix calibration data checksum, just in case...
	for(int i = 0; i < 14; i++) {
		calchecksum += calbuf[i];
	}
	calbuf[14] = calchecksum + 0x55;
	calbuf[15] = calchecksum + 0xAA;

	// Set calibration data on registers
	for (int i = 0x20; i <= 0x2F; i++) {
		wiimote_registers[i] = calbuf[i - 0x20]; // 0x20
		wiimote_registers[i + 0x10] = calbuf[i - 0x20]; // 0x30
	}

	// Initialize curbuf, otherwise, "Up+Right locked" bug...
	wiimote_write_buffer(curbuf, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, calbuf[2]>>2, calbuf[5]>>2, calbuf[8]>>3, calbuf[11]>>3, 0, 0);

	// Encryption disabled by default
	wiimote_registers[0xF0] = 0x55;
	wiimote_registers[0xFB] = 0x00;

	// Join I2C bus
	Wire.begin(0x52);

	Wire.onReceive(receive_bytes);
	Wire.onRequest(handle_request);
}
#endif
/*
 * Sample sketch which makes the Arduino to impersonate a Classic Controller.
 *
 * Use a PushButton connected to the digital pin 2 and GND. It will trigger
 * a D-Pad RIGHT button press of the impersonated Classic Controller.
 *
 * Copyright (c) 2011 Peter Brinkmann (peter.brinkmann@gmail.com)
 *
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include <Wire.h> // This seems redundant, but we need to declare this
                  // dependency in the pde file or else it won't be included
                  // in the build.


// Classic Controller Buttons
int bdl = 0; // D-Pad Left state
int bdr = 0; // D-Pad Right state
int bdu = 0; // D-Pad Up state
int bdd = 0; // D-Pad Down state
int ba = 0; // A button state
int bb = 0; // B button state
int bx = 0; // X button state
int by = 0; // Y button state
int bl = 0; // L button state
int br = 0; // R button state
int bm = 0; // MINUS button state
int bp = 0; // PLUS button state
int bhome = 0; // HOME button state
int bzl = 0; // ZL button state
int bzr = 0; // ZR button state

/*
 * Analog Buttons.
 * They are initialized with center values from the calibration buffer.
 */
byte lx = calbuf[2]>>2;
byte ly = calbuf[5]>>2;
byte rx = calbuf[8]>>3;
byte ry = calbuf[11]>>3;

// Wiimote button data stream
byte *stream_callback(byte *buffer) {
	wiimote_write_buffer(buffer, bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
			bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	return buffer;
}

void setup()
{
	// Set PushButton pins as input, turning pull-up on
	pinMode(13, OUTPUT);

	// Prepare wiimote communications
	wiimote_stream = stream_callback;
	wiimote_init();

    digitalWrite(13, LOW);

    Serial.begin(38400);
    delay(100);
}

#define KEY_UP           0
#define KEY_RIGHT        1
#define KEY_DOWN         2
#define KEY_LEFT         3
#define KEY_RIGHT_TURN   4
#define KEY_LEFT_TURN    5
#define KEY_START        6
#define KEY_WAIT         7

void loop()
{
    digitalWrite(13, LOW);

    if (Serial.available() <= 0)
        return;

    int x = Serial.read();
    x &= 0xFF;

    if ((x >> KEY_WAIT) & 1) {
        int wait = x & 0x7F;
        digitalWrite(13, HIGH);
        delay(wait);
        return;
    }

    if ((x >> KEY_UP) & 1) {
        bdu = 1;
    } else {
        bdu = 0;
    }
    if ((x >> KEY_RIGHT) & 1) {
        bdr = 1;
    } else {
        bdr = 0;
    }
    if ((x >> KEY_DOWN) & 1) {
        bdd = 1;
    } else {
        bdd = 0;
    }
    if ((x >> KEY_LEFT) & 1) {
        bdl = 1;
    } else {
        bdl = 0;
    }
    if ((x >> KEY_RIGHT_TURN) & 1) {
        ba = 1;
    } else {
        ba = 0;
    }
    if ((x >> KEY_LEFT_TURN) & 1) {
        bb = 1;
    } else {
        bb = 0;
    }
    if ((x >> KEY_START) & 1) {
        bp = 1;
    } else {
        bp = 0;
    }

    delay(34);
}
