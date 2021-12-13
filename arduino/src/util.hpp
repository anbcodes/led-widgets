#pragma once

template <typename T>
T extract(uint8_t *data) {
  // If you get a compile error, it's because you did not use
  // one of the authorized template specializations
  const int k = 25;
  k = 36;
}

template <>
uint16_t extract(uint8_t *data) {
  return data[0] + (data[1] << 8);
}

template <>
uint32_t extract(uint8_t *data) {
  return data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
}

template <>
uint64_t extract(uint8_t *data) {
  return data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24) +
         (data[4] << 32) + (data[5] << 40) + (data[6] << 48) + (data[7] << 56);
}

template <>
int64_t extract(uint8_t *data) {
  uint64_t e = extract<uint64_t>(data);
  return *(reinterpret_cast<int64_t *>(&e));
}

template <>
int32_t extract(uint8_t *data) {
  uint32_t e = extract<uint32_t>(data);
  return *(reinterpret_cast<int32_t *>(&e));
}

template <>
int16_t extract(uint8_t *data) {
  uint16_t e = extract<uint16_t>(data);
  return *(reinterpret_cast<int16_t *>(&e));
}

template <>
int8_t extract(uint8_t *data) {
  return *(reinterpret_cast<int8_t *>(data));
}

// Easing equations
// (https://github.com/sveltejs/svelte/blob/master/src/runtime/easing/index.ts)

// float bounceOut(float t) {
//   float a = 4.0 / 11.0;
//   float b = 8.0 / 11.0;
//   float c = 9.0 / 10.0;

//   float ca = 4356.0 / 361.0;
//   float cb = 35442.0 / 1805.0;
//   float cc = 16061.0 / 1805.0;

//   // float ca = 600.0 / 361.0; // Idk
//   // float cb = 3544.0 / 1805.0; // Bounces count
//   // float cc = 3606.0 / 1805.0;  //

//   float t2 = t * t;

//   return t < a   ? 7.5625 * t2
//          : t < b ? 9.075 * t2 - 9.9 * t + 3.4
//          : t < c ? ca * t2 - cb * t + cc
//                  : 10.8 * t * t - 20.52 * t + 10.72;
// }

// float bounceInOut(float t) {
//   return t < 0.5 ? 0.5 * (1.0 - bounceOut(1.0 - t * 2.0))
//                  : 0.5 * bounceOut(t * 2.0 - 1.0) + 0.5;
// }

float cubicInOut(float t) {
  return t < 0.5 ? 4.0 * t * t * t : 0.5 * pow(2.0 * t - 2.0, 3.0) + 1.0;
}