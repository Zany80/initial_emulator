opcodesFD=new opcode[256]{
	//0x00
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::addIYRR,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0x10
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::addIYRR,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0x20
	&Z80::nop,&Z80::ldIYNN,&Z80::ld_NN_IY,&Z80::incIY,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::addIYRR,&Z80::ldIY_NN_,&Z80::decIY,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0x30
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::inc_IYd_,&Z80::dec_IYd_,&Z80::ld_IYd_N,&Z80::nop,
	&Z80::nop,&Z80::addIYRR	,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0x40
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0x50
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::ldR_IYd_,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::ldR_IYd_,&Z80::nop,
	//0x60
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::ldR_IYd_,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::ldR_IYd_,&Z80::nop,
	//0x70
	&Z80::ld_IYd_R,&Z80::ld_IYd_R,&Z80::ld_IYd_R,&Z80::ld_IYd_R,&Z80::ld_IYd_R,&Z80::ld_IYd_R,&Z80::nop,&Z80::ld_IYd_R,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::ldR_IYd_,&Z80::nop,
	//0x80
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::addA_IYd_,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::adcA_IYd_,&Z80::nop,
	//0x90
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::sub_IYd_,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::sbcA_IYd_,&Z80::nop,
	//0xA0
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::and_IYd_,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::xor_IYd_,&Z80::nop,
	//0xB0
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::or_IYd_,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::cp_IYd_,&Z80::nop,
	//0xC0
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::FDCB,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0xD0
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0xE0
	&Z80::nop,&Z80::popIY,&Z80::nop,&Z80::ex_SP_IY,&Z80::nop,&Z80::pushIY,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	//0xF0
	&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
	&Z80::nop,&Z80::ldSPIY,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,&Z80::nop,
};
