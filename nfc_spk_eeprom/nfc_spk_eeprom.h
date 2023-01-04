#include <stdint.h>
#include <sdk_errors.h>
#include <nrf_drv_twi.h>

#define MAX_SPK_COUNT_PER_NFC               5
#define MAX_NFC_COUNT                       5

#defien NO_MORE_ITEM                        -1

// @brief data type for SPK. we did this to anticipate there is a change in spk id data size
typedef uint16_t spk_id_t;

// @brief
typedef struct
{
    spk_id_t spk_id;
    uint32_t counter;
}spk_t

// @brief 
typedef struct
{
    uint8_t         index_spk_list[MAX_SPK_COUNT_PER_NFC];
    uint8_t         count_spk_list;
    spk_t           spk_list[MAX_SPK_COUNT_PER_NFC];
    uint8_t         nfc_id_len;              ///< UID length.
    uint8_t         nfc_id[MAX_NFC_A_ID_LEN]; ///< NFC-A UID.
}active_nfc_t;

// @brief
typedef struct
{
    uint8_t         index_nfc_list[MAX_NFC_COUNT];
    uint8_t         count_nfc_list;
    active_nfc_t    nfc_list[MAX_NFC_COUNT];
}nfc_spk_reg_t

static inline uint8_t nfc_spk_start_address_complete_set(uint8_t nfc_index);

static ret_code_t nfc_spk_eeprom_init(nrf_drv_twi_t * const p_twi_master);

static ret_code_t

static ret_code_t nfc_spk_eeprom_retrieve_all_data();

static ret_code_t nfc_spk_save_spk(uint8_t * p_nfc_id, uint8_t nfc_id_len);