NAME = fruit64
SIZE = 2M

LIBDIR = ./lib
SRCDIR = ./src
OBJDIR = ./obj
OUTDIR = ./out
#RESDIR = ./res

SOURCES := $(wildcard $(SRCDIR)/*.c $(LIBDIR)/*/*.c)
VPATH := $(dir $(SOURCES))
OBJECTS = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

ROOTDIR = $(N64_INST)
GCCN64 = $(ROOTDIR)/bin/mips64-elf-

CC = $(GCCN64)gcc
#AS = $(GCCN64)as
LD = $(GCCN64)ld
OBJCOPY = $(GCCN64)objcopy
N64TOOL = $(ROOTDIR)/bin/n64tool
CHKSUM64 = $(ROOTDIR)/bin/chksum64
#MKDFS = $(ROOTDIR)/bin/mkdfs

#ASFLAGS = -mtune=vr4300 -march=vr4300
CFLAGS = -std=gnu99 -march=vr4300 -mtune=vr4300 -O2 -Wall -I$(LIBDIR) -I./libdragon/include -I$(ROOTDIR)/mips64-elf/include -I$(ROOTDIR)/include
LDFLAGS = -L$(ROOTDIR)/mips64-elf/lib -L$(ROOTDIR)/lib -ldragon -lmikmod -lc -lm -ldragonsys -Tn64ld.x  --gc-sections
N64TOOLFLAGS = -l $(SIZE) -h "$(LIBDIR)/header.ed64" -t "EverDrive OS"

$(NAME).z64: $ $(NAME).elf #$(NAME).dfs
	$(OBJCOPY) $(OUTDIR)/$(NAME).elf $(OUTDIR)/$(NAME).bin -O binary
	rm -f $(OUTDIR)/$(NAME).z64
	$(N64TOOL) $(N64TOOLFLAGS) -o $(OUTDIR)/$(NAME).z64 $(OUTDIR)/$(NAME).bin #-s 1M $(OUTDIR)/$(NAME).dfs
	$(CHKSUM64) $(OUTDIR)/$(NAME).z64

$(NAME).elf : $(OBJECTS)
	@mkdir -p $(OUTDIR)
	$(LD) -o $(OUTDIR)/$(NAME).elf $(OBJECTS) $(LDFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#$(NAME).dfs:
#	$(MKDFS) $(OUTDIR)/$(NAME).dfs $(RESDIR)

clean:
	rm -rf $(OBJDIR) $(OUTDIR)