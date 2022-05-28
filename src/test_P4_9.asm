READ USR_x
JMP_0_iter_in: LOAD 0
STORE TMP_2_loop_rhs
LOAD USR_x
SUB TMP_2_loop_rhs
BRZNEG JMP_1_iter_out
COPY TMP_3_write USR_x
WRITE TMP_3_write
LOAD USR_x
PUSH
STACKW 0
LOAD 1
STORE TMP_4_sub_rhs
STACKR 0
POP
SUB TMP_4_sub_rhs
STORE USR_x
BR JMP_0_iter_in
JMP_1_iter_out: STOP
USR_x 0
TMP_2_loop_rhs 0
TMP_3_write 0
TMP_4_sub_rhs 0
