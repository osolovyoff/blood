#pragma once
#ifndef blood_crypto_aes_hxx__
#define blood_crypto_aes_hxx__

typedef unsigned char   byte;

class AES
{

#if !defined(AES_128) && !defined(AES_192) && !defined(AES_256)
	/// ������������ ��������� ������ ������ 128 ���.       @ingroup AES
#define AES_128     
	/// ������������ ��������� ������ ������ 192 ���.       @ingroup AES
#define AES_192
	/// ������������ ��������� ������ ������ 256 ���.       @ingroup AES
#define AES_256
#endif

public:
	enum status {
		success = 0,    ///< ��� ������.
		wrong_key_lenght = ~0    ///< �������� ����� �����.
	};

	static const unsigned  block_size = 16;

	void encrypt( const byte in[block_size], byte out[block_size]);
	void decrypt( const byte in[block_size], byte out[block_size]);

	status expand_key( const byte* key, const unsigned  size);

protected:
	unsigned    rounds;
	typedef unsigned long   u32;
	u32     encryption_round_key[64];
	u32 	decryption_round_key[64];

	static  u32 s_box[256];     ///< ������ ������
	static  u32 r_box[256];     ///< �������� ������
	static  u32 ft[4][256];     ///< ����������� ������ ������
	static  u32 rt[4][256];     ///< ����������� �������� ������

	void inline initialize_tables();
	static const unsigned   basis = 1 << 8 | 1 << 4 | 1 << 3 | 1 << 1 | 1 << 0;

	byte static inline lfsr2(byte    a);

	u32 static inline rot_left_8(u32 value);
	u32 static inline get(const byte array[4]);

	void static inline put(u32  value, byte array[4]);
	u32 static inline fb(u32 byte, unsigned position);
	u32 static inline rb(u32 byte, unsigned position);

};

#endif//blood_crypto_aes_hxx__