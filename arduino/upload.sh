rm -rf /tmp/arduino-upload

arduino-cli compile --output-dir=build --build-path=/tmp/arduino-upload -b arduino:samd:mkrwifi1010 .

arduinoOTA -address "$1" -port 65280 -username arduino -password "LEDS1234" -sketch "/tmp/arduino-upload/arduino.ino.bin" -upload /sketch -b

rm -rf /tmp/arduino-upload
