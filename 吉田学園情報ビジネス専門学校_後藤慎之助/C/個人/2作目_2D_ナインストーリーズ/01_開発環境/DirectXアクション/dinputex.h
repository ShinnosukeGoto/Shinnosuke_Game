//=============================================================================
//
// X-Bow Hunter [dinputex.h]の処理 (キーボード)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _DINPUTEX_H_
#define _DINPUTEX_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//構造体定義
//=============================================================================
//static DIOBJECTDATAFORMAT diodfKeyData[] =
//{
//	{ &GUID_Key, 0, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(0), 0 },
//	{ &GUID_Key, 1, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(1), 0 },
//	{ &GUID_Key, 2, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(2), 0 },
//	{ &GUID_Key, 3, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(3), 0 },
//	{ &GUID_Key, 4, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(4), 0 },
//	{ &GUID_Key, 5, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(5), 0 },
//	{ &GUID_Key, 6, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(6), 0 },
//	{ &GUID_Key, 7, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(7), 0 },
//	{ &GUID_Key, 8, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(8), 0 },
//	{ &GUID_Key, 9, DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(9), 0 },
//	{ &GUID_Key, 10, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(10), 0 },
//	{ &GUID_Key, 11, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(11), 0 },
//	{ &GUID_Key, 12, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(12), 0 },
//	{ &GUID_Key, 13, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(13), 0 },
//	{ &GUID_Key, 14, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(14), 0 },
//	{ &GUID_Key, 15, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(15), 0 },
//	{ &GUID_Key, 16, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(16), 0 },
//	{ &GUID_Key, 17, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(17), 0 },
//	{ &GUID_Key, 18, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(18), 0 },
//	{ &GUID_Key, 19, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(19), 0 },
//	{ &GUID_Key, 20, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(20), 0 },
//	{ &GUID_Key, 21, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(21), 0 },
//	{ &GUID_Key, 22, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(22), 0 },
//	{ &GUID_Key, 23, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(23), 0 },
//	{ &GUID_Key, 24, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(24), 0 },
//	{ &GUID_Key, 25, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(25), 0 },
//	{ &GUID_Key, 26, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(26), 0 },
//	{ &GUID_Key, 27, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(27), 0 },
//	{ &GUID_Key, 28, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(28), 0 },
//	{ &GUID_Key, 29, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(29), 0 },
//	{ &GUID_Key, 30, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(30), 0 },
//	{ &GUID_Key, 31, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(31), 0 },
//	{ &GUID_Key, 32, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(32), 0 },
//	{ &GUID_Key, 33, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(33), 0 },
//	{ &GUID_Key, 34, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(34), 0 },
//	{ &GUID_Key, 35, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(35), 0 },
//	{ &GUID_Key, 36, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(36), 0 },
//	{ &GUID_Key, 37, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(37), 0 },
//	{ &GUID_Key, 38, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(38), 0 },
//	{ &GUID_Key, 39, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(39), 0 },
//	{ &GUID_Key, 40, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(40), 0 },
//	{ &GUID_Key, 41, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(41), 0 },
//	{ &GUID_Key, 42, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(42), 0 },
//	{ &GUID_Key, 43, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(43), 0 },
//	{ &GUID_Key, 44, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(44), 0 },
//	{ &GUID_Key, 45, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(45), 0 },
//	{ &GUID_Key, 46, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(46), 0 },
//	{ &GUID_Key, 47, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(47), 0 },
//	{ &GUID_Key, 48, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(48), 0 },
//	{ &GUID_Key, 49, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(49), 0 },
//	{ &GUID_Key, 50, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(50), 0 },
//	{ &GUID_Key, 51, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(51), 0 },
//	{ &GUID_Key, 52, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(52), 0 },
//	{ &GUID_Key, 53, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(53), 0 },
//	{ &GUID_Key, 54, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(54), 0 },
//	{ &GUID_Key, 55, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(55), 0 },
//	{ &GUID_Key, 56, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(56), 0 },
//	{ &GUID_Key, 57, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(57), 0 },
//	{ &GUID_Key, 58, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(58), 0 },
//	{ &GUID_Key, 59, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(59), 0 },
//	{ &GUID_Key, 60, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(60), 0 },
//	{ &GUID_Key, 61, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(61), 0 },
//	{ &GUID_Key, 62, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(62), 0 },
//	{ &GUID_Key, 63, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(63), 0 },
//	{ &GUID_Key, 64, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(64), 0 },
//	{ &GUID_Key, 65, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(65), 0 },
//	{ &GUID_Key, 66, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(66), 0 },
//	{ &GUID_Key, 67, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(67), 0 },
//	{ &GUID_Key, 68, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(68), 0 },
//	{ &GUID_Key, 69, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(69), 0 },
//	{ &GUID_Key, 70, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(70), 0 },
//	{ &GUID_Key, 71, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(71), 0 },
//	{ &GUID_Key, 72, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(72), 0 },
//	{ &GUID_Key, 73, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(73), 0 },
//	{ &GUID_Key, 74, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(74), 0 },
//	{ &GUID_Key, 75, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(75), 0 },
//	{ &GUID_Key, 76, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(76), 0 },
//	{ &GUID_Key, 77, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(77), 0 },
//	{ &GUID_Key, 78, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(78), 0 },
//	{ &GUID_Key, 79, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(79), 0 },
//	{ &GUID_Key, 80, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(80), 0 },
//	{ &GUID_Key, 81, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(81), 0 },
//	{ &GUID_Key, 82, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(82), 0 },
//	{ &GUID_Key, 83, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(83), 0 },
//	{ &GUID_Key, 84, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(84), 0 },
//	{ &GUID_Key, 85, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(85), 0 },
//	{ &GUID_Key, 86, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(86), 0 },
//	{ &GUID_Key, 87, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(87), 0 },
//	{ &GUID_Key, 88, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(88), 0 },
//	{ &GUID_Key, 89, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(89), 0 },
//	{ &GUID_Key, 90, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(90), 0 },
//	{ &GUID_Key, 91, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(91), 0 },
//	{ &GUID_Key, 92, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(92), 0 },
//	{ &GUID_Key, 93, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(93), 0 },
//	{ &GUID_Key, 94, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(94), 0 },
//	{ &GUID_Key, 95, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(95), 0 },
//	{ &GUID_Key, 96, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(96), 0 },
//	{ &GUID_Key, 97, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(97), 0 },
//	{ &GUID_Key, 98, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(98), 0 },
//	{ &GUID_Key, 99, DIRECTINPUT_VERSION| DIDFT_BUTTON | DIDFT_MAKEINSTANCE(99), 0 },
//	{ &GUID_Key, 100,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(100), 0 },
//	{ &GUID_Key, 101,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(101), 0 },
//	{ &GUID_Key, 102,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(102), 0 },
//	{ &GUID_Key, 103,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(103), 0 },
//	{ &GUID_Key, 104,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(104), 0 },
//	{ &GUID_Key, 105,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(105), 0 },
//	{ &GUID_Key, 106,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(106), 0 },
//	{ &GUID_Key, 107,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(107), 0 },
//	{ &GUID_Key, 108,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(108), 0 },
//	{ &GUID_Key, 109,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(109), 0 },
//	{ &GUID_Key, 110,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(110), 0 },
//	{ &GUID_Key, 111,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(111), 0 },
//	{ &GUID_Key, 112,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(112), 0 },
//	{ &GUID_Key, 113,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(113), 0 },
//	{ &GUID_Key, 114,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(114), 0 },
//	{ &GUID_Key, 115,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(115), 0 },
//	{ &GUID_Key, 116,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(116), 0 },
//	{ &GUID_Key, 117,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(117), 0 },
//	{ &GUID_Key, 118,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(118), 0 },
//	{ &GUID_Key, 119,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(119), 0 },
//	{ &GUID_Key, 120,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(120), 0 },
//	{ &GUID_Key, 121,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(121), 0 },
//	{ &GUID_Key, 122,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(122), 0 },
//	{ &GUID_Key, 123,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(123), 0 },
//	{ &GUID_Key, 124,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(124), 0 },
//	{ &GUID_Key, 125,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(125), 0 },
//	{ &GUID_Key, 126,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(126), 0 },
//	{ &GUID_Key, 127,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(127), 0 },
//	{ &GUID_Key, 128,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(128), 0 },
//	{ &GUID_Key, 129,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(129), 0 },
//	{ &GUID_Key, 130,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(130), 0 },
//	{ &GUID_Key, 131,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(131), 0 },
//	{ &GUID_Key, 132,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(132), 0 },
//	{ &GUID_Key, 133,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(133), 0 },
//	{ &GUID_Key, 134,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(134), 0 },
//	{ &GUID_Key, 135,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(135), 0 },
//	{ &GUID_Key, 136,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(136), 0 },
//	{ &GUID_Key, 137,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(137), 0 },
//	{ &GUID_Key, 138,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(138), 0 },
//	{ &GUID_Key, 139,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(139), 0 },
//	{ &GUID_Key, 140,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(140), 0 },
//	{ &GUID_Key, 141,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(141), 0 },
//	{ &GUID_Key, 142,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(142), 0 },
//	{ &GUID_Key, 143,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(143), 0 },
//	{ &GUID_Key, 144,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(144), 0 },
//	{ &GUID_Key, 145,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(145), 0 },
//	{ &GUID_Key, 146,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(146), 0 },
//	{ &GUID_Key, 147,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(147), 0 },
//	{ &GUID_Key, 148,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(148), 0 },
//	{ &GUID_Key, 149,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(149), 0 },
//	{ &GUID_Key, 150,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(150), 0 },
//	{ &GUID_Key, 151,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(151), 0 },
//	{ &GUID_Key, 152,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(152), 0 },
//	{ &GUID_Key, 153,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(153), 0 },
//	{ &GUID_Key, 154,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(154), 0 },
//	{ &GUID_Key, 155,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(155), 0 },
//	{ &GUID_Key, 156,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(156), 0 },
//	{ &GUID_Key, 157,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(157), 0 },
//	{ &GUID_Key, 158,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(158), 0 },
//	{ &GUID_Key, 159,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(159), 0 },
//	{ &GUID_Key, 160,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(160), 0 },
//	{ &GUID_Key, 161,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(161), 0 },
//	{ &GUID_Key, 162,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(162), 0 },
//	{ &GUID_Key, 163,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(163), 0 },
//	{ &GUID_Key, 164,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(164), 0 },
//	{ &GUID_Key, 165,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(165), 0 },
//	{ &GUID_Key, 166,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(166), 0 },
//	{ &GUID_Key, 167,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(167), 0 },
//	{ &GUID_Key, 168,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(168), 0 },
//	{ &GUID_Key, 169,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(169), 0 },
//	{ &GUID_Key, 170,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(170), 0 },
//	{ &GUID_Key, 171,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(171), 0 },
//	{ &GUID_Key, 172,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(172), 0 },
//	{ &GUID_Key, 173,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(173), 0 },
//	{ &GUID_Key, 174,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(174), 0 },
//	{ &GUID_Key, 175,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(175), 0 },
//	{ &GUID_Key, 176,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(176), 0 },
//	{ &GUID_Key, 177,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(177), 0 },
//	{ &GUID_Key, 178,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(178), 0 },
//	{ &GUID_Key, 179,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(179), 0 },
//	{ &GUID_Key, 180,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(180), 0 },
//	{ &GUID_Key, 181,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(181), 0 },
//	{ &GUID_Key, 182,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(182), 0 },
//	{ &GUID_Key, 183,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(183), 0 },
//	{ &GUID_Key, 184,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(184), 0 },
//	{ &GUID_Key, 185,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(185), 0 },
//	{ &GUID_Key, 186,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(186), 0 },
//	{ &GUID_Key, 187,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(187), 0 },
//	{ &GUID_Key, 188,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(188), 0 },
//	{ &GUID_Key, 189,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(189), 0 },
//	{ &GUID_Key, 190,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(190), 0 },
//	{ &GUID_Key, 191,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(191), 0 },
//	{ &GUID_Key, 192,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(192), 0 },
//	{ &GUID_Key, 193,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(193), 0 },
//	{ &GUID_Key, 194,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(194), 0 },
//	{ &GUID_Key, 195,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(195), 0 },
//	{ &GUID_Key, 196,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(196), 0 },
//	{ &GUID_Key, 197,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(197), 0 },
//	{ &GUID_Key, 198,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(198), 0 },
//	{ &GUID_Key, 199,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(199), 0 },
//	{ &GUID_Key, 200,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(200), 0 },
//	{ &GUID_Key, 201,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(201), 0 },
//	{ &GUID_Key, 202,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(202), 0 },
//	{ &GUID_Key, 203,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(203), 0 },
//	{ &GUID_Key, 204,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(204), 0 },
//	{ &GUID_Key, 205,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(205), 0 },
//	{ &GUID_Key, 206,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(206), 0 },
//	{ &GUID_Key, 207,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(207), 0 },
//	{ &GUID_Key, 208,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(208), 0 },
//	{ &GUID_Key, 209,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(209), 0 },
//	{ &GUID_Key, 210,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(210), 0 },
//	{ &GUID_Key, 211,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(211), 0 },
//	{ &GUID_Key, 212,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(212), 0 },
//	{ &GUID_Key, 213,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(213), 0 },
//	{ &GUID_Key, 214,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(214), 0 },
//	{ &GUID_Key, 215,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(215), 0 },
//	{ &GUID_Key, 216,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(216), 0 },
//	{ &GUID_Key, 217,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(217), 0 },
//	{ &GUID_Key, 218,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(218), 0 },
//	{ &GUID_Key, 219,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(219), 0 },
//	{ &GUID_Key, 220,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(220), 0 },
//	{ &GUID_Key, 221,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(221), 0 },
//	{ &GUID_Key, 222,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(222), 0 },
//	{ &GUID_Key, 223,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(223), 0 },
//	{ &GUID_Key, 224,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(224), 0 },
//	{ &GUID_Key, 225,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(225), 0 },
//	{ &GUID_Key, 226,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(226), 0 },
//	{ &GUID_Key, 227,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(227), 0 },
//	{ &GUID_Key, 228,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(228), 0 },
//	{ &GUID_Key, 229,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(229), 0 },
//	{ &GUID_Key, 230,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(230), 0 },
//	{ &GUID_Key, 231,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(231), 0 },
//	{ &GUID_Key, 232,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(232), 0 },
//	{ &GUID_Key, 233,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(233), 0 },
//	{ &GUID_Key, 234,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(234), 0 },
//	{ &GUID_Key, 235,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(235), 0 },
//	{ &GUID_Key, 236,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(236), 0 },
//	{ &GUID_Key, 237,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(237), 0 },
//	{ &GUID_Key, 238,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(238), 0 },
//	{ &GUID_Key, 239,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(239), 0 },
//	{ &GUID_Key, 240,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(240), 0 },
//	{ &GUID_Key, 241,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(241), 0 },
//	{ &GUID_Key, 242,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(242), 0 },
//	{ &GUID_Key, 243,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(243), 0 },
//	{ &GUID_Key, 244,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(244), 0 },
//	{ &GUID_Key, 245,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(245), 0 },
//	{ &GUID_Key, 246,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(246), 0 },
//	{ &GUID_Key, 247,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(247), 0 },
//	{ &GUID_Key, 248,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(248), 0 },
//	{ &GUID_Key, 249,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(249), 0 },
//	{ &GUID_Key, 250,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(250), 0 },
//	{ &GUID_Key, 251,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(251), 0 },
//	{ &GUID_Key, 252,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(252), 0 },
//	{ &GUID_Key, 253,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(253), 0 },
//	{ &GUID_Key, 254,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(254), 0 },
//	{ &GUID_Key, 255,DIRECTINPUT_VERSION | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(255), 0 }
//};
//
//const DIDATAFORMAT c_dfDIKeyboard =
//{ 24, 16, DIDF_RELAXIS, 256, 256, &diodfKeyData[0] };
//
//static DIOBJECTDATAFORMAT diodfJoyData[] =
//{
//	{ &GUID_XAxis, 0, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_YAxis, 4, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_ZAxis, 8, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_RxAxis, 12, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_RyAxis, 16, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_RzAxis, 20, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_Slider, 24, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_Slider, 28, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_AXIS, DIDOI_ASPECTPOSITION },
//	{ &GUID_POV, 32, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_POV, 0 },
//	{ &GUID_POV, 36, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_POV, 0 },
//	{ &GUID_POV, 40, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_POV, 0 },
//	{ &GUID_POV, 44, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_POV, 0 },
//	{ NULL, 48, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 49, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 50, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 51, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 52, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 53, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 54, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 55, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 56, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 57, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 58, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 59, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 60, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 61, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 62, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 63, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 64, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 65, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 66, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 67, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 68, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 69, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 70, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 71, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 72, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 73, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 74, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 75, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 76, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 77, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 78, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 },
//	{ NULL, 79, DIRECTINPUT_VERSION | DIDFT_ANYINSTANCE | DIDFT_BUTTON, 0 }
//};
//
//const DIDATAFORMAT c_dfDIJoystick =
//{ 24, 16, DIDF_ABSAXIS, 80, 44, &diodfJoyData[0] };

//上のやつ？

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitPad(HINSTANCE hInstance, HWND hWnd);
void UninitPad(void);
void UpdatePad(void);

//bool GetKeyboardPress(int nKey);
//bool GetKeyboardTrigger(int nKey);
//bool GetKeyboardRelease(int nKey);

BOOL    CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
BOOL    CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);

#endif