/*
 * Copyright (C) 2022 Sony Interactive Entertainment Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CryptoAlgorithmAES_CFB.h"

#if ENABLE(WEB_CRYPTO)

#include "CryptoAlgorithmAesCbcCfbParams.h"
#include "CryptoKeyAES.h"
#include "OpenSSLUtilities.h"

namespace WebCore {

static std::optional<Vector<uint8_t>> cfb8(const Vector<uint8_t>& key, const Vector<uint8_t>& iv, const Vector<uint8_t>& input, bool encrypt)
{
    if (iv.size() != AES_BLOCK_SIZE)
        return std::nullopt;

    AESKey aesKey;
    if (!aesKey.setKey(key, AES_ENCRYPT))
        return std::nullopt;

    // Instead of memmoving the input vector every time, we have a AES_BLOCK_SIZE * 2 length buffer
    // and shift the input position (shiftRegister + shift) as well as the feedback position
    // (shiftRegister + shift + AES_BLOCK_SIZE) until the feedback position reaches
    // the end of the buffer.
    uint8_t shiftRegister[AES_BLOCK_SIZE * 2];
    memcpy(shiftRegister, iv.data(), AES_BLOCK_SIZE);
    size_t shift = 0;

    Vector<uint8_t> output(input.size());

    uint8_t encryptedBlock[AES_BLOCK_SIZE];
    for (size_t i = 0; i < output.size(); i++) {
        AES_encrypt(shiftRegister + shift, encryptedBlock, aesKey.key());

        // In the CFB8 mode the first byte (most significant 8 bits) of the encrypted block
        // is used as a key stream. The output stream is generated by XORing the input and the key stream.
        output[i] = input[i] ^ encryptedBlock[0];

        // Feed back the 8 bit cipher to the shift register. The cipher stream is
        // "output" in the encryption mode and "input" in the decryption mode.
        shiftRegister[AES_BLOCK_SIZE + (shift++)] = encrypt ? output[i] : input[i];
        if (shift == AES_BLOCK_SIZE) {
            memcpy(shiftRegister, shiftRegister + AES_BLOCK_SIZE, AES_BLOCK_SIZE);
            shift = 0;
        }
    }
    memset(encryptedBlock, 0, sizeof encryptedBlock);

    return output;
}

static std::optional<Vector<uint8_t>> cryptEncrypt(const Vector<uint8_t>& key, const Vector<uint8_t>& iv, Vector<uint8_t>&& plainText)
{
    return cfb8(key, iv, plainText, true);
}

static std::optional<Vector<uint8_t>> cryptDecrypt(const Vector<uint8_t>& key, const Vector<uint8_t>& iv, const Vector<uint8_t>& cipherText)
{
    return cfb8(key, iv, cipherText, false);
}

ExceptionOr<Vector<uint8_t>> CryptoAlgorithmAES_CFB::platformEncrypt(const CryptoAlgorithmAesCbcCfbParams& parameters, const CryptoKeyAES& key, const Vector<uint8_t>& plainText)
{
    auto output = cryptEncrypt(key.key(), parameters.ivVector(), Vector<uint8_t>(plainText));
    if (!output)
        return Exception { OperationError };
    return WTFMove(*output);
}

ExceptionOr<Vector<uint8_t>> CryptoAlgorithmAES_CFB::platformDecrypt(const CryptoAlgorithmAesCbcCfbParams& parameters, const CryptoKeyAES& key, const Vector<uint8_t>& cipherText)
{
    auto output = cryptDecrypt(key.key(), parameters.ivVector(), cipherText);
    if (!output)
        return Exception { OperationError };
    return WTFMove(*output);
}

} // namespace WebCore

#endif // ENABLE(WEB_CRYPTO)
