SDCC=sdcc
STM8FLASH=C:\stm8flash-master\stm8flash.exe
OBJECTS=ledpulse.ihx

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECTS)
	$(STM8FLASH) -c stlinkv2 -p stm8s103f3 -w $(OBJECTS)

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $<
