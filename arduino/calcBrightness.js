function sRGBtoLin(colorChannel) {
  // Send this function a decimal sRGB gamma encoded color value
  // between 0.0 and 1.0, and it returns a linearized value.

if ( colorChannel <= 0.04045 ) {
      return colorChannel / 12.92;
  } else {
      return Math.pow((( colorChannel + 0.055)/1.055),2.4);
  }
}


function YtoLstar(Y) {
  // Send this function a luminance value between 0.0 and 1.0,
  // and it returns L* which is "perceptual lightness"

if ( Y <= (216/24389)) {       // The CIE standard states 0.008856 but 216/24389 is the intent for 0.008856451679036
      return Y * (24389/27);  // The CIE standard states 903.3, but 24389/27 is the intent, making 903.296296296296296
  } else {
      return Math.pow(Y,(1/3)) * 116 - 16;
  }
}

let vR = 255 / 255
let vG = 140 / 255
let vB = 40 / 255

let Y = (0.2126 * sRGBtoLin(vR) + 0.7152 * sRGBtoLin(vG) + 0.0722 * sRGBtoLin(vB))

console.log(YtoLstar(Y))