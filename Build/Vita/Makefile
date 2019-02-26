TARGET		:= AndromedaTests
TITLE		:= ANDRO0001

OBJS   = 	../../../Andromeda-Lib/Libs/tinyxml/tinystr.o \
			../../../Andromeda-Lib/Libs/tinyxml/tinyxmlerror.o \
			../../../Andromeda-Lib/Libs/tinyxml/tinyxmlparser.o \
			../../../Andromeda-Lib/Libs/tinyxml/tinyxml.o \
			../../Examples/ExampleGameManager.o \
			../../Examples/TestHelper.o \
			../../Examples/InputHelper.o \
			../../Examples/Basic/Test1.o \
			../../Examples/Basic/Test2.o \
			../../Examples/Basic/Test3.o \
			../../Examples/Basic/Test4.o \
			../../Examples/Basic/Test5.o \
			../../Examples/Basic/Test6.o \
			../../Examples/Basic/TestSprite.o \
			../../Examples/Basic/TestCam3d.o \
			../../Examples/Basic/TestToTexture.o \
			../../Examples/Bullet/TestBullet1.o \
			../../Examples/Bullet/TestBullet2.o \
			../../Examples/Bullet/TestBullet3.o \
			../../Examples/Bullet/TestBullet4.o \
			../../Examples/Bullet/BasicCar.o \
			../../Examples/Input/TouchTest.o \
			../../Examples/FreeTypeTests/TestFreeTypeAtlas.o \
			../../Examples/FreeTypeTests/TestFreeTypeFont.o \
			../../Examples/Models/TestObjModel.o \
			../../Examples/Models/TestObjKart.o \
			../../Examples/Models/TestMd5Model.o \
			../../Examples/Lightening/TestLight1.o \
			../../Examples/Lightening/TestLight2.o \
			../../Examples/Lightening/TestLight3.o \
			../../Examples/Other/TextureTest.o \
			../../Examples/Other/ShaderTest.o \
			../../Examples/Other/VertexArrayTest.o \
			../../Examples/Other/ObjModelTest.o \
			../../Examples/Other/FontTest.o \
			../../Examples/Andromeda.o
			
LIBS =  -lBulletDynamics -lBulletCollision -lLinearMath -lvorbisfile -lvorbisenc -lvorbis -lspeexdsp -logg -lSceDisplay_stub -lSceGxm_stub -lSceCommonDialog_stub -lSceCtrl_stub -lSceSysmodule_stub -lSceTouch_stub -lSceAudio_stub -lSceNet_stub -lSceNetCtl_stub -lSceAvPlayer_stub -lfreetype -lm -lc -lpng -lz -L../../../Andromeda-Lib/Build/Vita/ -lAndromedaLib

PREFIX  	= arm-vita-eabi
CC      	= $(PREFIX)-gcc
CXX      	= $(PREFIX)-g++
STRIP 		= $(PREFIX)-strip
CXXFLAGS  	= -Wl,-q -O3 -fpermissive -ffast-math -mtune=cortex-a9 -mfloat-abi=hard -mfpu=neon -std=gnu++11 -DANDROMEDA_VITA -DHAVE_OGGVORBIS -DWANT_FASTWAV -DHAVE_LIBSPEEXDSP -DUSE_AUDIO_RESAMPLER 
CXXFLAGS	+= -I../../ -I../../../Andromeda-Lib -I../../../Andromeda-Lib/Libs -I../../../Andromeda-Lib/Libs/glm -I../../../Andromeda-Lib/Libs/freetype/include -I../../../Andromeda-Lib/Libs/freetype_gl -I../../JellyPhysics -I../../Libs/tinyxml -I../../../Andromeda-Lib/Libs/stb -I../../../Andromeda-Lib/Libs/tiny_obj_loader -I../../../Andromeda-Lib/Libs/bullet/include/bullet
CFLAGS  	= -Wl,-q -O3 -std=c99 -ffast-math -mtune=cortex-a9 -mfloat-abi=hard -mfpu=neon -DANDROMEDA_VITA -DHAVE_OGGVORBIS -DWANT_FASTWAV  -DHAVE_LIBSPEEXDSP -DUSE_AUDIO_RESAMPLER 
CFLAGS  	+= -I../../ -I../../../Andromeda-Lib -I.../../../Andromeda-Lib/Libs -I../../../Andromeda-Lib/Libs/glm -I../../../Andromeda-Lib/Libs/freetype/include -I../../../Andromeda-Lib/Libs/freetype_gl -I../../JellyPhysics -I../../Libs/tinyxml -I../../../Andromeda-Lib/Libs/stb -I../../../Andromeda-Lib/Libs/tiny_obj_loader -I../../../Andromeda-Lib/Libs/bullet/include/bullet
ASFLAGS 	= $(CFLAGS)

all: $(TARGET).velf

%.velf: %.elf
	$(STRIP) -g $< 
	vita-elf-create $< $@
	vita-mksfoex -s TITLE_ID=$(TITLE) "Andromeda Tests" param.sfo
	vita-make-fself -s $@ eboot.bin

$(TARGET).elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $(CFLAGS)  $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS)
