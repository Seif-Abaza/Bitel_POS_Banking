#==============================================================================
#
#    makefile
#
#==============================================================================
#==============================================================================
#
# Memory Layout
#
LAYOUT	= release

#
# some usefull variables
#

BT_HDE		= ../BF_SDK
INCLUDE		= ../BF_SDK/include
BT_DRV		= ../BF_SDK/Library/bt_driver
BUILD		= .
INSTALL		= ../BIN
LIBRARY		= ../BF_SDK/Library/Static_Lib
BTLIB		= ../BF_SDK/Library/BTLIB
BTEMEA_LIB  = ../BF_SDK/Library/BTEMEA_LIB
SCRIPT		= ../BF_SDK/SCRIPTS

#LIB		= .
#LIBPATH	= .

#LIBATOF	    = $(LIBRARY)/atoflib.a
LIBC		= $(LIBRARY)/libc.a
LIBE		= $(LIBRARY)/libe.a
LIBM		= $(LIBRARY)/libm.a
LIBEMV		= $(LIBRARY)/emvlib.a
LIBEMVL2	= $(LIBRARY)/emvl2d.a
LIBSTDC++	= $(LIBRARY)/libstdc++.a
LIBSUPC++	= $(LIBRARY)/libsupc++.a
#LIBCONTLESS	= $(LIBRARY)/libcontactless.a

include makefile_settings

AS		= sde-gcc -xassembler-with-cpp
ASFLAGS	= -march=4kec -mips32r2 -c -D_32_ -D_ASSEMBLER_ -EL -gstabs+


CC		= sde-gcc
CFLAGS	= -march=4kec -mips32r2 -mno-float -EL -gdwarf-2 \
		     -ffixed-14 -ffixed-15 -G0 -fomit-frame-pointer -fsigned-char \
		     -Wimplicit -Wformat -D_32_ -c -mlong-calls -D XMALLOC_USER -D NO_FILESYSTEM -D SINGLE_THREADED $(GCC_OPT) \
		     -DPROJ_NAME="$(PROJ_NAME)" -DPROJ_MODE="$(PROJ_MODE)"

CPP		= sde-g++
CPPFLAGS	= -march=4kec -mips32r2 -mno-float -EL -gdwarf-2 -fno-exceptions -fno-rtti \
	     		-ffixed-14 -ffixed-15 -G0 -fomit-frame-pointer -fsigned-char \
			-Wimplicit -Wformat -D_32_ -c -mlong-calls -D XMALLOC_USER -D NO_FILESYSTEM -D SINGLE_THREADED $(GCC_OPT) \
			-DPROJ_NAME="$(PROJ_NAME)" -DPROJ_MODE="$(PROJ_MODE)"
		     
		     

LD		    = sde-ld
#LDFLAG		= -EL -T $(SCRIPT)/ic5100_flash_$(LAYOUT).ld
LDFLAG		= -EL -T ic5100_CPP_flash_$(LAYOUT).ld

GDB		= sde-insight
#GDBFLAGS = -x $(SCRIPT)/ic3100_app_$(LAYOUT).ini
GDBFLAGS = -x $(SCRIPT)/ic5100_flash_$(LAYOUT).ini


OBJCOPY	= sde-objcopy
OBJDUMP	= sde-objdump
OBJCONV	= sde-conv
OBJVER	= sde-strings

#
# Executable file name
#

#
# Object file list
#
SRC_CPP		= $(wildcard ./*.cpp ./xml/pugixml/*.cpp ./xml/*.cpp ./iso/*.cpp ./card/*.cpp ./fs/*.cpp ./internal/*.cpp ./tasks/*.cpp  ./Applications/*.cpp ./Applications/emv/*.cpp)
SRC			= $(wildcard ./*.c ./mf/*.c ../BF_SDK/Source/*.c ./Applications/*.c ./Applications/resources/*.c ./Applications/gsoap/*.c ./Applications/emv/*.c  ./gzip/*.c ../BF_SDK/Source/ssl/*.c)
SRC1		= $(wildcard ./*.S)
OBJECTS		= $(SRC:.c=.o) $(SRC1:.S=.o) $(SRC_CPP:.cpp=.o)

#
# Assembler compilation
#
%.o: %.S
	@mkdir -p $(BUILD)
	$(AS) $(ASFLAGS) $(patsubst %, -I%,$(INCLUDE)) $< \
	-o $(patsubst %, $(BUILD)/%, $@)
%.o: %.s
	@mkdir -p $(BUILD)
	$(AS) $(ASFLAGS) $(patsubst %, -I%,$(INCLUDE)) $< \
	-o $(patsubst %, $(BUILD)/%, $@)

#
# C compilation
#
%.o: %.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(patsubst %, -I%,$(INCLUDE) $(BT_INCLUDE)) $< \
	-o $(patsubst %, $(BUILD)/%, $@)

#
# C++ compilation
#
%.o: %.cpp
	@mkdir -p $(BUILD)
	$(CPP) $(CPPFLAGS) $(patsubst %, -I%,$(INCLUDE) $(BT_INCLUDE)) $< \
	-o $(patsubst %, $(BUILD)/%, $@) 
	
	
include makefile_dep


all: $(OBJECTS) $(BT_DRV)/btutils.o
	 $(LD) $(LDFLAG) -L $(BUILD) -L ./LIB -L $(BT_DRV)  \
	-o "$(INSTALL)/$(PROJ_TARGET).elf"  -Map "$(INSTALL)/$(PROJ_TARGET).map"  \
	--start-group  $(FIRMWARE) $(LIBC) $(LIBE) $(LIBM) $(LIBEMV) $(LIBEMVL2)   \
	$(LIBSTDC++) $(LIBSUPC++) \
	$(patsubst %, $(BUILD)/%, $(OBJECTS) $(BT_DRV)/cstartup.o $(BT_DRV)/btutils.o $(BT_DRV)/sfont.o  $(BTLIB)/libc.o\
	$(BT_DRV)/asmutil.o $(BT_DRV)/prtutil.o $(BT_DRV)/prtfont.o \
	$(BTLIB)/frmwri.o $(BT_DRV)/CryptoLib.o  $(BT_DRV)/ppdrv.o \
	$(BTLIB)/BEEPLIB1.o $(BTLIB)/UTILLIB.o \
	$(BTLIB)/timerlib.o $(BTLIB)/timerlib_1.o $(BTLIB)/lcdlib.o $(BTLIB)/lcdlib2.o \
	$(BTLIB)/iso8583.o $(BTLIB)/commlib.o   $(BTLIB)/sysinfo.o \
	$(BTLIB)/msrlib.o $(BTLIB)/msrlib_1.o $(BTLIB)/MAGTEK_MSR.o $(BTLIB)/rtclib.o $(BTLIB)/rtclib_1.o \
	$(BTLIB)/KEYLIB.o $(BTLIB)/KEYLIB_1.o $(BTLIB)/beeplib.o  $(BTLIB)/DES_ECB.o \
	$(BTLIB)/strtok.o  $(BTLIB)/han13.o   )   --end-group  #$(LIBBTIC)$(BTLIB)/WiFiLIB.o $(LIBATOF) #$(BTLIB)/PRINTF.o $(BTLIB)/commlib_1.o $(BTLIB)/bitel_tl.o $(LIBCONTLESS)

	
image: all
	$(OBJCONV) -v -l "$(INSTALL)/$(PROJ_TARGET).elf"
ifeq ($(LAYOUT),release)
	$(OBJCONV) -v -x 1,2,3,4 -o "$(INSTALL)/$(PROJ_TARGET).s19" "$(INSTALL)/$(PROJ_TARGET).elf"
endif

debug:
	$(GDB) $(GDBFLAGS) "$(INSTALL)/$(PROJ_TARGET).elf"


clean:
	rm -f $(OBJECTS) "$(INSTALL)/$(PROJ_TARGET).elf" "$(INSTALL)/$(PROJ_TARGET).map"

depend:
	makedepend -f makefile_dep $(SRC_CPP) -o .o -I $(INCLUDE) 2>/dev/null
	makedepend -f makefile_dep $(SRC) -o .o -I $(INCLUDE) -a 2>/dev/null




# EOF #
# DO NOT DELETE
