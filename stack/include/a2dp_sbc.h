/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Changes from Qualcomm Innovation Center are provided under the following license:
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

//
// A2DP Codec API for low complexity subband codec (SBC)
//

#ifndef A2DP_SBC_H
#define A2DP_SBC_H

#include "a2dp_codec_api.h"
#include "a2dp_sbc_constants.h"
#include "avdt_api.h"

/* data type for the SBC Codec Information Element */
typedef struct {
  uint8_t samp_freq;    /* Sampling frequency */
  uint8_t ch_mode;      /* Channel mode */
  uint8_t block_len;    /* Block length */
  uint8_t num_subbands; /* Number of subbands */
  uint8_t alloc_method; /* Allocation method */
  uint8_t min_bitpool;  /* Minimum bitpool */
  uint8_t max_bitpool;  /* Maximum bitpool */
  btav_a2dp_codec_bits_per_sample_t bits_per_sample;
} tA2DP_SBC_CIE;

class A2dpCodecConfigSbc : public A2dpCodecConfig {
 public:
  A2dpCodecConfigSbc(btav_a2dp_codec_priority_t codec_priority);
  virtual ~A2dpCodecConfigSbc();

  bool init() override;
  period_ms_t encoderIntervalMs() const override;
  bool setCodecConfig(const uint8_t* p_peer_codec_info, bool is_capability,
                      uint8_t* p_result_codec_config) override;

 private:
  bool useRtpHeaderMarkerBit() const override;
  bool updateEncoderUserConfig(
      const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
      bool* p_restart_input, bool* p_restart_output,
      bool* p_config_updated) override;
  void debug_codec_dump(int fd) override;
};

class A2dpCodecConfigSbcSink : public A2dpCodecConfig {
 public:
  A2dpCodecConfigSbcSink(btav_a2dp_codec_priority_t codec_priority);
  virtual ~A2dpCodecConfigSbcSink();

  bool init() override;
  period_ms_t encoderIntervalMs() const override;
  bool setCodecConfig(const uint8_t* p_peer_codec_info, bool is_capability,
                      uint8_t* p_result_codec_config) override;

 private:
  bool useRtpHeaderMarkerBit() const override;
  bool updateEncoderUserConfig(
      const tA2DP_ENCODER_INIT_PEER_PARAMS* p_peer_params,
      bool* p_restart_input, bool* p_restart_output,
      bool* p_config_updated) override;
};

// Checks whether the codec capabilities contain a valid A2DP SBC Source codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid SBC codec,
// otherwise false.
bool A2DP_IsSourceCodecValidSbc(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid A2DP SBC Sink codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid SBC codec,
// otherwise false.
bool A2DP_IsSinkCodecValidSbc(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP SBC Source
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid SBC codec,
// otherwise false.
bool A2DP_IsPeerSourceCodecValidSbc(const uint8_t* p_codec_info);

// Checks whether the codec capabilities contain a valid peer A2DP SBC Sink
// codec.
// NOTE: only codecs that are implemented are considered valid.
// Returns true if |p_codec_info| contains information about a valid SBC codec,
// otherwise false.
bool A2DP_IsPeerSinkCodecValidSbc(const uint8_t* p_codec_info);

// Checks whether A2DP SBC Sink codec is supported.
// |p_codec_info| contains information about the codec capabilities.
// Returns true if the A2DP SBC Sink codec is supported, otherwise false.
bool A2DP_IsSinkCodecSupportedSbc(const uint8_t* p_codec_info);

// Checks whether an A2DP SBC Source codec for a peer Source device is
// supported.
// |p_codec_info| contains information about the codec capabilities of the
// peer device.
// Returns true if the A2DP SBC Source codec for a peer Source device is
// supported, otherwise false.
bool A2DP_IsPeerSourceCodecSupportedSbc(const uint8_t* p_codec_info);

// Initialize state with the default A2DP SBC codec.
// The initialized state with the codec capabilities is stored in
// |p_codec_info|.
void A2DP_InitDefaultCodecSbc(uint8_t* p_codec_info);

// Builds A2DP preferred SBC Sink capability from SBC Source capability.
// |p_src_cap| is the Source capability to use.
// |p_pref_cfg| is the result Sink capability to store.
// Returns |A2DP_SUCCESS| on success, otherwise the corresponding A2DP error
// status code.
tA2DP_STATUS A2DP_BuildSrc2SinkConfigSbc(const uint8_t* p_src_cap,
                                         uint8_t* p_pref_cfg);

// Gets the A2DP SBC codec name for a given |p_codec_info|.
const char* A2DP_CodecNameSbc(const uint8_t* p_codec_info);

// Checks whether two A2DP SBC codecs |p_codec_info_a| and |p_codec_info_b|
// have the same type.
// Returns true if the two codecs have the same type, otherwise false.
bool A2DP_CodecTypeEqualsSbc(const uint8_t* p_codec_info_a,
                             const uint8_t* p_codec_info_b);

// Checks whether two A2DP SBC codecs |p_codec_info_a| and |p_codec_info_b|
// are exactly the same.
// Returns true if the two codecs are exactly the same, otherwise false.
// If the codec type is not SBC, the return value is false.
bool A2DP_CodecEqualsSbc(const uint8_t* p_codec_info_a,
                         const uint8_t* p_codec_info_b);

// Gets the track sample rate value for the A2DP SBC codec.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the track sample rate on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackSampleRateSbc(const uint8_t* p_codec_info);

// Gets the channel count for the A2DP SBC codec.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the channel count on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetTrackChannelCountSbc(const uint8_t* p_codec_info);

// Gets the number of subbands for the A2DP SBC codec.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the number of subbands on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetNumberOfSubbandsSbc(const uint8_t* p_codec_info);

// Gets the number of blocks for the A2DP SBC codec.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the number of blocks on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetNumberOfBlocksSbc(const uint8_t* p_codec_info);

// Gets the allocation method code for the A2DP SBC codec.
// The actual value is codec-specific - see |A2DP_SBC_IE_ALLOC_MD_*|.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the allocation method code on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetAllocationMethodCodeSbc(const uint8_t* p_codec_info);

// Gets the channel mode code for the A2DP SBC codec.
// The actual value is codec-specific - see |A2DP_SBC_IE_CH_MD_*|.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the channel mode code on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetChannelModeCodeSbc(const uint8_t* p_codec_info);

// Gets the sampling frequency code for the A2DP SBC codec.
// The actual value is codec-specific - see |A2DP_SBC_IE_SAMP_FREQ_*|.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the sampling frequency code on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetSamplingFrequencyCodeSbc(const uint8_t* p_codec_info);

// Gets the minimum bitpool for the A2DP SBC codec.
// The actual value is codec-specific - see |A2DP_SBC_IE_MIN_BITPOOL|.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the minimum bitpool on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetMinBitpoolSbc(const uint8_t* p_codec_info);

// Gets the maximum bitpool for the A2DP SBC codec.
// The actual value is codec-specific - see |A2DP_SBC_IE_MAX_BITPOOL|.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the maximum bitpool on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetMaxBitpoolSbc(const uint8_t* p_codec_info);

// Gets the channel type for the A2DP SBC Sink codec:
// 1 for mono, or 3 for dual/stereo/joint.
// |p_codec_info| is a pointer to the SBC codec_info to decode.
// Returns the channel type on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetSinkTrackChannelTypeSbc(const uint8_t* p_codec_info);

// Computes the number of frames to process in a time window for the A2DP
// SBC sink codec. |time_interval_ms| is the time interval (in milliseconds).
// |p_codec_info| is a pointer to the codec_info to decode.
// Returns the number of frames to process on success, or -1 if |p_codec_info|
// contains invalid codec information.
int A2DP_GetSinkFramesCountToProcessSbc(uint64_t time_interval_ms,
                                        const uint8_t* p_codec_info);

// Gets the A2DP SBC audio data timestamp from an audio packet.
// |p_codec_info| contains the codec information.
// |p_data| contains the audio data.
// The timestamp is stored in |p_timestamp|.
// Returns true on success, otherwise false.
bool A2DP_GetPacketTimestampSbc(const uint8_t* p_codec_info,
                                const uint8_t* p_data, uint32_t* p_timestamp);

// Builds A2DP SBC codec header for audio data.
// |p_codec_info| contains the codec information.
// |p_buf| contains the audio data.
// |frames_per_packet| is the number of frames in this packet.
// Returns true on success, otherwise false.
bool A2DP_BuildCodecHeaderSbc(const uint8_t* p_codec_info, BT_HDR* p_buf,
                              uint16_t frames_per_packet);

// Decodes and displays A2DP SBC codec info when using |LOG_DEBUG|.
// |p_codec_info| is a pointer to the SBC codec_info to decode and display.
// Returns true if the codec information is valid, otherwise false.
bool A2DP_DumpCodecInfoSbc(const uint8_t* p_codec_info);

// Gets the A2DP SBC encoder interface that can be used to encode and prepare
// A2DP packets for transmission - see |tA2DP_ENCODER_INTERFACE|.
// |p_codec_info| contains the codec information.
// Returns the A2DP SBC encoder interface if the |p_codec_info| is valid and
// supported, otherwise NULL.
const tA2DP_ENCODER_INTERFACE* A2DP_GetEncoderInterfaceSbc(
    const uint8_t* p_codec_info);

// Adjusts the A2DP SBC codec, based on local support and Bluetooth
// specification.
// |p_codec_info| contains the codec information to adjust.
// Returns true if |p_codec_info| is valid and supported, otherwise false.
bool A2DP_AdjustCodecSbc(uint8_t* p_codec_info);

// Gets the A2DP SBC Source codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_SourceCodecIndexSbc(const uint8_t* p_codec_info);

// Gets the A2DP SBC Source codec index for a given |p_codec_info|.
// Returns the corresponding |btav_a2dp_codec_index_t| on success,
// otherwise |BTAV_A2DP_CODEC_INDEX_MAX|.
btav_a2dp_codec_index_t A2DP_SinkCodecIndexSbc(const uint8_t* p_codec_info);

// Gets the A2DP SBC Source codec name.
const char* A2DP_CodecIndexStrSbc(void);

// Gets the A2DP SBC Sink codec name.
const char* A2DP_CodecIndexStrSbcSink(void);

// Initializes A2DP SBC Source codec information into |tAVDT_CFG| configuration
// entry pointed by |p_cfg|.
bool A2DP_InitCodecConfigSbc(tAVDT_CFG* p_cfg);

// Initializes A2DP SBC Sink codec information into |tAVDT_CFG| configuration
// entry pointed by |p_cfg|.
bool A2DP_InitCodecConfigSbcSink(tAVDT_CFG* p_cfg);

// Get SBC bitrate for offload mode
// |config| contains codec info
// |is_peer_edr| flag is true if peer is edr else false
// |p_codec_info| contains codec param info
// Returns |uint16_t| bitrate value
uint16_t A2DP_GetOffloadBitrateSbc(A2dpCodecConfig* config, bool is_peer_edr,
                                   const uint8_t* p_codec_info);

// Checks peer initiated setconfig with DUT supported config
// and returns proper status.
tA2DP_STATUS A2DP_IsCodecConfigMatchSbc(const uint8_t* p_codec_info);


tA2DP_STATUS A2DP_ParseInfoSbc(tA2DP_SBC_CIE* p_ie,
                               const uint8_t* p_codec_info,
                               bool is_capability);

int A2DP_GetBitsPerSampleSbc(const uint8_t* p_codec_info);

uint16_t A2DP_GetOffloadBitrateSbcUsingCodecInfo(const uint8_t *codec_info,
                                                 bool peer_edr);
#endif  // A2DP_SBC_H
