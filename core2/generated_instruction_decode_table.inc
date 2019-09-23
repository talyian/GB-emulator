// ['opcode', 'bytes', 'duration', 'flags', 'code']
// ['0', '1', '4', '----', 'NOP()']
// ['1', '3', '12', '----', 'LD(BC,U16)']
// ['2', '1', '8', '----', 'LD(Load(BC),A)']
// ['3', '1', '8', '----', 'INC(BC)']
// ['4', '1', '4', 'Z0H-', 'INC(B)']
// ['5', '1', '4', 'Z1H-', 'DEC(B)']
// ['6', '2', '8', '----', 'LD(B,U8)']
// ['7', '1', '4', '000C', '{ii.RLC(A);registers.setFZ(0);}']
// ['8', '3', '20', '----', 'LD(Load16(U16),SP)']
// ['9', '1', '8', '-0HC', 'ADD(HL,BC)']
// ['A', '1', '8', '----', 'LD(A,Load(BC))']
// ['B', '1', '8', '----', 'DEC(BC)']
// ['C', '1', '4', 'Z0H-', 'INC(C)']
// ['D', '1', '4', 'Z1H-', 'DEC(C)']
// ['E', '2', '8', '----', 'LD(C,U8)']
// ['F', '1', '4', '000C', '{ii.RRC(A);registers.setFZ(0);}']
// ['10', '2', '4', '----', 'STOP()']
// ['11', '3', '12', '----', 'LD(DE,U16)']
// ['12', '1', '8', '----', 'LD(Load(DE),A)']
// ['13', '1', '8', '----', 'INC(DE)']
// ['14', '1', '4', 'Z0H-', 'INC(D)']
// ['15', '1', '4', 'Z1H-', 'DEC(D)']
// ['16', '2', '8', '----', 'LD(D,U8)']
// ['17', '1', '4', '000C', '{ii.RL(A);registers.setFZ(0);}']
// ['18', '2', '12', '----', 'JR(CT,I8)']
// ['19', '1', '8', '-0HC', 'ADD(HL,DE)']
// ['1A', '1', '8', '----', 'LD(A,Load(DE))']
// ['1B', '1', '8', '----', 'DEC(DE)']
// ['1C', '1', '4', 'Z0H-', 'INC(E)']
// ['1D', '1', '4', 'Z1H-', 'DEC(E)']
// ['1E', '2', '8', '----', 'LD(E,U8)']
// ['1F', '1', '4', '000C', '{ii.RR(A);registers.setFZ(0);}']
// ['20', '2', '12/8', '----', 'JR(CNZ,I8)']
// ['21', '3', '12', '----', 'LD(HL,U16)']
// ['22', '1', '8', '----', '{ii.LD(Load(HL),A);ii.INC(HL);}']
// ['23', '1', '8', '----', 'INC(HL)']
// ['24', '1', '4', 'Z0H-', 'INC(H)']
// ['25', '1', '4', 'Z1H-', 'DEC(H)']
// ['26', '2', '8', '----', 'LD(H,U8)']
// ['27', '1', '4', 'Z-0C', 'DAA()']
// ['28', '2', '12/8', '----', 'JR(CZ,I8)']
// ['29', '1', '8', '-0HC', 'ADD(HL,HL)']
// ['2A', '1', '8', '----', '{ii.LD(A,Load(HL));ii.INC(HL);}']
// ['2B', '1', '8', '----', 'DEC(HL)']
// ['2C', '1', '4', 'Z0H-', 'INC(L)']
// ['2D', '1', '4', 'Z1H-', 'DEC(L)']
// ['2E', '2', '8', '----', 'LD(L,U8)']
// ['2F', '1', '4', '-11-', 'CPL()']
// ['30', '2', '12/8', '----', 'JR(CNC,I8)']
// ['31', '3', '12', '----', 'LD(SP,U16)']
// ['32', '1', '8', '----', '{ii.LD(Load(HL),A);ii.DEC(HL);}']
// ['33', '1', '8', '----', 'INC(SP)']
// ['34', '1', '12', 'Z0H-', 'INC(Load(HL))']
// ['35', '1', '12', 'Z1H-', 'DEC(Load(HL))']
// ['36', '2', '12', '----', 'LD(Load(HL),U8)']
// ['37', '1', '4', '-001', 'SCF()']
// ['38', '2', '12/8', '----', 'JR(CC,I8)']
// ['39', '1', '8', '-0HC', 'ADD(HL,SP)']
// ['3A', '1', '8', '----', '{ii.LD(A,Load(HL));ii.DEC(HL);}/*LD(A,Load(HLDEC))*/']
// ['3B', '1', '8', '----', 'DEC(SP)']
// ['3C', '1', '4', 'Z0H-', 'INC(A)']
// ['3D', '1', '4', 'Z1H-', 'DEC(A)']
// ['3E', '2', '8', '----', 'LD(A,U8)']
// ['3F', '1', '4', '-00C', 'CCF()']
// ['40', '1', '4', '----', 'LD(B,B)']
// ['41', '1', '4', '----', 'LD(B,C)']
// ['42', '1', '4', '----', 'LD(B,D)']
// ['43', '1', '4', '----', 'LD(B,E)']
// ['44', '1', '4', '----', 'LD(B,H)']
// ['45', '1', '4', '----', 'LD(B,L)']
// ['46', '1', '8', '----', 'LD(B,Load(HL))']
// ['47', '1', '4', '----', 'LD(B,A)']
// ['48', '1', '4', '----', 'LD(C,B)']
// ['49', '1', '4', '----', 'LD(C,C)']
// ['4A', '1', '4', '----', 'LD(C,D)']
// ['4B', '1', '4', '----', 'LD(C,E)']
// ['4C', '1', '4', '----', 'LD(C,H)']
// ['4D', '1', '4', '----', 'LD(C,L)']
// ['4E', '1', '8', '----', 'LD(C,Load(HL))']
// ['4F', '1', '4', '----', 'LD(C,A)']
// ['50', '1', '4', '----', 'LD(D,B)']
// ['51', '1', '4', '----', 'LD(D,C)']
// ['52', '1', '4', '----', 'LD(D,D)']
// ['53', '1', '4', '----', 'LD(D,E)']
// ['54', '1', '4', '----', 'LD(D,H)']
// ['55', '1', '4', '----', 'LD(D,L)']
// ['56', '1', '8', '----', 'LD(D,Load(HL))']
// ['57', '1', '4', '----', 'LD(D,A)']
// ['58', '1', '4', '----', 'LD(E,B)']
// ['59', '1', '4', '----', 'LD(E,C)']
// ['5A', '1', '4', '----', 'LD(E,D)']
// ['5B', '1', '4', '----', 'LD(E,E)']
// ['5C', '1', '4', '----', 'LD(E,H)']
// ['5D', '1', '4', '----', 'LD(E,L)']
// ['5E', '1', '8', '----', 'LD(E,Load(HL))']
// ['5F', '1', '4', '----', 'LD(E,A)']
// ['60', '1', '4', '----', 'LD(H,B)']
// ['61', '1', '4', '----', 'LD(H,C)']
// ['62', '1', '4', '----', 'LD(H,D)']
// ['63', '1', '4', '----', 'LD(H,E)']
// ['64', '1', '4', '----', 'LD(H,H)']
// ['65', '1', '4', '----', 'LD(H,L)']
// ['66', '1', '8', '----', 'LD(H,Load(HL))']
// ['67', '1', '4', '----', 'LD(H,A)']
// ['68', '1', '4', '----', 'LD(L,B)']
// ['69', '1', '4', '----', 'LD(L,C)']
// ['6A', '1', '4', '----', 'LD(L,D)']
// ['6B', '1', '4', '----', 'LD(L,E)']
// ['6C', '1', '4', '----', 'LD(L,H)']
// ['6D', '1', '4', '----', 'LD(L,L)']
// ['6E', '1', '8', '----', 'LD(L,Load(HL))']
// ['6F', '1', '4', '----', 'LD(L,A)']
// ['70', '1', '8', '----', 'LD(Load(HL),B)']
// ['71', '1', '8', '----', 'LD(Load(HL),C)']
// ['72', '1', '8', '----', 'LD(Load(HL),D)']
// ['73', '1', '8', '----', 'LD(Load(HL),E)']
// ['74', '1', '8', '----', 'LD(Load(HL),H)']
// ['75', '1', '8', '----', 'LD(Load(HL),L)']
// ['76', '1', '4', '----', 'HALT()']
// ['77', '1', '8', '----', 'LD(Load(HL),A)']
// ['78', '1', '4', '----', 'LD(A,B)']
// ['79', '1', '4', '----', 'LD(A,C)']
// ['7A', '1', '4', '----', 'LD(A,D)']
// ['7B', '1', '4', '----', 'LD(A,E)']
// ['7C', '1', '4', '----', 'LD(A,H)']
// ['7D', '1', '4', '----', 'LD(A,L)']
// ['7E', '1', '8', '----', 'LD(A,Load(HL))']
// ['7F', '1', '4', '----', 'LD(A,A)']
// ['80', '1', '4', 'Z0HC', 'ADD(A,B)']
// ['81', '1', '4', 'Z0HC', 'ADD(A,C)']
// ['82', '1', '4', 'Z0HC', 'ADD(A,D)']
// ['83', '1', '4', 'Z0HC', 'ADD(A,E)']
// ['84', '1', '4', 'Z0HC', 'ADD(A,H)']
// ['85', '1', '4', 'Z0HC', 'ADD(A,L)']
// ['86', '1', '8', 'Z0HC', 'ADD(A,Load(HL))']
// ['87', '1', '4', 'Z0HC', 'ADD(A,A)']
// ['88', '1', '4', 'Z0HC', 'ADC(A,B)']
// ['89', '1', '4', 'Z0HC', 'ADC(A,C)']
// ['8A', '1', '4', 'Z0HC', 'ADC(A,D)']
// ['8B', '1', '4', 'Z0HC', 'ADC(A,E)']
// ['8C', '1', '4', 'Z0HC', 'ADC(A,H)']
// ['8D', '1', '4', 'Z0HC', 'ADC(A,L)']
// ['8E', '1', '8', 'Z0HC', 'ADC(A,Load(HL))']
// ['8F', '1', '4', 'Z0HC', 'ADC(A,A)']
// ['90', '1', '4', 'Z1HC', 'SUB(B)']
// ['91', '1', '4', 'Z1HC', 'SUB(C)']
// ['92', '1', '4', 'Z1HC', 'SUB(D)']
// ['93', '1', '4', 'Z1HC', 'SUB(E)']
// ['94', '1', '4', 'Z1HC', 'SUB(H)']
// ['95', '1', '4', 'Z1HC', 'SUB(L)']
// ['96', '1', '8', 'Z1HC', 'SUB(Load(HL))']
// ['97', '1', '4', 'Z1HC', 'SUB(A)']
// ['98', '1', '4', 'Z1HC', 'SBC(B)']
// ['99', '1', '4', 'Z1HC', 'SBC(C)']
// ['9A', '1', '4', 'Z1HC', 'SBC(D)']
// ['9B', '1', '4', 'Z1HC', 'SBC(E)']
// ['9C', '1', '4', 'Z1HC', 'SBC(H)']
// ['9D', '1', '4', 'Z1HC', 'SBC(L)']
// ['9E', '1', '8', 'Z1HC', 'SBC(Load(HL))']
// ['9F', '1', '4', 'Z1HC', 'SBC(A)']
// ['A0', '1', '4', 'Z010', 'AND(B)']
// ['A1', '1', '4', 'Z010', 'AND(C)']
// ['A2', '1', '4', 'Z010', 'AND(D)']
// ['A3', '1', '4', 'Z010', 'AND(E)']
// ['A4', '1', '4', 'Z010', 'AND(H)']
// ['A5', '1', '4', 'Z010', 'AND(L)']
// ['A6', '1', '8', 'Z010', 'AND(Load(HL))']
// ['A7', '1', '4', 'Z010', 'AND(A)']
// ['A8', '1', '4', 'Z000', 'XOR(B)']
// ['A9', '1', '4', 'Z000', 'XOR(C)']
// ['AA', '1', '4', 'Z000', 'XOR(D)']
// ['AB', '1', '4', 'Z000', 'XOR(E)']
// ['AC', '1', '4', 'Z000', 'XOR(H)']
// ['AD', '1', '4', 'Z000', 'XOR(L)']
// ['AE', '1', '8', 'Z000', 'XOR(Load(HL))']
// ['AF', '1', '4', 'Z000', 'XOR(A)']
// ['B0', '1', '4', 'Z000', 'OR(B)']
// ['B1', '1', '4', 'Z000', 'OR(C)']
// ['B2', '1', '4', 'Z000', 'OR(D)']
// ['B3', '1', '4', 'Z000', 'OR(E)']
// ['B4', '1', '4', 'Z000', 'OR(H)']
// ['B5', '1', '4', 'Z000', 'OR(L)']
// ['B6', '1', '8', 'Z000', 'OR(Load(HL))']
// ['B7', '1', '4', 'Z000', 'OR(A)']
// ['B8', '1', '4', 'Z1HC', 'CP(B)']
// ['B9', '1', '4', 'Z1HC', 'CP(C)']
// ['BA', '1', '4', 'Z1HC', 'CP(D)']
// ['BB', '1', '4', 'Z1HC', 'CP(E)']
// ['BC', '1', '4', 'Z1HC', 'CP(H)']
// ['BD', '1', '4', 'Z1HC', 'CP(L)']
// ['BE', '1', '8', 'Z1HC', 'CP(Load(HL))']
// ['BF', '1', '4', 'Z1HC', 'CP(A)']
// ['C0', '1', '20/8', '----', 'RET(CNZ)']
// ['C1', '1', '12', '----', 'POP(BC)']
// ['C2', '3', '16/12', '----', 'JP(CNZ,U16)']
// ['C3', '3', '16', '----', 'JP(CT,U16)']
// ['C4', '3', '24/12', '----', 'CALL(CNZ,U16)']
// ['C5', '1', '16', '----', 'PUSH(BC)']
// ['C6', '2', '8', 'Z0HC', 'ADD(A,U8)']
// ['C7', '1', '16', '----', 'RST(0x00)']
// ['C8', '1', '20/8', '----', 'RET(CZ)']
// ['C9', '1', '16', '----', 'RET(CT)']
// ['CA', '3', '16/12', '----', 'JP(CZ,U16)']
// ['CB', '1', '4', '----', 'PREFIX(CB)']
// ['CC', '3', '24/12', '----', 'CALL(CZ,U16)']
// ['CD', '3', '24', '----', 'CALL(CT,U16)']
// ['CE', '2', '8', 'Z0HC', 'ADC(A,U8)']
// ['CF', '1', '16', '----', 'RST(0x08)']
// ['D0', '1', '20/8', '----', 'RET(CNC)']
// ['D1', '1', '12', '----', 'POP(DE)']
// ['D2', '3', '16/12', '----', 'JP(CNC,U16)']
// ['D3', '', '', '', 'ERR()']
// ['D4', '3', '24/12', '----', 'CALL(CNC,U16)']
// ['D5', '1', '16', '----', 'PUSH(DE)']
// ['D6', '2', '8', 'Z1HC', 'SUB(U8)']
// ['D7', '1', '16', '----', 'RST(0x10)']
// ['D8', '1', '20/8', '----', 'RET(CC)']
// ['D9', '1', '16', '----', '{ii.RET(CT);ii.EI();}']
// ['DA', '3', '16/12', '----', 'JP(CC,U16)']
// ['DB', '', '', '', 'ERR()']
// ['DC', '3', '24/12', '----', 'CALL(CC,U16)']
// ['DD', '', '', '', 'ERR()']
// ['DE', '2', '8', 'Z1HC', 'SBC(U8)']
// ['DF', '1', '16', '----', 'RST(0x18)']
// ['E0', '2', '12', '----', 'LD(IO(U8),A)']
// ['E1', '1', '12', '----', 'POP(HL)']
// ['E2', '2', '8', '----', 'LD(IO(C),A)']
// ['E3', '', '', '', 'ERR()']
// ['E4', '', '', '', 'ERR()']
// ['E5', '1', '16', '----', 'PUSH(HL)']
// ['E6', '2', '8', 'Z010', 'AND(U8)']
// ['E7', '1', '16', '----', 'RST(0x20)']
// ['E8', '2', '16', '00HC', 'ADD(SP,I8)']
// ['E9', '1', '4', '----', 'JP(CT,(HL))']
// ['EA', '3', '16', '----', 'LD(Load(U16),A)']
// ['EB', '', '', '', 'ERR()']
// ['EC', '', '', '', 'ERR()']
// ['ED', '', '', '', 'ERR()']
// ['EE', '2', '8', 'Z000', 'XOR(U8)']
// ['EF', '1', '16', '----', 'RST(0x28)']
// ['F0', '2', '12', '----', 'LD(A,IO(U8))']
// ['F1', '1', '12', 'ZNHC', 'POP(AF)']
// ['F2', '2', '8', '----', 'LD(A,IO(C))']
// ['F3', '1', '4', '----', 'DI()']
// ['F4', '', '', '', 'ERR()']
// ['F5', '1', '16', '----', 'PUSH(AF)']
// ['F6', '2', '8', 'Z000', 'OR(U8)']
// ['F7', '1', '16', '----', 'RST(0x30)']
// ['F8', '2', '12', '00HC', 'LDHLSP(I8)']
// ['F9', '1', '8', '----', 'LD(SP,HL)']
// ['FA', '3', '16', '----', 'LD(A,Load(U16))']
// ['FB', '1', '4', '----', 'EI()']
// ['FC', '', '', '', 'ERR()']
// ['FD', '', '', '', 'ERR()']
// ['FE', '2', '8', 'Z1HC', 'CP(U8)']
// ['FF', '1', '16', '----', 'RST(0x38)']
// ['100', '2', '8', 'Z00C', 'RLC(B)']
// ['101', '2', '8', 'Z00C', 'RLC(C)']
// ['102', '2', '8', 'Z00C', 'RLC(D)']
// ['103', '2', '8', 'Z00C', 'RLC(E)']
// ['104', '2', '8', 'Z00C', 'RLC(H)']
// ['105', '2', '8', 'Z00C', 'RLC(L)']
// ['106', '2', '16', 'Z00C', 'RLC(Load(HL))']
// ['107', '2', '8', 'Z00C', 'RLC(A)']
// ['108', '2', '8', 'Z00C', 'RRC(B)']
// ['109', '2', '8', 'Z00C', 'RRC(C)']
// ['10A', '2', '8', 'Z00C', 'RRC(D)']
// ['10B', '2', '8', 'Z00C', 'RRC(E)']
// ['10C', '2', '8', 'Z00C', 'RRC(H)']
// ['10D', '2', '8', 'Z00C', 'RRC(L)']
// ['10E', '2', '16', 'Z00C', 'RRC(Load(HL))']
// ['10F', '2', '8', 'Z00C', 'RRC(A)']
// ['110', '2', '8', 'Z00C', 'RL(B)']
// ['111', '2', '8', 'Z00C', 'RL(C)']
// ['112', '2', '8', 'Z00C', 'RL(D)']
// ['113', '2', '8', 'Z00C', 'RL(E)']
// ['114', '2', '8', 'Z00C', 'RL(H)']
// ['115', '2', '8', 'Z00C', 'RL(L)']
// ['116', '2', '16', 'Z00C', 'RL(Load(HL))']
// ['117', '2', '8', 'Z00C', 'RL(A)']
// ['118', '2', '8', 'Z00C', 'RR(B)']
// ['119', '2', '8', 'Z00C', 'RR(C)']
// ['11A', '2', '8', 'Z00C', 'RR(D)']
// ['11B', '2', '8', 'Z00C', 'RR(E)']
// ['11C', '2', '8', 'Z00C', 'RR(H)']
// ['11D', '2', '8', 'Z00C', 'RR(L)']
// ['11E', '2', '16', 'Z00C', 'RR(Load(HL))']
// ['11F', '2', '8', 'Z00C', 'RR(A)']
// ['120', '2', '8', 'Z00C', 'SLA(B)']
// ['121', '2', '8', 'Z00C', 'SLA(C)']
// ['122', '2', '8', 'Z00C', 'SLA(D)']
// ['123', '2', '8', 'Z00C', 'SLA(E)']
// ['124', '2', '8', 'Z00C', 'SLA(H)']
// ['125', '2', '8', 'Z00C', 'SLA(L)']
// ['126', '2', '16', 'Z00C', 'SLA(Load(HL))']
// ['127', '2', '8', 'Z00C', 'SLA(A)']
// ['128', '2', '8', 'Z000', 'SRA(B)']
// ['129', '2', '8', 'Z000', 'SRA(C)']
// ['12A', '2', '8', 'Z000', 'SRA(D)']
// ['12B', '2', '8', 'Z000', 'SRA(E)']
// ['12C', '2', '8', 'Z000', 'SRA(H)']
// ['12D', '2', '8', 'Z000', 'SRA(L)']
// ['12E', '2', '16', 'Z000', 'SRA(Load(HL))']
// ['12F', '2', '8', 'Z000', 'SRA(A)']
// ['130', '2', '8', 'Z000', 'SWAP(B)']
// ['131', '2', '8', 'Z000', 'SWAP(C)']
// ['132', '2', '8', 'Z000', 'SWAP(D)']
// ['133', '2', '8', 'Z000', 'SWAP(E)']
// ['134', '2', '8', 'Z000', 'SWAP(H)']
// ['135', '2', '8', 'Z000', 'SWAP(L)']
// ['136', '2', '16', 'Z000', 'SWAP(Load(HL))']
// ['137', '2', '8', 'Z000', 'SWAP(A)']
// ['138', '2', '8', 'Z00C', 'SRL(B)']
// ['139', '2', '8', 'Z00C', 'SRL(C)']
// ['13A', '2', '8', 'Z00C', 'SRL(D)']
// ['13B', '2', '8', 'Z00C', 'SRL(E)']
// ['13C', '2', '8', 'Z00C', 'SRL(H)']
// ['13D', '2', '8', 'Z00C', 'SRL(L)']
// ['13E', '2', '16', 'Z00C', 'SRL(Load(HL))']
// ['13F', '2', '8', 'Z00C', 'SRL(A)']
// ['140', '2', '8', 'Z01-', 'BIT(0,B)']
// ['141', '2', '8', 'Z01-', 'BIT(0,C)']
// ['142', '2', '8', 'Z01-', 'BIT(0,D)']
// ['143', '2', '8', 'Z01-', 'BIT(0,E)']
// ['144', '2', '8', 'Z01-', 'BIT(0,H)']
// ['145', '2', '8', 'Z01-', 'BIT(0,L)']
// ['146', '2', '16', 'Z01-', 'BIT(0,Load(HL))']
// ['147', '2', '8', 'Z01-', 'BIT(0,A)']
// ['148', '2', '8', 'Z01-', 'BIT(1,B)']
// ['149', '2', '8', 'Z01-', 'BIT(1,C)']
// ['14A', '2', '8', 'Z01-', 'BIT(1,D)']
// ['14B', '2', '8', 'Z01-', 'BIT(1,E)']
// ['14C', '2', '8', 'Z01-', 'BIT(1,H)']
// ['14D', '2', '8', 'Z01-', 'BIT(1,L)']
// ['14E', '2', '16', 'Z01-', 'BIT(1,Load(HL))']
// ['14F', '2', '8', 'Z01-', 'BIT(1,A)']
// ['150', '2', '8', 'Z01-', 'BIT(2,B)']
// ['151', '2', '8', 'Z01-', 'BIT(2,C)']
// ['152', '2', '8', 'Z01-', 'BIT(2,D)']
// ['153', '2', '8', 'Z01-', 'BIT(2,E)']
// ['154', '2', '8', 'Z01-', 'BIT(2,H)']
// ['155', '2', '8', 'Z01-', 'BIT(2,L)']
// ['156', '2', '16', 'Z01-', 'BIT(2,Load(HL))']
// ['157', '2', '8', 'Z01-', 'BIT(2,A)']
// ['158', '2', '8', 'Z01-', 'BIT(3,B)']
// ['159', '2', '8', 'Z01-', 'BIT(3,C)']
// ['15A', '2', '8', 'Z01-', 'BIT(3,D)']
// ['15B', '2', '8', 'Z01-', 'BIT(3,E)']
// ['15C', '2', '8', 'Z01-', 'BIT(3,H)']
// ['15D', '2', '8', 'Z01-', 'BIT(3,L)']
// ['15E', '2', '16', 'Z01-', 'BIT(3,Load(HL))']
// ['15F', '2', '8', 'Z01-', 'BIT(3,A)']
// ['160', '2', '8', 'Z01-', 'BIT(4,B)']
// ['161', '2', '8', 'Z01-', 'BIT(4,C)']
// ['162', '2', '8', 'Z01-', 'BIT(4,D)']
// ['163', '2', '8', 'Z01-', 'BIT(4,E)']
// ['164', '2', '8', 'Z01-', 'BIT(4,H)']
// ['165', '2', '8', 'Z01-', 'BIT(4,L)']
// ['166', '2', '16', 'Z01-', 'BIT(4,Load(HL))']
// ['167', '2', '8', 'Z01-', 'BIT(4,A)']
// ['168', '2', '8', 'Z01-', 'BIT(5,B)']
// ['169', '2', '8', 'Z01-', 'BIT(5,C)']
// ['16A', '2', '8', 'Z01-', 'BIT(5,D)']
// ['16B', '2', '8', 'Z01-', 'BIT(5,E)']
// ['16C', '2', '8', 'Z01-', 'BIT(5,H)']
// ['16D', '2', '8', 'Z01-', 'BIT(5,L)']
// ['16E', '2', '16', 'Z01-', 'BIT(5,Load(HL))']
// ['16F', '2', '8', 'Z01-', 'BIT(5,A)']
// ['170', '2', '8', 'Z01-', 'BIT(6,B)']
// ['171', '2', '8', 'Z01-', 'BIT(6,C)']
// ['172', '2', '8', 'Z01-', 'BIT(6,D)']
// ['173', '2', '8', 'Z01-', 'BIT(6,E)']
// ['174', '2', '8', 'Z01-', 'BIT(6,H)']
// ['175', '2', '8', 'Z01-', 'BIT(6,L)']
// ['176', '2', '16', 'Z01-', 'BIT(6,Load(HL))']
// ['177', '2', '8', 'Z01-', 'BIT(6,A)']
// ['178', '2', '8', 'Z01-', 'BIT(7,B)']
// ['179', '2', '8', 'Z01-', 'BIT(7,C)']
// ['17A', '2', '8', 'Z01-', 'BIT(7,D)']
// ['17B', '2', '8', 'Z01-', 'BIT(7,E)']
// ['17C', '2', '8', 'Z01-', 'BIT(7,H)']
// ['17D', '2', '8', 'Z01-', 'BIT(7,L)']
// ['17E', '2', '16', 'Z01-', 'BIT(7,Load(HL))']
// ['17F', '2', '8', 'Z01-', 'BIT(7,A)']
// ['180', '2', '8', '----', 'RES(0,B)']
// ['181', '2', '8', '----', 'RES(0,C)']
// ['182', '2', '8', '----', 'RES(0,D)']
// ['183', '2', '8', '----', 'RES(0,E)']
// ['184', '2', '8', '----', 'RES(0,H)']
// ['185', '2', '8', '----', 'RES(0,L)']
// ['186', '2', '16', '----', 'RES(0,Load(HL))']
// ['187', '2', '8', '----', 'RES(0,A)']
// ['188', '2', '8', '----', 'RES(1,B)']
// ['189', '2', '8', '----', 'RES(1,C)']
// ['18A', '2', '8', '----', 'RES(1,D)']
// ['18B', '2', '8', '----', 'RES(1,E)']
// ['18C', '2', '8', '----', 'RES(1,H)']
// ['18D', '2', '8', '----', 'RES(1,L)']
// ['18E', '2', '16', '----', 'RES(1,Load(HL))']
// ['18F', '2', '8', '----', 'RES(1,A)']
// ['190', '2', '8', '----', 'RES(2,B)']
// ['191', '2', '8', '----', 'RES(2,C)']
// ['192', '2', '8', '----', 'RES(2,D)']
// ['193', '2', '8', '----', 'RES(2,E)']
// ['194', '2', '8', '----', 'RES(2,H)']
// ['195', '2', '8', '----', 'RES(2,L)']
// ['196', '2', '16', '----', 'RES(2,Load(HL))']
// ['197', '2', '8', '----', 'RES(2,A)']
// ['198', '2', '8', '----', 'RES(3,B)']
// ['199', '2', '8', '----', 'RES(3,C)']
// ['19A', '2', '8', '----', 'RES(3,D)']
// ['19B', '2', '8', '----', 'RES(3,E)']
// ['19C', '2', '8', '----', 'RES(3,H)']
// ['19D', '2', '8', '----', 'RES(3,L)']
// ['19E', '2', '16', '----', 'RES(3,Load(HL))']
// ['19F', '2', '8', '----', 'RES(3,A)']
// ['1A0', '2', '8', '----', 'RES(4,B)']
// ['1A1', '2', '8', '----', 'RES(4,C)']
// ['1A2', '2', '8', '----', 'RES(4,D)']
// ['1A3', '2', '8', '----', 'RES(4,E)']
// ['1A4', '2', '8', '----', 'RES(4,H)']
// ['1A5', '2', '8', '----', 'RES(4,L)']
// ['1A6', '2', '16', '----', 'RES(4,Load(HL))']
// ['1A7', '2', '8', '----', 'RES(4,A)']
// ['1A8', '2', '8', '----', 'RES(5,B)']
// ['1A9', '2', '8', '----', 'RES(5,C)']
// ['1AA', '2', '8', '----', 'RES(5,D)']
// ['1AB', '2', '8', '----', 'RES(5,E)']
// ['1AC', '2', '8', '----', 'RES(5,H)']
// ['1AD', '2', '8', '----', 'RES(5,L)']
// ['1AE', '2', '16', '----', 'RES(5,Load(HL))']
// ['1AF', '2', '8', '----', 'RES(5,A)']
// ['1B0', '2', '8', '----', 'RES(6,B)']
// ['1B1', '2', '8', '----', 'RES(6,C)']
// ['1B2', '2', '8', '----', 'RES(6,D)']
// ['1B3', '2', '8', '----', 'RES(6,E)']
// ['1B4', '2', '8', '----', 'RES(6,H)']
// ['1B5', '2', '8', '----', 'RES(6,L)']
// ['1B6', '2', '16', '----', 'RES(6,Load(HL))']
// ['1B7', '2', '8', '----', 'RES(6,A)']
// ['1B8', '2', '8', '----', 'RES(7,B)']
// ['1B9', '2', '8', '----', 'RES(7,C)']
// ['1BA', '2', '8', '----', 'RES(7,D)']
// ['1BB', '2', '8', '----', 'RES(7,E)']
// ['1BC', '2', '8', '----', 'RES(7,H)']
// ['1BD', '2', '8', '----', 'RES(7,L)']
// ['1BE', '2', '16', '----', 'RES(7,Load(HL))']
// ['1BF', '2', '8', '----', 'RES(7,A)']
// ['1C0', '2', '8', '----', 'SET(0,B)']
// ['1C1', '2', '8', '----', 'SET(0,C)']
// ['1C2', '2', '8', '----', 'SET(0,D)']
// ['1C3', '2', '8', '----', 'SET(0,E)']
// ['1C4', '2', '8', '----', 'SET(0,H)']
// ['1C5', '2', '8', '----', 'SET(0,L)']
// ['1C6', '2', '16', '----', 'SET(0,Load(HL))']
// ['1C7', '2', '8', '----', 'SET(0,A)']
// ['1C8', '2', '8', '----', 'SET(1,B)']
// ['1C9', '2', '8', '----', 'SET(1,C)']
// ['1CA', '2', '8', '----', 'SET(1,D)']
// ['1CB', '2', '8', '----', 'SET(1,E)']
// ['1CC', '2', '8', '----', 'SET(1,H)']
// ['1CD', '2', '8', '----', 'SET(1,L)']
// ['1CE', '2', '16', '----', 'SET(1,Load(HL))']
// ['1CF', '2', '8', '----', 'SET(1,A)']
// ['1D0', '2', '8', '----', 'SET(2,B)']
// ['1D1', '2', '8', '----', 'SET(2,C)']
// ['1D2', '2', '8', '----', 'SET(2,D)']
// ['1D3', '2', '8', '----', 'SET(2,E)']
// ['1D4', '2', '8', '----', 'SET(2,H)']
// ['1D5', '2', '8', '----', 'SET(2,L)']
// ['1D6', '2', '16', '----', 'SET(2,Load(HL))']
// ['1D7', '2', '8', '----', 'SET(2,A)']
// ['1D8', '2', '8', '----', 'SET(3,B)']
// ['1D9', '2', '8', '----', 'SET(3,C)']
// ['1DA', '2', '8', '----', 'SET(3,D)']
// ['1DB', '2', '8', '----', 'SET(3,E)']
// ['1DC', '2', '8', '----', 'SET(3,H)']
// ['1DD', '2', '8', '----', 'SET(3,L)']
// ['1DE', '2', '16', '----', 'SET(3,Load(HL))']
// ['1DF', '2', '8', '----', 'SET(3,A)']
// ['1E0', '2', '8', '----', 'SET(4,B)']
// ['1E1', '2', '8', '----', 'SET(4,C)']
// ['1E2', '2', '8', '----', 'SET(4,D)']
// ['1E3', '2', '8', '----', 'SET(4,E)']
// ['1E4', '2', '8', '----', 'SET(4,H)']
// ['1E5', '2', '8', '----', 'SET(4,L)']
// ['1E6', '2', '16', '----', 'SET(4,Load(HL))']
// ['1E7', '2', '8', '----', 'SET(4,A)']
// ['1E8', '2', '8', '----', 'SET(5,B)']
// ['1E9', '2', '8', '----', 'SET(5,C)']
// ['1EA', '2', '8', '----', 'SET(5,D)']
// ['1EB', '2', '8', '----', 'SET(5,E)']
// ['1EC', '2', '8', '----', 'SET(5,H)']
// ['1ED', '2', '8', '----', 'SET(5,L)']
// ['1EE', '2', '16', '----', 'SET(5,Load(HL))']
// ['1EF', '2', '8', '----', 'SET(5,A)']
// ['1F0', '2', '8', '----', 'SET(6,B)']
// ['1F1', '2', '8', '----', 'SET(6,C)']
// ['1F2', '2', '8', '----', 'SET(6,D)']
// ['1F3', '2', '8', '----', 'SET(6,E)']
// ['1F4', '2', '8', '----', 'SET(6,H)']
// ['1F5', '2', '8', '----', 'SET(6,L)']
// ['1F6', '2', '16', '----', 'SET(6,Load(HL))']
// ['1F7', '2', '8', '----', 'SET(6,A)']
// ['1F8', '2', '8', '----', 'SET(7,B)']
// ['1F9', '2', '8', '----', 'SET(7,C)']
// ['1FA', '2', '8', '----', 'SET(7,D)']
// ['1FB', '2', '8', '----', 'SET(7,E)']
// ['1FC', '2', '8', '----', 'SET(7,H)']
// ['1FD', '2', '8', '----', 'SET(7,L)']
// ['1FE', '2', '16', '----', 'SET(7,Load(HL))']
// ['1FF', '2', '8', '----', 'SET(7,A)']