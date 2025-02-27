#include "golioth_fw_update.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_flash_partitions.h"
#include "esp_app_format.h"
#include "esp_system.h"
#include <string.h>  // memcpy

#define TAG "fw_update_esp_idf"

static esp_ota_handle_t _update_handle;
static const esp_partition_t* _update_partition;

bool fw_update_is_pending_verify(void) {
    const esp_partition_t* running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;
    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {
        if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
            return true;
        }
    }
    return false;
}

void fw_update_rollback(void) {
    esp_ota_mark_app_invalid_rollback_and_reboot();
}

void fw_update_reboot(void) {
    esp_restart();
}

void fw_update_cancel_rollback(void) {
    esp_ota_mark_app_valid_cancel_rollback();
}

golioth_status_t fw_update_handle_block(
        const uint8_t* block,
        size_t block_size,
        size_t offset,
        size_t total_size) {
    esp_err_t err = ESP_OK;

    if (offset == 0) {
        _update_partition = esp_ota_get_next_update_partition(NULL);
        assert(_update_partition);
        GLTH_LOGI(
                TAG,
                "Writing to partition subtype %d at offset 0x%" PRIx32,
                _update_partition->subtype,
                _update_partition->address);

        GLTH_LOGI(TAG, "Erasing flash");
        err = esp_ota_begin(_update_partition, OTA_SIZE_UNKNOWN, &_update_handle);
        if (err != ESP_OK) {
            GLTH_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
            esp_ota_abort(_update_handle);
            return GOLIOTH_ERR_FAIL;
        }
    }

    err = esp_ota_write(_update_handle, (const void*)block, block_size);
    if (err != ESP_OK) {
        GLTH_LOGE(TAG, "esp_ota_write failed (%s)", esp_err_to_name(err));
        esp_ota_abort(_update_handle);
        return GOLIOTH_ERR_FAIL;
    }

    return GOLIOTH_OK;
}

void fw_update_post_download(void) {
    // Nothing to do
}

golioth_status_t fw_update_validate(void) {
    assert(_update_handle);
    esp_err_t err = esp_ota_end(_update_handle);
    if (err != ESP_OK) {
        if (err == ESP_ERR_OTA_VALIDATE_FAILED) {
            GLTH_LOGE(TAG, "Image validation failed, image is corrupted");
        } else {
            GLTH_LOGE(TAG, "esp_ota_end failed (%s)!", esp_err_to_name(err));
        }

        return GOLIOTH_ERR_FAIL;
    }

    return GOLIOTH_OK;
}

golioth_status_t fw_update_change_boot_image(void) {
    assert(_update_partition);

    GLTH_LOGI(TAG, "Setting boot partition");
    esp_err_t err = esp_ota_set_boot_partition(_update_partition);
    if (err != ESP_OK) {
        GLTH_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
        return GOLIOTH_ERR_FAIL;
    }
    return GOLIOTH_OK;
}

void fw_update_end(void) {
    if (_update_handle) {
        esp_ota_end(_update_handle);
    }
}

golioth_status_t fw_update_read_current_image_at_offset(
        uint8_t* buf,
        size_t bufsize,
        size_t offset) {
    const esp_partition_t* current_partition = esp_ota_get_running_partition();
    if (!current_partition) {
        GLTH_LOGE(TAG, "current_partition invalid");
        return GOLIOTH_ERR_FAIL;
    }

    esp_err_t read_status = esp_partition_read(current_partition, offset, buf, bufsize);
    if (read_status != ESP_OK) {
        GLTH_LOGE(TAG, "esp_partition_read error: %s", esp_err_to_name(read_status));
        return GOLIOTH_ERR_FAIL;
    }

    return GOLIOTH_OK;
}
