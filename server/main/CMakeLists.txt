set(TOMCRYPT_SRCS src/ciphers/aes/aes.c src/ciphers/aes/aes_tab.c src/ciphers/anubis.c src/ciphers/blowfish.c 
src/ciphers/camellia.c src/ciphers/cast5.c src/ciphers/des.c src/ciphers/kasumi.c src/ciphers/khazad.c 
src/ciphers/kseed.c src/ciphers/multi2.c src/ciphers/noekeon.c src/ciphers/rc2.c src/ciphers/rc5.c 
src/ciphers/rc6.c src/ciphers/safer/safer.c src/ciphers/safer/saferp.c src/ciphers/skipjack.c 
src/ciphers/twofish/twofish.c src/ciphers/xtea.c src/encauth/ccm/ccm_add_aad.c 
src/encauth/ccm/ccm_add_nonce.c src/encauth/ccm/ccm_done.c src/encauth/ccm/ccm_init.c 
src/encauth/ccm/ccm_memory.c src/encauth/ccm/ccm_process.c src/encauth/ccm/ccm_reset.c 
src/encauth/ccm/ccm_test.c src/encauth/chachapoly/chacha20poly1305_add_aad.c 
src/encauth/chachapoly/chacha20poly1305_decrypt.c src/encauth/chachapoly/chacha20poly1305_done.c 
src/encauth/chachapoly/chacha20poly1305_encrypt.c src/encauth/chachapoly/chacha20poly1305_init.c 
src/encauth/chachapoly/chacha20poly1305_memory.c src/encauth/chachapoly/chacha20poly1305_setiv.c 
src/encauth/chachapoly/chacha20poly1305_setiv_rfc7905.c 
src/encauth/chachapoly/chacha20poly1305_test.c src/encauth/eax/eax_addheader.c 
src/encauth/eax/eax_decrypt.c src/encauth/eax/eax_decrypt_verify_memory.c src/encauth/eax/eax_done.c 
src/encauth/eax/eax_encrypt.c src/encauth/eax/eax_encrypt_authenticate_memory.c 
src/encauth/eax/eax_init.c src/encauth/eax/eax_test.c src/encauth/gcm/gcm_add_aad.c 
src/encauth/gcm/gcm_add_iv.c src/encauth/gcm/gcm_done.c src/encauth/gcm/gcm_gf_mult.c 
src/encauth/gcm/gcm_init.c src/encauth/gcm/gcm_memory.c src/encauth/gcm/gcm_mult_h.c 
src/encauth/gcm/gcm_process.c src/encauth/gcm/gcm_reset.c src/encauth/gcm/gcm_test.c 
src/encauth/ocb/ocb_decrypt.c src/encauth/ocb/ocb_decrypt_verify_memory.c 
src/encauth/ocb/ocb_done_decrypt.c src/encauth/ocb/ocb_done_encrypt.c src/encauth/ocb/ocb_encrypt.c 
src/encauth/ocb/ocb_encrypt_authenticate_memory.c src/encauth/ocb/ocb_init.c src/encauth/ocb/ocb_ntz.c 
src/encauth/ocb/ocb_shift_xor.c src/encauth/ocb/ocb_test.c src/encauth/ocb/s_ocb_done.c 
src/encauth/ocb3/ocb3_add_aad.c src/encauth/ocb3/ocb3_decrypt.c src/encauth/ocb3/ocb3_decrypt_last.c 
src/encauth/ocb3/ocb3_decrypt_verify_memory.c src/encauth/ocb3/ocb3_done.c 
src/encauth/ocb3/ocb3_encrypt.c src/encauth/ocb3/ocb3_encrypt_authenticate_memory.c 
src/encauth/ocb3/ocb3_encrypt_last.c src/encauth/ocb3/ocb3_init.c src/encauth/ocb3/ocb3_int_ntz.c 
src/encauth/ocb3/ocb3_int_xor_blocks.c src/encauth/ocb3/ocb3_test.c src/hashes/blake2b.c 
src/hashes/blake2s.c src/hashes/chc/chc.c src/hashes/helper/hash_file.c 
src/hashes/helper/hash_filehandle.c src/hashes/helper/hash_memory.c 
src/hashes/helper/hash_memory_multi.c src/hashes/md2.c src/hashes/md4.c src/hashes/md5.c 
src/hashes/rmd128.c src/hashes/rmd160.c src/hashes/rmd256.c src/hashes/rmd320.c src/hashes/sha1.c 
src/hashes/sha2/sha224.c src/hashes/sha2/sha256.c src/hashes/sha2/sha384.c src/hashes/sha2/sha512.c 
src/hashes/sha2/sha512_224.c src/hashes/sha2/sha512_256.c src/hashes/sha3.c src/hashes/sha3_test.c 
src/hashes/tiger.c src/hashes/whirl/whirl.c src/mac/blake2/blake2bmac.c 
src/mac/blake2/blake2bmac_file.c src/mac/blake2/blake2bmac_memory.c 
src/mac/blake2/blake2bmac_memory_multi.c src/mac/blake2/blake2bmac_test.c src/mac/blake2/blake2smac.c 
src/mac/blake2/blake2smac_file.c src/mac/blake2/blake2smac_memory.c 
src/mac/blake2/blake2smac_memory_multi.c src/mac/blake2/blake2smac_test.c src/mac/f9/f9_done.c 
src/mac/f9/f9_file.c src/mac/f9/f9_init.c src/mac/f9/f9_memory.c src/mac/f9/f9_memory_multi.c 
src/mac/f9/f9_process.c src/mac/f9/f9_test.c src/mac/hmac/hmac_done.c src/mac/hmac/hmac_file.c 
src/mac/hmac/hmac_init.c src/mac/hmac/hmac_memory.c src/mac/hmac/hmac_memory_multi.c 
src/mac/hmac/hmac_process.c src/mac/hmac/hmac_test.c src/mac/omac/omac_done.c src/mac/omac/omac_file.c 
src/mac/omac/omac_init.c src/mac/omac/omac_memory.c src/mac/omac/omac_memory_multi.c 
src/mac/omac/omac_process.c src/mac/omac/omac_test.c src/mac/pelican/pelican.c 
src/mac/pelican/pelican_memory.c src/mac/pelican/pelican_test.c src/mac/pmac/pmac_done.c 
src/mac/pmac/pmac_file.c src/mac/pmac/pmac_init.c src/mac/pmac/pmac_memory.c 
src/mac/pmac/pmac_memory_multi.c src/mac/pmac/pmac_ntz.c src/mac/pmac/pmac_process.c 
src/mac/pmac/pmac_shift_xor.c src/mac/pmac/pmac_test.c src/mac/poly1305/poly1305.c 
src/mac/poly1305/poly1305_file.c src/mac/poly1305/poly1305_memory.c 
src/mac/poly1305/poly1305_memory_multi.c src/mac/poly1305/poly1305_test.c src/mac/xcbc/xcbc_done.c 
src/mac/xcbc/xcbc_file.c src/mac/xcbc/xcbc_init.c src/mac/xcbc/xcbc_memory.c 
src/mac/xcbc/xcbc_memory_multi.c src/mac/xcbc/xcbc_process.c src/mac/xcbc/xcbc_test.c 
src/math/fp/ltc_ecc_fp_mulmod.c src/math/gmp_desc.c src/math/ltm_desc.c src/math/multi.c 
src/math/radix_to_bin.c src/math/rand_bn.c src/math/rand_prime.c src/math/tfm_desc.c src/misc/adler32.c 
src/misc/base64/base64_decode.c src/misc/base64/base64_encode.c src/misc/burn_stack.c 
src/misc/compare_testvector.c src/misc/crc32.c src/misc/crypt/crypt.c src/misc/crypt/crypt_argchk.c 
src/misc/crypt/crypt_cipher_descriptor.c src/misc/crypt/crypt_cipher_is_valid.c 
src/misc/crypt/crypt_constants.c src/misc/crypt/crypt_find_cipher.c 
src/misc/crypt/crypt_find_cipher_any.c src/misc/crypt/crypt_find_cipher_id.c 
src/misc/crypt/crypt_find_hash.c src/misc/crypt/crypt_find_hash_any.c 
src/misc/crypt/crypt_find_hash_id.c src/misc/crypt/crypt_find_hash_oid.c 
src/misc/crypt/crypt_find_prng.c src/misc/crypt/crypt_fsa.c src/misc/crypt/crypt_hash_descriptor.c 
src/misc/crypt/crypt_hash_is_valid.c src/misc/crypt/crypt_inits.c 
src/misc/crypt/crypt_ltc_mp_descriptor.c src/misc/crypt/crypt_prng_descriptor.c 
src/misc/crypt/crypt_prng_is_valid.c src/misc/crypt/crypt_prng_rng_descriptor.c 
src/misc/crypt/crypt_register_all_ciphers.c src/misc/crypt/crypt_register_all_hashes.c 
src/misc/crypt/crypt_register_all_prngs.c src/misc/crypt/crypt_register_cipher.c 
src/misc/crypt/crypt_register_hash.c src/misc/crypt/crypt_register_prng.c src/misc/crypt/crypt_sizes.c 
src/misc/crypt/crypt_unregister_cipher.c src/misc/crypt/crypt_unregister_hash.c 
src/misc/crypt/crypt_unregister_prng.c src/misc/error_to_string.c src/misc/hkdf/hkdf.c 
src/misc/hkdf/hkdf_test.c src/misc/mem_neq.c src/misc/pk_get_oid.c src/misc/pkcs5/pkcs_5_1.c 
src/misc/pkcs5/pkcs_5_2.c src/misc/pkcs5/pkcs_5_test.c src/misc/zeromem.c src/modes/cbc/cbc_decrypt.c 
src/modes/cbc/cbc_done.c src/modes/cbc/cbc_encrypt.c src/modes/cbc/cbc_getiv.c 
src/modes/cbc/cbc_setiv.c src/modes/cbc/cbc_start.c src/modes/cfb/cfb_decrypt.c 
src/modes/cfb/cfb_done.c src/modes/cfb/cfb_encrypt.c src/modes/cfb/cfb_getiv.c 
src/modes/cfb/cfb_setiv.c src/modes/cfb/cfb_start.c src/modes/ctr/ctr_decrypt.c 
src/modes/ctr/ctr_done.c src/modes/ctr/ctr_encrypt.c src/modes/ctr/ctr_getiv.c 
src/modes/ctr/ctr_setiv.c src/modes/ctr/ctr_start.c src/modes/ctr/ctr_test.c 
src/modes/ecb/ecb_decrypt.c src/modes/ecb/ecb_done.c src/modes/ecb/ecb_encrypt.c 
src/modes/ecb/ecb_start.c src/modes/f8/f8_decrypt.c src/modes/f8/f8_done.c src/modes/f8/f8_encrypt.c 
src/modes/f8/f8_getiv.c src/modes/f8/f8_setiv.c src/modes/f8/f8_start.c src/modes/f8/f8_test_mode.c 
src/modes/lrw/lrw_decrypt.c src/modes/lrw/lrw_done.c src/modes/lrw/lrw_encrypt.c 
src/modes/lrw/lrw_getiv.c src/modes/lrw/lrw_process.c src/modes/lrw/lrw_setiv.c 
src/modes/lrw/lrw_start.c src/modes/lrw/lrw_test.c src/modes/ofb/ofb_decrypt.c src/modes/ofb/ofb_done.c 
src/modes/ofb/ofb_encrypt.c src/modes/ofb/ofb_getiv.c src/modes/ofb/ofb_setiv.c 
src/modes/ofb/ofb_start.c src/modes/xts/xts_decrypt.c src/modes/xts/xts_done.c 
src/modes/xts/xts_encrypt.c src/modes/xts/xts_init.c src/modes/xts/xts_mult_x.c 
src/modes/xts/xts_test.c src/pk/asn1/der/bit/der_decode_bit_string.c 
src/pk/asn1/der/bit/der_decode_raw_bit_string.c src/pk/asn1/der/bit/der_encode_bit_string.c 
src/pk/asn1/der/bit/der_encode_raw_bit_string.c src/pk/asn1/der/bit/der_length_bit_string.c 
src/pk/asn1/der/boolean/der_decode_boolean.c src/pk/asn1/der/boolean/der_encode_boolean.c 
src/pk/asn1/der/boolean/der_length_boolean.c src/pk/asn1/der/choice/der_decode_choice.c 
src/pk/asn1/der/generalizedtime/der_decode_generalizedtime.c 
src/pk/asn1/der/generalizedtime/der_encode_generalizedtime.c 
src/pk/asn1/der/generalizedtime/der_length_generalizedtime.c 
src/pk/asn1/der/ia5/der_decode_ia5_string.c src/pk/asn1/der/ia5/der_encode_ia5_string.c 
src/pk/asn1/der/ia5/der_length_ia5_string.c src/pk/asn1/der/integer/der_decode_integer.c 
src/pk/asn1/der/integer/der_encode_integer.c src/pk/asn1/der/integer/der_length_integer.c 
src/pk/asn1/der/object_identifier/der_decode_object_identifier.c 
src/pk/asn1/der/object_identifier/der_encode_object_identifier.c 
src/pk/asn1/der/object_identifier/der_length_object_identifier.c 
src/pk/asn1/der/octet/der_decode_octet_string.c src/pk/asn1/der/octet/der_encode_octet_string.c 
src/pk/asn1/der/octet/der_length_octet_string.c 
src/pk/asn1/der/printable_string/der_decode_printable_string.c 
src/pk/asn1/der/printable_string/der_encode_printable_string.c 
src/pk/asn1/der/printable_string/der_length_printable_string.c 
src/pk/asn1/der/sequence/der_decode_sequence_ex.c 
src/pk/asn1/der/sequence/der_decode_sequence_flexi.c 
src/pk/asn1/der/sequence/der_decode_sequence_multi.c 
src/pk/asn1/der/sequence/der_decode_subject_public_key_info.c 
src/pk/asn1/der/sequence/der_encode_sequence_ex.c 
src/pk/asn1/der/sequence/der_encode_sequence_multi.c 
src/pk/asn1/der/sequence/der_encode_subject_public_key_info.c 
src/pk/asn1/der/sequence/der_length_sequence.c src/pk/asn1/der/sequence/der_sequence_free.c 
src/pk/asn1/der/sequence/der_sequence_shrink.c src/pk/asn1/der/set/der_encode_set.c 
src/pk/asn1/der/set/der_encode_setof.c src/pk/asn1/der/short_integer/der_decode_short_integer.c 
src/pk/asn1/der/short_integer/der_encode_short_integer.c 
src/pk/asn1/der/short_integer/der_length_short_integer.c 
src/pk/asn1/der/teletex_string/der_decode_teletex_string.c 
src/pk/asn1/der/teletex_string/der_length_teletex_string.c 
src/pk/asn1/der/utctime/der_decode_utctime.c src/pk/asn1/der/utctime/der_encode_utctime.c 
src/pk/asn1/der/utctime/der_length_utctime.c src/pk/asn1/der/utf8/der_decode_utf8_string.c 
src/pk/asn1/der/utf8/der_encode_utf8_string.c src/pk/asn1/der/utf8/der_length_utf8_string.c 
src/pk/dh/dh.c src/pk/dh/dh_check_pubkey.c src/pk/dh/dh_export.c src/pk/dh/dh_export_key.c 
src/pk/dh/dh_free.c src/pk/dh/dh_generate_key.c src/pk/dh/dh_import.c src/pk/dh/dh_set.c 
src/pk/dh/dh_set_pg_dhparam.c src/pk/dh/dh_shared_secret.c src/pk/dsa/dsa_decrypt_key.c 
src/pk/dsa/dsa_encrypt_key.c src/pk/dsa/dsa_export.c src/pk/dsa/dsa_free.c 
src/pk/dsa/dsa_generate_key.c src/pk/dsa/dsa_generate_pqg.c src/pk/dsa/dsa_import.c 
src/pk/dsa/dsa_make_key.c src/pk/dsa/dsa_set.c src/pk/dsa/dsa_set_pqg_dsaparam.c 
src/pk/dsa/dsa_shared_secret.c src/pk/dsa/dsa_sign_hash.c src/pk/dsa/dsa_verify_hash.c 
src/pk/dsa/dsa_verify_key.c src/pk/ecc/ecc.c src/pk/ecc/ecc_ansi_x963_export.c 
src/pk/ecc/ecc_ansi_x963_import.c src/pk/ecc/ecc_decrypt_key.c src/pk/ecc/ecc_encrypt_key.c 
src/pk/ecc/ecc_export.c src/pk/ecc/ecc_free.c src/pk/ecc/ecc_get_size.c src/pk/ecc/ecc_import.c 
src/pk/ecc/ecc_make_key.c src/pk/ecc/ecc_shared_secret.c src/pk/ecc/ecc_sign_hash.c 
src/pk/ecc/ecc_sizes.c src/pk/ecc/ecc_test.c src/pk/ecc/ecc_verify_hash.c 
src/pk/ecc/ltc_ecc_is_valid_idx.c src/pk/ecc/ltc_ecc_map.c src/pk/ecc/ltc_ecc_mul2add.c 
src/pk/ecc/ltc_ecc_mulmod.c src/pk/ecc/ltc_ecc_mulmod_timing.c src/pk/ecc/ltc_ecc_points.c 
src/pk/ecc/ltc_ecc_projective_add_point.c src/pk/ecc/ltc_ecc_projective_dbl_point.c 
src/pk/katja/katja_decrypt_key.c src/pk/katja/katja_encrypt_key.c src/pk/katja/katja_export.c 
src/pk/katja/katja_exptmod.c src/pk/katja/katja_free.c src/pk/katja/katja_import.c 
src/pk/katja/katja_make_key.c src/pk/pkcs1/pkcs_1_i2osp.c src/pk/pkcs1/pkcs_1_mgf1.c 
src/pk/pkcs1/pkcs_1_oaep_decode.c src/pk/pkcs1/pkcs_1_oaep_encode.c src/pk/pkcs1/pkcs_1_os2ip.c 
src/pk/pkcs1/pkcs_1_pss_decode.c src/pk/pkcs1/pkcs_1_pss_encode.c src/pk/pkcs1/pkcs_1_v1_5_decode.c 
src/pk/pkcs1/pkcs_1_v1_5_encode.c src/pk/rsa/rsa_decrypt_key.c src/pk/rsa/rsa_encrypt_key.c 
src/pk/rsa/rsa_export.c src/pk/rsa/rsa_exptmod.c src/pk/rsa/rsa_free.c src/pk/rsa/rsa_get_size.c 
src/pk/rsa/rsa_import.c src/pk/rsa/rsa_import_pkcs8.c src/pk/rsa/rsa_import_x509.c 
src/pk/rsa/rsa_make_key.c src/pk/rsa/rsa_set.c src/pk/rsa/rsa_sign_hash.c 
src/pk/rsa/rsa_sign_saltlen_get.c src/pk/rsa/rsa_verify_hash.c src/prngs/chacha20.c src/prngs/fortuna.c 
src/prngs/rc4.c src/prngs/rng_get_bytes.c src/prngs/rng_make_prng.c src/prngs/sober128.c 
src/prngs/sprng.c src/prngs/yarrow.c src/stream/chacha/chacha_crypt.c src/stream/chacha/chacha_done.c 
src/stream/chacha/chacha_ivctr32.c src/stream/chacha/chacha_ivctr64.c 
src/stream/chacha/chacha_keystream.c src/stream/chacha/chacha_setup.c src/stream/chacha/chacha_test.c 
src/stream/rc4/rc4_stream.c src/stream/rc4/rc4_test.c src/stream/sober128/sober128_stream.c 
src/stream/sober128/sober128_test.c)

set(TOMLIBMATH_SRCS bn_cutoffs.c bn_deprecated.c bn_mp_2expt.c bn_mp_abs.c bn_mp_add.c bn_mp_add_d.c bn_mp_addmod.c 
bn_mp_and.c bn_mp_clamp.c bn_mp_clear.c bn_mp_clear_multi.c bn_mp_cmp.c bn_mp_cmp_d.c bn_mp_cmp_mag.c 
bn_mp_cnt_lsb.c bn_mp_complement.c bn_mp_copy.c bn_mp_count_bits.c bn_mp_decr.c bn_mp_div.c bn_mp_div_2.c 
bn_mp_div_2d.c bn_mp_div_3.c bn_mp_div_d.c bn_mp_dr_is_modulus.c bn_mp_dr_reduce.c bn_mp_dr_setup.c 
bn_mp_error_to_string.c bn_mp_exch.c bn_mp_expt_u32.c bn_mp_exptmod.c bn_mp_exteuclid.c bn_mp_fread.c 
bn_mp_from_sbin.c bn_mp_from_ubin.c bn_mp_fwrite.c bn_mp_gcd.c bn_mp_get_i32.c bn_mp_get_i64.c 
bn_mp_get_l.c bn_mp_get_ll.c bn_mp_get_mag_u32.c bn_mp_get_mag_u64.c bn_mp_get_mag_ul.c 
bn_mp_get_mag_ull.c bn_mp_grow.c bn_mp_incr.c bn_mp_init.c bn_mp_init_copy.c bn_mp_init_i32.c 
bn_mp_init_i64.c bn_mp_init_l.c bn_mp_init_ll.c bn_mp_init_multi.c bn_mp_init_set.c bn_mp_init_size.c 
bn_mp_init_u32.c bn_mp_init_u64.c bn_mp_init_ul.c bn_mp_init_ull.c bn_mp_invmod.c bn_mp_is_square.c 
bn_mp_iseven.c bn_mp_isodd.c bn_mp_kronecker.c bn_mp_lcm.c bn_mp_log_u32.c bn_mp_lshd.c bn_mp_mod.c 
bn_mp_mod_2d.c bn_mp_mod_d.c bn_mp_montgomery_calc_normalization.c bn_mp_montgomery_reduce.c 
bn_mp_montgomery_setup.c bn_mp_mul.c bn_mp_mul_2.c bn_mp_mul_2d.c bn_mp_mul_d.c bn_mp_mulmod.c bn_mp_neg.c 
bn_mp_or.c bn_mp_pack.c bn_mp_pack_count.c bn_mp_prime_fermat.c bn_mp_prime_frobenius_underwood.c 
bn_mp_prime_is_prime.c bn_mp_prime_miller_rabin.c bn_mp_prime_next_prime.c 
bn_mp_prime_rabin_miller_trials.c bn_mp_prime_rand.c bn_mp_prime_strong_lucas_selfridge.c 
bn_mp_radix_size.c bn_mp_radix_smap.c bn_mp_rand.c bn_mp_read_radix.c bn_mp_reduce.c bn_mp_reduce_2k.c 
bn_mp_reduce_2k_l.c bn_mp_reduce_2k_setup.c bn_mp_reduce_2k_setup_l.c bn_mp_reduce_is_2k.c 
bn_mp_reduce_is_2k_l.c bn_mp_reduce_setup.c bn_mp_root_u32.c bn_mp_rshd.c bn_mp_sbin_size.c bn_mp_set.c 
bn_mp_set_i32.c bn_mp_set_i64.c bn_mp_set_l.c bn_mp_set_ll.c bn_mp_set_u32.c bn_mp_set_u64.c 
bn_mp_set_ul.c bn_mp_set_ull.c bn_mp_shrink.c bn_mp_signed_rsh.c bn_mp_sqr.c bn_mp_sqrmod.c bn_mp_sqrt.c 
bn_mp_sqrtmod_prime.c bn_mp_sub.c bn_mp_sub_d.c bn_mp_submod.c bn_mp_to_radix.c bn_mp_to_sbin.c 
bn_mp_to_ubin.c bn_mp_ubin_size.c bn_mp_unpack.c bn_mp_xor.c bn_mp_zero.c bn_prime_tab.c bn_s_mp_add.c 
bn_s_mp_balance_mul.c bn_s_mp_exptmod.c bn_s_mp_exptmod_fast.c bn_s_mp_get_bit.c bn_s_mp_invmod_fast.c 
bn_s_mp_invmod_slow.c bn_s_mp_karatsuba_mul.c bn_s_mp_karatsuba_sqr.c bn_s_mp_montgomery_reduce_fast.c 
bn_s_mp_mul_digs.c bn_s_mp_mul_digs_fast.c bn_s_mp_mul_high_digs.c bn_s_mp_mul_high_digs_fast.c 
bn_s_mp_prime_is_divisible.c bn_s_mp_rand_jenkins.c bn_s_mp_rand_platform.c bn_s_mp_reverse.c 
bn_s_mp_sqr.c bn_s_mp_sqr_fast.c bn_s_mp_sub.c bn_s_mp_toom_mul.c bn_s_mp_toom_sqr.c)

set(DROPBEAR_DIR ${CMAKE_SOURCE_DIR}/../dropbear)
list(TRANSFORM TOMLIBMATH_SRCS PREPEND "${DROPBEAR_DIR}/libtommath/")
list(TRANSFORM TOMCRYPT_SRCS PREPEND "${DROPBEAR_DIR}/libtomcrypt/")
set(DROPBEAR_INCLUDE_DIR ${DROPBEAR_DIR}/../dropbear/src)
set(TOMCRYPT_INCLUDE_DIR ${DROPBEAR_DIR}/../dropbear/libtomcrypt/src/headers)
set(TOMCRYPT_INCLUDE_DIR2 ${DROPBEAR_DIR}/libtommath)
set(PORT_DIR ${CMAKE_SOURCE_DIR}/../port)
set(DROPBEAR_SRCS
    # COMMONOBJS 
    ${DROPBEAR_DIR}/src/dbutil.c
    ${DROPBEAR_DIR}/src/buffer.c
    ${DROPBEAR_DIR}/src/dbhelpers.c
    ${DROPBEAR_DIR}/src/dss.c
    ${DROPBEAR_DIR}/src/bignum.c
    ${DROPBEAR_DIR}/src/signkey.c
    ${DROPBEAR_DIR}/src/rsa.c
    ${DROPBEAR_DIR}/src/dbrandom.c
    ${DROPBEAR_DIR}/src/queue.c
    ${DROPBEAR_DIR}/src/atomicio.c
    ${DROPBEAR_DIR}/src/compat.c
    ${DROPBEAR_DIR}/src/fake-rfc2553.c
    ${DROPBEAR_DIR}/src/ltc_prng.c
    ${DROPBEAR_DIR}/src/ecc.c
    ${DROPBEAR_DIR}/src/ecdsa.c
    ${DROPBEAR_DIR}/src/sk-ecdsa.c
    ${DROPBEAR_DIR}/src/crypto_desc.c
    ${DROPBEAR_DIR}/src/curve25519.c
    ${DROPBEAR_DIR}/src/ed25519.c
    ${DROPBEAR_DIR}/src/sk-ed25519.c
    ${DROPBEAR_DIR}/src/dbmalloc.c
    ${DROPBEAR_DIR}/src/gensignkey.c
    ${DROPBEAR_DIR}/src/gendss.c
    ${DROPBEAR_DIR}/src/genrsa.c
    ${DROPBEAR_DIR}/src/gened25519.c
    # CLISVROBJS (common client/server objects)
    ${DROPBEAR_DIR}/src/common-session.c
    ${DROPBEAR_DIR}/src/packet.c
    ${DROPBEAR_DIR}/src/common-algo.c
    ${DROPBEAR_DIR}/src/common-kex.c
    ${DROPBEAR_DIR}/src/common-channel.c
    ${DROPBEAR_DIR}/src/common-chansession.c
    ${DROPBEAR_DIR}/src/termcodes.c
    # ${DROPBEAR_DIR}/src/loginrec.c
    ${DROPBEAR_DIR}/src/tcp-accept.c
    ${DROPBEAR_DIR}/src/listener.c
    ${DROPBEAR_DIR}/src/process-packet.c
    ${DROPBEAR_DIR}/src/dh_groups.c
    ${DROPBEAR_DIR}/src/common-runopts.c
    ${DROPBEAR_DIR}/src/circbuffer.c
    ${DROPBEAR_DIR}/src/list.c
    ${DROPBEAR_DIR}/src/netio.c
    ${DROPBEAR_DIR}/src/chachapoly.c
    ${DROPBEAR_DIR}/src/gcm.c
    ${DROPBEAR_DIR}/src/kex-x25519.c
    ${DROPBEAR_DIR}/src/kex-dh.c
    ${DROPBEAR_DIR}/src/kex-ecdh.c
    ${DROPBEAR_DIR}/src/kex-pqhybrid.c
    ${DROPBEAR_DIR}/src/sntrup761.c
    ${DROPBEAR_DIR}/src/mlkem768.c
    # SVROBJS (server-specific objects)
    ${DROPBEAR_DIR}/src/svr-kex.c
    ${DROPBEAR_DIR}/src/svr-auth.c
    # ${DROPBEAR_DIR}/src/sshpty.c
    ${DROPBEAR_DIR}/src/svr-authpasswd.c
    ${DROPBEAR_DIR}/src/svr-authpubkey.c
    ${DROPBEAR_DIR}/src/svr-authpubkeyoptions.c
    ${DROPBEAR_DIR}/src/svr-session.c
    ${DROPBEAR_DIR}/src/svr-service.c
    # ${DROPBEAR_DIR}/src/svr-chansession.c
    ${DROPBEAR_DIR}/src/svr-runopts.c
    ${DROPBEAR_DIR}/src/svr-agentfwd.c
    # ${DROPBEAR_DIR}/src/svr-main.c
    ${DROPBEAR_DIR}/src/svr-x11fwd.c
    ${DROPBEAR_DIR}/src/svr-tcpfwd.c
    ${DROPBEAR_DIR}/src/svr-authpam.c)

idf_component_register(SRCS "server.c" ${DROPBEAR_SRCS} "../../port/idf_stubs.c"
                    ${TOMLIBMATH_SRCS} 
                    ${TOMCRYPT_SRCS}
                    INCLUDE_DIRS "." ${DROPBEAR_DIR} ${PORT_DIR} ${TOMCRYPT_INCLUDE_DIR} ${DROPBEAR_INCLUDE_DIR}
                    ${TOMCRYPT_INCLUDE_DIR2})

set_source_files_properties(${TOMCRYPT_SRCS} PROPERTIES COMPILE_DEFINITIONS LTC_SOURCE)


set_source_files_properties(${DROPBEAR_DIR}/src/ed25519.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/svr-kex.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/ecdsa.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/svr-runopts.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/signkey.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/common-algo.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/svr-authpubkey.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/svr-tcpfwd.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/sk-ed25519.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/sk-ecdsa.c PROPERTIES COMPILE_OPTIONS "-Wno-pointer-sign")
set_source_files_properties(${DROPBEAR_DIR}/src/sshpty.c PROPERTIES COMPILE_OPTIONS "-Wno-format")
# /home/david/repos/dropbear/dropbear/src/.c:406:58: