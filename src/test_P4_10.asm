READ USR_x
JMP_0_iter_in: LOAD 0
STORE TMP_2_loop_rhs
LOAD USR_x
SUB TMP_2_loop_rhs
BRZNEG JMP_1_iter_out
COPY TMP_3_write USR_x
WRITE TMP_3_write
COPY TMP_4_add_lhs USR_x
LOAD 1
ADD TMP_4_add_lhs
PUSH
STACKW 0
LOAD 2
STORE TMP_5_div_rhs
STACKR 0
POP
DIV TMP_5_div_rhs
STORE TMP_6_iff_rhs
LOAD USR_x
PUSH
STACKW 0
LOAD 2
STORE TMP_7_div_rhs
STACKR 0
POP
DIV TMP_7_div_rhs
SUB TMP_6_iff_rhs
BRNEG JMP_2_iff
BRPOS JMP_2_iff
LOAD 0
STORE TMP_8_write
WRITE TMP_8_write
JMP_2_iff: LOAD USR_x
PUSH
STACKW 0
LOAD 1
STORE TMP_9_sub_rhs
STACKR 0
POP
SUB TMP_9_sub_rhs
STORE USR_x
BR JMP_0_iter_in
JMP_1_iter_out: STOP
USR_x 0
TMP_2_loop_rhs 0
TMP_3_write 0
TMP_4_add_lhs 0
TMP_5_div_rhs 0
TMP_6_iff_rhs 0
TMP_7_div_rhs 0
TMP_8_write 0
TMP_9_sub_rhs 0
