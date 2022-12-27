/**
 * Copyright (c) 2015 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**
 * @file
 * @brief An example of use of Adafruit tag reader combined with Type 2 Tag parser.
 *
 * @sa nfc_adafruit_tag_reader_example
 */

/**
 * @defgroup nfc_adafruit_tag_reader_example This example presents combined use of the Adafruit tag reader
 *      (@ref adafruit_pn532) library with Type 2 Tag parser (@ref nfc_type_2_tag_parser).

 */

#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "bsp.h"
#include "hardfault.h"
#include "nrf_delay.h"
#include "sdk_macros.h"
#include "sdk_config.h"

#include "adafruit_pn532.h"
#include "nfc_t2t_parser.h"
#include "nfc_t4t_cc_file.h"
#include "nfc_t4t_hl_detection_procedures.h"
#include "nfc_ndef_msg_parser.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_button.h"
#include "app_timer.h"

#define SEL_RES_CASCADE_BIT_NUM            3                                              /// Number of Cascade bit within SEL_RES byte.
#define SEL_RES_TAG_PLATFORM_MASK          0x60                                           /// Mask of Tag Platform bit group within SEL_RES byte.
#define SEL_RES_TAG_PLATFORM_BIT_OFFSET    5                                              /// Offset of the Tag Platform bit group within SEL_RES byte.

#define TAG_TYPE_2_UID_LENGTH              7                                              /// Length of the Tag's UID.
#define TAG_TYPE_2_DATA_AREA_SIZE_OFFSET   (T2T_CC_BLOCK_OFFSET + 2)                      /// Offset of the byte with Tag's Data size.
#define TAG_TYPE_2_DATA_AREA_MULTIPLICATOR 8                                              /// Multiplicator for a value stored in the Tag's Data size byte.
#define TAG_TYPE_2_FIRST_DATA_BLOCK_NUM    (T2T_FIRST_DATA_BLOCK_OFFSET / T2T_BLOCK_SIZE) /// First block number with Tag's Data.
#define TAG_TYPE_2_BLOCKS_PER_EXCHANGE     (T2T_MAX_DATA_EXCHANGE / T2T_BLOCK_SIZE)       /// Number of blocks fetched in single Tag's Read command.

#define TAG_TYPE_4_NDEF_FILE_SIZE           255                                           /// Size of the buffer for NDEF file.
#define TAG_TYPE_4_NLEN_FIELD_SIZE          2                                             /// Size of NLEN field inside NDEF file.

#define BUTTON_COUNTER_UP                   15
#define BUTTON_COUNTER_DOWN                 26
#define BUTTON_DONE                         20

#define led 31


typedef struct{
    bool active;
    int counter;
    uint8_t nfc_id_len;              ///< UID length.
    uint8_t nfc_id[MAX_NFC_A_ID_LEN]; ///< NFC-A UID.
}active_nfc;


// object to store the received nfc
nfc_a_tag_info m_nfc_tag;

// object to store current active nfc
active_nfc m_active_nfc;

/**
 * @brief Possible Tag Types.
 */
typedef enum
{
    NFC_T2T = 0x00,      ///< Type 2 Tag Platform.
    NFC_T4T = 0x01,      ///< Type 4A Tag Platform.
    NFC_TT_NOT_SUPPORTED ///< Tag Type not supported.
} nfc_tag_type_t;


/**
 * @brief Macro for handling errors returne by Type 4 Tag modules.
 */
#define T4T_ERROR_HANDLE(ERR_CODE, LOG) \
    if (ERR_CODE != NRF_SUCCESS)        \
    {                                   \
        NRF_LOG_INFO(LOG, ERR_CODE);    \
        return NRF_ERROR_INTERNAL;      \
    }


/**
 * @brief Function for initializations not directly related to Adafruit.
 */
void utils_setup(void)
{
    //bsp_board_init(BSP_INIT_LEDS);
    ret_code_t err;
    //err = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    err = app_timer_init();
    APP_ERROR_CHECK(err);
}


/**
 * @brief Function for analyzing NDEF data coming either from a Type 2 Tag TLV block or
 *        Type 4 Tag NDEF file.
 */
void ndef_data_analyze(uint8_t * p_ndef_msg_buff, uint32_t nfc_data_len)
{
    ret_code_t err_code;

    uint8_t  desc_buf[NFC_NDEF_PARSER_REQIRED_MEMO_SIZE_CALC(MAX_NDEF_RECORDS)];
    uint32_t desc_buf_len = sizeof(desc_buf);

    err_code = ndef_msg_parser(desc_buf,
                               &desc_buf_len,
                               p_ndef_msg_buff,
                               &nfc_data_len);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_INFO("Error during parsing a NDEF message.");
    }

    ndef_msg_printout((nfc_ndef_msg_desc_t *) desc_buf);
}


/**
 * @brief Function for reading data  from a Type 2 Tag Platform.
 */
ret_code_t t2t_data_read(nfc_a_tag_info * p_tag_info, uint8_t * buffer, uint32_t buffer_size)
{
    ret_code_t err_code;
    uint8_t    block_num = 0;

    // Not enough size in the buffer to read a tag header.
    if (buffer_size < T2T_FIRST_DATA_BLOCK_OFFSET)
    {
        return NRF_ERROR_NO_MEM;
    }

    if (p_tag_info->nfc_id_len != TAG_TYPE_2_UID_LENGTH)
    {
        return NRF_ERROR_NOT_SUPPORTED;
    }

    // Read blocks 0 - 3 to get the header information.
    err_code = adafruit_pn532_tag2_read(block_num, buffer);
    if (err_code)
    {
        NRF_LOG_INFO("Failed to read blocks: %d-%d", block_num,
                     block_num + T2T_END_PAGE_OFFSET);
        return NRF_ERROR_INTERNAL;
    }

    uint16_t data_bytes_in_tag = TAG_TYPE_2_DATA_AREA_MULTIPLICATOR *
                                 buffer[TAG_TYPE_2_DATA_AREA_SIZE_OFFSET];

    if (data_bytes_in_tag + T2T_FIRST_DATA_BLOCK_OFFSET > buffer_size)
    {
        return NRF_ERROR_NO_MEM;
    }

    uint8_t blocks_to_read = data_bytes_in_tag / T2T_BLOCK_SIZE;

    for (block_num = TAG_TYPE_2_FIRST_DATA_BLOCK_NUM;
         block_num < blocks_to_read;
         block_num += TAG_TYPE_2_BLOCKS_PER_EXCHANGE)
    {
        uint16_t offset_for_block = T2T_BLOCK_SIZE * block_num;
        err_code = adafruit_pn532_tag2_read(block_num, buffer + offset_for_block);
        if (err_code)
        {
            NRF_LOG_INFO("Failed to read blocks: %d-%d",
                         block_num,
                         block_num + T2T_END_PAGE_OFFSET);
            return NRF_ERROR_INTERNAL;
        }
    }

    return NRF_SUCCESS;
}


/**
 * @brief Function for analyzing data from a Type 2 Tag Platform.
 *
 * This function parses content of a Type 2 Tag Platform and prints it out.
 */
void t2t_data_analyze(uint8_t * buffer)
{
    ret_code_t err_code;

    // Static declaration of Type 2 Tag structure.
    NFC_TYPE_2_TAG_DESC_DEF(test_1, MAX_TLV_BLOCKS);
    type_2_tag_t * test_type_2_tag = &NFC_TYPE_2_TAG_DESC(test_1);

    err_code = type_2_tag_parse(test_type_2_tag, buffer);
    if (err_code == NRF_ERROR_NO_MEM)
    {
        NRF_LOG_INFO("Not enough memory to read whole tag. Printing what've been read.");
    }
    else if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_INFO("Error during parsing a tag. Printing what could've been read.");
    }

    type_2_tag_printout(test_type_2_tag);

    tlv_block_t * p_tlv_block = test_type_2_tag->p_tlv_block_array;
    uint32_t      i;

    for (i = 0; i < test_type_2_tag->tlv_count; i++)
    {
        if (p_tlv_block->tag == TLV_NDEF_MESSAGE)
        {
            ndef_data_analyze(p_tlv_block->p_value, p_tlv_block->length);
            p_tlv_block++;
        }
    }
}


/**
 * @brief Function for reading and analyzing data from a Type 2 Tag Platform.
 *
 * This function reads content of a Type 2 Tag Platform, parses it and prints it out.
 */
ret_code_t t2t_data_read_and_analyze(nfc_a_tag_info * p_tag_info)
{
    ret_code_t     err_code;
    static uint8_t t2t_data[TAG_TYPE_2_DATA_BUFFER_SIZE]; // Buffer for tag data.

    err_code = t2t_data_read(p_tag_info, t2t_data, TAG_TYPE_2_DATA_BUFFER_SIZE);
    VERIFY_SUCCESS(err_code);

    t2t_data_analyze(t2t_data);

    return NRF_SUCCESS;
}


/**
 * @brief Function for reading and analyzing data from a Type 4 Tag Platform.
 *
 * This function reads content of a Type 4 Tag Platform, parses it and prints it out.
 */
ret_code_t t4t_data_read_and_analyze(nfc_a_tag_info * p_tag_info)
{
    ret_code_t err_code;

    // Static declaration of Type 4 Tag structure.
    NFC_T4T_CC_DESC_DEF(cc_file, MAX_TLV_BLOCKS);
    static uint8_t ndef_files_buffs[MAX_TLV_BLOCKS][TAG_TYPE_4_NDEF_FILE_SIZE];

    err_code = nfc_t4t_ndef_tag_app_select();
    T4T_ERROR_HANDLE(err_code, "Error (0x%X) during NDEF Tag Application Select Procedure.");

    err_code = nfc_t4t_cc_select();
    T4T_ERROR_HANDLE(err_code, "Error (0x%X) during CC Select Procedure.");

    nfc_t4t_capability_container_t * cc_file = &NFC_T4T_CC_DESC(cc_file);
    err_code = nfc_t4t_cc_read(cc_file);
    T4T_ERROR_HANDLE(err_code, "Error (0x%X) during CC Read Procedure.");

    nfc_t4t_tlv_block_t * p_tlv_block = cc_file->p_tlv_block_array;
    uint32_t              i;

    for (i = 0; i < cc_file->tlv_count; i++)
    {
        if ((p_tlv_block->type == NDEF_FILE_CONTROL_TLV) ||
            (p_tlv_block->value.read_access == CONTROL_FILE_READ_ACCESS_GRANTED))
        {
            err_code = nfc_t4t_file_select(p_tlv_block->value.file_id);
            T4T_ERROR_HANDLE(err_code, "Error (0x%X) during NDEF Select Procedure.");

            err_code = nfc_t4t_ndef_read(cc_file, ndef_files_buffs[i], TAG_TYPE_4_NDEF_FILE_SIZE);
            T4T_ERROR_HANDLE(err_code, "Error (0x%X) during NDEF Read Procedure.");
        }

        p_tlv_block++;
    }

    nfc_t4t_cc_file_printout(cc_file);

    p_tlv_block = cc_file->p_tlv_block_array;

    for (i = 0; i < cc_file->tlv_count; i++)
    {
        if ((p_tlv_block->type == NDEF_FILE_CONTROL_TLV) ||
            (p_tlv_block->value.file.p_content != NULL))
        {
            ndef_data_analyze(p_tlv_block->value.file.p_content + TAG_TYPE_4_NLEN_FIELD_SIZE,
                              p_tlv_block->value.file.len - TAG_TYPE_4_NLEN_FIELD_SIZE);
        }

        p_tlv_block++;
    }

    return NRF_SUCCESS;
}


/**
 * @brief Function for identifying Tag Platform Type.
 */
nfc_tag_type_t tag_type_identify(uint8_t sel_res)
{
    uint8_t platform_config;

    // Check if Cascade bit in SEL_RES response is cleared. Cleared bit indicates that NFCID1 complete.
    if (!IS_SET(sel_res, SEL_RES_CASCADE_BIT_NUM))
    {
        // Extract platform configuration from SEL_RES response.
        platform_config = (sel_res & SEL_RES_TAG_PLATFORM_MASK) >> SEL_RES_TAG_PLATFORM_BIT_OFFSET;
        if (platform_config < NFC_TT_NOT_SUPPORTED)
        {
            return (nfc_tag_type_t) platform_config;
        }
    }

    return NFC_TT_NOT_SUPPORTED;
}


/**
 * @brief Function for detecting a Tag, identifying its Type and reading data from it.
 *
 * This function waits for a Tag to appear in the field. When a Tag is detected, Tag Platform
 * Type (2/4) is identified and appropriate read procedure is run.
 */
ret_code_t tag_detect_and_read()
{
    ret_code_t     err_code;
    //nfc_a_tag_info t_tag;

    // Detect a NFC-A Tag in the field and initiate a communication. This function activates
    // the NFC RF field. If a Tag is present, basic information about detected Tag is returned
    // in tag info structure.
    err_code = adafruit_pn532_nfc_a_target_init(&m_nfc_tag, TAG_DETECT_TIMEOUT);
  
    if (err_code != NRF_SUCCESS)
    {
        printf("Error NRF_ERROR_NOT_FOUND\n");
        return NRF_ERROR_NOT_FOUND;
    }
    adafruit_pn532_tag_info_printout(&m_nfc_tag);
    

    nfc_tag_type_t tag_type = tag_type_identify(m_nfc_tag.sel_res);
    switch (tag_type)
    {
        case NFC_T2T:
            NRF_LOG_INFO("Type 2 Tag Platform detected. ");
            return t2t_data_read_and_analyze(&m_nfc_tag);

        case NFC_T4T:
            NRF_LOG_INFO("Type 4 Tag Platform detected. ");
            return t4t_data_read_and_analyze(&m_nfc_tag);

        default:
            printf("Tag is not type 2 nor type 4, but it is sufficient to be processed\n");
            return NRF_SUCCESS;
            //return NRF_SUCCESS;
    }
}


/**
 * @brief Function for waiting specified time after a Tag read operation.
 */
void after_read_delay(void)
{
    ret_code_t err_code;

    // Turn off the RF field.
    err_code = adafruit_pn532_field_off();
    APP_ERROR_CHECK(err_code);
    nrf_delay_ms(TAG_AFTER_READ_DELAY);
}

static void button_event_handler(uint8_t pin_no, uint8_t action){
  printf("Button event handler called with pin : %d\n",pin_no);
  if(action==APP_BUTTON_PUSH){
      if(m_active_nfc.active){
        switch(pin_no){
          case BUTTON_COUNTER_UP:
            m_active_nfc.counter++;
            printf("Counter up\n");
            printf("Current counter : %d\n",m_active_nfc.counter);
            break;

          case BUTTON_COUNTER_DOWN:
            printf("Counter down\n");
            if(m_active_nfc.counter<=0){
              printf("Current counter is 0 or less\n");
            }else{
              (m_active_nfc).counter--;
              printf("Current counter : %d",m_active_nfc.counter);
            }
            break;

          case BUTTON_DONE:
            printf("Counting done!\n");
            m_active_nfc.active=false;
            m_active_nfc.active=0;
            m_active_nfc.nfc_id_len=0;
            for(int i=0;i<MAX_NFC_A_ID_LEN;i++){
              m_active_nfc.nfc_id[i] = 0;
            }
            nrf_gpio_pin_clear(led);
            break;
        }
      }
      else{
        printf("There is no Tag registered now\n");
        NRF_LOG_INFO("There is no Tag registered now\n")
    }
 }

}

static void button_init(){
  ret_code_t err;
  //static app_button_cfg_t button = {.pin_no = 8,
  //                                  .active_state = APP_BUTTON_ACTIVE_LOW,
  //                                  .pull_cfg = NRF_GPIO_PIN_PULLUP,
  //                                  .button_handler = button_event_handler}
  static app_button_cfg_t buttons[] = {
                                      { .pin_no = BUTTON_COUNTER_UP,
                                        .active_state = APP_BUTTON_ACTIVE_LOW,
                                        .pull_cfg = NRF_GPIO_PIN_PULLUP,
                                        .button_handler = button_event_handler
                                        },
                                      {
                                        .pin_no = BUTTON_COUNTER_DOWN,
                                        .active_state = APP_BUTTON_ACTIVE_LOW,
                                        .pull_cfg = NRF_GPIO_PIN_PULLUP,
                                        .button_handler = button_event_handler
                                      },
                                      {
                                        .pin_no = BUTTON_DONE,
                                        .active_state = APP_BUTTON_ACTIVE_LOW,
                                        .pull_cfg = NRF_GPIO_PIN_PULLUP,
                                        .button_handler = button_event_handler
                                      }
  };
  uint8_t button_count = sizeof(buttons)/sizeof(app_button_cfg_t);
  printf("Button count %d\n",button_count);
  err = app_button_init(buttons,button_count,APP_TIMER_TICKS(100));
  APP_ERROR_CHECK(err);
  printf("Button has initialized\n");
  err = app_button_enable();
  APP_ERROR_CHECK(err);
  printf("Button has enabled\n");
}

static void after_found(){
  if(m_active_nfc.active){
    printf("There is active NFC : ");
    for(int i=0;i<m_active_nfc.nfc_id_len;i++){
      printf("0x%X ",m_active_nfc.nfc_id[i]);
    }
    printf("\r\n");
  }

  else{
    m_active_nfc.active = true;
    m_active_nfc.counter = 0;
    m_active_nfc.nfc_id_len = m_nfc_tag.nfc_id_len;
    memset(m_active_nfc.nfc_id,0,MAX_NFC_A_ID_LEN);
    for(int i = 0;i<m_nfc_tag.nfc_id_len;i++){
      m_active_nfc.nfc_id[i] = m_nfc_tag.nfc_id[i];
    }
    printf("NFC Activated\n");

    printf("Activated NFC : ");
    for(int i=0;i<m_active_nfc.nfc_id_len;i++){
      printf("0x%X ",m_active_nfc.nfc_id[i]);
    }
    printf("\r\n");
    nrf_gpio_pin_set(led);
  }
}


int main(void)
{
    ret_code_t err_code;


    utils_setup();
    button_init();
    nrf_gpio_cfg_output(led);

    
    printf("Initialization...!");
    //nrf_gpio_pin_set(led);
    nrf_delay_ms(1000);
    err_code = adafruit_pn532_init(false);
    //nrf_gpio_pin_clear(led);
    nrf_delay_ms(1000);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("NFC Adafruit tag reader example started.");

    for (;;)
    {
        //nrf_gpio_pin_set(led);
        err_code = tag_detect_and_read();
        switch (err_code)
        {
            case NRF_SUCCESS:
                printf("Found\n");
                //nrf_gpio_pin_toggle(led);
                after_found();
                after_read_delay();
                
                break;

            case NRF_ERROR_NO_MEM:
                NRF_LOG_INFO("Declared buffer for T2T is to small to store tag data.");
                printf("Declared buffer for T2T is to small to store tag data.\n");
                after_read_delay();
                break;

            case NRF_ERROR_NOT_FOUND:
                NRF_LOG_INFO("No Tag found.");
                printf("No Tag found.\n");
                //nrf_gpio_pin_toggle(led);
                // No delay here as we want to search for another tag immediately.
                break;

            case NRF_ERROR_NOT_SUPPORTED:
                NRF_LOG_INFO("Tag not supported.");
                printf("Tag not supported.\n");
                //nrf_gpio_pin_toggle(led);
                after_read_delay();
                break;

            default:
                NRF_LOG_INFO("Error during tag read.");
                printf("Error during tag read.\n");
                err_code = adafruit_pn532_field_off();
                break;
        }
        NRF_LOG_FLUSH();
    }
}


/** @} */ /* End of group nfc_adafruit_tag_reader_example */

