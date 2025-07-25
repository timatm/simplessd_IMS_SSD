/*
 * Copyright (C) 2017 CAMELab
 *
 * This file is part of SimpleSSD.
 *
 * SimpleSSD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifndef __HIL_NVME_DEF__
#define __HIL_NVME_DEF__

#include <cinttypes>
#include <cstring>

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

#define NSID_NONE 0x00000000
#define NSID_LOWEST 0x00000001
#define NSID_ALL 0xFFFFFFFF

#define MAKE_SGL_ID(type, subtype)                                             \
  (uint8_t)(((type << 4) & 0xF0) | (subtype & 0x0F))
#define SGL_TYPE(id) (uint8_t)(id >> 4)
#define SGL_SUBTYPE(id) (uint8_t)(id & 0x0F)

#define OCSSD_VENDOR 0x1D1D
#define OCSSD_SSVID_1_2 0x0102
#define OCSSD_SSVID_2_0 0x0200

typedef union _HealthInfo {
  uint8_t data[0x200];
  struct {
    uint8_t status;
    uint16_t temperature;
    uint8_t availableSpare;
    uint8_t spareThreshold;
    uint8_t lifeUsed;
    uint8_t reserved[26];
    uint64_t readL;
    uint64_t readH;
    uint64_t writeL;
    uint64_t writeH;
    uint64_t readCommandL;
    uint64_t readCommandH;
    uint64_t writeCommandL;
    uint64_t writeCommandH;
  };

  _HealthInfo() { memset(data, 0, 0x200); }
} HealthInfo;

typedef enum : uint8_t {
  TYPE_DATA_BLOCK_DESCRIPTOR = 0x00,
  TYPE_BIT_BUCKET_DESCRIPTOR = 0x01,
  TYPE_SEGMENT_DESCRIPTOR = 0x02,
  TYPE_LAST_SEGMENT_DESCRIPTOR = 0x03,
  TYPE_KEYED_DATA_BLOCK_DESCRIPTOR = 0x04
} SGL_DESCRIPTOR_TYPE;

typedef enum : uint8_t {
  SUBTYPE_ADDRESS = 0x00,
  SUBTYPE_OFFSET = 0x01,
  SUBTYPE_NVME_TRANSPORT_SPECIFIC = 0x02
} SGL_DESCRIPTOR_SUBTYPE;

typedef enum {
  PRIORITY_URGENT,
  PRIORITY_HIGH,
  PRIORITY_MEDIUM,
  PRIORITY_LOW
} SQUEUE_PRIORITY;

typedef enum {
  REG_CONTROLLER_CAPABILITY = 0x00,
  REG_VERSION = 0x08,
  REG_INTERRUPT_MASK_SET = 0x0C,
  REG_INTERRUPT_MASK_CLEAR = 0x10,
  REG_CONTROLLER_CONFIG = 0x14,
  REG_CONTROLLER_STATUS = 0x1C,
  REG_NVM_SUBSYSTEM_RESET = 0x20,
  REG_ADMIN_QUEUE_ATTRIBUTE = 0x24,
  REG_ADMIN_SQUEUE_BASE_ADDR = 0x28,
  REG_ADMIN_CQUEUE_BASE_ADDR = 0x30,
  REG_CMB_LOCATION = 0x38,
  REG_CMB_SIZE = 0x3C,
  REG_DOORBELL_BEGIN = 0x1000
} COMMAND_REGISTER;

typedef enum { ROUND_ROBIN, WEIGHTED_ROUND_ROBIN } ARBITRATION_METHOD;

typedef enum {
  OPCODE_PROPERTY_SET = 0x00,
  OPCODE_CONNECT = 0x01,
  OPCODE_PROPERTY_GET = 0x04,
  OPCODE_AUTHENTICATION_SEND = 0x05,
  OPCODE_AUTHENTICATION_RECEIVE = 0x06
} FABRIC_OPCODE;

typedef enum {
  OPCODE_DELETE_IO_SQUEUE = 0x00,
  OPCODE_CREATE_IO_SQUEUE = 0x01,
  OPCODE_GET_LOG_PAGE = 0x02,
  OPCODE_DELETE_IO_CQUEUE = 0x04,
  OPCODE_CREATE_IO_CQUEUE = 0x05,
  OPCODE_IDENTIFY = 0x06,
  OPCODE_ABORT = 0x08,
  OPCODE_SET_FEATURES = 0x09,
  OPCODE_GET_FEATURES = 0x0A,
  OPCODE_ASYNC_EVENT_REQ = 0x0C,
  OPCODE_NAMESPACE_MANAGEMENT = 0x0D,
  OPCODE_FIRMWARE_COMMIT = 0x10,
  OPCODE_FIRMWARE_DOWNLOAD = 0x11,
  OPCODE_DEVICE_SELF_TEST = 0x14,
  OPCODE_NAMESPACE_ATTACHMENT = 0x15,
  OPCODE_KEEPALIVE = 0x18,
  OPCODE_DIRECTIVE_SEND = 0x19,
  OPCODE_DIRECTIVE_RECEIVE = 0x1A,
  OPCODE_VIRTUALIZATION_MANAGEMENT = 0x1C,
  OPCODE_NVME_MI_SEND = 0x1D,
  OPCODE_NVME_MI_RECEIVE = 0x1E,
  OPCODE_DOORBELL_BUFFER_CONFIG = 0x7C,
  OPCODE_FORMAT_NVM = 0x80,
  OPCODE_SECURITY_SEND = 0x81,
  OPCODE_SECURITY_RECEIVE = 0x82,
  OPCODE_SANITIZE = 0x84,

  // OpenChannel SSD 1.2
  OPCODE_DEVICE_IDENTIFICATION = 0xE2,
  OPCODE_SET_BAD_BLOCK_TABLE = 0xF1,
  OPCODE_GET_BAD_BLOCK_TABLE = 0xF2,

  // OpenChannel SSD 2.0
  OPCODE_GEOMETRY = 0xE2
} ADMIN_OPCODE;

typedef enum {
  OPCODE_FLUSH = 0x00,
  OPCODE_WRITE = 0x01,
  OPCODE_READ = 0x02,
  OPCODE_WRITE_UNCORRECTABLE = 0x04,
  OPCODE_COMPARE = 0x05,
  OPCODE_WRITE_ZEROS = 0x08,
  OPCODE_DATASET_MANAGEMEMT = 0x09,
  OPCODE_RESERVATION_REGISTER = 0x0D,
  OPCODE_RESERVATION_REPORT = 0x0E,
  OPCODE_RESERVATION_ACQUIRE = 0x11,
  OPCODE_RESERVATION_RELEASE = 0x15,

  // Custom opcodes
  OPCODE_WRITE_SSTABLE = 0x80,
  OPCODE_READ_SSTABLE,
  OPCODE_SEARCH_KEY,
  OPCODE_INIT_IMS,
  
  // OpenChannel SSD 1.2
  OPCODE_PHYSICAL_BLOCK_ERASE = 0x90,
  OPCODE_PHYSICAL_PAGE_WRITE,
  OPCODE_PHYSICAL_PAGE_READ,
  OPCODE_PHYSICAL_PAGE_RAW_WRITE = 0x95,
  OPCODE_PHYSICAL_PAGE_RAW_READ,

  // OpenChannel SSD 2.0
  OPCODE_VECTOR_CHUNK_RESET = 0x90,
  OPCODE_VECTOR_CHUNK_WRITE,
  OPCODE_VECTOR_CHUNK_READ,
  OPCODE_VECTOR_CHUNK_COPY
} NVM_OPCODE;

typedef enum {
  LOG_ERROR_INFORMATION = 0x01,
  LOG_SMART_HEALTH_INFORMATION,
  LOG_FIRMWARE_SLOT_INFORMATION,
  LOG_CHANGED_NAMESPACE_LIST,
  LOG_COMMAND_EFFECTS_LOG,
  LOG_RESERVATION_NOTIFICATION = 0x80,

  // OpenChannel SSD
  LOG_CHUNK_INFORMATION = 0xCA
} LOG_PAGE;

typedef enum {
  CNS_IDENTIFY_NAMESPACE = 0x00,
  CNS_IDENTIFY_CONTROLLER = 0x01,
  CNS_ACTIVE_NAMESPACE_LIST = 0x02,
  CNS_ALLOCATED_NAMESPACE_LIST = 0x10,
  CNS_IDENTIFY_ALLOCATED_NAMESPACE = 0x11,
  CNS_ATTACHED_CONTROLLER_LIST = 0x12,
  CNS_CONTROLLER_LIST = 0x13
} IDENTIFY_CNS;

typedef enum {
  FEATURE_ARBITRATION = 0x01,
  FEATURE_POWER_MANAGEMENT,
  FEATURE_LBA_RANGE_TYPE,
  FEATURE_TEMPERATURE_THRESHOLD,
  FEATURE_ERROR_RECOVERY,
  FEATURE_VOLATILE_WRITE_CACHE,
  FEATURE_NUMBER_OF_QUEUES,
  FEATURE_INTERRUPT_COALESCING,
  FEATURE_INTERRUPT_VECTOR_CONFIGURATION,
  FEATURE_WRITE_ATOMICITY_NORMAL,
  FEATURE_ASYNC_EVENT_CONFIGURATION,
  FEATURE_AUTO_POWER_STATE_TRANSITION,
  FEATURE_HOST_MEMORY_BUFFER,
  FEATURE_TIMESTAMP,
  FEATURE_KEEPALIVE_TIMER,
  FEATURE_HOST_CONTROLLED_THERMAL_MANAGEMENT,
  FEATURE_NON_OPERATIONAL_POWER_STATE_CONFIG,
  FEATURE_SOFTWARE_PROGRESS_MARKER = 0x80,
  FEATURE_HOST_IDENTIFIER,
  FEATURE_RESERVATION_NOTIFICATION_MASK,
  FEATURE_RESERVATION_PERSISTANCE,

  // OpenChannel SSD
  FEATURE_MEDIA_FEEDBACH = 0xCA
} FEATURE;

typedef enum {
  TYPE_GENERIC_COMMAND_STATUS,   // -> NVME_STATUS_CODE
  TYPE_COMMAND_SPECIFIC_STATUS,  // -> NVME_ERROR_CODE
  TYPE_MEDIA_AND_DATA_INTEGRITY_ERROR,
} STATUS_CODE_TYPE;

typedef enum {
  /** Generic Command Status **/
  STATUS_SUCCESS,
  STATUS_INVALID_OPCODE,
  STATUS_INVALID_FIELD,
  STATUS_CID_CONFLICT,
  STATUS_DATA_TRANSFER_ERROR,
  STATUS_ABORT_DUE_TO_POWER_LOSS,
  STATUS_INTERNAL_ERROR,
  STATUS_ABORT_REQUESTED,
  STATUS_ABORT_DUE_TO_SQ_DELETE,
  STATUS_ABORT_DUE_TO_FAILED_FUSE,
  STATUS_ABORT_DUE_TO_MISSING_FUSE,
  STATUS_ABORT_INVALID_NAMESPACE,
  STATUS_ABORT_INVALID_FORMAT = STATUS_ABORT_INVALID_NAMESPACE,
  STATUS_COMMAND_SEQUENCE_ERROR,
  STATUS_INVALID_SGL_SEGMENT_DESCRIPTOR,
  STATUS_INVALID_NUMBER_OF_SGL_DESCRIPTOR,
  STATUS_INVALID_DATA_SGL_LENGTH,
  STATUS_INVALID_METADATA_SGL_LENGTH,
  STATUS_INVALID_SGL_DESCRIPTOR_TYPE,
  STATUS_INVALID_USE_OF_MEMORY_BUFFER,
  STATUS_PRP_OFFSET_INVALID,
  STATUS_ATOMIC_WRITE_UNIT_EXCEEDED,
  STATUS_INVALID_SGL_OFFSET = 0x16,
  STATUS_INVALID_SGL_SUB_TYPE,
  STATUS_HOST_ID_INCONSISTENT_FORMAT,
  STATUS_KEEPALIVE_TIMEOUT_EXPIRED,
  STATUS_INVALID_KEEPALIVE_TIMEOUT,

  /** NVM Command Status **/
  STATUS_LBA_OUT_OF_RANGE = 0x80,
  STATUS_CAPACITY_EXCEEDED,
  STATUS_NAMESPACE_NOT_READY,
  STATUE_RESERVATION_CONFLICT,
  STATUS_FORMAT_IN_PROGRESS
} STATUS_CODE;

typedef enum {
  /** Generic Command Errors **/
  STATUS_INVALID_COMPLETION_QUEUE,
  STATUS_INVALID_QUEUE_ID,
  STATUS_INVALID_QUEUE_SIZE,
  STATUS_ABORT_COMMAND_LIMIT_EXCEEDED,
  STATUS_ASYNC_EVENT_REQUEST_LIMIT_EXCEEDED = 0x05,
  STATUS_INVALID_FIRMWARE_SLOT,
  STATUS_INVALID_FIRMWARE_IMAGE,
  STATUS_INVALID_INTERRUPT_VECTOR,
  STATUS_INVALID_LOG_PAGE,
  STATUS_INVALID_FORMAT,
  STATUS_FIRMWARE_ACTIVATION_REQUIRES_CONVENTIONAL_RESET,
  STATUS_INVALID_QUEUE_DELETE,
  STATUS_FEATURE_ID_NOT_SAVEABLE,
  STATUS_FEATURE_NOT_CHANGABLE,
  STATUS_FEATURE_NOT_NAMESPACE_SPECIFIC,
  STATUS_FIRMWARE_ACTIVATION_REQUIRES_NVM_SUBSYSTEM_RESET,
  STATUS_FIRMWARE_ACTIVATION_REQUIRES_RESET,
  STATUS_FIRMWARE_ACTIVATION_REQUIRES_MAXIMUM_TIME_VIOLATION,
  STATUS_FIRMWARE_ACTIVATION_PROHIBITED,
  STATUS_OVERLAPPING_RANGE,
  STATUS_NAMESPACE_INSUFFICIENT_CAPACITY,
  STATUS_NAMESPACE_ID_UNAVAILABLE,
  STATUS_NAMESPACE_ALREADY_ATTACHED = 0x18,
  STATUS_NAMESPACE_IS_PRIVATE,
  STATUS_NAMESPACE_NOT_ATTACHED,
  STATUS_THIN_PROVISIONING_NOT_SUPPORTED,
  STATUS_CONTROLLER_LIST_INVALID,

  /** NVM Command Errors **/
  STATUS_ATTRIBUTE_CONFLICT = 0x80,
  STATUS_INVALID_PROTECTION_INFORMATION,
  STATUS_WRITE_TO_READ_ONLY_RANGE,

  /** custom command Errors **/
  STATUS_LBN_INVALID = 0x90,
  STATUS_IMS_INIT_FAILD,
  STATUS_IMS_INIT_SUCCESS,
  STATUS_IMS_MONITOR_FAILD,
  STATUS_IMS_MONITOR_SUCCESS
} ERROR_CODE;

typedef enum {
  /** I/O Command Media Errors **/
  STATUS_WRITE_FAULT = 0x80,
  STATUS_UNRECOVERED_READ_ERROR,
  STATUS_END_TO_END_GUARD_CHECK_ERROR,
  STATUS_END_TO_END_APPLICATION_TAG_CHECK_ERROR,
  STATUS_END_TO_END_REFERENCE_TAG_CHECK_ERROR,
  STATUS_COMPARE_FAILURE,
  STATUS_ACCESS_DENIED,
  STATUS_DEALLOCATED_OR_UNWRITTEN_LOGICAL_BLOCK
} MEDIA_ERROR_CODE;

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD

#endif
