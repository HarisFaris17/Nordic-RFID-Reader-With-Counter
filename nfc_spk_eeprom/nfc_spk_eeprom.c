#include "nfc_spk_eeprom.h"
#include "eeprom.h"
#include "nrf_log.h"

// consider 1 page of NFC infomation (NFC ID length, NFC ID, and SPK count) plus maximum number of SPK page per NFC as complete set

#define     MAX_SIZE_LIST_SPK_WITH_COUNTER                (MAX_SPK_COUNT_PER_NFC*sizeof(spk_t))
#define     ADDR_COUNT_NFC_LIST                           START_ADDR_DATA
#define     PAGE_COUNT_PER_NFC_WITH_MAX_LIST_SPK          (1+MAX_SPK_COUNT_PER_NFC)                /* one page to contain NFC ID length, NFC
                                                                                                     ID, and SPK Count. after that page, reserved
                                                                                                     maximum number of SPK page per NFC. Alias
                                                                                                     this macro defines number of pages of one
                                                                                                     complete set*/
#define     COMPREHENSIVE_PAGE_COUNT                      (1+ PAGE_COUNT_PER_NFC_WITH_MAX_LIST_SPK \
                                                            * MAX_NFC_COUNT)
static nfc_spk_reg_t m_reg;

static inline uint8_t nfc_spk_start_address_complete_set(uint8_t nfc_index)
{
    return ADDR_COUNT_NFC_LIST+nfc_index*PAGE_COUNT_PER_NFC_WITH_MAX_LIST_SPK;
}

static ret_code_t nfc_spk_eeprom_init(nrf_drv_twi_t * const p_twi_master)
{
    ret_code_t err;
    err = eeprom_init(p_twi_master);
    if (err != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Can't init eeprom!");
        return err;
    }
    
    if ((START_ADDR_DATA % BYTE_PER_PAGE) != 0)
    {
        NRF_LOG_ERROR("We temporarily doesn't support start address for data to be not divisible by 32, "
                      "since it doesn't represent the start address of any page of EEPROM it will make our flow for"
                      "storing/retrieving data to/from eeprom doesn't work");
        return NRF_ERROR_INVALID_ADDR;
    }

    return NRF_SUCCESS;
    
}

static ret_code_t nfc_spk_eeprom_retrieve_comprehensive()
{
    
}

static ret_code_t nfc_spk_retrieve_complete_set_by_index(uint8_t index)
{
    ret_code_t err;
    err = 
}

static ret_code_t nfc_spk_save_spk(uint8_t * p_nfc_id, uint8_t nfc_id_len)
{
    ret_code_t err;
    
    if (m_reg.count_nfc_list == 0)
    {
        NRF_LOG_ERROR("Can't save SPK, since no active NFC!")
        return NRF_ERROR_INVALID_LENGTH;
    }

    bool isSame = true;
    for (int i = 0; i<m_reg.count_nfc_list; i++)
    {
        uint8_t index_nfc = m_reg.index_nfc_list[i];

        // the length of NFC id of current index in register difference to that of specified in parameter
        if (m_reg.nfc_list[index_nfc].nfc_id_len != nfc_id_len)
        {
            continue;
        }

        // the length of NFC id same but let's compare each byte of NFC id
        isSame = memcmp(m_reg.nfc_list[index_nfc], p_nfc_id, nfc_id_len);
        if (!isSame)
        {
            continue;
        }

        // the nfc id of current index of register is the same as specified in the parameter
        break;
    }
    
    if (!isSame)
    {
        NRF_LOG_ERROR("No matching NFC ID in the register with the NFC ID specified in parameter");
        return NRF_ERROR_INVALID_DATA;
    }

    
}