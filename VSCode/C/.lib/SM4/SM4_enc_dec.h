//
// Created by 赵睿智 on 2022/4/7.
//

#ifndef CRYPTO_EXPERIMENT_SM4_ENC_DEC_H
#define CRYPTO_EXPERIMENT_SM4_ENC_DEC_H

#define ui unsigned int

extern ui SM4_S_box[16][16];
extern ui Fk[4];
extern ui Ck[32];

void str2group_block(ui (*group)[33][4], char *hex_32byte_str);
void str2group(ui (*group)[4], char *hex_32byte_str);
void SM4_str2key(ui (*MK)[4], char *hex_32byte_str);
ui r_sl(ui hex_8byte_num, int sl_bit);
ui nl_tran(ui hex_8byte_num);
ui l_tran_cipher(ui hex_8byte_num);
ui l_tran_key(ui hex_8byte_num);
ui T_func(ui hex_8byte_num, char mode);
void extend_key(ui (*K)[36], const ui Mk[4]);
void SM4(ui (*group)[33][4], const ui K[36], char mode);
void CTR_counter_add(ui (*counter)[4], ui add_num);
void OFB_lshift_fill(ui (*iv_temp)[4], int byte, const ui *fill_byte_array);
void output_group(ui group[33][4]);
void output_key_list(ui key_list[36]);

#endif //CRYPTO_EXPERIMENT_SM4_ENC_DEC_H
