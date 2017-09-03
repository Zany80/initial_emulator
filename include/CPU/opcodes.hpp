//prefixes
void DD(uint8_t opcode);
void FD(uint8_t opcode);
void ED(uint8_t opcode);
//8-bit load group
void nop(uint8_t opcode);
void ldRRx(uint8_t opcode);
void ldRN(uint8_t opcode);
void ldR_HL_(uint8_t opcode);
void ldR_IXd_(uint8_t opcode);
void ldR_IYd_(uint8_t opcode);
void ld_HL_R(uint8_t opcode);
void ld_IXd_R(uint8_t opcode);
void ld_IYd_R(uint8_t opcode);
void ld_HL_N(uint8_t opcode);
void ld_IXd_N(uint8_t opcode);
void ld_IYd_N(uint8_t opcode);
void ldA_BC_(uint8_t opcode);
void ldA_DE_(uint8_t opcode);
void ldA_NN_(uint8_t opcode);
void ld_BC_A(uint8_t opcode);
void ld_DE_A(uint8_t opcode);
void ld_NN_A(uint8_t opcode);
void ldAI(uint8_t opcode);
void ldAR(uint8_t opcode);
void ldIA(uint8_t opcode);
void ldRA(uint8_t opcode);
//16-bit load group
void ldDDNN(uint8_t opcode);
void ldIXNN(uint8_t opcode);
void ldIYNN(uint8_t opcode);
void ldHL_NN_(uint8_t opcode);
void ldDD_NN_(uint8_t opcode);
void ldIX_NN_(uint8_t opcode);
void ldIY_NN_(uint8_t opcode);
void ld_NN_HL(uint8_t opcode);
void ld_NN_DD(uint8_t opcode);
void ld_NN_IX(uint8_t opcode);
void ld_NN_IY(uint8_t opcode);
void ldSPHL(uint8_t opcode);
void ldSPIX(uint8_t opcode);
void ldSPIY(uint8_t opcode);
void pushQQ(uint8_t opcode);
void pushIX(uint8_t opcode);
void pushIY(uint8_t opcode);
void popQQ(uint8_t opcode);
void popIX(uint8_t opcode);
void popIY(uint8_t opcode);
//exchange, block transfer, and search group
void exDEHL(uint8_t opcode);
void exAFAF2(uint8_t opcode);
void exx(uint8_t opcode);
void ex_SP_HL(uint8_t opcode);
void ex_SP_IX(uint8_t opcode);
void ex_SP_IY(uint8_t opcode);
void ldi(uint8_t opcode);
void ldir(uint8_t opcode);
void ldd(uint8_t opcode);
void lddr(uint8_t opcode);
void cpi(uint8_t opcode);
void cpir(uint8_t opcode);
void cpd(uint8_t opcode);
void cpdr(uint8_t opcode);
//general purpose arithmetic and cpu control group
void halt(uint8_t opcode);
//input and output group
void out_N_A(uint8_t opcode);
//convenience functions - not opcodes per se, but used exclusively by them to prevent typing the same commands repeatedly
//these don't have to have the signature `void opcode(uint8_t)`.
inline void push(uint16_t value);
inline uint16_t pop();
inline void cp(uint8_t value);
