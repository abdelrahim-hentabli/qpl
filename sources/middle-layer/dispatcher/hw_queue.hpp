/*******************************************************************************
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef QPL_SOURCES_MIDDLE_LAYER_DISPATCHER_HW_QUEUE_HPP_
#define QPL_SOURCES_MIDDLE_LAYER_DISPATCHER_HW_QUEUE_HPP_

#include <atomic>

#include "qpl/c_api/status.h"
#include "hw_status.h"

#define TOTAL_OP_CFG_BIT_GROUPS 8u // 256 bits / 32 bit groups

namespace qpl::ml::dispatcher {

class hw_queue {
public:
    using descriptor_t = void;

    hw_queue() noexcept = default;

    hw_queue(const hw_queue &) noexcept = delete;

    auto operator=(const hw_queue &other) noexcept -> hw_queue & = delete;

    hw_queue(hw_queue &&other) noexcept;

    auto operator=(hw_queue &&other) noexcept -> hw_queue &;

    auto initialize_new_queue(descriptor_t *wq_descriptor_ptr) noexcept -> hw_accelerator_status;

    [[nodiscard]] auto get_portal_ptr() const noexcept -> void *;

    [[nodiscard]] auto enqueue_descriptor(void *desc_ptr) const noexcept -> qpl_status;

    [[nodiscard]] auto priority() const noexcept -> int32_t;

    [[nodiscard]] auto get_block_on_fault() const noexcept -> bool;

    [[nodiscard]] auto is_operation_supported(uint32_t operation) const noexcept -> bool;

    [[nodiscard]] auto get_op_configuration_support() const noexcept -> bool;

    [[nodiscard]] auto get_op_config_by_index(uint32_t index) const noexcept -> uint32_t;

    void set_portal_ptr(void *portal_ptr) noexcept;

    virtual ~hw_queue() noexcept;

private:
    bool                          block_on_fault_ = false;
    int32_t                       priority_       = 0u;
    uint64_t                      portal_mask_    = 0u;               /**< Mask for incrementing portals */
    mutable void                  *portal_ptr_    = nullptr;
    mutable std::atomic<uint64_t> portal_offset_  = 0u;               /**< Portal for enqcmd (mod page size)*/
    bool                          op_cfg_enabled_ = false;
    uint32_t                      op_cfg_register_[TOTAL_OP_CFG_BIT_GROUPS]  =
                                    {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; /**< OPCFG register content */
};

}
#endif //QPL_SOURCES_MIDDLE_LAYER_DISPATCHER_HW_QUEUE_HPP_
