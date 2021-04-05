
DMX_INC =  -I/opt/vc/include/ -I /opt/vc/include/interface/vmcs_host/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
EGL_INC = 
GLES_INC = -ICommon/
INCLUDES = $(DMX_INC) $(EGL_INC) $(GLES_INC)

CFLAGS = -g $(INCLUDES)
LDFLAGS =  -lm -L/opt/vc/lib/ -lbrcmEGL -lbrcmGLESv2 -lbrcmOpenVG -lbcm_host -lvcos -lvchiq_arm

all: demo1 sprite_scale gui

demo1: demo1.o sprites.o vc.o
	cc $(CFLAGS) -o demo1 demo1.o sprites.o vc.o $(LDFLAGS)
	
gui: gui.o rpi.o
	cc $(CFLAGS) -o gui gui.o rpi.o $(LDFLAGS)
	
sprite_scale: sprite_scale.o sprites.o rpi.o
	cc $(CFLAGS) -o sprite_scale sprite_scale.o sprites.o rpi.o $(LDFLAGS)

demo1.o: demo1.c spritedata1.h sprites.h rpi.h

gui.o: gui.c rpi.h

sprite_scale.o: sprite_scale.c spritedata1.h sprites.h rpi.h

sprites.o: sprites.c sprites.h

rpi.o: rpi.h

vc.o: vc.h

clean:
	rm *.o


