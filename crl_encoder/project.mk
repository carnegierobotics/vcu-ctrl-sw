LIB_CRL_ENC_OBJ:= $(LIB_ENCODER_OBJ) $(LIB_APP_OBJ)
LIB_CRL_ENC_A=$(BIN)/libcrl_enc.a


$(LIB_CRL_ENC_A): $(LIB_CRL_ENC_OBJ)
lib_crl_enc_a: $(LIB_CRL_ENC_A)

TARGETS+=lib_crl_enc_a
