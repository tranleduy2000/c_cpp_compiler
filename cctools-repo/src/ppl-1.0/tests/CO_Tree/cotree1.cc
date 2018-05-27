/* Test the CO_Tree class.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://bugseng.com/products/ppl/ . */

#include "ppl_test.hh"

#if PPL_USE_SPARSE_MATRIX

#include <vector>
#include <algorithm>
#include <set>

namespace {

bool
test01() {

  CO_Tree tree;

  for (CO_Tree::iterator i = tree.begin(), i_end = tree.end(); i != i_end; ++i)
    ;

  return true;
}

bool
test02() {

  CO_Tree tree;

  tree.insert(0, Coefficient_zero());
  tree.insert(1, Coefficient_zero());
  tree.insert(2, Coefficient_zero());
  tree.insert(3, Coefficient_zero());
  tree.insert(4, Coefficient_zero());

  return true;
}

bool
test03() {

  // Sequential insertion, sequential erase.

  CO_Tree tree;

  for (unsigned n = 0; n < 500; ++n)
    tree.insert(n, 0);

  for (unsigned n = 0; n < 500; ++n)
    tree.erase(n);

  if (!tree.empty())
    return false;

  return true;
}

bool
test04() {

  // Sequential insertion, backwards erase.

  CO_Tree tree;

  for (unsigned n = 0; n < 500; ++n)
    tree.insert(n, 0);

  for (unsigned n = 500; n-- > 0; )
    tree.erase(n);

  if (!tree.empty())
    return false;

  return true;
}

bool
test05() {

  // Backwards insertion, sequential erase.

  CO_Tree tree;

  for (unsigned n = 500; n-- > 0; )
    tree.insert(n, 0);

  for (unsigned n = 0; n < 500; ++n)
    tree.erase(n);

  if (!tree.empty())
    return false;

  return true;
}

bool
test06() {

  // Backwards insertion, backwards erase.

  CO_Tree tree;

  for (unsigned n = 500; --n > 0; )
    tree.insert(n, 0);

  for (unsigned n = 500; --n > 0; )
    tree.erase(n);

  if (!tree.empty())
    return false;

  return true;
}

bool
test07() {

  // Sequential insertion, pseudo-random erase.

  CO_Tree tree;

  for (unsigned n = 0; n < 500; ++n)
    tree.insert(n, 0);

  tree.erase(110);
  tree.erase(290);
  tree.erase(11);
  tree.erase(69);
  tree.erase(209);
  tree.erase(468);
  tree.erase(17);
  tree.erase(293);
  tree.erase(164);
  tree.erase(350);
  tree.erase(115);
  tree.erase(322);
  tree.erase(361);
  tree.erase(2);
  tree.erase(446);
  tree.erase(281);
  tree.erase(400);
  tree.erase(375);
  tree.erase(153);
  tree.erase(116);
  tree.erase(143);
  tree.erase(329);
  tree.erase(420);
  tree.erase(133);
  tree.erase(363);
  tree.erase(192);
  tree.erase(27);
  tree.erase(275);
  tree.erase(104);
  tree.erase(213);
  tree.erase(352);
  tree.erase(427);
  tree.erase(273);
  tree.erase(288);
  tree.erase(396);
  tree.erase(114);
  tree.erase(341);
  tree.erase(413);
  tree.erase(201);
  tree.erase(280);
  tree.erase(66);
  tree.erase(227);
  tree.erase(406);
  tree.erase(79);
  tree.erase(259);
  tree.erase(176);
  tree.erase(106);
  tree.erase(0);
  tree.erase(328);
  tree.erase(270);
  tree.erase(449);
  tree.erase(165);
  tree.erase(163);
  tree.erase(43);
  tree.erase(391);
  tree.erase(202);
  tree.erase(49);
  tree.erase(105);
  tree.erase(149);
  tree.erase(318);
  tree.erase(387);
  tree.erase(389);
  tree.erase(141);
  tree.erase(408);
  tree.erase(486);
  tree.erase(354);
  tree.erase(8);
  tree.erase(33);
  tree.erase(421);
  tree.erase(385);
  tree.erase(25);
  tree.erase(485);
  tree.erase(196);
  tree.erase(31);
  tree.erase(82);
  tree.erase(434);
  tree.erase(423);
  tree.erase(358);
  tree.erase(255);
  tree.erase(287);
  tree.erase(23);
  tree.erase(122);
  tree.erase(489);
  tree.erase(19);
  tree.erase(126);
  tree.erase(44);
  tree.erase(120);
  tree.erase(131);
  tree.erase(332);
  tree.erase(448);
  tree.erase(238);
  tree.erase(264);
  tree.erase(454);
  tree.erase(218);
  tree.erase(157);
  tree.erase(436);
  tree.erase(225);
  tree.erase(437);
  tree.erase(443);
  tree.erase(179);
  tree.erase(265);
  tree.erase(475);
  tree.erase(180);
  tree.erase(487);
  tree.erase(339);
  tree.erase(492);
  tree.erase(395);
  tree.erase(491);
  tree.erase(223);
  tree.erase(113);
  tree.erase(92);
  tree.erase(48);
  tree.erase(61);
  tree.erase(127);
  tree.erase(190);
  tree.erase(67);
  tree.erase(484);
  tree.erase(439);
  tree.erase(355);
  tree.erase(243);
  tree.erase(392);
  tree.erase(159);
  tree.erase(74);
  tree.erase(337);
  tree.erase(151);
  tree.erase(458);
  tree.erase(480);
  tree.erase(334);
  tree.erase(419);
  tree.erase(309);
  tree.erase(301);
  tree.erase(125);
  tree.erase(407);
  tree.erase(496);
  tree.erase(187);
  tree.erase(50);
  tree.erase(368);
  tree.erase(283);
  tree.erase(244);
  tree.erase(170);
  tree.erase(118);
  tree.erase(457);
  tree.erase(181);
  tree.erase(479);
  tree.erase(401);
  tree.erase(494);
  tree.erase(99);
  tree.erase(236);
  tree.erase(240);
  tree.erase(147);
  tree.erase(233);
  tree.erase(172);
  tree.erase(266);
  tree.erase(32);
  tree.erase(210);
  tree.erase(161);
  tree.erase(156);
  tree.erase(178);
  tree.erase(221);
  tree.erase(78);
  tree.erase(85);
  tree.erase(135);
  tree.erase(145);
  tree.erase(356);
  tree.erase(397);
  tree.erase(450);
  tree.erase(276);
  tree.erase(41);
  tree.erase(414);
  tree.erase(14);
  tree.erase(22);
  tree.erase(29);
  tree.erase(34);
  tree.erase(498);
  tree.erase(250);
  tree.erase(36);
  tree.erase(320);
  tree.erase(268);
  tree.erase(195);
  tree.erase(382);
  tree.erase(441);
  tree.erase(235);
  tree.erase(346);
  tree.erase(476);
  tree.erase(217);
  tree.erase(335);
  tree.erase(121);
  tree.erase(94);
  tree.erase(278);
  tree.erase(272);
  tree.erase(207);
  tree.erase(463);
  tree.erase(150);
  tree.erase(432);
  tree.erase(410);
  tree.erase(208);
  tree.erase(70);
  tree.erase(84);
  tree.erase(186);
  tree.erase(6);
  tree.erase(224);
  tree.erase(9);
  tree.erase(60);
  tree.erase(175);
  tree.erase(430);
  tree.erase(128);
  tree.erase(129);
  tree.erase(465);
  tree.erase(459);
  tree.erase(289);
  tree.erase(261);
  tree.erase(26);
  tree.erase(461);
  tree.erase(279);
  tree.erase(245);
  tree.erase(478);
  tree.erase(403);
  tree.erase(45);
  tree.erase(359);
  tree.erase(327);
  tree.erase(393);
  tree.erase(373);
  tree.erase(304);
  tree.erase(83);
  tree.erase(160);
  tree.erase(198);
  tree.erase(103);
  tree.erase(367);
  tree.erase(76);
  tree.erase(73);
  tree.erase(167);
  tree.erase(291);
  tree.erase(215);
  tree.erase(219);
  tree.erase(119);
  tree.erase(456);
  tree.erase(197);
  tree.erase(477);
  tree.erase(222);
  tree.erase(174);
  tree.erase(451);
  tree.erase(214);
  tree.erase(112);
  tree.erase(464);
  tree.erase(262);
  tree.erase(47);
  tree.erase(347);
  tree.erase(111);
  tree.erase(148);
  tree.erase(308);
  tree.erase(340);
  tree.erase(100);
  tree.erase(130);
  tree.erase(323);
  tree.erase(312);
  tree.erase(292);
  tree.erase(35);
  tree.erase(306);
  tree.erase(58);
  tree.erase(353);
  tree.erase(452);
  tree.erase(91);
  tree.erase(319);
  tree.erase(330);
  tree.erase(473);
  tree.erase(488);
  tree.erase(134);
  tree.erase(315);
  tree.erase(253);
  tree.erase(374);
  tree.erase(384);
  tree.erase(95);
  tree.erase(370);
  tree.erase(13);
  tree.erase(183);
  tree.erase(136);
  tree.erase(313);
  tree.erase(307);
  tree.erase(239);
  tree.erase(258);
  tree.erase(405);
  tree.erase(56);
  tree.erase(228);
  tree.erase(455);
  tree.erase(317);
  tree.erase(497);
  tree.erase(102);
  tree.erase(117);
  tree.erase(68);
  tree.erase(234);
  tree.erase(51);
  tree.erase(107);
  tree.erase(349);
  tree.erase(348);
  tree.erase(416);
  tree.erase(88);
  tree.erase(89);
  tree.erase(366);
  tree.erase(109);
  tree.erase(189);
  tree.erase(333);
  tree.erase(3);
  tree.erase(394);
  tree.erase(267);
  tree.erase(269);
  tree.erase(246);
  tree.erase(152);
  tree.erase(173);
  tree.erase(438);
  tree.erase(24);
  tree.erase(15);
  tree.erase(390);
  tree.erase(284);
  tree.erase(360);
  tree.erase(371);
  tree.erase(81);
  tree.erase(65);
  tree.erase(299);
  tree.erase(132);
  tree.erase(98);
  tree.erase(303);
  tree.erase(139);
  tree.erase(453);
  tree.erase(402);
  tree.erase(20);
  tree.erase(54);
  tree.erase(499);
  tree.erase(260);
  tree.erase(285);
  tree.erase(381);
  tree.erase(357);
  tree.erase(248);
  tree.erase(362);
  tree.erase(62);
  tree.erase(203);
  tree.erase(411);
  tree.erase(444);
  tree.erase(388);
  tree.erase(10);
  tree.erase(342);
  tree.erase(229);
  tree.erase(481);
  tree.erase(369);
  tree.erase(378);
  tree.erase(38);
  tree.erase(77);
  tree.erase(415);
  tree.erase(466);
  tree.erase(404);
  tree.erase(90);
  tree.erase(101);
  tree.erase(169);
  tree.erase(435);
  tree.erase(296);
  tree.erase(282);
  tree.erase(63);
  tree.erase(52);
  tree.erase(40);
  tree.erase(231);
  tree.erase(302);
  tree.erase(18);
  tree.erase(383);
  tree.erase(194);
  tree.erase(351);
  tree.erase(254);
  tree.erase(431);
  tree.erase(199);
  tree.erase(80);
  tree.erase(300);
  tree.erase(140);
  tree.erase(324);
  tree.erase(286);
  tree.erase(188);
  tree.erase(386);
  tree.erase(344);
  tree.erase(166);
  tree.erase(4);
  tree.erase(226);
  tree.erase(316);
  tree.erase(158);
  tree.erase(447);
  tree.erase(86);
  tree.erase(398);
  tree.erase(108);
  tree.erase(230);
  tree.erase(310);
  tree.erase(495);
  tree.erase(171);
  tree.erase(380);
  tree.erase(249);
  tree.erase(433);
  tree.erase(16);
  tree.erase(470);
  tree.erase(277);
  tree.erase(21);
  tree.erase(372);
  tree.erase(252);
  tree.erase(424);
  tree.erase(144);
  tree.erase(377);
  tree.erase(59);
  tree.erase(46);
  tree.erase(55);
  tree.erase(429);
  tree.erase(474);
  tree.erase(321);
  tree.erase(399);
  tree.erase(471);
  tree.erase(237);
  tree.erase(442);
  tree.erase(97);
  tree.erase(220);
  tree.erase(445);
  tree.erase(326);
  tree.erase(37);
  tree.erase(336);
  tree.erase(343);
  tree.erase(412);
  tree.erase(409);
  tree.erase(460);
  tree.erase(57);
  tree.erase(168);
  tree.erase(295);
  tree.erase(247);
  tree.erase(482);
  tree.erase(425);
  tree.erase(256);
  tree.erase(96);
  tree.erase(53);
  tree.erase(469);
  tree.erase(162);
  tree.erase(493);
  tree.erase(294);
  tree.erase(177);
  tree.erase(212);
  tree.erase(30);
  tree.erase(5);
  tree.erase(193);
  tree.erase(483);
  tree.erase(124);
  tree.erase(87);
  tree.erase(64);
  tree.erase(490);
  tree.erase(155);
  tree.erase(422);
  tree.erase(191);
  tree.erase(75);
  tree.erase(325);
  tree.erase(1);
  tree.erase(182);
  tree.erase(28);
  tree.erase(364);
  tree.erase(42);
  tree.erase(39);
  tree.erase(376);
  tree.erase(467);
  tree.erase(426);
  tree.erase(205);
  tree.erase(365);
  tree.erase(137);
  tree.erase(297);
  tree.erase(462);
  tree.erase(241);
  tree.erase(123);
  tree.erase(206);
  tree.erase(440);
  tree.erase(216);
  tree.erase(146);
  tree.erase(142);
  tree.erase(72);
  tree.erase(379);
  tree.erase(472);
  tree.erase(305);
  tree.erase(271);
  tree.erase(298);
  tree.erase(232);
  tree.erase(242);
  tree.erase(184);
  tree.erase(138);
  tree.erase(154);
  tree.erase(200);
  tree.erase(71);
  tree.erase(211);
  tree.erase(274);
  tree.erase(263);
  tree.erase(311);
  tree.erase(428);
  tree.erase(331);
  tree.erase(7);
  tree.erase(345);
  tree.erase(185);
  tree.erase(338);
  tree.erase(251);
  tree.erase(417);
  tree.erase(12);
  tree.erase(93);
  tree.erase(204);
  tree.erase(257);
  tree.erase(418);
  tree.erase(314);

  if (!tree.empty())
    return false;

  return true;
}

bool test08() {

  // Pseudo-random insertion, pseudo-random erase (in the same order).

  CO_Tree tree;

  tree.insert(110, 0);
  tree.insert(290, 0);
  tree.insert(11, 0);
  tree.insert(69, 0);
  tree.insert(209, 0);
  tree.insert(468, 0);
  tree.insert(17, 0);
  tree.insert(293, 0);
  tree.insert(164, 0);
  tree.insert(350, 0);
  tree.insert(115, 0);
  tree.insert(322, 0);
  tree.insert(361, 0);
  tree.insert(2, 0);
  tree.insert(446, 0);
  tree.insert(281, 0);
  tree.insert(400, 0);
  tree.insert(375, 0);
  tree.insert(153, 0);
  tree.insert(116, 0);
  tree.insert(143, 0);
  tree.insert(329, 0);
  tree.insert(420, 0);
  tree.insert(133, 0);
  tree.insert(363, 0);
  tree.insert(192, 0);
  tree.insert(27, 0);
  tree.insert(275, 0);
  tree.insert(104, 0);
  tree.insert(213, 0);
  tree.insert(352, 0);
  tree.insert(427, 0);
  tree.insert(273, 0);
  tree.insert(288, 0);
  tree.insert(396, 0);
  tree.insert(114, 0);
  tree.insert(341, 0);
  tree.insert(413, 0);
  tree.insert(201, 0);
  tree.insert(280, 0);
  tree.insert(66, 0);
  tree.insert(227, 0);
  tree.insert(406, 0);
  tree.insert(79, 0);
  tree.insert(259, 0);
  tree.insert(176, 0);
  tree.insert(106, 0);
  tree.insert(0, 0);
  tree.insert(328, 0);
  tree.insert(270, 0);
  tree.insert(449, 0);
  tree.insert(165, 0);
  tree.insert(163, 0);
  tree.insert(43, 0);
  tree.insert(391, 0);
  tree.insert(202, 0);
  tree.insert(49, 0);
  tree.insert(105, 0);
  tree.insert(149, 0);
  tree.insert(318, 0);
  tree.insert(387, 0);
  tree.insert(389, 0);
  tree.insert(141, 0);
  tree.insert(408, 0);
  tree.insert(486, 0);
  tree.insert(354, 0);
  tree.insert(8, 0);
  tree.insert(33, 0);
  tree.insert(421, 0);
  tree.insert(385, 0);
  tree.insert(25, 0);
  tree.insert(485, 0);
  tree.insert(196, 0);
  tree.insert(31, 0);
  tree.insert(82, 0);
  tree.insert(434, 0);
  tree.insert(423, 0);
  tree.insert(358, 0);
  tree.insert(255, 0);
  tree.insert(287, 0);
  tree.insert(23, 0);
  tree.insert(122, 0);
  tree.insert(489, 0);
  tree.insert(19, 0);
  tree.insert(126, 0);
  tree.insert(44, 0);
  tree.insert(120, 0);
  tree.insert(131, 0);
  tree.insert(332, 0);
  tree.insert(448, 0);
  tree.insert(238, 0);
  tree.insert(264, 0);
  tree.insert(454, 0);
  tree.insert(218, 0);
  tree.insert(157, 0);
  tree.insert(436, 0);
  tree.insert(225, 0);
  tree.insert(437, 0);
  tree.insert(443, 0);
  tree.insert(179, 0);
  tree.insert(265, 0);
  tree.insert(475, 0);
  tree.insert(180, 0);
  tree.insert(487, 0);
  tree.insert(339, 0);
  tree.insert(492, 0);
  tree.insert(395, 0);
  tree.insert(491, 0);
  tree.insert(223, 0);
  tree.insert(113, 0);
  tree.insert(92, 0);
  tree.insert(48, 0);
  tree.insert(61, 0);
  tree.insert(127, 0);
  tree.insert(190, 0);
  tree.insert(67, 0);
  tree.insert(484, 0);
  tree.insert(439, 0);
  tree.insert(355, 0);
  tree.insert(243, 0);
  tree.insert(392, 0);
  tree.insert(159, 0);
  tree.insert(74, 0);
  tree.insert(337, 0);
  tree.insert(151, 0);
  tree.insert(458, 0);
  tree.insert(480, 0);
  tree.insert(334, 0);
  tree.insert(419, 0);
  tree.insert(309, 0);
  tree.insert(301, 0);
  tree.insert(125, 0);
  tree.insert(407, 0);
  tree.insert(496, 0);
  tree.insert(187, 0);
  tree.insert(50, 0);
  tree.insert(368, 0);
  tree.insert(283, 0);
  tree.insert(244, 0);
  tree.insert(170, 0);
  tree.insert(118, 0);
  tree.insert(457, 0);
  tree.insert(181, 0);
  tree.insert(479, 0);
  tree.insert(401, 0);
  tree.insert(494, 0);
  tree.insert(99, 0);
  tree.insert(236, 0);
  tree.insert(240, 0);
  tree.insert(147, 0);
  tree.insert(233, 0);
  tree.insert(172, 0);
  tree.insert(266, 0);
  tree.insert(32, 0);
  tree.insert(210, 0);
  tree.insert(161, 0);
  tree.insert(156, 0);
  tree.insert(178, 0);
  tree.insert(221, 0);
  tree.insert(78, 0);
  tree.insert(85, 0);
  tree.insert(135, 0);
  tree.insert(145, 0);
  tree.insert(356, 0);
  tree.insert(397, 0);
  tree.insert(450, 0);
  tree.insert(276, 0);
  tree.insert(41, 0);
  tree.insert(414, 0);
  tree.insert(14, 0);
  tree.insert(22, 0);
  tree.insert(29, 0);
  tree.insert(34, 0);
  tree.insert(498, 0);
  tree.insert(250, 0);
  tree.insert(36, 0);
  tree.insert(320, 0);
  tree.insert(268, 0);
  tree.insert(195, 0);
  tree.insert(382, 0);
  tree.insert(441, 0);
  tree.insert(235, 0);
  tree.insert(346, 0);
  tree.insert(476, 0);
  tree.insert(217, 0);
  tree.insert(335, 0);
  tree.insert(121, 0);
  tree.insert(94, 0);
  tree.insert(278, 0);
  tree.insert(272, 0);
  tree.insert(207, 0);
  tree.insert(463, 0);
  tree.insert(150, 0);
  tree.insert(432, 0);
  tree.insert(410, 0);
  tree.insert(208, 0);
  tree.insert(70, 0);
  tree.insert(84, 0);
  tree.insert(186, 0);
  tree.insert(6, 0);
  tree.insert(224, 0);
  tree.insert(9, 0);
  tree.insert(60, 0);
  tree.insert(175, 0);
  tree.insert(430, 0);
  tree.insert(128, 0);
  tree.insert(129, 0);
  tree.insert(465, 0);
  tree.insert(459, 0);
  tree.insert(289, 0);
  tree.insert(261, 0);
  tree.insert(26, 0);
  tree.insert(461, 0);
  tree.insert(279, 0);
  tree.insert(245, 0);
  tree.insert(478, 0);
  tree.insert(403, 0);
  tree.insert(45, 0);
  tree.insert(359, 0);
  tree.insert(327, 0);
  tree.insert(393, 0);
  tree.insert(373, 0);
  tree.insert(304, 0);
  tree.insert(83, 0);
  tree.insert(160, 0);
  tree.insert(198, 0);
  tree.insert(103, 0);
  tree.insert(367, 0);
  tree.insert(76, 0);
  tree.insert(73, 0);
  tree.insert(167, 0);
  tree.insert(291, 0);
  tree.insert(215, 0);
  tree.insert(219, 0);
  tree.insert(119, 0);
  tree.insert(456, 0);
  tree.insert(197, 0);
  tree.insert(477, 0);
  tree.insert(222, 0);
  tree.insert(174, 0);
  tree.insert(451, 0);
  tree.insert(214, 0);
  tree.insert(112, 0);
  tree.insert(464, 0);
  tree.insert(262, 0);
  tree.insert(47, 0);
  tree.insert(347, 0);
  tree.insert(111, 0);
  tree.insert(148, 0);
  tree.insert(308, 0);
  tree.insert(340, 0);
  tree.insert(100, 0);
  tree.insert(130, 0);
  tree.insert(323, 0);
  tree.insert(312, 0);
  tree.insert(292, 0);
  tree.insert(35, 0);
  tree.insert(306, 0);
  tree.insert(58, 0);
  tree.insert(353, 0);
  tree.insert(452, 0);
  tree.insert(91, 0);
  tree.insert(319, 0);
  tree.insert(330, 0);
  tree.insert(473, 0);
  tree.insert(488, 0);
  tree.insert(134, 0);
  tree.insert(315, 0);
  tree.insert(253, 0);
  tree.insert(374, 0);
  tree.insert(384, 0);
  tree.insert(95, 0);
  tree.insert(370, 0);
  tree.insert(13, 0);
  tree.insert(183, 0);
  tree.insert(136, 0);
  tree.insert(313, 0);
  tree.insert(307, 0);
  tree.insert(239, 0);
  tree.insert(258, 0);
  tree.insert(405, 0);
  tree.insert(56, 0);
  tree.insert(228, 0);
  tree.insert(455, 0);
  tree.insert(317, 0);
  tree.insert(497, 0);
  tree.insert(102, 0);
  tree.insert(117, 0);
  tree.insert(68, 0);
  tree.insert(234, 0);
  tree.insert(51, 0);
  tree.insert(107, 0);
  tree.insert(349, 0);
  tree.insert(348, 0);
  tree.insert(416, 0);
  tree.insert(88, 0);
  tree.insert(89, 0);
  tree.insert(366, 0);
  tree.insert(109, 0);
  tree.insert(189, 0);
  tree.insert(333, 0);
  tree.insert(3, 0);
  tree.insert(394, 0);
  tree.insert(267, 0);
  tree.insert(269, 0);
  tree.insert(246, 0);
  tree.insert(152, 0);
  tree.insert(173, 0);
  tree.insert(438, 0);
  tree.insert(24, 0);
  tree.insert(15, 0);
  tree.insert(390, 0);
  tree.insert(284, 0);
  tree.insert(360, 0);
  tree.insert(371, 0);
  tree.insert(81, 0);
  tree.insert(65, 0);
  tree.insert(299, 0);
  tree.insert(132, 0);
  tree.insert(98, 0);
  tree.insert(303, 0);
  tree.insert(139, 0);
  tree.insert(453, 0);
  tree.insert(402, 0);
  tree.insert(20, 0);
  tree.insert(54, 0);
  tree.insert(499, 0);
  tree.insert(260, 0);
  tree.insert(285, 0);
  tree.insert(381, 0);
  tree.insert(357, 0);
  tree.insert(248, 0);
  tree.insert(362, 0);
  tree.insert(62, 0);
  tree.insert(203, 0);
  tree.insert(411, 0);
  tree.insert(444, 0);
  tree.insert(388, 0);
  tree.insert(10, 0);
  tree.insert(342, 0);
  tree.insert(229, 0);
  tree.insert(481, 0);
  tree.insert(369, 0);
  tree.insert(378, 0);
  tree.insert(38, 0);
  tree.insert(77, 0);
  tree.insert(415, 0);
  tree.insert(466, 0);
  tree.insert(404, 0);
  tree.insert(90, 0);
  tree.insert(101, 0);
  tree.insert(169, 0);
  tree.insert(435, 0);
  tree.insert(296, 0);
  tree.insert(282, 0);
  tree.insert(63, 0);
  tree.insert(52, 0);
  tree.insert(40, 0);
  tree.insert(231, 0);
  tree.insert(302, 0);
  tree.insert(18, 0);
  tree.insert(383, 0);
  tree.insert(194, 0);
  tree.insert(351, 0);
  tree.insert(254, 0);
  tree.insert(431, 0);
  tree.insert(199, 0);
  tree.insert(80, 0);
  tree.insert(300, 0);
  tree.insert(140, 0);
  tree.insert(324, 0);
  tree.insert(286, 0);
  tree.insert(188, 0);
  tree.insert(386, 0);
  tree.insert(344, 0);
  tree.insert(166, 0);
  tree.insert(4, 0);
  tree.insert(226, 0);
  tree.insert(316, 0);
  tree.insert(158, 0);
  tree.insert(447, 0);
  tree.insert(86, 0);
  tree.insert(398, 0);
  tree.insert(108, 0);
  tree.insert(230, 0);
  tree.insert(310, 0);
  tree.insert(495, 0);
  tree.insert(171, 0);
  tree.insert(380, 0);
  tree.insert(249, 0);
  tree.insert(433, 0);
  tree.insert(16, 0);
  tree.insert(470, 0);
  tree.insert(277, 0);
  tree.insert(21, 0);
  tree.insert(372, 0);
  tree.insert(252, 0);
  tree.insert(424, 0);
  tree.insert(144, 0);
  tree.insert(377, 0);
  tree.insert(59, 0);
  tree.insert(46, 0);
  tree.insert(55, 0);
  tree.insert(429, 0);
  tree.insert(474, 0);
  tree.insert(321, 0);
  tree.insert(399, 0);
  tree.insert(471, 0);
  tree.insert(237, 0);
  tree.insert(442, 0);
  tree.insert(97, 0);
  tree.insert(220, 0);
  tree.insert(445, 0);
  tree.insert(326, 0);
  tree.insert(37, 0);
  tree.insert(336, 0);
  tree.insert(343, 0);
  tree.insert(412, 0);
  tree.insert(409, 0);
  tree.insert(460, 0);
  tree.insert(57, 0);
  tree.insert(168, 0);
  tree.insert(295, 0);
  tree.insert(247, 0);
  tree.insert(482, 0);
  tree.insert(425, 0);
  tree.insert(256, 0);
  tree.insert(96, 0);
  tree.insert(53, 0);
  tree.insert(469, 0);
  tree.insert(162, 0);
  tree.insert(493, 0);
  tree.insert(294, 0);
  tree.insert(177, 0);
  tree.insert(212, 0);
  tree.insert(30, 0);
  tree.insert(5, 0);
  tree.insert(193, 0);
  tree.insert(483, 0);
  tree.insert(124, 0);
  tree.insert(87, 0);
  tree.insert(64, 0);
  tree.insert(490, 0);
  tree.insert(155, 0);
  tree.insert(422, 0);
  tree.insert(191, 0);
  tree.insert(75, 0);
  tree.insert(325, 0);
  tree.insert(1, 0);
  tree.insert(182, 0);
  tree.insert(28, 0);
  tree.insert(364, 0);
  tree.insert(42, 0);
  tree.insert(39, 0);
  tree.insert(376, 0);
  tree.insert(467, 0);
  tree.insert(426, 0);
  tree.insert(205, 0);
  tree.insert(365, 0);
  tree.insert(137, 0);
  tree.insert(297, 0);
  tree.insert(462, 0);
  tree.insert(241, 0);
  tree.insert(123, 0);
  tree.insert(206, 0);
  tree.insert(440, 0);
  tree.insert(216, 0);
  tree.insert(146, 0);
  tree.insert(142, 0);
  tree.insert(72, 0);
  tree.insert(379, 0);
  tree.insert(472, 0);
  tree.insert(305, 0);
  tree.insert(271, 0);
  tree.insert(298, 0);
  tree.insert(232, 0);
  tree.insert(242, 0);
  tree.insert(184, 0);
  tree.insert(138, 0);
  tree.insert(154, 0);
  tree.insert(200, 0);
  tree.insert(71, 0);
  tree.insert(211, 0);
  tree.insert(274, 0);
  tree.insert(263, 0);
  tree.insert(311, 0);
  tree.insert(428, 0);
  tree.insert(331, 0);
  tree.insert(7, 0);
  tree.insert(345, 0);
  tree.insert(185, 0);
  tree.insert(338, 0);
  tree.insert(251, 0);
  tree.insert(417, 0);
  tree.insert(12, 0);
  tree.insert(93, 0);
  tree.insert(204, 0);
  tree.insert(257, 0);
  tree.insert(418, 0);
  tree.insert(314, 0);

  tree.erase(110);
  tree.erase(290);
  tree.erase(11);
  tree.erase(69);
  tree.erase(209);
  tree.erase(468);
  tree.erase(17);
  tree.erase(293);
  tree.erase(164);
  tree.erase(350);
  tree.erase(115);
  tree.erase(322);
  tree.erase(361);
  tree.erase(2);
  tree.erase(446);
  tree.erase(281);
  tree.erase(400);
  tree.erase(375);
  tree.erase(153);
  tree.erase(116);
  tree.erase(143);
  tree.erase(329);
  tree.erase(420);
  tree.erase(133);
  tree.erase(363);
  tree.erase(192);
  tree.erase(27);
  tree.erase(275);
  tree.erase(104);
  tree.erase(213);
  tree.erase(352);
  tree.erase(427);
  tree.erase(273);
  tree.erase(288);
  tree.erase(396);
  tree.erase(114);
  tree.erase(341);
  tree.erase(413);
  tree.erase(201);
  tree.erase(280);
  tree.erase(66);
  tree.erase(227);
  tree.erase(406);
  tree.erase(79);
  tree.erase(259);
  tree.erase(176);
  tree.erase(106);
  tree.erase(0);
  tree.erase(328);
  tree.erase(270);
  tree.erase(449);
  tree.erase(165);
  tree.erase(163);
  tree.erase(43);
  tree.erase(391);
  tree.erase(202);
  tree.erase(49);
  tree.erase(105);
  tree.erase(149);
  tree.erase(318);
  tree.erase(387);
  tree.erase(389);
  tree.erase(141);
  tree.erase(408);
  tree.erase(486);
  tree.erase(354);
  tree.erase(8);
  tree.erase(33);
  tree.erase(421);
  tree.erase(385);
  tree.erase(25);
  tree.erase(485);
  tree.erase(196);
  tree.erase(31);
  tree.erase(82);
  tree.erase(434);
  tree.erase(423);
  tree.erase(358);
  tree.erase(255);
  tree.erase(287);
  tree.erase(23);
  tree.erase(122);
  tree.erase(489);
  tree.erase(19);
  tree.erase(126);
  tree.erase(44);
  tree.erase(120);
  tree.erase(131);
  tree.erase(332);
  tree.erase(448);
  tree.erase(238);
  tree.erase(264);
  tree.erase(454);
  tree.erase(218);
  tree.erase(157);
  tree.erase(436);
  tree.erase(225);
  tree.erase(437);
  tree.erase(443);
  tree.erase(179);
  tree.erase(265);
  tree.erase(475);
  tree.erase(180);
  tree.erase(487);
  tree.erase(339);
  tree.erase(492);
  tree.erase(395);
  tree.erase(491);
  tree.erase(223);
  tree.erase(113);
  tree.erase(92);
  tree.erase(48);
  tree.erase(61);
  tree.erase(127);
  tree.erase(190);
  tree.erase(67);
  tree.erase(484);
  tree.erase(439);
  tree.erase(355);
  tree.erase(243);
  tree.erase(392);
  tree.erase(159);
  tree.erase(74);
  tree.erase(337);
  tree.erase(151);
  tree.erase(458);
  tree.erase(480);
  tree.erase(334);
  tree.erase(419);
  tree.erase(309);
  tree.erase(301);
  tree.erase(125);
  tree.erase(407);
  tree.erase(496);
  tree.erase(187);
  tree.erase(50);
  tree.erase(368);
  tree.erase(283);
  tree.erase(244);
  tree.erase(170);
  tree.erase(118);
  tree.erase(457);
  tree.erase(181);
  tree.erase(479);
  tree.erase(401);
  tree.erase(494);
  tree.erase(99);
  tree.erase(236);
  tree.erase(240);
  tree.erase(147);
  tree.erase(233);
  tree.erase(172);
  tree.erase(266);
  tree.erase(32);
  tree.erase(210);
  tree.erase(161);
  tree.erase(156);
  tree.erase(178);
  tree.erase(221);
  tree.erase(78);
  tree.erase(85);
  tree.erase(135);
  tree.erase(145);
  tree.erase(356);
  tree.erase(397);
  tree.erase(450);
  tree.erase(276);
  tree.erase(41);
  tree.erase(414);
  tree.erase(14);
  tree.erase(22);
  tree.erase(29);
  tree.erase(34);
  tree.erase(498);
  tree.erase(250);
  tree.erase(36);
  tree.erase(320);
  tree.erase(268);
  tree.erase(195);
  tree.erase(382);
  tree.erase(441);
  tree.erase(235);
  tree.erase(346);
  tree.erase(476);
  tree.erase(217);
  tree.erase(335);
  tree.erase(121);
  tree.erase(94);
  tree.erase(278);
  tree.erase(272);
  tree.erase(207);
  tree.erase(463);
  tree.erase(150);
  tree.erase(432);
  tree.erase(410);
  tree.erase(208);
  tree.erase(70);
  tree.erase(84);
  tree.erase(186);
  tree.erase(6);
  tree.erase(224);
  tree.erase(9);
  tree.erase(60);
  tree.erase(175);
  tree.erase(430);
  tree.erase(128);
  tree.erase(129);
  tree.erase(465);
  tree.erase(459);
  tree.erase(289);
  tree.erase(261);
  tree.erase(26);
  tree.erase(461);
  tree.erase(279);
  tree.erase(245);
  tree.erase(478);
  tree.erase(403);
  tree.erase(45);
  tree.erase(359);
  tree.erase(327);
  tree.erase(393);
  tree.erase(373);
  tree.erase(304);
  tree.erase(83);
  tree.erase(160);
  tree.erase(198);
  tree.erase(103);
  tree.erase(367);
  tree.erase(76);
  tree.erase(73);
  tree.erase(167);
  tree.erase(291);
  tree.erase(215);
  tree.erase(219);
  tree.erase(119);
  tree.erase(456);
  tree.erase(197);
  tree.erase(477);
  tree.erase(222);
  tree.erase(174);
  tree.erase(451);
  tree.erase(214);
  tree.erase(112);
  tree.erase(464);
  tree.erase(262);
  tree.erase(47);
  tree.erase(347);
  tree.erase(111);
  tree.erase(148);
  tree.erase(308);
  tree.erase(340);
  tree.erase(100);
  tree.erase(130);
  tree.erase(323);
  tree.erase(312);
  tree.erase(292);
  tree.erase(35);
  tree.erase(306);
  tree.erase(58);
  tree.erase(353);
  tree.erase(452);
  tree.erase(91);
  tree.erase(319);
  tree.erase(330);
  tree.erase(473);
  tree.erase(488);
  tree.erase(134);
  tree.erase(315);
  tree.erase(253);
  tree.erase(374);
  tree.erase(384);
  tree.erase(95);
  tree.erase(370);
  tree.erase(13);
  tree.erase(183);
  tree.erase(136);
  tree.erase(313);
  tree.erase(307);
  tree.erase(239);
  tree.erase(258);
  tree.erase(405);
  tree.erase(56);
  tree.erase(228);
  tree.erase(455);
  tree.erase(317);
  tree.erase(497);
  tree.erase(102);
  tree.erase(117);
  tree.erase(68);
  tree.erase(234);
  tree.erase(51);
  tree.erase(107);
  tree.erase(349);
  tree.erase(348);
  tree.erase(416);
  tree.erase(88);
  tree.erase(89);
  tree.erase(366);
  tree.erase(109);
  tree.erase(189);
  tree.erase(333);
  tree.erase(3);
  tree.erase(394);
  tree.erase(267);
  tree.erase(269);
  tree.erase(246);
  tree.erase(152);
  tree.erase(173);
  tree.erase(438);
  tree.erase(24);
  tree.erase(15);
  tree.erase(390);
  tree.erase(284);
  tree.erase(360);
  tree.erase(371);
  tree.erase(81);
  tree.erase(65);
  tree.erase(299);
  tree.erase(132);
  tree.erase(98);
  tree.erase(303);
  tree.erase(139);
  tree.erase(453);
  tree.erase(402);
  tree.erase(20);
  tree.erase(54);
  tree.erase(499);
  tree.erase(260);
  tree.erase(285);
  tree.erase(381);
  tree.erase(357);
  tree.erase(248);
  tree.erase(362);
  tree.erase(62);
  tree.erase(203);
  tree.erase(411);
  tree.erase(444);
  tree.erase(388);
  tree.erase(10);
  tree.erase(342);
  tree.erase(229);
  tree.erase(481);
  tree.erase(369);
  tree.erase(378);
  tree.erase(38);
  tree.erase(77);
  tree.erase(415);
  tree.erase(466);
  tree.erase(404);
  tree.erase(90);
  tree.erase(101);
  tree.erase(169);
  tree.erase(435);
  tree.erase(296);
  tree.erase(282);
  tree.erase(63);
  tree.erase(52);
  tree.erase(40);
  tree.erase(231);
  tree.erase(302);
  tree.erase(18);
  tree.erase(383);
  tree.erase(194);
  tree.erase(351);
  tree.erase(254);
  tree.erase(431);
  tree.erase(199);
  tree.erase(80);
  tree.erase(300);
  tree.erase(140);
  tree.erase(324);
  tree.erase(286);
  tree.erase(188);
  tree.erase(386);
  tree.erase(344);
  tree.erase(166);
  tree.erase(4);
  tree.erase(226);
  tree.erase(316);
  tree.erase(158);
  tree.erase(447);
  tree.erase(86);
  tree.erase(398);
  tree.erase(108);
  tree.erase(230);
  tree.erase(310);
  tree.erase(495);
  tree.erase(171);
  tree.erase(380);
  tree.erase(249);
  tree.erase(433);
  tree.erase(16);
  tree.erase(470);
  tree.erase(277);
  tree.erase(21);
  tree.erase(372);
  tree.erase(252);
  tree.erase(424);
  tree.erase(144);
  tree.erase(377);
  tree.erase(59);
  tree.erase(46);
  tree.erase(55);
  tree.erase(429);
  tree.erase(474);
  tree.erase(321);
  tree.erase(399);
  tree.erase(471);
  tree.erase(237);
  tree.erase(442);
  tree.erase(97);
  tree.erase(220);
  tree.erase(445);
  tree.erase(326);
  tree.erase(37);
  tree.erase(336);
  tree.erase(343);
  tree.erase(412);
  tree.erase(409);
  tree.erase(460);
  tree.erase(57);
  tree.erase(168);
  tree.erase(295);
  tree.erase(247);
  tree.erase(482);
  tree.erase(425);
  tree.erase(256);
  tree.erase(96);
  tree.erase(53);
  tree.erase(469);
  tree.erase(162);
  tree.erase(493);
  tree.erase(294);
  tree.erase(177);
  tree.erase(212);
  tree.erase(30);
  tree.erase(5);
  tree.erase(193);
  tree.erase(483);
  tree.erase(124);
  tree.erase(87);
  tree.erase(64);
  tree.erase(490);
  tree.erase(155);
  tree.erase(422);
  tree.erase(191);
  tree.erase(75);
  tree.erase(325);
  tree.erase(1);
  tree.erase(182);
  tree.erase(28);
  tree.erase(364);
  tree.erase(42);
  tree.erase(39);
  tree.erase(376);
  tree.erase(467);
  tree.erase(426);
  tree.erase(205);
  tree.erase(365);
  tree.erase(137);
  tree.erase(297);
  tree.erase(462);
  tree.erase(241);
  tree.erase(123);
  tree.erase(206);
  tree.erase(440);
  tree.erase(216);
  tree.erase(146);
  tree.erase(142);
  tree.erase(72);
  tree.erase(379);
  tree.erase(472);
  tree.erase(305);
  tree.erase(271);
  tree.erase(298);
  tree.erase(232);
  tree.erase(242);
  tree.erase(184);
  tree.erase(138);
  tree.erase(154);
  tree.erase(200);
  tree.erase(71);
  tree.erase(211);
  tree.erase(274);
  tree.erase(263);
  tree.erase(311);
  tree.erase(428);
  tree.erase(331);
  tree.erase(7);
  tree.erase(345);
  tree.erase(185);
  tree.erase(338);
  tree.erase(251);
  tree.erase(417);
  tree.erase(12);
  tree.erase(93);
  tree.erase(204);
  tree.erase(257);
  tree.erase(418);
  tree.erase(314);

  if (!tree.empty())
    return false;

  return true;
}

bool
test09() {

  // Pseudo-random insertion and erases, pseudo-randomly interleaved.

  CO_Tree tree;

  tree.insert(172261, 5);
  tree.insert(690360, 5);
  tree.erase(228023);
  tree.erase(81);
  tree.erase(903190);
  tree.erase(618996);
  tree.erase(214677);
  tree.insert(730690, 5);
  tree.insert(764524, 5);
  tree.erase(349614);
  tree.insert(328205, 5);
  tree.insert(726312, 5);
  tree.insert(565100, 5);
  tree.insert(602726, 5);
  tree.insert(204916, 5);
  tree.insert(325578, 5);
  tree.erase(528946);
  tree.insert(302647, 5);
  tree.insert(799051, 5);
  tree.insert(799631, 5);
  tree.erase(830857);
  tree.erase(541312);
  tree.insert(439214, 5);
  tree.erase(193512);
  tree.insert(14412, 5);
  tree.erase(909610);
  tree.erase(966189);
  tree.insert(806355, 5);
  tree.erase(356620);
  tree.erase(198987);
  tree.insert(498338, 5);
  tree.insert(487770, 5);
  tree.insert(56856, 5);
  tree.erase(300606);
  tree.insert(125849, 5);
  tree.erase(107205);
  tree.erase(35217);
  tree.insert(34945, 5);
  tree.erase(436873);
  tree.insert(710873, 5);
  tree.erase(804289);
  tree.erase(826607);
  tree.insert(772757, 5);
  tree.insert(334471, 5);
  tree.erase(591100);
  tree.erase(723618);
  tree.insert(58025, 5);
  tree.insert(633074, 5);
  tree.erase(518157);
  tree.erase(3493);
  tree.insert(550270, 5);
  tree.erase(633417);
  tree.erase(275569);
  tree.erase(92622);
  tree.insert(413173, 5);
  tree.erase(196431);
  tree.insert(456682, 5);
  tree.insert(504292, 5);
  tree.erase(205057);
  tree.erase(391521);
  tree.erase(888574);
  tree.erase(401947);
  tree.erase(359231);
  tree.erase(610537);
  tree.insert(485054, 5);
  tree.insert(554098, 5);
  tree.erase(241081);
  tree.insert(653516, 5);
  tree.insert(372231, 5);
  tree.erase(261796);
  tree.insert(582338, 5);
  tree.insert(519218, 5);
  tree.erase(513970);
  tree.insert(784812, 5);
  tree.erase(894977);
  tree.erase(31536);
  tree.insert(324176, 5);
  tree.erase(279207);
  tree.erase(984857);
  tree.insert(593499, 5);
  tree.erase(20127);
  tree.insert(505236, 5);
  tree.insert(367818, 5);
  tree.erase(810563);
  tree.erase(421244);
  tree.erase(41805);
  tree.insert(563291, 5);
  tree.erase(558955);
  tree.insert(133589, 5);
  tree.insert(828993, 5);
  tree.insert(552805, 5);
  tree.insert(844822, 5);
  tree.insert(326717, 5);
  tree.insert(593093, 5);
  tree.insert(530126, 5);
  tree.erase(781486);
  tree.erase(850543);
  tree.insert(327814, 5);
  tree.erase(478179);
  tree.erase(474762);
  tree.erase(727088);
  tree.erase(935710);
  tree.insert(110294, 5);
  tree.erase(400346);
  tree.erase(871137);
  tree.erase(305153);
  tree.erase(122573);
  tree.insert(300925, 5);
  tree.erase(306710);
  tree.insert(277217, 5);
  tree.insert(596963, 5);
  tree.erase(387090);
  tree.insert(378130, 5);
  tree.insert(698571, 5);
  tree.erase(369633);
  tree.erase(304789);
  tree.erase(722604);
  tree.erase(419805);
  tree.insert(767868, 5);
  tree.erase(109485);
  tree.insert(82195, 5);
  tree.erase(62949);
  tree.insert(80967, 5);
  tree.insert(686763, 5);
  tree.erase(290596);
  tree.erase(740865);
  tree.insert(539036, 5);
  tree.erase(367770);
  tree.insert(359211, 5);
  tree.insert(322532, 5);
  tree.insert(272379, 5);
  tree.erase(858270);
  tree.insert(384172, 5);
  tree.erase(344234);
  tree.insert(647283, 5);
  tree.insert(307398, 5);
  tree.insert(901063, 5);
  tree.erase(966950);
  tree.insert(250573, 5);
  tree.insert(886059, 5);
  tree.insert(134047, 5);
  tree.insert(945082, 5);
  tree.erase(271232);
  tree.insert(622954, 5);
  tree.erase(411898);
  tree.insert(875640, 5);
  tree.insert(89159, 5);
  tree.insert(679262, 5);
  tree.erase(561041);
  tree.insert(141723, 5);
  tree.insert(26272, 5);
  tree.insert(454154, 5);
  tree.erase(335821);
  tree.erase(909365);
  tree.erase(591171);
  tree.insert(160269, 5);
  tree.insert(938701, 5);
  tree.erase(914653);
  tree.erase(450907);
  tree.erase(356728);
  tree.insert(515797, 5);
  tree.insert(547084, 5);
  tree.insert(515334, 5);
  tree.insert(110991, 5);
  tree.insert(798898, 5);
  tree.erase(801052);
  tree.erase(218189);
  tree.erase(752506);
  tree.insert(709016, 5);
  tree.insert(173109, 5);
  tree.erase(490000);
  tree.insert(58109, 5);
  tree.erase(955081);
  tree.insert(671338, 5);
  tree.erase(59426);
  tree.erase(785147);
  tree.erase(776787);
  tree.erase(696532);
  tree.insert(591281, 5);
  tree.erase(884850);
  tree.erase(576590);
  tree.insert(215350, 5);
  tree.erase(973813);
  tree.erase(381494);
  tree.erase(146081);
  tree.erase(15720);
  tree.erase(887982);
  tree.erase(97487);
  tree.erase(79296);
  tree.erase(765404);
  tree.insert(796892, 5);
  tree.erase(230297);
  tree.insert(399134, 5);
  tree.erase(898506);
  tree.erase(767057);
  tree.insert(380595, 5);
  tree.erase(501962);
  tree.erase(687483);
  tree.insert(80154, 5);
  tree.erase(191309);
  tree.erase(139932);
  tree.insert(895021, 5);
  tree.insert(313563, 5);
  tree.insert(903682, 5);
  tree.erase(277685);
  tree.insert(564285, 5);
  tree.insert(735990, 5);
  tree.erase(197314);
  tree.insert(754116, 5);
  tree.insert(641892, 5);
  tree.erase(395528);
  tree.erase(897525);
  tree.insert(651136, 5);
  tree.insert(889618, 5);
  tree.erase(170337);
  tree.insert(506582, 5);
  tree.erase(804310);
  tree.erase(370888);
  tree.erase(426815);
  tree.insert(543437, 5);
  tree.erase(460008);
  tree.insert(811783, 5);
  tree.insert(418657, 5);
  tree.erase(363827);
  tree.insert(621269, 5);
  tree.erase(726651);
  tree.erase(60910);
  tree.insert(430639, 5);
  tree.insert(241888, 5);
  tree.insert(992393, 5);
  tree.erase(433890);
  tree.insert(755199, 5);
  tree.insert(416931, 5);
  tree.erase(388777);
  tree.erase(400657);
  tree.insert(580952, 5);
  tree.erase(72641);
  tree.erase(89368);
  tree.insert(918184, 5);
  tree.erase(696776);
  tree.erase(975266);
  tree.insert(588954, 5);
  tree.insert(80308, 5);
  tree.erase(297278);
  tree.erase(372555);
  tree.insert(250774, 5);
  tree.erase(305000);
  tree.erase(560997);
  tree.erase(648412);
  tree.erase(598382);
  tree.erase(914693);
  tree.insert(942439, 5);
  tree.insert(88421, 5);
  tree.erase(994985);
  tree.erase(1354);
  tree.erase(578762);
  tree.insert(631541, 5);
  tree.insert(561852, 5);
  tree.insert(703662, 5);
  tree.insert(550399, 5);
  tree.erase(665154);
  tree.erase(399015);
  tree.insert(839851, 5);
  tree.insert(724790, 5);
  tree.erase(942491);
  tree.insert(570037, 5);
  tree.erase(18859);
  tree.insert(360871, 5);
  tree.insert(576987, 5);
  tree.insert(146590, 5);
  tree.erase(563970);
  tree.insert(587665, 5);
  tree.erase(893069);
  tree.erase(907361);
  tree.erase(41351);
  tree.insert(189300, 5);
  tree.insert(291638, 5);
  tree.erase(709364);
  tree.erase(581032);
  tree.insert(136104, 5);
  tree.erase(273679);
  tree.erase(413412);
  tree.insert(734969, 5);
  tree.insert(916170, 5);
  tree.insert(162844, 5);
  tree.insert(406649, 5);
  tree.insert(304465, 5);
  tree.insert(922326, 5);
  tree.insert(660183, 5);
  tree.erase(826969);
  tree.erase(320152);
  tree.erase(924393);
  tree.insert(637289, 5);
  tree.erase(259631);
  tree.erase(584264);
  tree.erase(774548);
  tree.erase(101877);
  tree.erase(666833);
  tree.insert(994949, 5);
  tree.erase(665155);
  tree.erase(678468);
  tree.insert(400960, 5);
  tree.erase(98823);
  tree.insert(213171, 5);
  tree.insert(185677, 5);
  tree.insert(493245, 5);
  tree.erase(572761);
  tree.insert(150323, 5);
  tree.insert(84100, 5);
  tree.insert(461075, 5);
  tree.insert(322042, 5);
  tree.insert(42659, 5);
  tree.erase(456289);
  tree.insert(293469, 5);
  tree.insert(841551, 5);
  tree.insert(125383, 5);
  tree.erase(63133);
  tree.erase(19304);
  tree.insert(365981, 5);
  tree.erase(953666);
  tree.erase(788967);
  tree.insert(90192, 5);
  tree.erase(380902);
  tree.insert(88131, 5);
  tree.insert(683174, 5);
  tree.erase(649718);
  tree.insert(301183, 5);
  tree.erase(945487);
  tree.insert(434573, 5);
  tree.erase(725062);
  tree.erase(713933);
  tree.erase(312496);
  tree.insert(893141, 5);
  tree.erase(971726);
  tree.insert(596980, 5);
  tree.erase(843485);
  tree.insert(372305, 5);
  tree.insert(264029, 5);
  tree.erase(206898);
  tree.insert(734562, 5);
  tree.insert(417719, 5);
  tree.insert(411641, 5);
  tree.insert(593010, 5);
  tree.insert(992726, 5);
  tree.erase(628789);
  tree.insert(303708, 5);
  tree.erase(600938);
  tree.erase(152493);
  tree.erase(980710);
  tree.insert(785905, 5);
  tree.insert(49613, 5);
  tree.erase(963638);
  tree.insert(79421, 5);
  tree.erase(207829);
  tree.erase(96180);
  tree.erase(209095);
  tree.erase(843024);
  tree.insert(749154, 5);
  tree.insert(10569, 5);
  tree.insert(979969, 5);
  tree.insert(492373, 5);
  tree.insert(498433, 5);
  tree.erase(932587);
  tree.erase(620094);
  tree.erase(291499);
  tree.erase(7339);
  tree.insert(551742, 5);
  tree.insert(312086, 5);
  tree.erase(231349);
  tree.erase(950186);
  tree.erase(495011);
  tree.insert(874133, 5);
  tree.erase(812722);
  tree.erase(806773);
  tree.erase(881519);
  tree.insert(495354, 5);
  tree.insert(103124, 5);
  tree.erase(16259);
  tree.erase(677418);
  tree.erase(981712);
  tree.erase(558705);
  tree.insert(342733, 5);
  tree.erase(992734);
  tree.erase(774315);
  tree.erase(691087);
  tree.erase(100669);
  tree.insert(916487, 5);
  tree.insert(556837, 5);
  tree.insert(598089, 5);
  tree.insert(585205, 5);
  tree.insert(666704, 5);
  tree.erase(402557);
  tree.erase(623403);
  tree.insert(321892, 5);
  tree.erase(571522);
  tree.erase(302443);
  tree.erase(325361);
  tree.insert(273378, 5);
  tree.erase(332700);
  tree.erase(574882);
  tree.erase(804899);
  tree.erase(242589);
  tree.insert(650353, 5);
  tree.insert(966948, 5);
  tree.insert(163036, 5);
  tree.insert(277107, 5);
  tree.insert(665417, 5);
  tree.insert(115921, 5);
  tree.insert(98480, 5);
  tree.insert(105994, 5);
  tree.insert(774123, 5);
  tree.erase(832933);
  tree.insert(86317, 5);
  tree.insert(933931, 5);
  tree.erase(186709);
  tree.erase(959156);
  tree.insert(217069, 5);
  tree.erase(712995);
  tree.insert(1171, 5);
  tree.erase(148569);
  tree.erase(264801);
  tree.insert(26652, 5);
  tree.erase(105340);
  tree.erase(251743);
  tree.insert(613091, 5);
  tree.erase(906527);
  tree.insert(798878, 5);
  tree.insert(3050, 5);
  tree.insert(362124, 5);
  tree.erase(304213);
  tree.insert(478499, 5);
  tree.insert(56794, 5);
  tree.insert(465115, 5);
  tree.erase(79342);
  tree.erase(482437);
  tree.erase(663198);
  tree.insert(169939, 5);
  tree.insert(226513, 5);
  tree.erase(865128);
  tree.erase(511804);
  tree.erase(352346);
  tree.erase(898138);
  tree.erase(190495);
  tree.insert(36421, 5);
  tree.insert(387226, 5);
  tree.insert(134158, 5);
  tree.erase(120356);
  tree.insert(77645, 5);
  tree.insert(993446, 5);
  tree.erase(568111);
  tree.erase(417603);
  tree.erase(255825);
  tree.insert(470216, 5);
  tree.erase(379174);
  tree.insert(960596, 5);
  tree.insert(846267, 5);
  tree.insert(342013, 5);
  tree.erase(980519);
  tree.insert(194650, 5);
  tree.insert(117832, 5);
  tree.insert(390279, 5);
  tree.insert(963953, 5);
  tree.erase(959295);
  tree.insert(96107, 5);
  tree.erase(714937);
  tree.insert(944976, 5);
  tree.insert(444584, 5);
  tree.erase(720083);
  tree.insert(199492, 5);
  tree.erase(766496);
  tree.insert(22939, 5);
  tree.erase(505735);
  tree.insert(389873, 5);
  tree.insert(930164, 5);
  tree.erase(52251);
  tree.erase(682751);
  tree.insert(816339, 5);
  tree.insert(953165, 5);
  tree.insert(688302, 5);
  tree.erase(761079);
  tree.erase(262547);
  tree.erase(568484);
  tree.erase(939561);
  tree.erase(621931);
  tree.erase(420528);
  tree.erase(815494);
  tree.erase(517543);
  tree.erase(841123);
  tree.insert(840187, 5);
  tree.erase(524643);
  tree.insert(851988, 5);
  tree.insert(851320, 5);
  tree.insert(854098, 5);
  tree.insert(993018, 5);
  tree.insert(886463, 5);
  tree.insert(494695, 5);
  tree.insert(976505, 5);
  tree.erase(856142);
  tree.insert(868098, 5);
  tree.erase(571472);

  return true;
}

bool
test10() {

  CO_Tree tree;

  CO_Tree::iterator itr = tree.end();

  itr = tree.insert(itr, 1, Coefficient(10));

  itr = tree.end();

  itr = tree.insert(itr, 2, 0);
  itr = tree.insert(itr, 2, 0);
  itr = tree.insert(itr, 4, 0);
  itr = tree.insert(itr, 6, 0);
  itr = tree.insert(itr, 8, 0);
  itr = tree.insert(itr, 10, 0);
  itr = tree.insert(itr, 12, 0);

  if (itr.index() != 12)
    return false;

  --itr;
  --itr;

  itr = tree.insert(itr, 5, 0);
  itr = tree.erase(itr);

  if (itr.index() != 6)
    return false;

  itr = tree.insert(itr, 5, 0);
  itr = tree.erase(itr);

  if (itr.index() != 6)
    return false;

  return true;
}

bool
test11() {

  CO_Tree tree;

  tree.insert(1, 10);

  tree.erase_element_and_shift_left(1);

  if (!tree.empty())
    return false;

  tree.increase_keys_from(1, 5);

  if (!tree.empty())
    return false;

  tree.insert(1, 0);
  tree.insert(2, 0);
  tree.insert(3, 0);
  tree.insert(4, 0);

  tree.increase_keys_from(2, 5);

  CO_Tree::iterator itr = tree.begin();

  if (itr.index() != 1)
    return false;

  ++itr;

  if (itr.index() != 7)
    return false;

  ++itr;

  if (itr.index() != 8)
    return false;

  ++itr;

  if (itr.index() != 9)
    return false;

  ++itr;

  if (itr != tree.end())
    return false;

  return true;
}

bool
test12() {

  CO_Tree tree;

  tree.insert(1, 10);
  tree.insert(2, 10);
  tree.insert(3, 10);

  tree.clear();

  if (!tree.empty())
    return false;

  if (tree.erase(1) != tree.end())
    return false;

  CO_Tree::iterator itr = tree.end();

  if (tree.bisect_near(itr, 1) != tree.end())
    return false;

  CO_Tree::const_iterator citr = tree.cend();

  if (tree.bisect_near(citr, 1) != tree.cend())
    return false;

  if (static_cast<const CO_Tree&>(tree).bisect(1) != tree.end())
    return false;

  return true;
}

bool
test13() {

  CO_Tree tree;

  tree.insert(2, 10);
  tree.insert(2, 10);
  tree.insert(4, 10);
  tree.insert(6, 10);
  tree.insert(8, 10);
  tree.insert(10, 10);
  tree.insert(12, 10);
  tree.insert(14, 10);

  CO_Tree::iterator itr;

  itr = tree.bisect(12);

  if (itr.index() != 12)
    return false;

  itr = tree.bisect_near(itr, 4);

  if (itr.index() != 4)
    return false;

  itr = tree.bisect(12);
  itr = tree.bisect_near(itr, 10);

  if (itr.index() != 10)
    return false;

  return true;
}

bool
test14() {

  // Iterating on an empty tree.

  CO_Tree tree;

  if (tree.begin() != tree.end())
    return false;

  if (tree.cbegin() != tree.cend())
    return false;

  tree.erase(1);
  tree.bisect(1);
  static_cast<const CO_Tree&>(tree).bisect(1);

  return true;
}

bool
test15() {

  // Test iterator::swap(), const_iterator::swap(),
  // iterator::operator*() and const_iterator::operator*().

  CO_Tree tree;

  tree.insert(1, 1);
  tree.insert(2, 2);

  CO_Tree::iterator itr1 = tree.bisect(1);
  CO_Tree::iterator itr2 = tree.bisect(2);

  swap(itr1, itr2);

  if (itr2.index() != 1)
    return false;

  if (*itr2 != 1)
    return false;

  if (itr1.index() != 2)
    return false;

  if (*itr1 != 2)
    return false;

  CO_Tree::const_iterator itr3 = tree.bisect(1);
  CO_Tree::const_iterator itr4 = tree.bisect(2);

  swap(itr3, itr4);

  if (itr4.index() != 1)
    return false;

  if (*itr4 != 1)
    return false;

  if (itr3.index() != 2)
    return false;

  if (*itr3 != 2)
    return false;

  return true;
}

bool
test16() {

  // Test iterators' and const_iterators' postfix increment and decrement
  // operators.

  CO_Tree tree;

  tree.insert(1, 0);
  tree.insert(2, 0);
  tree.insert(3, 0);

  CO_Tree::iterator itr = tree.bisect(1);

  itr++;

  if (itr.index() != 2)
    return false;

  itr++;

  if (itr.index() != 3)
    return false;

  itr--;

  if (itr.index() != 2)
    return false;

  itr--;

  if (itr.index() != 1)
    return false;

  CO_Tree::const_iterator itr2 = tree.bisect(1);

  itr2++;

  if (itr2.index() != 2)
    return false;

  itr2++;

  if (itr2.index() != 3)
    return false;

  itr2--;

  if (itr2.index() != 2)
    return false;

  itr2--;

  if (itr2.index() != 1)
    return false;

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
END_MAIN

#else // !PPL_USE_SPARSE_MATRIX

// A fake test to avoid compiler warnings in BEGIN_MAIN.
bool test01() {
  return true;
}

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN

#endif // !PPL_USE_SPARSE_MATRIX
