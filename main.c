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
#include "nrf_drv_twi.h"
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

#include "ssd1306_oled.h"

#include "app_button.h"
#include "app_timer.h"

//#include "nrf_fstorage.h"
//#include "nrf_fstorage_nvmc.h"

#include "eeprom.h"

#include "app_eeprom.h"

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

#define DELAY_CHANGE_STATE_DISPLAY          2000                                          /// delay state change from START to COUNTING or from COUNTING DONE to IDLE

#define STORE_VAR_START_ADDR_FLASH          0x3e000                                       /// starting address to store data in flash
#define STORE_VAR_END_ADDR_FLASH            0x3ffff                                       /// end address (exclusive) to store data in flash
#define OFFSET_ADDR_COUNTER                 0x0                                           /// offset address of counter reference to STORE_VAR_START_ADDR_FLASH in flash to store the counter 
#define OFFSET_ADDR_LENGTH_RFID_ID          (OFFSET_ADDR_COUNTER+4)                       /// offset address of length of RFID id in flash reference to OFFSET_ADDR_COUNTER (note : the counter is 4-byte, i.e. uint32_t)
#define OFFSET_ADDR_RFID_ID                 (OFFSET_ADDR_LENGTH_RFID_ID+4)                /* offset address of RFID id in flash reference OFFSET_ADDR_LENGTH_RFID_ID 
                                                                                          (note : one can't write only 1 byte to the flash, the flash only allow 
                                                                                          at least a page of bytes, our hypothesis one page is 4 bytes @ ref to
                                                                                          nrf_storage_nvmc*/
#define START_ADDR_DATA                     0x0CC0                                        /* The 5 bits LSB represents the address in the page specified for the 12 bits upper the 5 bits LSB.
                                                                                             this 5 bits LSB will be incremented internally by eeprom after writing a byte (the pointer incremented)
                                                                                             . After writing in the last address of the specifed page, the pointer will be rolled over to the first
                                                                                             address in the page
                                                                                          */
#define INDEX_OF_COUNTER                    0
#define INDEX_OF_LENGTH_NFC                 4
#define INDEX_OF_NFC_ID                     5

#define MAX_SPK_COUNT_PER_NFC               5
#define MAX_NFC_ID_COUNT                    5

typedef struct{
    bool active;
    uint32_t counter;
    uint8_t nfc_id_len;              ///< UID length.
    uint8_t nfc_id[MAX_NFC_A_ID_LEN]; ///< NFC-A UID.
}active_nfc;



/**
 * @brief Possible Tag Types.
 */
typedef enum
{
    NFC_T2T = 0x00,      ///< Type 2 Tag Platform.
    NFC_T4T = 0x01,      ///< Type 4A Tag Platform.
    NFC_TT_NOT_SUPPORTED ///< Tag Type not supported.
} nfc_tag_type_t;

// @brief Possible state of display
typedef enum
{
    DISPLAY_IDLE,
    DISPLAY_START,
    DISPLAY_COUNTING,
    DISPLAY_COUNTING_DONE,
} display_state_type_t;


// =================Declarations=================

static void update_display_counter();

static void button_init();

static void update_display_state(display_state_type_t display_state);

static void timer_handler(void * p_context);

static void update_display_state(display_state_type_t display_state);

static void display_counting_done();

static void save_eeprom();

static void read_eeprom();

//static void fstorage_handler(nrf_fstorage_evt_t * p_evt);

// @brief current display state
static display_state_type_t m_display_state;

// @brief object to store the received nfc
nfc_a_tag_info m_nfc_tag;

// @brief object to store current active nfc
active_nfc m_active_nfc;

// @brief object of twi
static nrf_drv_twi_t m_twi_master = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE);

// @brief object timer
APP_TIMER_DEF(m_timer);

// @brief data to store/send data from/to EEPROM
static eeprom_data m_data;


// object fstorage to store the counting and 
//NRF_FSTORAGE_DEF(nrf_fstorage_t m_fstorage) = {
//  // the start address in flash to store bytes
//  .start_addr = 0x3e000,
//  // the end address (the end address is exclusive) in flash to store bytes.
//  /** the total bytes that can be stored between start_addr and end_addr is 
//  *end_addr-start_addr = 0x3ffff - 0x3e000 = 0x1fff (it is more than enough to store just RFID id, 
//  the length of RFID id, and counter)
//  **/
//  .end_addr   = 0x3ffff,
//  .evt_handler= fstorage_handler,
//};

//static void fstorage_handler(nrf_fstorage_evt_t * p_evt){
//    if(p_evt->result != NRF_SUCCESS){
//      printf("The operation to fstorage failed!\n");
//      return;
//    }
//    else{
//      if(p_evt->id == NRF_FSTORAGE_EVT_WRITE_RESULT){
//        printf("Writing operation on fstorage succedd!\n");
//      }
//      else if(p_evt->id == NRF_FSTORAGE_EVT_READ_RESULT){
//        printf("Reading operation on fstorage succedd!\n");
//      }
//      else if(p_evt->id == NRF_FSTORAGE_EVT_ERASE_RESULT){
//        printf("Erase operation on fstorage succedd!\n");
//      }
//    }
//}

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
    err = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    button_init();
    nrf_gpio_cfg_output(led);

    err = app_timer_init();
    APP_ERROR_CHECK(err);
    err = app_timer_create(&m_timer,APP_TIMER_MODE_SINGLE_SHOT,timer_handler);
    APP_ERROR_CHECK(err);

    //err = nrf_fstorage_init(&m_fstorage,&nrf_fstorage_nvmc,NULL);
    //APP_ERROR_CHECK(err);
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

static void twi_event_handler(nrf_drv_twi_evt_t const * p_event,void * p_context){
  if(p_event->type==NRF_DRV_TWI_EVT_ADDRESS_NACK){
    NRF_LOG_INFO("No Acknowledgement after transmitted slave address byte");
  }
  else if(p_event->type==NRF_DRV_TWI_EVT_DATA_NACK){
    NRF_LOG_INFO("No Acknowledgement after transmitted data address byte");
  }
  else if(p_event->type==NRF_DRV_TWI_EVT_DONE){
    NRF_LOG_INFO("Data byte transmitted");
  }
}

ret_code_t i2c_init(){
  ret_code_t err_code;
  nrf_drv_twi_config_t twi_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  twi_config.scl = SCL_I2C_PIN;
  twi_config.sda = SDA_I2C_PIN;
  printf("Creating I2C..\n");

  //@ Note : the event handler of twi should be null (don't know why)
  err_code = nrf_drv_twi_init(&m_twi_master,&twi_config,NULL,NULL);
  if(err_code != NRF_SUCCESS){
    printf("Failed to init TWI, error code : %d\n",err_code);
    return err_code;
  }

  nrf_drv_twi_enable(&m_twi_master);
  return NRF_SUCCESS;
}


ret_code_t pn532_init(){
  ret_code_t err_code;
  err_code = adafruit_pn532_init(&m_twi_master,false);
  return err_code;
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

//================Definitions================

static void button_event_handler(uint8_t pin_no, uint8_t action){
  ret_code_t err;
  printf("Button event handler called with pin : %d\n",pin_no);
  if (action==APP_BUTTON_PUSH)
  {
      if (m_active_nfc.active)
      {
        switch(pin_no)
        {
          case BUTTON_COUNTER_UP:
            m_active_nfc.counter++;
            printf("Counter up\n");
            printf("Current counter : %d\n",m_active_nfc.counter);
            update_display_counter();
            save_eeprom();
            break;

          case BUTTON_COUNTER_DOWN:
            printf("Counter down\n");
            if(m_active_nfc.counter<=0)
            {
                printf("Current counter is 0 or less\n");
            }

            else
            {
                (m_active_nfc).counter--;
                printf("Current counter : %d",m_active_nfc.counter);
                update_display_counter();
                save_eeprom();
            }
            break;

          case BUTTON_DONE:
              printf("Counting done!\n");
              m_active_nfc.active=false;
              m_active_nfc.active=0;
              m_active_nfc.counter=0;
              m_active_nfc.nfc_id_len=0;
              for(int i=0;i<MAX_NFC_A_ID_LEN;i++){
                m_active_nfc.nfc_id[i] = 0;
              }
              nrf_gpio_pin_clear(led);
              update_display_state(DISPLAY_COUNTING_DONE);
              save_eeprom();
              err = app_timer_start(m_timer,
                                    APP_TIMER_TICKS(DELAY_CHANGE_STATE_DISPLAY),
                                    &m_display_state);
              APP_ERROR_CHECK(err);
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
  static app_button_cfg_t buttons[] = {
                                      { .pin_no = BUTTON_COUNTER_UP,
                                        .active_state = APP_BUTTON_ACTIVE_LOW,
                                        .pull_cfg = NRF_GPIO_PIN_PULLUP,
                                        .button_handler = button_event_handler
                                        },
                                      //{
                                      //  .pin_no = BUTTON_COUNTER_DOWN,
                                      //  .active_state = APP_BUTTON_ACTIVE_LOW,
                                      //  .pull_cfg = NRF_GPIO_PIN_PULLUP,
                                      //  .button_handler = button_event_handler
                                      //},
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
  printf("Button has been initialized\n");

  err = app_button_enable();
  APP_ERROR_CHECK(err);

  printf("Button has enabled\n");
}

static void after_found(){
  if(m_active_nfc.active)
  {
      printf("There is active NFC : ");
      for(int i=0;i<m_active_nfc.nfc_id_len;i++)
      {
          printf("0x%X ",m_active_nfc.nfc_id[i]);
      }
      printf("\n\r");
  }

  else{
    m_active_nfc.active = true;
    m_active_nfc.counter = 0;
    m_active_nfc.nfc_id_len = m_nfc_tag.nfc_id_len;
    memset(m_active_nfc.nfc_id, 0, MAX_NFC_A_ID_LEN);
    for(int i = 0;  i<m_nfc_tag.nfc_id_len; i++)
    {
        m_active_nfc.nfc_id[i] = m_nfc_tag.nfc_id[i];
    }
    printf("NFC Activated\n");

    printf("Activated NFC : ");
    for(int i=0;i<m_active_nfc.nfc_id_len;i++){
      printf("0x%X ",m_active_nfc.nfc_id[i]);
    }
    printf("\n\r");

    nrf_gpio_pin_set(led);
    update_display_state(DISPLAY_START);
    app_timer_start(m_timer,APP_TIMER_TICKS(DELAY_CHANGE_STATE_DISPLAY),&m_display_state);

    save_eeprom();

    nrf_delay_ms(1000);

    read_eeprom();
  }
}

static void timer_handler(void * p_context){
  printf("Timer handler called\n");
  display_state_type_t display_state = *(display_state_type_t *)p_context;

  if (display_state == DISPLAY_START){
    update_display_state(DISPLAY_COUNTING);
  }
  else if (display_state == DISPLAY_COUNTING_DONE){
    update_display_state(DISPLAY_IDLE);
  }

}

static void display_idle(){
    ssd1306_clear_display();
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58, SSD1306_LCDHEIGHT / 2 - 8, 'I', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58 + 18, SSD1306_LCDHEIGHT / 2 - 8, 'D', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58 + 36, SSD1306_LCDHEIGHT / 2 - 8, 'L', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58 + 54, SSD1306_LCDHEIGHT / 2 - 8, 'E', WHITE, BLACK, 3);
    ssd1306_display();
}

static void display_start(){
    ssd1306_clear_display();
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 40, SSD1306_LCDHEIGHT / 2 - 8, 'S', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 40 + 18, SSD1306_LCDHEIGHT / 2 - 8, 'T', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 40 + 36, SSD1306_LCDHEIGHT / 2 - 8, 'A', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 40 + 54, SSD1306_LCDHEIGHT / 2 - 8, 'R', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 40 + 72, SSD1306_LCDHEIGHT / 2 - 8, 'T', WHITE, BLACK, 3);
    ssd1306_display();
}

static void update_display_counter(){
    // @ this can be optimized
    static int16_t start = 6;
    ssd1306_clear_display();
    ssd1306_draw_char(start, 6, 'C', WHITE, BLACK, 1);
    ssd1306_draw_char(start + 6, 6, 'o', WHITE, BLACK, 1);
    ssd1306_draw_char(start + 12, 6, 'u', WHITE, BLACK, 1);
    ssd1306_draw_char(start + 18, 6, 'n', WHITE, BLACK, 1);
    ssd1306_draw_char(start + 24, 6, 't', WHITE, BLACK, 1);
    auto counter = m_active_nfc.counter;
    // maximum 4 digit counter, since the last element to store '\0' from sprintf
    char counter_string[5];
    uint8_t digits_counter = snprintf(counter_string,5,"%d",counter);
    for(int i = 0;i<digits_counter;i++){
      ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 40 + i*18, SSD1306_LCDHEIGHT / 2 - 8, counter_string[i], WHITE, BLACK, 3);
    }
    
    ssd1306_display();
}

static void display_counting_done(){
    ssd1306_clear_display();
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58, SSD1306_LCDHEIGHT / 2 - 8, 'D', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58 + 18, SSD1306_LCDHEIGHT / 2 - 8, 'O', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58 + 36, SSD1306_LCDHEIGHT / 2 - 8, 'N', WHITE, BLACK, 3);
    ssd1306_draw_char(SSD1306_LCDWIDTH / 2 - 58 + 54, SSD1306_LCDHEIGHT / 2 - 8, 'E', WHITE, BLACK, 3);
    ssd1306_display();
}

static void update_display_state(display_state_type_t display_state){
  if  (display_state == DISPLAY_START)
  {
      display_start();
      m_display_state = DISPLAY_START;
  }
  else if(display_state == DISPLAY_COUNTING_DONE)
  {
      display_counting_done();
      m_display_state = DISPLAY_COUNTING_DONE;
  }
  else if(display_state == DISPLAY_IDLE)
  {
      display_idle();
      m_display_state == DISPLAY_IDLE;
  }

  else if (display_state == DISPLAY_COUNTING)
  {
      update_display_counter();
      m_display_state = DISPLAY_COUNTING;
  }
}


static void save_eeprom()
{
    ret_code_t err;
    // the counter will be stored big endianly
    ASSIGN_32BIT_TO_8BIT_ARRAY(m_active_nfc.counter, (&(m_data.p_data[INDEX_OF_COUNTER])));
    m_data.p_data[INDEX_OF_LENGTH_NFC] = m_active_nfc.nfc_id_len;
    memcpy(&(m_data.p_data[INDEX_OF_NFC_ID]), m_active_nfc.nfc_id, m_active_nfc.nfc_id_len);
    uint8_t total_data_length = INDEX_OF_NFC_ID + m_active_nfc.nfc_id_len;
    m_data.length = total_data_length;
    
    
    err = eeprom_write_data(&m_data, START_ADDR_DATA);
    APP_ERROR_CHECK(err);

    printf("Saved data to EEPROM : ");
    for(int i = 0; i<total_data_length; i++)
    {
        printf("0x%X ", m_data.p_data[i]);
    }
    printf("\n\r");

}

static void read_eeprom()
{
    ret_code_t err;
    memset(m_data.p_data, 0, MAX_BYTE_PER_TRX);
    printf("Reseted buffer : ");
    for(int i = 0; i<MAX_BYTE_PER_TRX; i++)
    {
        printf("0x%X ", m_data.p_data[i]);
    }
    printf("\n\r");

    err = eeprom_read_data(&m_data, START_ADDR_DATA, MAX_BYTE_PER_TRX);
    APP_ERROR_CHECK(err);

    printf("Received buffer : ");
    for(int i = 0; i<MAX_BYTE_PER_TRX; i++)
    {
        printf("0x%X ", m_data.p_data[i]);
    }
    printf("\n\r");

    
}

static void startup_eeprom(){
    printf("Retrieve data from EEPROM when starting up\n");
    // the data retrieved from eeprom will be saved in m_data
    read_eeprom();
    
    if (m_data.p_data[INDEX_OF_LENGTH_NFC] == 0)
    {
        printf("No active data previouslly\n");
        return;
    }
    else
    {
        printf("Assign m_data to m_active_t\n");
        m_active_nfc.active = true;
        m_active_nfc.counter = CONVERT_8BIT_ARRAY_TO_32BIT(m_data.p_data);
        m_active_nfc.nfc_id_len = m_data.p_data[INDEX_OF_LENGTH_NFC];
        memcpy(m_active_nfc.nfc_id, &(m_data.p_data[INDEX_OF_NFC_ID]), m_active_nfc.nfc_id_len);
        printf("Retrieved active data : \n");
        printf("Counter : %d\n",  m_active_nfc.counter);
        printf("Length of NFC ID : %d\n", m_active_nfc.nfc_id_len);
        printf("NFC ID : ");
        for(int i = 0; i<m_active_nfc.nfc_id_len; i++)
        {
            printf("0x%X ",m_active_nfc.nfc_id[i]);
        }
        printf("\n\r");

        update_display_state(DISPLAY_COUNTING);
    }
}

int main(void)
{
    ret_code_t err_code;

    utils_setup();
    
    printf("Initialization...!\n");
    NRF_LOG_INFO("Initialization...!");
    NRF_LOG_FLUSH();

    err_code = i2c_init();
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("I2C initialized");
    NRF_LOG_FLUSH();

    err_code = pn532_init();
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("PN532 Initialized");
    NRF_LOG_FLUSH();

    printf("SSD1306 Initializing!\n");
    ssd1306_init_i2c_2(&m_twi_master);
    printf("SSD1306 Initialized\n");
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, false);
    printf("Display Idle SSD1306\n");
    update_display_state(DISPLAY_IDLE);

    printf("Initializing EEPROM!\n");
    err_code = eeprom_init(&m_twi_master);
    APP_ERROR_CHECK(err_code);
    printf("EEPROM initialization success\n");

    startup_eeprom();
    for (;;)
    {
        err_code = tag_detect_and_read();
        switch (err_code)
        {
            case NRF_SUCCESS:
                printf("Found\n");
                NRF_LOG_INFO("Found");
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
                // No delay here as we want to search for another tag immediately.
                break;

            case NRF_ERROR_NOT_SUPPORTED:
                NRF_LOG_INFO("Tag not supported.");
                printf("Tag not supported.\n");
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

