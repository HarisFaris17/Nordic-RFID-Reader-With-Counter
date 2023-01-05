#include "nfc_spk_eeprom.h"
#include "eeprom.h"
#include "nrf_log.h"

// an SPK page contains 2 byte SPK Id plus the counter that attached to it
// consider 1 page of NFC infomation (NFC ID length, NFC ID, and SPK count) plus maximum number of SPK page per NFC as complete set
/* The order of page for NFC with SPK application is page 1 to store how many active NFC, index of complete set of every active NFC with SPK
    , then page 2 to store index 1 of a complete set of active NFC with SPK, then page 3 to store index 2 of a complete set of active NFC 
    with SPK
*/


//#define     MAX_SIZE_LIST_SPK_WITH_COUNTER                (MAX_SPK_COUNT_PER_NFC*sizeof(spk_t))
#define     ADDR_COUNT_NFC_LIST_WITH_INDEXES              START_ADDR_DATA                          /* this is the first page for comprehensive
                                                                                                      storing for NFC with SPK application.
                                                                                                    */
#define     PAGE_COUNT_PER_NFC_WITH_MAX_LIST_SPK          (1+MAX_SPK_COUNT_PER_NFC)                /* one page to contain NFC ID length, NFC
                                                                                                     ID, and SPK Count. after that page, reserved
                                                                                                     maximum number of SPK page per NFC. Alias
                                                                                                     this macro defines number of pages of one
                                                                                                     complete set*/
#define     COMPREHENSIVE_PAGE_COUNT                      (1+ PAGE_COUNT_PER_NFC_WITH_MAX_LIST_SPK \
                                                            * MAX_NFC_COUNT)


#define     OFFSET_COUNT_NFC_LIST                         0
#define     OFFSET_INDEXES_NFC_LIST                       (OFFSET_COUNT_NFC_LIST+1)

#define     OFFSET_NFC_ID_LENGTH                          0
#define     OFFSET_NFC_ID                                 (OFFSET_NFC_ID_LENGTH+1)
//#define     OFFSET_COUNT_SPK_PER_NFC                      (OFFSET_NFC_ID+1)
//#define     OFFSET_INDEXES_SPK_PER_NFC                    (OFFSET_COUNT_SPK_PER_NFC+1)

#define     OFFSET_SPK_ID                                 0
#define     OFFSET_COUNTER                                (OFFSET_SPK_ID+sizeof(spk_id_t))   


static nfc_spk_reg_t m_reg;

static inline uint8_t nfc_spk_start_address_complete_set(uint8_t nfc_index)
{
    return ADDR_COUNT_NFC_LIST_WITH_INDEXES+nfc_index*PAGE_COUNT_PER_NFC_WITH_MAX_LIST_SPK+1;           /*since the very first page in eeprom of 
                                                                                                          this application is used to store
                                                                                                          the active NFC indexes, which means the page
                                                                                                          is not used to store any part of complete set
                                                                                                          but rather the first complete set (nfc index 0)
                                                                                                          stored after that very first page (i.e. the second
                                                                                                          page), hence it is necessary to add 1 to the
                                                                                                          return value since the nfc index starts from 0
                                                                                                         */
} 

static inline uint8_t nfc_spk_start_address_spk_in_complete_set(uint8_t nfc_index, uint8_t spk_index)
{
    return nfc_spk_start_address_complete_set(nfc_index)+spk_index+1;                                    /*since the first page of a complete 
                                                                                                          set not contains any spk, and spk(s) 
                                                                                                          starts after the first page also spk_index
                                                                                                          starts at 0, therefore it is necessary
                                                                                                          to add 1 to the addition of start address of
                                                                                                          index of specified nfc and index of spk to
                                                                                                          really return the absolute page number of
                                                                                                          the spk in eeprom  
                                                                                                        */
}

static ret_code_t nfc_spk_eeprom_init(nrf_drv_twi_t * const p_twi_master)
{
    ret_code_t err;
    err = eeprom_init(p_twi_master);
    if (err != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Failed init eeprom in function %s!", __func__);
        return err;
    }
    
    if ((ADDR_COUNT_NFC_LIST_WITH_INDEXES % BYTES_PER_PAGE) != 0)
    {
        NRF_LOG_ERROR("We temporarily doesn't support start address for data to be not divisible by 32, "
                      "since it doesn't represent the start address of any page of EEPROM it will make our flow for"
                      "storing/retrieving data to/from eeprom screwed");
        return NRF_ERROR_INVALID_ADDR;
    }

    return NRF_SUCCESS;
}

static ret_code_t nfc_spk_retrieve_comprehensive()
{
    ret_code_t err;
    //uint8_t num_of_active_nfc;
    //uint8_t active_indexes_list[MAX_NFC_COUNT];

    // get the number of active NFC and coresponding NFC indexes
    err = nfc_spk_retrieve_start_page(&m_reg.count_nfc_list, m_reg.index_nfc_list);
    if (err != NRF_SUCCESS) 
    {
        NRF_LOG_ERROR("Error while retrieving start page in function %s",__func__);
        return err;
    }

    for (int i = 0; i < m_reg.count_nfc_list; i++)
    {
        err = nfc_spk_retrieve_complete_set_by_nfc_index(m_reg.index_nfc_list[i]);
        if (err != NRF_SUCCESS)
        {
            NRF_LOG_ERROR("Failed to retrieve complete set with NFC index %d in function %s", m_reg.index_nfc_list[i], __func__);
            return err;
        }
    }

    return NRF_SUCCESS;
}

static ret_code_t nfc_spk_retrieve_complete_set_by_nfc_index(uint8_t nfc_index)
{
    ret_code_t err;
    eeprom_data data;

    if (nfc_index < 0 || nfc_index >= MAX_NFC_COUNT)
    {
        NRF_LOG_ERROR("The nfc_index %d is not in range of [0, MAX_NFC_COUNT) in function %s", nfc_index, __func__);
        return NRF_ERROR_INVALID_PARAM;
    }

    //get the first page of complete set, i.e. the NFC ID length, NFC ID, count of active SPK for specified NFC ID, and indexes of active SPK
    err = eeprom_read_data(&data, nfc_spk_start_address_complete_set(nfc_index), BYTES_PER_PAGE);
    if (err != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Error when retrieving first page of a complete set in index NFC %d", nfc_index);
        return err;
    }
    m_reg.nfc_list[nfc_index].nfc_id_len = data.p_data[OFFSET_NFC_ID_LENGTH];
    memcpy(m_reg.nfc_list[nfc_index].nfc_id, &(data.p_data[OFFSET_NFC_ID]), m_reg.nfc_list[nfc_index].nfc_id_len);
    m_reg.nfc_list[nfc_index].count_spk_list = data.p_data[OFFSET_NFC_ID + m_reg.nfc_list[nfc_index].nfc_id_len];
    memcpy(m_reg.nfc_list[nfc_index].index_spk_list, &(data.p_data[OFFSET_NFC_ID + m_reg.nfc_list[nfc_index].nfc_id_len + 1]), m_reg.nfc_list[nfc_index].count_spk_list);

    // get each SPK id and counter in the specified nfc_index
    for (int i = 0; i < m_reg.nfc_list[nfc_index].count_spk_list; i++)
    {
        uint8_t index_in_spk_list = m_reg.nfc_list[nfc_index].index_spk_list[i];
        err = nfc_spk_retrieve_spk(nfc_index, index_in_spk_list);
        if (err != NRF_SUCCESS)
        {
            NRF_LOG_ERROR("Failed to retreive SPK with NFC index %d in function %s", nfc_index, __func__);
            return err;
        }
    }

    return NRF_SUCCESS;
}

static ret_code_t nfc_spk_retrieve_spk(uint8_t nfc_index, uint8_t spk_index)
{
    ret_code_t err;
    eeprom_data data;

    if (nfc_index < 0 || nfc_index >= MAX_NFC_COUNT)
    {
        NRF_LOG_ERROR("The nfc_index %d is not in range of [0, MAX_NFC_COUNT) in function %s", nfc_index, __func__);
        return NRF_ERROR_INVALID_PARAM;
    }

    if (spk_index < 0 || spk_index >= MAX_SPK_COUNT_PER_NFC)
    {
        NRF_LOG_ERROR("The spk_index %d is not in range of [0, MAX_SPK_COUNT_PER_NFC) in function %s", spk_index, __func__);
        return NRF_ERROR_INVALID_PARAM;
    }

    err = eeprom_read_data(&data, 
                            nfc_spk_start_address_spk_in_complete_set(nfc_index, spk_index),
                            6);
    if (err != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Failed to retrieve SPK with index : %d in NFC index : %d, in function %s", spk_index, nfc_index, __func__);
        return err;
    }

    // remember that the SPK Id stored in EEPROM is big endian, hence no need to swap the retrieved SPK Id
    m_reg.nfc_list[nfc_index].spk_list[spk_index].spk_id  = CONVERT_8BIT_ARRAY_TO_16BIT((&(data.p_data[OFFSET_SPK_ID])));
    m_reg.nfc_list[nfc_index].spk_list[spk_index].counter = CONVERT_8BIT_ARRAY_TO_32BIT((&(data.p_data[OFFSET_COUNTER])));
    return NRF_SUCCESS;
}

static ret_code_t nfc_spk_retrieve_start_page(uint8_t * p_num_of_active_nfc, uint8_t * p_indexes_list)
{
    ret_code_t err;
    eeprom_data data;
    
    err = eeprom_read_data(&data, ADDR_COUNT_NFC_LIST_WITH_INDEXES, 1+MAX_NFC_COUNT);
    if (err != NRF_SUCCESS)
    {
        NRF_LOG_INFO("Failed to retreive the first page of NFC SPK");
        return err;
    }
    uint8_t p_num_of_active_nfc = data.p_data[OFFSET_COUNT_NFC_LIST];

    for (int i = 0; i<num_of_active_nfc; i++)
    {
        p_indexes_list[i] = data.p_data[OFFSET_INDEXES_NFC_LIST+i];
    }

    return NRF_SUCCESS;
}

static ret_code_t nfc_spk_save_comprehensive()
{
    ret_code_t err;
    

    return NRF_SUCCESS;
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
        isSame = memcmp(m_reg.nfc_list[index_nfc].nfc_id, p_nfc_id, nfc_id_len);
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