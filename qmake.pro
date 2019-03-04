TEMPLATE = app

#Executable Name
TARGET = TextureMap
CONFIG = debug

#Destination
DESTDIR = .
OBJECTS_DIR = ./build/

#Header Files to Include
HEADERS	+= 	../common/Shader.h					    \	
			../common/Vector.h					    \
			../common.Mesh.h					    \   
			../common/Matrix.h					    \
            ../common/SphericalCameraManipulator.h  \
            ../common/Texture.h                     \				

#Sources
SOURCES += 	main.cpp							    \
			../common/Shader.cpp				    \
			../common/Vector.cpp				    \
			../common/Mesh.cpp					    \
			../common/Matrix.cpp				    \					
            ../common/SphericalCameraManipulator.cpp\				
            ../common/Texture.cpp                   \				

#Include Path
INCLUDEPATH += 	./ 								\
			../common/ 							\


#Library Libraries
LIBS +=	-lglut									\
		-lGLEW									\
        -lGLU                                   \         
		-lGL									\
