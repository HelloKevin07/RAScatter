/* -*- c++ -*- */
/* 
 * Copyright 2022 <Kai Huang (k.huang[AT]pitt.edu)>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_RFID_TAG_DECODER_IMPL_H
#define INCLUDED_RFID_TAG_DECODER_IMPL_H

#include <rfid/tag_decoder.h>
#include <vector>
#include "rfid/global_vars.h"
#include <time.h>
#include <numeric>
#include <fstream>
namespace gr {
namespace rfid {

  class tag_decoder_impl : public tag_decoder
  {
    private:
    
      float n_samples_TAG_BIT;
      int s_rate;
      std::vector<float> pulse_bit;
      float T_global;
      gr_complex h_est;
      float alpha_CFO;
      char * char_bits;
      char * char_bits_HANDLE;

      int preamble_fm0_start;
      int preamble_m8_start;
      std::vector<gr_complex> EPC_samples_complex;
      int EPC_index;
      std::vector<float> tag_detection_EPC(std::vector<gr_complex> &EPC_samples_complex, int index, int flag);
      std::vector<float> tag_detection_RN16(std::vector<gr_complex> &RN16_samples_complex, int index, int flag);      
      std::vector<float> tag_detection_HANDLE(std::vector<gr_complex> &HANDLE_samples_complex, int index, int flag); 
      std::vector<float> tag_detection_READ(std::vector<gr_complex> &HANDLE_samples_complex, int index, int flag);
      std::vector<float> data_decoding(std::vector<float> & tag_bits, std::vector<gr_complex> & data, float T, int num_bits, int index, int M);    
      int tag_sync(const gr_complex * in, int size, int flag);
      int check_crc(char * bits, int num_bits);
      void update_slot();
      void performance_evaluation();

    public:
      tag_decoder_impl(int sample_rate, std::vector<int> output_sizes);
      ~tag_decoder_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
      gr_vector_int &ninput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items);
    };

  } // namespace rfid
} // namespace gr

#endif /* INCLUDED_RFID_TAG_DECODER_IMPL_H */

