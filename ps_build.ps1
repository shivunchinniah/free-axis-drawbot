arduino-cli compile -b arduino:megaavr:nona4809:mode=off  --clean   
#arduino-cli compile -b arduino:megaavr:nona4809:mode=off  --clean  --warnings all --build-property "bootloader.OSCCFG=0x02" --build-property "build.f_cpu=20000000L" -v --clean
# arduino-cli compile -b arduino:megaavr:nona4809 --build-property "build.f_cpu=20000000L" --build-property "bootloader.OSCCFG=0x02"  --clean -v --show-properties --format json

# arduino-cli burn-bootloader -b arduino:megaavr:nona4809:mode=off --build-property "bootloader.OSCCFG=0x02" --build-property "build.f_cpu=20000000L" -p COM5