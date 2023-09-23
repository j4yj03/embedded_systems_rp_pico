# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/VSRM/sdk/pico/pico-sdk/tools/elf2uf2"
  "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/elf2uf2"
  "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pwm/elf2uf2"
  "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pwm/elf2uf2/tmp"
  "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pwm/elf2uf2/src/ELF2UF2Build-stamp"
  "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pwm/elf2uf2/src"
  "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pwm/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pwm/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pwm/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
