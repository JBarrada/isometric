#include <stdint.h>
#include <idt.h>
#include <io.h>
#include <pic.h>
#include <keyboard.h>
#include <pit.h>

static uint32_t idt_location = 0;
static uint32_t idtr_location = 0;
static uint16_t idt_size = 0x800;

static uint8_t test_success = 0;
static uint32_t test_timeout = 0x1000;

static uint8_t __idt_setup = 0;

void idt_init()
{
	idt_location = 0x2000;
	idtr_location = 0x10F0;
	__idt_setup = 1;

	initalize_idt();
	
	*(uint16_t*)idtr_location = idt_size - 1;
	*(uint32_t*)(idtr_location + 2) = idt_location;
	_set_idtr();
	
	
	// consider getting rid of interrupt test 
	asm volatile("int $0x30");
	while(test_timeout-- != 0)
	{
		if(test_success != 0)
			break;
	}
	return;
}

void generic_interrupt_handler(struct cpu_state s) {
	if(s.vector<20) {
		// exception
	}

	if(s.vector==32) {
		pit_irq();
	}
	
	if(s.vector==33) {
		keyboard_irq();
	}
	
	if(s.vector==0x30) {
		test_success = 1;
	}
	
	pic_send_eoi(s.vector);
}

void idt_register_interrupt(uint8_t i, uint32_t callback)
{
	*(uint16_t*)(idt_location + 8*i + 0) = (uint16_t)(callback & 0x0000ffff);
	*(uint16_t*)(idt_location + 8*i + 2) = (uint16_t)0x8;
	*(uint8_t*) (idt_location + 8*i + 4) = 0x00;
	*(uint8_t*) (idt_location + 8*i + 5) = 0x8e; // 0 | IDT_32BIT_INTERRUPT_GATE | IDT_PRESENT;
	*(uint16_t*)(idt_location + 8*i + 6) = (uint16_t)((callback & 0xffff0000) >> 16);
	
	return;
}

void initalize_idt(){
	idt_register_interrupt(0, (uint32_t)&interrupt_wrapper_0);
	idt_register_interrupt(1, (uint32_t)&interrupt_wrapper_1);
	idt_register_interrupt(2, (uint32_t)&interrupt_wrapper_2);
	idt_register_interrupt(3, (uint32_t)&interrupt_wrapper_3);
	idt_register_interrupt(4, (uint32_t)&interrupt_wrapper_4);
	idt_register_interrupt(5, (uint32_t)&interrupt_wrapper_5);
	idt_register_interrupt(6, (uint32_t)&interrupt_wrapper_6);
	idt_register_interrupt(7, (uint32_t)&interrupt_wrapper_7);
	idt_register_interrupt(8, (uint32_t)&interrupt_wrapper_8);
	idt_register_interrupt(9, (uint32_t)&interrupt_wrapper_9);
	idt_register_interrupt(10, (uint32_t)&interrupt_wrapper_10);
	idt_register_interrupt(11, (uint32_t)&interrupt_wrapper_11);
	idt_register_interrupt(12, (uint32_t)&interrupt_wrapper_12);
	idt_register_interrupt(13, (uint32_t)&interrupt_wrapper_13);
	idt_register_interrupt(14, (uint32_t)&interrupt_wrapper_14);
	idt_register_interrupt(15, (uint32_t)&interrupt_wrapper_15);
	idt_register_interrupt(16, (uint32_t)&interrupt_wrapper_16);
	idt_register_interrupt(17, (uint32_t)&interrupt_wrapper_17);
	idt_register_interrupt(18, (uint32_t)&interrupt_wrapper_18);
	idt_register_interrupt(19, (uint32_t)&interrupt_wrapper_19);
	idt_register_interrupt(20, (uint32_t)&interrupt_wrapper_20);
	idt_register_interrupt(21, (uint32_t)&interrupt_wrapper_21);
	idt_register_interrupt(22, (uint32_t)&interrupt_wrapper_22);
	idt_register_interrupt(23, (uint32_t)&interrupt_wrapper_23);
	idt_register_interrupt(24, (uint32_t)&interrupt_wrapper_24);
	idt_register_interrupt(25, (uint32_t)&interrupt_wrapper_25);
	idt_register_interrupt(26, (uint32_t)&interrupt_wrapper_26);
	idt_register_interrupt(27, (uint32_t)&interrupt_wrapper_27);
	idt_register_interrupt(28, (uint32_t)&interrupt_wrapper_28);
	idt_register_interrupt(29, (uint32_t)&interrupt_wrapper_29);
	idt_register_interrupt(30, (uint32_t)&interrupt_wrapper_30);
	idt_register_interrupt(31, (uint32_t)&interrupt_wrapper_31);
	idt_register_interrupt(32, (uint32_t)&interrupt_wrapper_32);
	idt_register_interrupt(33, (uint32_t)&interrupt_wrapper_33);
	idt_register_interrupt(34, (uint32_t)&interrupt_wrapper_34);
	idt_register_interrupt(35, (uint32_t)&interrupt_wrapper_35);
	idt_register_interrupt(36, (uint32_t)&interrupt_wrapper_36);
	idt_register_interrupt(37, (uint32_t)&interrupt_wrapper_37);
	idt_register_interrupt(38, (uint32_t)&interrupt_wrapper_38);
	idt_register_interrupt(39, (uint32_t)&interrupt_wrapper_39);
	idt_register_interrupt(40, (uint32_t)&interrupt_wrapper_40);
	idt_register_interrupt(41, (uint32_t)&interrupt_wrapper_41);
	idt_register_interrupt(42, (uint32_t)&interrupt_wrapper_42);
	idt_register_interrupt(43, (uint32_t)&interrupt_wrapper_43);
	idt_register_interrupt(44, (uint32_t)&interrupt_wrapper_44);
	idt_register_interrupt(45, (uint32_t)&interrupt_wrapper_45);
	idt_register_interrupt(46, (uint32_t)&interrupt_wrapper_46);
	idt_register_interrupt(47, (uint32_t)&interrupt_wrapper_47);
	idt_register_interrupt(48, (uint32_t)&interrupt_wrapper_48);
	idt_register_interrupt(49, (uint32_t)&interrupt_wrapper_49);
	idt_register_interrupt(50, (uint32_t)&interrupt_wrapper_50);
	idt_register_interrupt(51, (uint32_t)&interrupt_wrapper_51);
	idt_register_interrupt(52, (uint32_t)&interrupt_wrapper_52);
	idt_register_interrupt(53, (uint32_t)&interrupt_wrapper_53);
	idt_register_interrupt(54, (uint32_t)&interrupt_wrapper_54);
	idt_register_interrupt(55, (uint32_t)&interrupt_wrapper_55);
	idt_register_interrupt(56, (uint32_t)&interrupt_wrapper_56);
	idt_register_interrupt(57, (uint32_t)&interrupt_wrapper_57);
	idt_register_interrupt(58, (uint32_t)&interrupt_wrapper_58);
	idt_register_interrupt(59, (uint32_t)&interrupt_wrapper_59);
	idt_register_interrupt(60, (uint32_t)&interrupt_wrapper_60);
	idt_register_interrupt(61, (uint32_t)&interrupt_wrapper_61);
	idt_register_interrupt(62, (uint32_t)&interrupt_wrapper_62);
	idt_register_interrupt(63, (uint32_t)&interrupt_wrapper_63);
	idt_register_interrupt(64, (uint32_t)&interrupt_wrapper_64);
	idt_register_interrupt(65, (uint32_t)&interrupt_wrapper_65);
	idt_register_interrupt(66, (uint32_t)&interrupt_wrapper_66);
	idt_register_interrupt(67, (uint32_t)&interrupt_wrapper_67);
	idt_register_interrupt(68, (uint32_t)&interrupt_wrapper_68);
	idt_register_interrupt(69, (uint32_t)&interrupt_wrapper_69);
	idt_register_interrupt(70, (uint32_t)&interrupt_wrapper_70);
	idt_register_interrupt(71, (uint32_t)&interrupt_wrapper_71);
	idt_register_interrupt(72, (uint32_t)&interrupt_wrapper_72);
	idt_register_interrupt(73, (uint32_t)&interrupt_wrapper_73);
	idt_register_interrupt(74, (uint32_t)&interrupt_wrapper_74);
	idt_register_interrupt(75, (uint32_t)&interrupt_wrapper_75);
	idt_register_interrupt(76, (uint32_t)&interrupt_wrapper_76);
	idt_register_interrupt(77, (uint32_t)&interrupt_wrapper_77);
	idt_register_interrupt(78, (uint32_t)&interrupt_wrapper_78);
	idt_register_interrupt(79, (uint32_t)&interrupt_wrapper_79);
	idt_register_interrupt(80, (uint32_t)&interrupt_wrapper_80);
	idt_register_interrupt(81, (uint32_t)&interrupt_wrapper_81);
	idt_register_interrupt(82, (uint32_t)&interrupt_wrapper_82);
	idt_register_interrupt(83, (uint32_t)&interrupt_wrapper_83);
	idt_register_interrupt(84, (uint32_t)&interrupt_wrapper_84);
	idt_register_interrupt(85, (uint32_t)&interrupt_wrapper_85);
	idt_register_interrupt(86, (uint32_t)&interrupt_wrapper_86);
	idt_register_interrupt(87, (uint32_t)&interrupt_wrapper_87);
	idt_register_interrupt(88, (uint32_t)&interrupt_wrapper_88);
	idt_register_interrupt(89, (uint32_t)&interrupt_wrapper_89);
	idt_register_interrupt(90, (uint32_t)&interrupt_wrapper_90);
	idt_register_interrupt(91, (uint32_t)&interrupt_wrapper_91);
	idt_register_interrupt(92, (uint32_t)&interrupt_wrapper_92);
	idt_register_interrupt(93, (uint32_t)&interrupt_wrapper_93);
	idt_register_interrupt(94, (uint32_t)&interrupt_wrapper_94);
	idt_register_interrupt(95, (uint32_t)&interrupt_wrapper_95);
	idt_register_interrupt(96, (uint32_t)&interrupt_wrapper_96);
	idt_register_interrupt(97, (uint32_t)&interrupt_wrapper_97);
	idt_register_interrupt(98, (uint32_t)&interrupt_wrapper_98);
	idt_register_interrupt(99, (uint32_t)&interrupt_wrapper_99);
	idt_register_interrupt(100, (uint32_t)&interrupt_wrapper_100);
	idt_register_interrupt(101, (uint32_t)&interrupt_wrapper_101);
	idt_register_interrupt(102, (uint32_t)&interrupt_wrapper_102);
	idt_register_interrupt(103, (uint32_t)&interrupt_wrapper_103);
	idt_register_interrupt(104, (uint32_t)&interrupt_wrapper_104);
	idt_register_interrupt(105, (uint32_t)&interrupt_wrapper_105);
	idt_register_interrupt(106, (uint32_t)&interrupt_wrapper_106);
	idt_register_interrupt(107, (uint32_t)&interrupt_wrapper_107);
	idt_register_interrupt(108, (uint32_t)&interrupt_wrapper_108);
	idt_register_interrupt(109, (uint32_t)&interrupt_wrapper_109);
	idt_register_interrupt(110, (uint32_t)&interrupt_wrapper_110);
	idt_register_interrupt(111, (uint32_t)&interrupt_wrapper_111);
	idt_register_interrupt(112, (uint32_t)&interrupt_wrapper_112);
	idt_register_interrupt(113, (uint32_t)&interrupt_wrapper_113);
	idt_register_interrupt(114, (uint32_t)&interrupt_wrapper_114);
	idt_register_interrupt(115, (uint32_t)&interrupt_wrapper_115);
	idt_register_interrupt(116, (uint32_t)&interrupt_wrapper_116);
	idt_register_interrupt(117, (uint32_t)&interrupt_wrapper_117);
	idt_register_interrupt(118, (uint32_t)&interrupt_wrapper_118);
	idt_register_interrupt(119, (uint32_t)&interrupt_wrapper_119);
	idt_register_interrupt(120, (uint32_t)&interrupt_wrapper_120);
	idt_register_interrupt(121, (uint32_t)&interrupt_wrapper_121);
	idt_register_interrupt(122, (uint32_t)&interrupt_wrapper_122);
	idt_register_interrupt(123, (uint32_t)&interrupt_wrapper_123);
	idt_register_interrupt(124, (uint32_t)&interrupt_wrapper_124);
	idt_register_interrupt(125, (uint32_t)&interrupt_wrapper_125);
	idt_register_interrupt(126, (uint32_t)&interrupt_wrapper_126);
	idt_register_interrupt(127, (uint32_t)&interrupt_wrapper_127);
	idt_register_interrupt(128, (uint32_t)&interrupt_wrapper_128);
	idt_register_interrupt(129, (uint32_t)&interrupt_wrapper_129);
	idt_register_interrupt(130, (uint32_t)&interrupt_wrapper_130);
	idt_register_interrupt(131, (uint32_t)&interrupt_wrapper_131);
	idt_register_interrupt(132, (uint32_t)&interrupt_wrapper_132);
	idt_register_interrupt(133, (uint32_t)&interrupt_wrapper_133);
	idt_register_interrupt(134, (uint32_t)&interrupt_wrapper_134);
	idt_register_interrupt(135, (uint32_t)&interrupt_wrapper_135);
	idt_register_interrupt(136, (uint32_t)&interrupt_wrapper_136);
	idt_register_interrupt(137, (uint32_t)&interrupt_wrapper_137);
	idt_register_interrupt(138, (uint32_t)&interrupt_wrapper_138);
	idt_register_interrupt(139, (uint32_t)&interrupt_wrapper_139);
	idt_register_interrupt(140, (uint32_t)&interrupt_wrapper_140);
	idt_register_interrupt(141, (uint32_t)&interrupt_wrapper_141);
	idt_register_interrupt(142, (uint32_t)&interrupt_wrapper_142);
	idt_register_interrupt(143, (uint32_t)&interrupt_wrapper_143);
	idt_register_interrupt(144, (uint32_t)&interrupt_wrapper_144);
	idt_register_interrupt(145, (uint32_t)&interrupt_wrapper_145);
	idt_register_interrupt(146, (uint32_t)&interrupt_wrapper_146);
	idt_register_interrupt(147, (uint32_t)&interrupt_wrapper_147);
	idt_register_interrupt(148, (uint32_t)&interrupt_wrapper_148);
	idt_register_interrupt(149, (uint32_t)&interrupt_wrapper_149);
	idt_register_interrupt(150, (uint32_t)&interrupt_wrapper_150);
	idt_register_interrupt(151, (uint32_t)&interrupt_wrapper_151);
	idt_register_interrupt(152, (uint32_t)&interrupt_wrapper_152);
	idt_register_interrupt(153, (uint32_t)&interrupt_wrapper_153);
	idt_register_interrupt(154, (uint32_t)&interrupt_wrapper_154);
	idt_register_interrupt(155, (uint32_t)&interrupt_wrapper_155);
	idt_register_interrupt(156, (uint32_t)&interrupt_wrapper_156);
	idt_register_interrupt(157, (uint32_t)&interrupt_wrapper_157);
	idt_register_interrupt(158, (uint32_t)&interrupt_wrapper_158);
	idt_register_interrupt(159, (uint32_t)&interrupt_wrapper_159);
	idt_register_interrupt(160, (uint32_t)&interrupt_wrapper_160);
	idt_register_interrupt(161, (uint32_t)&interrupt_wrapper_161);
	idt_register_interrupt(162, (uint32_t)&interrupt_wrapper_162);
	idt_register_interrupt(163, (uint32_t)&interrupt_wrapper_163);
	idt_register_interrupt(164, (uint32_t)&interrupt_wrapper_164);
	idt_register_interrupt(165, (uint32_t)&interrupt_wrapper_165);
	idt_register_interrupt(166, (uint32_t)&interrupt_wrapper_166);
	idt_register_interrupt(167, (uint32_t)&interrupt_wrapper_167);
	idt_register_interrupt(168, (uint32_t)&interrupt_wrapper_168);
	idt_register_interrupt(169, (uint32_t)&interrupt_wrapper_169);
	idt_register_interrupt(170, (uint32_t)&interrupt_wrapper_170);
	idt_register_interrupt(171, (uint32_t)&interrupt_wrapper_171);
	idt_register_interrupt(172, (uint32_t)&interrupt_wrapper_172);
	idt_register_interrupt(173, (uint32_t)&interrupt_wrapper_173);
	idt_register_interrupt(174, (uint32_t)&interrupt_wrapper_174);
	idt_register_interrupt(175, (uint32_t)&interrupt_wrapper_175);
	idt_register_interrupt(176, (uint32_t)&interrupt_wrapper_176);
	idt_register_interrupt(177, (uint32_t)&interrupt_wrapper_177);
	idt_register_interrupt(178, (uint32_t)&interrupt_wrapper_178);
	idt_register_interrupt(179, (uint32_t)&interrupt_wrapper_179);
	idt_register_interrupt(180, (uint32_t)&interrupt_wrapper_180);
	idt_register_interrupt(181, (uint32_t)&interrupt_wrapper_181);
	idt_register_interrupt(182, (uint32_t)&interrupt_wrapper_182);
	idt_register_interrupt(183, (uint32_t)&interrupt_wrapper_183);
	idt_register_interrupt(184, (uint32_t)&interrupt_wrapper_184);
	idt_register_interrupt(185, (uint32_t)&interrupt_wrapper_185);
	idt_register_interrupt(186, (uint32_t)&interrupt_wrapper_186);
	idt_register_interrupt(187, (uint32_t)&interrupt_wrapper_187);
	idt_register_interrupt(188, (uint32_t)&interrupt_wrapper_188);
	idt_register_interrupt(189, (uint32_t)&interrupt_wrapper_189);
	idt_register_interrupt(190, (uint32_t)&interrupt_wrapper_190);
	idt_register_interrupt(191, (uint32_t)&interrupt_wrapper_191);
	idt_register_interrupt(192, (uint32_t)&interrupt_wrapper_192);
	idt_register_interrupt(193, (uint32_t)&interrupt_wrapper_193);
	idt_register_interrupt(194, (uint32_t)&interrupt_wrapper_194);
	idt_register_interrupt(195, (uint32_t)&interrupt_wrapper_195);
	idt_register_interrupt(196, (uint32_t)&interrupt_wrapper_196);
	idt_register_interrupt(197, (uint32_t)&interrupt_wrapper_197);
	idt_register_interrupt(198, (uint32_t)&interrupt_wrapper_198);
	idt_register_interrupt(199, (uint32_t)&interrupt_wrapper_199);
	idt_register_interrupt(200, (uint32_t)&interrupt_wrapper_200);
	idt_register_interrupt(201, (uint32_t)&interrupt_wrapper_201);
	idt_register_interrupt(202, (uint32_t)&interrupt_wrapper_202);
	idt_register_interrupt(203, (uint32_t)&interrupt_wrapper_203);
	idt_register_interrupt(204, (uint32_t)&interrupt_wrapper_204);
	idt_register_interrupt(205, (uint32_t)&interrupt_wrapper_205);
	idt_register_interrupt(206, (uint32_t)&interrupt_wrapper_206);
	idt_register_interrupt(207, (uint32_t)&interrupt_wrapper_207);
	idt_register_interrupt(208, (uint32_t)&interrupt_wrapper_208);
	idt_register_interrupt(209, (uint32_t)&interrupt_wrapper_209);
	idt_register_interrupt(210, (uint32_t)&interrupt_wrapper_210);
	idt_register_interrupt(211, (uint32_t)&interrupt_wrapper_211);
	idt_register_interrupt(212, (uint32_t)&interrupt_wrapper_212);
	idt_register_interrupt(213, (uint32_t)&interrupt_wrapper_213);
	idt_register_interrupt(214, (uint32_t)&interrupt_wrapper_214);
	idt_register_interrupt(215, (uint32_t)&interrupt_wrapper_215);
	idt_register_interrupt(216, (uint32_t)&interrupt_wrapper_216);
	idt_register_interrupt(217, (uint32_t)&interrupt_wrapper_217);
	idt_register_interrupt(218, (uint32_t)&interrupt_wrapper_218);
	idt_register_interrupt(219, (uint32_t)&interrupt_wrapper_219);
	idt_register_interrupt(220, (uint32_t)&interrupt_wrapper_220);
	idt_register_interrupt(223, (uint32_t)&interrupt_wrapper_223);
	idt_register_interrupt(224, (uint32_t)&interrupt_wrapper_224);
	idt_register_interrupt(225, (uint32_t)&interrupt_wrapper_225);
	idt_register_interrupt(226, (uint32_t)&interrupt_wrapper_226);
	idt_register_interrupt(227, (uint32_t)&interrupt_wrapper_227);
	idt_register_interrupt(228, (uint32_t)&interrupt_wrapper_228);
	idt_register_interrupt(229, (uint32_t)&interrupt_wrapper_229);
	idt_register_interrupt(230, (uint32_t)&interrupt_wrapper_230);
	idt_register_interrupt(231, (uint32_t)&interrupt_wrapper_231);
	idt_register_interrupt(232, (uint32_t)&interrupt_wrapper_232);
	idt_register_interrupt(233, (uint32_t)&interrupt_wrapper_233);
	idt_register_interrupt(234, (uint32_t)&interrupt_wrapper_234);
	idt_register_interrupt(235, (uint32_t)&interrupt_wrapper_235);
	idt_register_interrupt(236, (uint32_t)&interrupt_wrapper_236);
	idt_register_interrupt(237, (uint32_t)&interrupt_wrapper_237);
	idt_register_interrupt(238, (uint32_t)&interrupt_wrapper_238);
	idt_register_interrupt(239, (uint32_t)&interrupt_wrapper_239);
	idt_register_interrupt(240, (uint32_t)&interrupt_wrapper_240);
	idt_register_interrupt(241, (uint32_t)&interrupt_wrapper_241);
	idt_register_interrupt(242, (uint32_t)&interrupt_wrapper_242);
	idt_register_interrupt(243, (uint32_t)&interrupt_wrapper_243);
	idt_register_interrupt(244, (uint32_t)&interrupt_wrapper_244);
	idt_register_interrupt(245, (uint32_t)&interrupt_wrapper_245);
	idt_register_interrupt(246, (uint32_t)&interrupt_wrapper_246);
	idt_register_interrupt(247, (uint32_t)&interrupt_wrapper_247);
	idt_register_interrupt(248, (uint32_t)&interrupt_wrapper_248);
	idt_register_interrupt(249, (uint32_t)&interrupt_wrapper_249);
	idt_register_interrupt(250, (uint32_t)&interrupt_wrapper_250);
	idt_register_interrupt(251, (uint32_t)&interrupt_wrapper_251);
	idt_register_interrupt(252, (uint32_t)&interrupt_wrapper_252);
	idt_register_interrupt(253, (uint32_t)&interrupt_wrapper_253);
	idt_register_interrupt(254, (uint32_t)&interrupt_wrapper_254);
	idt_register_interrupt(255, (uint32_t)&interrupt_wrapper_255);
}
