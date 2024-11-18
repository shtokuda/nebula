#pragma once

#include <cstddef>
#include <cstdint>
#include <ctime>

namespace nebula
{
namespace drivers
{
namespace velodyne_packet
{

/**
 * Raw Velodyne packet constants and structures.
 */
static const int RAW_SCAN_SIZE = 3;  // TODO: remove
static const int SCANS_PER_BLOCK = 32;  // TODO: remove
static const int CHANNELS_PER_BLOCK = 32;

static const double ROTATION_RESOLUTION = 0.01;     // [deg]
static const uint16_t ROTATION_MAX_UNITS = 360 * 100u;  // [deg/100]

static const size_t RETURN_MODE_INDEX = 1204;

/** @todo make this work for both big and little-endian machines */
static const uint16_t UPPER_BANK = 0xeeff;
static const uint16_t LOWER_BANK = 0xddff;

/** Return Modes **/
static const uint16_t RETURN_MODE_STRONGEST = 55;
static const uint16_t RETURN_MODE_LAST = 56;
static const uint16_t RETURN_MODE_DUAL = 57;

const int BLOCKS_PER_PACKET = 12;
const int PACKET_STATUS_SIZE = 4;
const int SCANS_PER_PACKET = (SCANS_PER_BLOCK * BLOCKS_PER_PACKET);
const int POINTS_PER_PACKET = (SCANS_PER_PACKET * RAW_SCAN_SIZE);

#pragma pack(push, 1)
/** \brief Raw Velodyne data block.
 *
 *  Each block contains data from either the upper or lower laser
 *  bank.  The device returns three times as many upper bank blocks.
 *
 *  use stdint.h types, so things work with both 64 and 32-bit machines
 */
struct raw_units{
  uint16_t distance;
  uint8_t reflectivity;
};

struct raw_block_t
{
  uint16_t flag;    ///< UPPER_BANK or LOWER_BANK
  uint16_t rotation;  ///< 0-35999, divide by 100 to get degrees
  raw_units units[SCANS_PER_BLOCK];
};

/** \brief Raw Velodyne packet.
 *
 *  revolution is described in the device manual as incrementing
 *    (mod 65536) for each physical turn of the device.  Our device
 *    seems to alternate between two different values every third
 *    packet.  One value increases, the other decreases.
 *
 *  \todo figure out if revolution is only present for one of the
 *  two types of status fields
 *
 *  status has either a temperature encoding or the microcode level
 */
struct raw_packet_t
{
  raw_block_t blocks[BLOCKS_PER_PACKET];
  uint32_t timestamp;
  uint16_t factory;
};
#pragma pack(pop)

/** \brief Velodyne echo types */
enum RETURN_TYPE {
  INVALID = 0,
  SINGLE_STRONGEST = 1,
  SINGLE_LAST = 2,
  DUAL_STRONGEST_FIRST = 3,
  DUAL_STRONGEST_LAST = 4,
  DUAL_WEAK_FIRST = 5,
  DUAL_WEAK_LAST = 6,
  DUAL_ONLY = 7
};

}  // namespace hesai_packet
}  // namespace drivers
}  // namespace nebula
